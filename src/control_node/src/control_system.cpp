#include <rclcpp/rclcpp.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>
#include <ctime>
#include <cstdlib>
#include <memory>
#include <iostream>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include "nav2_msgs/action/navigate_to_pose.hpp"
#include <std_msgs/msg/string.hpp>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>

using json = nlohmann::json;

class ControlNode : public rclcpp::Node {
public:
    using NavigateToPose = nav2_msgs::action::NavigateToPose;
    using GoalHandleNavigateToPose = rclcpp_action::ClientGoalHandle<NavigateToPose>;

    ControlNode() : Node("control_node") {

        init_medicine_mapping();
        init_room_mapping();
        load_all_patients("/home/jason9308/robot_ws/command_jason/patient.json");
        print_all_patients();

        location_map_["home"] = std::make_tuple(-0.585f, 0.391f, 0.234f, 0.972f); // 預設家裡的座標

        json_sub_ = this->create_subscription<std_msgs::msg::String>(
            "/json_modified", 10,
            [this](const std_msgs::msg::String::SharedPtr msg) {
                if (msg->data == "true") {
                    RCLCPP_INFO(this->get_logger(), "✅ 收到 JSON 修改通知！");
                    json_ready_ = true;
                }
            }
        );

        chat_decision_sub_ = this->create_subscription<std_msgs::msg::String>(
            "/chat_decision", 10,
            [this](const std_msgs::msg::String::SharedPtr msg) {
                chat_decision_ = msg->data;
                RCLCPP_INFO(this->get_logger(), "收到聊天決策回覆：%s", chat_decision_.c_str());
            }
        );
        
        nav_client_ = rclcpp_action::create_client<NavigateToPose>(this, "/navigate_to_pose");
        shutdown_pub_ = this->create_publisher<std_msgs::msg::String>("shutdown_signal", 10);
        write_status_to_json(); // 初始化狀態 JSON

        // 移動手臂到導航位置
        std::string cmd = "/bin/bash -c 'source /home/jason9308/robot_ws/install/setup.bash && "
                      "ros2 run move_arm move_arm_node 189 -0.3 316.4 2.965 -1.438 0.173 &'";

        std::system(cmd.c_str());

        main_loop();
    }

private:
    struct Task {
        std::string command;
        std::string target;
        std::string time;
    };

    // 用於實現任務排程
    std::vector<Task> scheduled_tasks_;     // 排程中的送藥任務（已排序）
    std::vector<Task> executable_tasks_;    // 可立即執行的非送藥任務

    // 用於儲存病人資訊
    std::unordered_map<std::string, std::tuple<float, float, float, float>> location_map_; // x, y, z, w
    std::unordered_map<std::string, std::unordered_map<std::string, int>> medicine_map_;
    std::map<int, std::array<int, 3>> pillbox_to_medicine_;
    std::map<int, std::array<float, 4>> room_to_location_;

    // 用於檢查json是否已經被網頁更改
    bool json_ready_ = false;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr json_sub_;
    rclcpp_action::Client<NavigateToPose>::SharedPtr nav_client_;

    // 用於狀態更新
    std::mutex status_mutex_;
    std::string status_ = "idle";

    // 用於關閉 json_file_watcher 和 speech_recognition
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr shutdown_pub_;
    int shutdown_signal_ = 0;
    
    // 用於新增病人資訊
    std::vector<nlohmann::json> patient_cache_;

    // 用於獲取是否要聊天
    std::string chat_decision_ = "undecided";
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr chat_decision_sub_;



    void main_loop() {

        if(shutdown_signal_ == 1)
            return;

        while (rclcpp::ok()) {
            
            if (shutdown_signal_ == 1) {
                break;
            }

            RCLCPP_INFO(this->get_logger(), "目前排程中的送藥任務：");
            for (const auto& task : scheduled_tasks_) {
                RCLCPP_INFO(this->get_logger(), "- Target: %s | Time: %s", task.target.c_str(), task.time.c_str());
            }

            executable_tasks_.clear();  // 每輪重設非送藥任務
            load_json("/home/jason9308/robot_ws/command_jason/origin.json");
            write_status_to_json();
            if (scheduled_tasks_.empty()) {
                // 沒有送藥任務，執行其他任務
                for (const auto& task : executable_tasks_) {
                    execute_command(task.command, task.target, task.time);
                }
    
            } else if (enough_time_before_next_medicine(5)) {
                // 如果離送藥還有 5 分鐘以上，就先執行其他任務
                for (const auto& task : executable_tasks_) {
                    execute_command(task.command, task.target, task.time);
                }
    
            } else {

                const Task& next_task = scheduled_tasks_.front();
                // 如果時間超過了 就直接執行
                if(is_past_time(next_task.time)) {
                    RCLCPP_INFO(this->get_logger(), "最近送藥任務 %s 的時間 %s 已過，立即執行...", next_task.target.c_str(), next_task.time.c_str());
                    // send_medicine_flow(next_task.target, next_task.time);
                    execute_command("send medicine", next_task.target, next_task.time);

                    scheduled_tasks_.erase(scheduled_tasks_.begin());  // 執行後移除
                    write_status_to_json(); // 更新刪除任務後的排程!!

                    continue;  // 直接跳到下一輪迴圈
                }


                // 時間快到了 → 等到時間到再執行第一個送藥任務
                RCLCPP_INFO(this->get_logger(), "距離最近送藥任務 %s 的時間 %s 過近，等待中...", next_task.target.c_str(), next_task.time.c_str());
                
                status_ = "waiting to deliver medicine";
                write_status_to_json();
                while (!check_time(next_task.time)) {
                    RCLCPP_INFO(this->get_logger(), "等待送藥時間 %s 到達...", next_task.time.c_str());
                    std::this_thread::sleep_for(std::chrono::seconds(10));
                }
    
                // send_medicine_flow(next_task.target, next_task.time);
                execute_command("send medicine", next_task.target, next_task.time);

                scheduled_tasks_.erase(scheduled_tasks_.begin());  // 執行後移除
                write_status_to_json(); // 更新刪除任務後的排程!!
            }
    
            // std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
    
    

    void load_json(const std::string &path) {

        // open json_file_watcher to modify
        std::string prefix = "/bin/bash -c 'source /home/jason9308/robot_ws/install/setup.bash && ";
        std::string cmd;
        cmd = prefix + "ros2 run json_file_watcher json_file_watcher' &";
        std::system(cmd.c_str());

        while (!json_ready_) {
            RCLCPP_INFO(this->get_logger(), "JSON 尚未準備好，等待中...");
            rclcpp::spin_some(this->get_node_base_interface());  // 處理 callback
            std::this_thread::sleep_for(std::chrono::seconds(2));

            if (!scheduled_tasks_.empty() && !enough_time_before_next_medicine(5)) {
                RCLCPP_WARN(this->get_logger(), "⚠️ 距離送藥時間太近，中止等待語音輸入！");
                
                // // 終止語音辨識
                // std::system("pkill -f speech_recognition");
                // std::this_thread::sleep_for(std::chrono::seconds(1));
                // // 終止 json_file_watcher
                // std::system("pkill -f json_file_watcher");

                // 發送關閉訊號
                std_msgs::msg::String shutdown_msg;
                shutdown_msg.data = "shutdown";
                shutdown_pub_->publish(shutdown_msg);
                RCLCPP_INFO(this->get_logger(), "發送關閉訊號給 json_file_watcher and speech_recognition");

                return;  // 直接 return，主 loop 會去處理送藥
            }
        }
        json_ready_ = false;

        RCLCPP_INFO(this->get_logger(), "開始讀取 JSON 檔案...");
        rclcpp::sleep_for(std::chrono::seconds(1));

        // 讀取 JSON 檔案
        std::ifstream file(path);
        if (!file.is_open()) {
            RCLCPP_ERROR(this->get_logger(), "無法開啟 JSON 檔案");
            return;
        }
    
        json j;
        file >> j;
    
        for (const auto& item : j) {
            Task task = {
                item["command"],
                item.value("target", ""),
                item["time"]
            };
    
            if (task.command == "send medicine") {
                if (is_past_time(task.time)) {
                    RCLCPP_WARN(this->get_logger(), "時間 %s 已過，略過送藥任務 %s", task.time.c_str(), task.target.c_str());
                    continue;
                } 
    
                // 插入排序（時間由小到大）
                auto insert_pos = std::find_if(scheduled_tasks_.begin(), scheduled_tasks_.end(),
                    [&](const Task& t) {
                        return compare_time_str(task.time, t.time);
                    });
                scheduled_tasks_.insert(insert_pos, task);
    
                RCLCPP_INFO(this->get_logger(), "新增送藥任務：%s %s", task.target.c_str(), task.time.c_str());
            } else {
                executable_tasks_.push_back(task);
            }
        }
    }

    void write_status_to_json() {
        std::lock_guard<std::mutex> lock(status_mutex_);
    
        const std::string json_path = "/home/jason9308/robot_ws/command_jason/status.json";
    
        // 嘗試開啟檔案並持續鎖定直到成功
        int fd = -1;
        while (rclcpp::ok()) {
            fd = open(json_path.c_str(), O_WRONLY | O_CREAT, 0644);
            if (fd == -1) {
                RCLCPP_WARN(this->get_logger(), "⚠️ 無法開啟 JSON 檔案，稍後再試...");
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                continue;
            }
    
            if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
                RCLCPP_INFO(this->get_logger(), "🕐 JSON 檔案鎖定中，等待釋放...");
                close(fd);
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                continue;
            }
    
            break;  // 鎖成功就離開 retry loop
        }
    
        // 準備寫入 JSON
        json j;
        j["current_task"]["status"] = status_;
        j["scheduled_tasks"] = json::array();
        for (const auto& task : scheduled_tasks_) {
            j["scheduled_tasks"].push_back({
                {"target", task.target},
                {"time", task.time}
            });
        }
    
        {
            std::ofstream file(json_path);
            if (file.is_open()) {
                file << j.dump(4);
                file.close();
                RCLCPP_INFO(this->get_logger(), "✅ 狀態 JSON 已更新");
            } else {
                RCLCPP_WARN(this->get_logger(), "⚠️ 無法寫入 JSON 檔案！");
            }
        }
    
        // 解鎖並關閉
        flock(fd, LOCK_UN);
        close(fd);
    }


    void init_medicine_mapping() {
        pillbox_to_medicine_ = {
            {1, {1, 5, 9}},
            {2, {2, 6, 10}},
            {3, {3, 7, 11}},
            {4, {4, 8, 12}}
        };
    }
    
    void init_room_mapping() {
        room_to_location_ = {
            {520, {2.998f, 2.708f, 0.331f, 0.944f}},
            {521, {11.902f, 5.695f, 0.209f, 0.978f}}
        };
    }


    void load_all_patients(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open()) {
            RCLCPP_ERROR(this->get_logger(), "Failed to open patient JSON file: %s", filename.c_str());
            return;
        }

        json j;
        try {
            file >> j;
        } catch (const std::exception& e) {
            RCLCPP_ERROR(this->get_logger(), "Failed to parse JSON: %s", e.what());
            return;
        }

        patient_cache_ = j["patient"].get<std::vector<json>>();

        for (const auto& patient : patient_cache_) {
            std::string name = patient["name"];
            int pillbox = std::stoi(patient["pillBox"].get<std::string>());
            int room = std::stoi(patient["room"].get<std::string>());

            if (pillbox_to_medicine_.count(pillbox)) {
                auto meds = pillbox_to_medicine_[pillbox];
                medicine_map_[name]["morning"] = meds[0];
                medicine_map_[name]["noon"] = meds[1];
                medicine_map_[name]["night"] = meds[2];
            } else {
                RCLCPP_WARN(this->get_logger(), "Unknown pillbox: %d for patient %s", pillbox, name.c_str());
            }

            if (room_to_location_.count(room)) {
                auto arr = room_to_location_[room];
                location_map_[name] = std::make_tuple(arr[0], arr[1], arr[2], arr[3]);

            } else {
                RCLCPP_WARN(this->get_logger(), "Unknown room: %d for patient %s", room, name.c_str());
            }
        }

        RCLCPP_INFO(this->get_logger(), "Loaded %lu patients from JSON", patient_cache_.size());
    }

    void load_new_patient(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open()) {
            RCLCPP_ERROR(this->get_logger(), "Cannot open file: %s", filename.c_str());
            return;
        }

        json j;
        try {
            file >> j;
        } catch (const std::exception& e) {
            RCLCPP_ERROR(this->get_logger(), "Failed to parse JSON: %s", e.what());
            return;
        }

        auto new_cache = j["patient"].get<std::vector<json>>();
        const auto& patient = new_cache.back();  // 最後一筆
        patient_cache_ = new_cache;

        std::string name = patient["name"];
        int pillbox = std::stoi(patient["pillBox"].get<std::string>());
        int room = patient["room"];

        if (pillbox_to_medicine_.count(pillbox)) {
            auto meds = pillbox_to_medicine_[pillbox];
            medicine_map_[name]["morning"] = meds[0];
            medicine_map_[name]["noon"] = meds[1];
            medicine_map_[name]["night"] = meds[2];
        } else {
            RCLCPP_WARN(this->get_logger(), "Unknown pillbox: %d for patient %s", pillbox, name.c_str());
        }

        if (room_to_location_.count(room)) {
            auto loc = room_to_location_[room];
            location_map_[name] = std::make_tuple(loc[0], loc[1], loc[2], loc[3]);
        } else {
            RCLCPP_WARN(this->get_logger(), "Unknown room: %d for patient %s", room, name.c_str());
        }

        RCLCPP_INFO(this->get_logger(), "New patient loaded: %s", name.c_str());
    }

    void print_all_patients() {
        RCLCPP_INFO(this->get_logger(), "=== 所有病人資訊如下 ===");
        for (const auto& [name, time_map] : medicine_map_) {
            RCLCPP_INFO(this->get_logger(), "病人：%s", name.c_str());
            for (const auto& [period, pill] : time_map) {
                RCLCPP_INFO(this->get_logger(), "  - %s: 藥包 #%d", period.c_str(), pill);
            }
    
            if (location_map_.count(name)) {
                auto [x, y, z, w] = location_map_[name];
                RCLCPP_INFO(this->get_logger(), "  - 位置 : x=%.2f, y=%.2f, z=%.2f, w=%.2f", x, y, z, w);
            } else {
                RCLCPP_WARN(this->get_logger(), "  - 無對應座標！");
            }
        }
    }
    

    std::string get_time_period_from_string(const std::string &time_str) {
        int hour = std::stoi(time_str.substr(0, 2));
        if (hour >= 5 && hour < 11) return "morning";
        if (hour >= 11 && hour < 17) return "noon";
        return "night";
    }

    bool check_time(const std::string &task_time) {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm *ltm = std::localtime(&now_c);
        char buffer[6];
        std::strftime(buffer, sizeof(buffer), "%H:%M", ltm);
        return task_time == std::string(buffer);
    }

    bool enough_time_before_next_medicine(int buffer_minutes) {
        if (scheduled_tasks_.empty()) return true;
    
        std::string next_time = scheduled_tasks_.front().time; // 因為有排序，取第一個就好
    
        int task_hour = std::stoi(next_time.substr(0, 2));
        int task_min = std::stoi(next_time.substr(3, 2));
        int task_total = task_hour * 60 + task_min;
    
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm *ltm = std::localtime(&now_c);
        int now_total = ltm->tm_hour * 60 + ltm->tm_min;
    
        return (task_total - now_total) > buffer_minutes;
    }

    
    bool is_past_time(const std::string& time_str) {
        int hour = std::stoi(time_str.substr(0, 2));
        int min = std::stoi(time_str.substr(3, 2));
        int total = hour * 60 + min;
    
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm *ltm = std::localtime(&now_c);
        int now_total = ltm->tm_hour * 60 + ltm->tm_min;
    
        return total < now_total;
    }

    bool compare_time_str(const std::string& t1, const std::string& t2) {
        int h1 = std::stoi(t1.substr(0, 2));
        int m1 = std::stoi(t1.substr(3, 2));
        int h2 = std::stoi(t2.substr(0, 2));
        int m2 = std::stoi(t2.substr(3, 2));
    
        return (h1 < h2) || (h1 == h2 && m1 < m2);
    }
    
    void play_audio(const std::string& audio_path) {
        std::string cmd = "mpg123 " + audio_path;
        int ret = std::system(cmd.c_str());
        if (ret != 0) {
            RCLCPP_WARN(this->get_logger(), "語音播放失敗！");
        }
    }

    void send_goal_and_wait(double x_, double y_, double z_, double w_) {
        auto goal_msg = NavigateToPose::Goal();
        goal_msg.pose.header.frame_id = "map";
        goal_msg.pose.header.stamp = this->now();
        goal_msg.pose.pose.position.x = x_;
        goal_msg.pose.pose.position.y = y_;
        goal_msg.pose.pose.orientation.z = z_;
        goal_msg.pose.pose.orientation.w = w_;

        RCLCPP_INFO(this->get_logger(), "📍 發送導航目標: x=%.2f, y=%.2f, z=%.2f, w=%.2f", x_, y_, z_, w_);
        
        // just test, not send goal
        // return;

        auto send_goal_future = nav_client_->async_send_goal(goal_msg);
        if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), send_goal_future)
            != rclcpp::FutureReturnCode::SUCCESS) {
            RCLCPP_ERROR(this->get_logger(), "❌ 送出導航目標失敗");
            return;
        }

        auto goal_handle = send_goal_future.get();
        if (!goal_handle) {
            RCLCPP_ERROR(this->get_logger(), "❌ 導航目標被拒絕");
            return;
        }

        auto result_future = nav_client_->async_get_result(goal_handle);
        if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), result_future)
            != rclcpp::FutureReturnCode::SUCCESS) {
            RCLCPP_ERROR(this->get_logger(), "❌ 等待導航結果失敗");
            return;
        }

        auto result = result_future.get();
        if (result.code == rclcpp_action::ResultCode::SUCCEEDED) {
            RCLCPP_INFO(this->get_logger(), "✅ 導航成功！");
        } else {
            RCLCPP_WARN(this->get_logger(), "⚠️ 導航失敗，狀態碼：%d", static_cast<int>(result.code));
        }
    }

    void execute_command(const std::string &command, const std::string &target, const std::string &time) {
        if (command == "send medicine") {
            // send_medicine_flow(target, time);
            send_medicine_full_flow(target, time);
        } else if (command == "go home") {
            go_home_flow();
        } else if (command == "chat") {
            chat_flow(target);
        } else if (command == "video call") {
            video_call_flow(target);
        } else if(command == "shutdown") {
            std_msgs::msg::String shutdown_msg;
            shutdown_msg.data = "shutdown";
            shutdown_pub_->publish(shutdown_msg);
            RCLCPP_INFO(this->get_logger(), "發送關閉訊號");
            std::this_thread::sleep_for(std::chrono::seconds(3));
            shutdown_signal_ = 1;
            RCLCPP_INFO(this->get_logger(), "關閉系統");

        } else if (command == "new patient") {
            load_new_patient("/home/jason9308/robot_ws/command_jason/new_patient.json");
            RCLCPP_INFO(this->get_logger(), "新增病人資料");
        } else {
            RCLCPP_WARN(this->get_logger(), "未知的指令：%s", command.c_str());
        }
    }

    void send_medicine_flow(const std::string &target, const std::string &time) {

        std::string period = get_time_period_from_string(time);
        int med_id = medicine_map_[target][period];
        RCLCPP_INFO(this->get_logger(), "任務時間為 %s,判定時段為 %s,%s 的藥包編號為 %d", time.c_str(), period.c_str(), target.c_str(), med_id);

        auto [x, y, z, w] = location_map_[target];
        RCLCPP_INFO(this->get_logger(), "導航至 %s 的座標位置 (%.2f, %.2f, %.2f, %.2f)", target.c_str(), x, y, z, w);
        
        status_ = "delivering medicine(moving to destination)";
        write_status_to_json();
        transition("navigation", target);

        status_ = "delivering medicine(face recognition)";
        write_status_to_json();
        transition("face_recognition", target);

        status_ = "delivering medicine(handing over the medication)";
        write_status_to_json();
        transition("arm_control", std::to_string(med_id));

        status_ = "idle";
        write_status_to_json();
    }

    void send_medicine_full_flow(const std::string &target, const std::string &time) {

        // 送藥
        std::string period = get_time_period_from_string(time);
        int med_id = medicine_map_[target][period];
        RCLCPP_INFO(this->get_logger(), "任務時間為 %s,判定時段為 %s,%s 的藥包編號為 %d", time.c_str(), period.c_str(), target.c_str(), med_id);

        auto [x, y, z, w] = location_map_[target];
        RCLCPP_INFO(this->get_logger(), "導航至 %s 的座標位置 (%.2f, %.2f, %.2f, %.2f)", target.c_str(), x, y, z, w);
        
        status_ = "delivering medicine(moving to destination)";
        write_status_to_json();
        transition("navigation", target);

        status_ = "delivering medicine(face recognition)";
        write_status_to_json();
        transition("face_recognition", target);

        std::string prefix = "/bin/bash -c 'source ~/robot_ws/install/setup.bash && ";
        std::string cmd1 = prefix + "ros2 service call /ufactory/motion_enable xarm_msgs/srv/SetInt16ById \"{id: 8, data: 1}\"'";
        std::string cmd2 = prefix + "ros2 service call /ufactory/set_mode xarm_msgs/srv/SetInt16 \"{data: 0}\"'";
        std::string cmd3 = prefix + "ros2 service call /ufactory/set_state xarm_msgs/srv/SetInt16 \"{data: 0}\"'";

        std::system(cmd1.c_str());
        std::system(cmd2.c_str());
        std::system(cmd3.c_str());

        // 加個延遲，確保 arm 完全 idle
        std::this_thread::sleep_for(std::chrono::milliseconds(800));

        status_ = "delivering medicine(handing over the medication)";
        write_status_to_json();
        transition("arm_control", std::to_string(med_id));

        // 視訊電話
        status_ = "video calling";
        write_status_to_json();
        transition("video_call", target);

        // 聊天(optional)
        status_ = "chatting";
        write_status_to_json();
        
        chat_decision_ = "undecided";

        // std::string prefix = "/bin/bash -c 'source /home/jason9308/robot_ws/install/setup.bash && ";
        std::string cmd;
        cmd = prefix + "ros2 run speech_recognition_pkg chat_decision' &";
        std::system(cmd.c_str());

        while (chat_decision_ == "undecided") {
            RCLCPP_INFO(this->get_logger(), "等待聊天決策...");
            rclcpp::spin_some(this->get_node_base_interface());
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        if(chat_decision_ == "yes")
        {
            transition("chatbot", target);
        }

        // auto [x, y, z, w] = location_map_["home"];
        // RCLCPP_INFO(this->get_logger(), "回家導航至座標位置 (%.2f, %.2f, %.2f, %.2f)", x, y, z, w);
        
        go_home_flow();

        status_ = "idle";
        write_status_to_json();

    }

    void go_home_flow() {
        status_ = "going home";
        write_status_to_json();
        auto [x, y, z, w] = location_map_["home"];
        RCLCPP_INFO(this->get_logger(), "回家導航至座標位置 (%.2f, %.2f, %.2f, %.2f)", x, y, z, w);
        transition("navigation", "home");
        status_ = "idle";
        write_status_to_json();
    }

    void chat_flow(const std::string &target) {
        // transition("navigation", target);
        // transition("face_recognition", target);
        status_ = "chatting";
        write_status_to_json();
        transition("chatbot", target);
        status_ = "idle";
        write_status_to_json();
    }

    void video_call_flow(const std::string &target) {
        // transition("navigation", target);
        // transition("face_recognition", target);
        status_ = "video calling";
        write_status_to_json();
        transition("video_call", target);
        status_ = "idle";
        write_status_to_json();
    }

    void transition(const std::string &state, const std::string &target) {
        RCLCPP_INFO(this->get_logger(), "Transitioning to %s for %s", state.c_str(), target.c_str());
        std::string prefix = "/bin/bash -c 'source /home/jason9308/robot_ws/install/setup.bash && ";
        std::string cmd;

        if (state == "navigation") 
        {

            // cmd = prefix + "ros2 run move_arm move_arm_node 189 -0.3 316.4 2.965 -1.438 0.173'";
            // std::system(cmd.c_str());

            auto [x, y, z, w] = location_map_[target];
            send_goal_and_wait(x, y, z, w);
            return;
            // cmd = prefix + "ros2 run navigation_server navigate_node " +
            //       std::to_string(x) + " " + std::to_string(y) + " " +
            //       std::to_string(z) + " " + std::to_string(w) + "'";
            
            // use send_goal_and_wait instead of system call
            
        } 
        
        else if (state == "face_recognition") 
        {
            // 執行 face_recognition node
            cmd = prefix + "ros2 run face_recognition face_recognition " + target + "'";
            // cmd = prefix + "ros2 run face_recognition face_recognition " + target + "' && ";

            // 執行 heart rate 預測，透過 conda activate + cd 到 rPPG-Toolbox 執行
            // cmd += "source ~/miniconda3/etc/profile.d/conda.sh && ";  // conda 初始化
            // cmd += "conda activate rppg-toolbox && ";                         // 啟動你的 conda 環境
            // cmd += "cd /home/jason9308/rPPG-Toolbox && ";             // 切換到程式資料夾
            // cmd += "python3 predict_chrom.py --video /home/jason9308/robot_ws/heart_rate/video/" + target + ".avi'";
        }

        
        else if (state == "arm_control") 
        {
            
            cmd = prefix + "ros2 run medicine_grabber medicine_grabber_node " + target + "'";
        } 
        
        else if (state == "chatbot") 
        {
            // cmd = prefix + "ros2 run chatbot chatbot_node'";
            cmd = prefix + "ros2 run chatbot chatbot_node 2> >(grep -v ALSA >&2)'";
        } 
        
        else if (state == "video_call") 
        {
            // 1. 關閉 Realsense Driver（安全 SIGINT）
            std::system("pkill -2 -f rs_launch.py");
            std::this_thread::sleep_for(std::chrono::seconds(3));

            // 2. 
            RCLCPP_INFO(this->get_logger(), "移動手臂至視訊位置");
            cmd = prefix + "ros2 run move_arm move_arm_node 401.1 -2.7 509.2 3.0364 -1.3345 0.0646'";
            std::system(cmd.c_str());
            // std::this_thread::sleep_for(std::chrono::seconds(1));

            // 3. 執行 videocall 並「阻塞等待」Python 程式結束
            std::string call_cmd = "cd ~/robot_ws && python3 linebot_meet/app2.py";
            int result = std::system(call_cmd.c_str());

            if (result != 0) {
                RCLCPP_WARN(this->get_logger(), "Video call 程式異常結束, return code = %d", result);
            }
            
            // 4. video call 結束後，重啟 Realsense Driver
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::string relaunch_cmd = 
            "gnome-terminal --title=\"Realsense Driver\" -- bash -c '"
            "sleep 0.1; "
            "xdotool search --name \"Realsense Driver\" windowminimize; "
            "source ~/robot_ws/install/setup.bash && "
            "ros2 launch realsense2_camera rs_launch.py"
            "'";
            std::system(relaunch_cmd.c_str());

            // 5.
            RCLCPP_INFO(this->get_logger(), "移動手臂至預設位置");
            // cmd = prefix + "ros2 run move_arm move_arm_node 200 0 250 3.14 0 0'";
            cmd = prefix + "ros2 run move_arm move_arm_node 189 -0.3 316.4 2.965 -1.438 0.173'";
            std::system(cmd.c_str());
            // std::this_thread::sleep_for(std::chrono::seconds(1));

            return; // 直接 return，因為這個流程不需要再執行其他命令
            
        } 
        
        else 
        {
            RCLCPP_WARN(this->get_logger(), "未知狀態：%s", state.c_str());
            return;
        }

        // Comment out this line to test without executing the command
        RCLCPP_INFO(this->get_logger(), "Executing command: %s", cmd.c_str());
        std::system(cmd.c_str());
        // rclcpp::sleep_for(std::chrono::seconds(3));

    }

};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ControlNode>();
    // rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

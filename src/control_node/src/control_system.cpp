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
        init_location_map();
        init_medicine_map();

        json_sub_ = this->create_subscription<std_msgs::msg::String>(
            "/json_modified", 10,
            [this](const std_msgs::msg::String::SharedPtr msg) {
                if (msg->data == "true") {
                    RCLCPP_INFO(this->get_logger(), "✅ 收到 JSON 修改通知！");
                    json_ready_ = true;
                }
            }
        );
        nav_client_ = rclcpp_action::create_client<NavigateToPose>(this, "/navigate_to_pose");
        write_status_to_json(); // 初始化狀態 JSON
        main_loop();
    }

private:
    struct Task {
        std::string command;
        std::string target;
        std::string time;
    };

    // std::vector<Task> command_list_;
    std::vector<Task> scheduled_tasks_;     // 排程中的送藥任務（已排序）
    std::vector<Task> executable_tasks_;    // 可立即執行的非送藥任務

    std::unordered_map<std::string, std::tuple<float, float, float, float>> location_map_; // x, y, z, w
    std::unordered_map<std::string, std::unordered_map<std::string, int>> medicine_map_;

    bool json_ready_ = false;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr json_sub_;
    rclcpp_action::Client<NavigateToPose>::SharedPtr nav_client_;

    std::mutex status_mutex_;
    std::string status_ = "idle";

    void main_loop() {
        while (rclcpp::ok()) {  

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
                // 時間快到了 → 等到時間到再執行第一個送藥任務
                const Task& next_task = scheduled_tasks_.front();
                RCLCPP_INFO(this->get_logger(), "距離最近送藥任務 %s 的時間 %s 過近，等待中...", next_task.target.c_str(), next_task.time.c_str());
                
                status_ = "waiting to deliver medicine";
                write_status_to_json();
                while (!check_time(next_task.time)) {
                    RCLCPP_INFO(this->get_logger(), "等待送藥時間 %s 到達...", next_task.time.c_str());
                    std::this_thread::sleep_for(std::chrono::seconds(10));
                }
    
                send_medicine_flow(next_task.target, next_task.time);
                scheduled_tasks_.erase(scheduled_tasks_.begin());  // 執行後移除
            }
    
            std::this_thread::sleep_for(std::chrono::seconds(5));
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
            std::this_thread::sleep_for(std::chrono::seconds(3));

            if (!scheduled_tasks_.empty() && !enough_time_before_next_medicine(5)) {
                RCLCPP_WARN(this->get_logger(), "⚠️ 距離送藥時間太近，中止等待語音輸入！");
                
                // 終止語音辨識
                std::system("pkill -f speech_recognition");
                std::this_thread::sleep_for(std::chrono::seconds(1));
                // 終止 json_file_watcher
                std::system("pkill -f json_file_watcher");
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
    
        const std::string json_path = "/home/jason9308/robot_ws/status.json";
    
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
    
    

    void init_location_map() {
        location_map_["dad"] = {1.0, 2.0, 0.0, 1.0};
        location_map_["mom"] = {2.0, 3.0, 0.0, 1.0};
        location_map_["grandpa"] = {4.0, 5.0, 0.0, 1.0};
        location_map_["grandma"] = {5.0, 6.0, 0.0, 1.0};
        location_map_["home"] = {0.0, 0.0, 0.0, 1.0};
    }

    void init_medicine_map() {
        medicine_map_["dad"] = {{"morning", 1}, {"noon", 2}, {"night", 3}};
        medicine_map_["mom"] = {{"morning", 4}, {"noon", 5}, {"night", 6}};
        medicine_map_["grandma"] = {{"morning", 7}, {"noon", 8}, {"night", 9}};
        medicine_map_["grandpa"] = {{"morning", 10}, {"noon", 11}, {"night", 12}};
        
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
            send_medicine_flow(target, time);
        } else if (command == "go home") {
            go_home_flow();
        } else if (command == "chat") {
            chat_flow(target);
        } else if (command == "video call") {
            video_call_flow(target);
        } else {
            RCLCPP_WARN(this->get_logger(), "未知的指令：%s", command.c_str());
        }
    }

    void send_medicine_flow(const std::string &target, const std::string &time) {

        status_ = "delivering medicine";
        write_status_to_json();
        while (!check_time(time)) {
            RCLCPP_INFO(this->get_logger(), "等待送藥時間 %s 到達...", time.c_str());
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }

        std::string period = get_time_period_from_string(time);
        int med_id = medicine_map_[target][period];
        RCLCPP_INFO(this->get_logger(), "任務時間為 %s,判定時段為 %s,%s 的藥包編號為 %d", time.c_str(), period.c_str(), target.c_str(), med_id);

        auto [x, y, z, w] = location_map_[target];
        RCLCPP_INFO(this->get_logger(), "導航至 %s 的座標位置 (%.2f, %.2f, %.2f, %.2f)", target.c_str(), x, y, z, w);

        transition("navigation", target);
        transition("face_recognition", target);
        transition("arm_control", std::to_string(med_id));

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
            auto [x, y, z, w] = location_map_[target];
            // cmd = prefix + "ros2 run navigation_server navigate_node " +
            //       std::to_string(x) + " " + std::to_string(y) + " " +
            //       std::to_string(z) + " " + std::to_string(w) + "'";
            
            // use send_goal_and_wait instead of system call
            send_goal_and_wait(x, y, z, w);
            return;
        } 
        
        // else if (state == "face_recognition") 
        // {
        //     cmd = prefix + "ros2 run face_recognition face_recognition " + target + "'";
        // }
        
        else if (state == "face_recognition") 
        {
            // 執行 face_recognition node
            cmd = prefix + "ros2 run face_recognition face_recognition " + target + "' && ";

            // 執行 heart rate 預測，透過 conda activate + cd 到 rPPG-Toolbox 執行
            cmd += "source ~/miniconda3/etc/profile.d/conda.sh && ";  // conda 初始化
            cmd += "conda activate rppg-toolbox && ";                         // 啟動你的 conda 環境
            cmd += "cd /home/jason9308/rPPG-Toolbox && ";             // 切換到程式資料夾
            cmd += "python3 predict_chrom.py --video /home/jason9308/robot_ws/heart_rate/video/" + target + ".avi'";
        }

        
        else if (state == "arm_control") 
        {
            cmd = prefix + "ros2 run aruco_grab boxhole_grab_node " + target + "'";
        } 
        
        else if (state == "chatbot") 
        {
            cmd = prefix + "ros2 run chatbot chatbot_node'";
        } 
        
        else if (state == "video_call") 
        {
            cmd = prefix + "ros2 run videocall_pkg call_node'";
        } 
        
        else 
        {
            RCLCPP_WARN(this->get_logger(), "未知狀態：%s", state.c_str());
            return;
        }

        // Comment out this line to test without executing the command
        // std::system(cmd.c_str());
        RCLCPP_INFO(this->get_logger(), "Executing command: %s", cmd.c_str());
        rclcpp::sleep_for(std::chrono::seconds(3));

    }

};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ControlNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

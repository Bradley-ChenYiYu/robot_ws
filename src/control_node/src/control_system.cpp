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

using json = nlohmann::json;

class ControlNode : public rclcpp::Node {
public:
    ControlNode() : Node("control_node") {
        init_location_map();
        init_medicine_map();
        main_loop();
    }

private:
    struct Task {
        std::string command;
        std::string target;
        std::string time;
    };

    std::vector<Task> command_list_;
    std::unordered_map<std::string, std::tuple<float, float, float, float>> location_map_; // x, y, z, w
    std::unordered_map<std::string, std::unordered_map<std::string, int>> medicine_map_;

    void main_loop() {
        while (rclcpp::ok()) {
            command_list_.clear();
            load_json("/home/jason9308/robot_ws/src/control_node/test.json");

            for (const auto& task : command_list_) {
                execute_command(task.command, task.target, task.time);
            }

            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }

    void load_json(const std::string &path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            RCLCPP_ERROR(this->get_logger(), "無法開啟 JSON 檔案");
            return;
        }
        json j;
        file >> j;

        for (const auto& item : j) {
            command_list_.push_back({
                item["command"],
                item.value("target", ""),
                item["time"]
            });
        }
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
        medicine_map_["grandpa"] = {{"morning", 10}, {"noon", 11}, {"night", 12}};
        medicine_map_["grandma"] = {{"morning", 1}, {"noon", 2}, {"night", 3}};
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
    }

    void go_home_flow() {
        auto [x, y, z, w] = location_map_["home"];
        RCLCPP_INFO(this->get_logger(), "回家導航至座標位置 (%.2f, %.2f, %.2f, %.2f)", x, y, z, w);
        transition("navigation", "home");
    }

    void chat_flow(const std::string &target) {
        // transition("navigation", target);
        // transition("face_recognition", target);
        transition("chatbot", target);
    }

    void video_call_flow(const std::string &target) {
        // transition("navigation", target);
        // transition("face_recognition", target);
        transition("video_call", target);
    }

    void transition(const std::string &state, const std::string &target) {
        RCLCPP_INFO(this->get_logger(), "Transitioning to %s for %s", state.c_str(), target.c_str());
        std::string prefix = "/bin/bash -c 'source /home/jason9308/robot_ws/install/setup.bash && ";
        std::string cmd;

        if (state == "navigation") 
        {
            auto [x, y, z, w] = location_map_[target];
            cmd = prefix + "ros2 run navigation_server navigate_node " +
                  std::to_string(x) + " " + std::to_string(y) + " " +
                  std::to_string(z) + " " + std::to_string(w) + "'";
        } 
        
        else if (state == "face_recognition") 
        {
            cmd = prefix + "ros2 run face_recognition face_recognition " + target + "'";
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

        std::system(cmd.c_str());
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

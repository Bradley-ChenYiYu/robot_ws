#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <json/json.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <csignal>

namespace fs = std::filesystem;

const std::string output_path = "/home/jason9308/robot_ws/command_jason/output.json";
const std::string origin_path = "/home/jason9308/robot_ws/command_jason/origin.json";

bool compare_json_files(const std::string& path1, const std::string& path2) {
    std::ifstream f1(path1);
    std::ifstream f2(path2);
    if (!f1 || !f2) return false;

    Json::Value json1, json2;
    f1 >> json1;
    f2 >> json2;

    return json1 == json2;
}

bool copy_file(const std::string& from, const std::string& to) {
    try {
        fs::copy_file(from, to, fs::copy_options::overwrite_existing);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "複製檔案失敗: " << e.what() << std::endl;
        return false;
    }
}

bool try_lock_file(const std::string& path, int& fd) {
    fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) return false;
    if (flock(fd, LOCK_SH | LOCK_NB) == -1) {
        close(fd);
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("json_file_watcher_node");
    
    auto publisher = node->create_publisher<std_msgs::msg::String>("/json_modified", 10);
    
    auto shutdown_sub = node->create_subscription<std_msgs::msg::String>(
        "shutdown_signal",
        10,
        [node](const std_msgs::msg::String::SharedPtr msg) {
            if (msg->data == "shutdown") {
                RCLCPP_INFO(node->get_logger(), "🔴 收到 shutdown 訊號，準備關閉 json_file_watcher...");
                rclcpp::shutdown();
            }
        }
    );

    // open another terminal and run speech_recognition node
    // std::string command = "gnome-terminal -- bash -c 'sleep 0.5; wmctrl -r :ACTIVE: -e 0,0,0,960,1080; source /home/jason9308/robot_ws/install/setup.bash && ros2 run speech_recognition_pkg speech_recognition 2> >(grep -v ALSA >&2)'";
    std::string command = "gnome-terminal -- bash -c 'sleep 0.1; wmctrl -r :ACTIVE: -e 0,0,0,966,1108; source /home/jason9308/robot_ws/install/setup.bash && ros2 run speech_recognition_pkg speech_recognition 2> >(grep -v ALSA >&2)'";

    // std::string command = "/bin/bash -c 'source /home/jason9308/robot_ws/install/setup.bash && ros2 run speech_recognition_pkg speech_recognition 2> >(grep -v ALSA >&2)' &";
    // std::string command = "/bin/bash -c 'source /home/jason9308/robot_ws/install/setup.bash && ros2 run speech_recognition_pkg speech_recognition' &";
    int ret = std::system(command.c_str());
    if (ret == 0) {
        RCLCPP_INFO(node->get_logger(), "speech_recognition node 啟動成功");
        std::this_thread::sleep_for(std::chrono::seconds(3));
    } else {
        RCLCPP_WARN(node->get_logger(), "speech_recognition node 啟動失敗");
    }

    RCLCPP_INFO(node->get_logger(), "[JSON Watcher] 開始監控 JSON 檔案...");

    while (rclcpp::ok()) {

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        int fd;
        if (!try_lock_file(output_path, fd)) {
            RCLCPP_INFO(node->get_logger(), "[JSON Watcher] output.json 正在被寫入，稍後再試...");
            continue;
        }

        bool is_same = compare_json_files(output_path, origin_path);
        flock(fd, LOCK_UN);
        close(fd);

        if (!is_same) {
            RCLCPP_INFO(node->get_logger(), "[JSON Watcher] 偵測到 JSON 不一致，複製並發送 topic ...");

            // std::system("pkill -f 'speech_recognition'");  // 終止語音辨識
            if (copy_file(output_path, origin_path)) {
                std_msgs::msg::String msg;
                msg.data = "true";
                publisher->publish(msg);
                std::this_thread::sleep_for(std::chrono::seconds(2)); 
                break;
            }
        } else {
            RCLCPP_INFO(node->get_logger(), "[JSON Watcher] JSON 檔案一致，繼續監控...");
        }

        rclcpp::spin_some(node);
    }

    rclcpp::shutdown();
    return 0;
} 

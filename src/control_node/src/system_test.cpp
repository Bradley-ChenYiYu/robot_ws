#include <rclcpp/rclcpp.hpp>
#include <cstdlib>
#include <string>

class FaceRecognitionNode : public rclcpp::Node {
public:
    FaceRecognitionNode() : Node("system_test") {
        RCLCPP_INFO(this->get_logger(), "啟動 face_recognition 節點...");
        
        std::string command = "/bin/bash -c 'source /home/jason9308/robot_ws/install/setup.bash && ros2 run face_recognition face_recognition'";
        int ret = std::system(command.c_str());


        if (ret == 0) {
            RCLCPP_INFO(this->get_logger(), "face_recognition 節點成功啟動");
        } else {
            RCLCPP_ERROR(this->get_logger(), "face_recognition 節點啟動失敗，錯誤碼: %d", ret);
        }
    }
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<FaceRecognitionNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
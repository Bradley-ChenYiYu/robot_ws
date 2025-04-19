#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <tuple>
#include <unordered_map>
#include <memory>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <filesystem>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <csignal>
#include <nlohmann/json.hpp>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/image_encodings.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"
#include "cv_bridge/cv_bridge.h"
#include "xarm_msgs/msg/robot_msg.hpp"
#include "xarm_msgs/srv/move_cartesian.hpp"
#include "xarm_msgs/srv/call.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/aruco.hpp"
#include "eigen3/Eigen/Eigen"
#include "yaml-cpp/yaml.h"
#define _USE_MATH_DEFINES


namespace fs = std::filesystem;

class ControlROS : public rclcpp::Node {
public:
    using NavigateToPose = nav2_msgs::action::NavigateToPose;
    using GoalHandleNavigateToPose = rclcpp_action::ClientGoalHandle<NavigateToPose>;
    
    ControlROS(): Node("control_ros") {
        // Initialize your node here
        setupSubscriptions();
        setupClients();
        init_location_map();
        init_medicine_map();
        // 讀取手眼校正結果
        if (!loadHandEyeCalibration()) {
            RCLCPP_ERROR(this->get_logger(), "Failed to load hand-eye calibration data!");
            rclcpp::shutdown();
        }

        main_loop();
    }

private:

    // class variables and methods in control_system
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
    void init_location_map();
    void init_medicine_map();
    void main_loop();
    std::string get_time_period_from_string(const std::string &time_str);
    bool check_time(const std::string &task_time);
    bool enough_time_before_next_medicine(int buffer_minutes);
    bool is_past_time(const std::string& time_str);
    bool compare_time_str(const std::string& t1, const std::string& t2);
    void execute_command(const std::string &command, const std::string &target, const std::string &time);
    void send_medicine_flow(const std::string &target, const std::string &time);
    void go_home_flow();
    void chat_flow(const std::string &target);
    void video_call_flow(const std::string &target);
    void transition(const std::string &state, const std::string &target);
    bool json_ready_ = false;

    // class variables and methods in boxhole_grab
    // 訂閱 RGB & 深度影像
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr rgb_subscription_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr depth_subscription_;
    cv::Mat rgb_image_, depth_image_;

    // 訂閱機械手臂回饋
    rclcpp::Subscription<xarm_msgs::msg::RobotMsg>::SharedPtr xarm_feedback_subscription_;
    std::array<double, 6> xarm_current_pose_;

    // 手臂控制
    rclcpp::Client<xarm_msgs::srv::MoveCartesian>::SharedPtr xarm_move_cartesian_client_;

    // 夾爪控制
    rclcpp::Client<xarm_msgs::srv::Call>::SharedPtr gripper_open_client_;
    rclcpp::Client<xarm_msgs::srv::Call>::SharedPtr gripper_close_client_;
    rclcpp::Client<xarm_msgs::srv::Call>::SharedPtr gripper_stop_client_;

    // 手眼校正參數
    cv::Mat cameraMatrix_;
    cv::Mat distCoeffs_;
    cv::Mat handEyeRotation_;
    cv::Mat handEyeTranslation_;

    // 訂閱手部偵測結果
    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr hand_sub_;

    std::vector<std::vector<cv::Point2f>> detected_marker_corners_;  // 存放 ArUco 角點
    std::vector<int> detected_marker_ids_;  // 存放 ArUco ID

    cv::Mat computeEEtoBaseTransform(const std::array<double, 6>& pose);
    void setupSubscriptions();
    void setupClients();
    bool loadHandEyeCalibration();
    void imageCallback(const sensor_msgs::msg::Image::SharedPtr msg);
    void depthCallback(const sensor_msgs::msg::Image::SharedPtr msg);
    void xarmFeedbackCallback(const xarm_msgs::msg::RobotMsg::SharedPtr msg);
    bool moveXArmToPose(const std::array<double, 6>& pose);
    bool wait_for_xarm_arrival(const std::array<double, 6>& target_pose, double tolerance);
    bool callGripperService(rclcpp::Client<xarm_msgs::srv::Call>::SharedPtr client);
    void openGripper();
    void closeGripper();
    void stopGripper();
    void play_audio(const std::string& path);

    float hole_width = 82.0;
    float hole_height = 82.0;

    cv::Point3f last_hand_pos_;
    rclcpp::Time last_hand_time_;
    int hand_stable_counter_ = 0;
    bool hand_delivery_triggered_ = false;
    const double hand_stability_threshold_ = 20.0; // mm
    void handCallback(const std_msgs::msg::Float32MultiArray::SharedPtr msg);
    void performHandDelivery(const cv::Point3f& hand_pos);

    // class variables and methods in json_file_watcher
    const std::string output_path = "/home/jason9308/robot_ws/command_jason/output.json";
    const std::string origin_path = "/home/jason9308/robot_ws/command_jason/origin.json";
    bool compare_json_files(const std::string& path1, const std::string& path2);
    bool copy_file(const std::string& from, const std::string& to);
    bool try_lock_file(const std::string& path, int& fd);

    // class variables and methods in navigate_to_pose
    rclcpp_action::Client<NavigateToPose>::SharedPtr action_client_;
    double x_, y_, z_, w_;
    void send_goal();
    
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ControlROS>();
    rclcpp::executors::MultiThreadedExecutor exec;
    exec.add_node(node);
    exec.spin();
    rclcpp::shutdown();
    return 0;
}

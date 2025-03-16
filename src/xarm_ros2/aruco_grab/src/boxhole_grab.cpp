#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <chrono>
#include <fstream>
#include <filesystem>

#include "eigen3/Eigen/Eigen"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/aruco.hpp"
#include "yaml-cpp/yaml.h"

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "cv_bridge/cv_bridge.h"
#include "sensor_msgs/image_encodings.hpp"

#include "xarm_msgs/msg/robot_msg.hpp"
#include "xarm_msgs/srv/move_cartesian.hpp"
#include "xarm_msgs/srv/call.hpp"


class BoxHoleXArmControl : public rclcpp::Node {
public:
    BoxHoleXArmControl() : Node("boxhole_xarm_control") {
        setupSubscriptions();
        setupClients();

        RCLCPP_INFO(this->get_logger(), "ArUco XArm Control Node Started.");

        // 讀取手眼校正結果
        if (!loadHandEyeCalibration()) {
            RCLCPP_ERROR(this->get_logger(), "Failed to load hand-eye calibration data!");
            rclcpp::shutdown();
        }

        // 打開夾爪
        openGripper();
        rclcpp::sleep_for(std::chrono::seconds(1));
        std::array<double, 6> initial_pose = {200, -50, 400, 3.14, 0, 0};
        if (!moveXArmToPose(initial_pose)) {
            RCLCPP_ERROR(this->get_logger(), "Failed to move to initial position!");
        } 
        else 
        {
            if (wait_for_xarm_arrival(initial_pose, 1.0)) {
                RCLCPP_INFO(this->get_logger(), "Hand reached initial position.");
            } else {
                RCLCPP_ERROR(this->get_logger(), "Hand failed to reach initial position in time.");
            }    
        }
        //rclcpp::sleep_for(std::chrono::seconds(5));
    }

    void performGrasping();
    int id;

private:
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

    float hole_width = 82.0;
    float hole_height = 82.0;
};

// **訂閱相機影像 & 機械手臂回饋**
void BoxHoleXArmControl::setupSubscriptions() {
    // 訂閱 RGB 影像
    rgb_subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
        "/camera/camera/color/image_raw", 10,
        std::bind(&BoxHoleXArmControl::imageCallback, this, std::placeholders::_1));

    // 訂閱深度影像
    depth_subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
        "/camera/camera/aligned_depth_to_color/image_raw", 10,
        std::bind(&BoxHoleXArmControl::depthCallback, this, std::placeholders::_1));

    // 訂閱機械手臂狀態回饋
    xarm_feedback_subscription_ = this->create_subscription<xarm_msgs::msg::RobotMsg>(
        "/ufactory/robot_states", 10,
        std::bind(&BoxHoleXArmControl::xarmFeedbackCallback, this, std::placeholders::_1));
}

// **初始化手臂控制**
void BoxHoleXArmControl::setupClients() {
    xarm_move_cartesian_client_ = this->create_client<xarm_msgs::srv::MoveCartesian>("/ufactory/set_position");
    gripper_open_client_ = this->create_client<xarm_msgs::srv::Call>("/ufactory/open_lite6_gripper");
    gripper_close_client_ = this->create_client<xarm_msgs::srv::Call>("/ufactory/close_lite6_gripper");
    gripper_stop_client_ = this->create_client<xarm_msgs::srv::Call>("/ufactory/stop_lite6_gripper");
}

cv::Mat BoxHoleXArmControl::computeEEtoBaseTransform(const std::array<double, 6>& pose) {
    double x = pose[0], y = pose[1], z = pose[2];
    double rx = pose[3], ry = pose[4], rz = pose[5];

    // **1. 建立旋轉矩陣 (ZYX 歐拉角轉換)**
    cv::Mat Rx = (cv::Mat_<double>(3, 3) << 
        1, 0, 0,
        0, cos(rx), -sin(rx),
        0, sin(rx), cos(rx)
    );

    cv::Mat Ry = (cv::Mat_<double>(3, 3) << 
        cos(ry), 0, sin(ry),
        0, 1, 0,
        -sin(ry), 0, cos(ry)
    );

    cv::Mat Rz = (cv::Mat_<double>(3, 3) << 
        cos(rz), -sin(rz), 0,
        sin(rz), cos(rz), 0,
        0, 0, 1
    );

    cv::Mat R = Rz * Ry * Rx;  // ZYX 順序的旋轉矩陣

    // **2. 建立 4x4 變換矩陣**
    cv::Mat T_EE2Base = cv::Mat::eye(4, 4, CV_64F);
    R.copyTo(T_EE2Base(cv::Rect(0, 0, 3, 3)));  // 設定旋轉部分
    T_EE2Base.at<double>(0, 3) = x;
    T_EE2Base.at<double>(1, 3) = y;
    T_EE2Base.at<double>(2, 3) = z;

    return T_EE2Base;
}


// **讀取手眼校正結果**
bool BoxHoleXArmControl::loadHandEyeCalibration() {
    std::string yaml_path = "/home/jason9308/robot_ws/src/xarm_ros2/aruco_grab/camera_calibration_result2.yaml";
    YAML::Node config = YAML::LoadFile(yaml_path);

    if (!config["cameraMatrix"] || !config["distCoeffs"] || !config["handEyeRotation"] || !config["handEyeTranslation"]) {
        RCLCPP_ERROR(this->get_logger(), "Invalid hand-eye calibration YAML format!");
        return false;
    }
    
    auto camera_data = config["cameraMatrix"]["data"].as<std::vector<double>>();
    cameraMatrix_ = cv::Mat(3, 3, CV_64F, camera_data.data()).clone();

    auto dist_data = config["distCoeffs"]["data"].as<std::vector<double>>();
    distCoeffs_ = cv::Mat(1, 5, CV_64F, dist_data.data()).clone();

    auto rot_data = config["handEyeRotation"]["data"].as<std::vector<double>>();
    handEyeRotation_ = cv::Mat(3, 3, CV_64F, rot_data.data()).clone();

    auto trans_data = config["handEyeTranslation"]["data"].as<std::vector<double>>();
    handEyeTranslation_ = cv::Mat(3, 1, CV_64F, trans_data.data()).clone();

    RCLCPP_INFO(this->get_logger(), "Hand-eye calibration data loaded successfully.");

    RCLCPP_INFO(this->get_logger(), "Loaded hand-eye translation: %.5f, %.5f, %.5f",
            handEyeTranslation_.at<double>(0),
            handEyeTranslation_.at<double>(1),
            handEyeTranslation_.at<double>(2));


    return true;
}

// **接收深度影像**
void BoxHoleXArmControl::depthCallback(const sensor_msgs::msg::Image::SharedPtr msg) {
    try {
        depth_image_ = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_32FC1)->image.clone();
    } catch (cv_bridge::Exception& e) {
        RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
    }
}

// **接收機械手臂狀態回饋**
void BoxHoleXArmControl::xarmFeedbackCallback(const xarm_msgs::msg::RobotMsg::SharedPtr msg) {
    if (msg->pose.size() >= 6) {
        for (size_t i = 0; i < 6; i++) {
            xarm_current_pose_[i] = msg->pose[i];
        }
    }
}


void BoxHoleXArmControl::imageCallback(const sensor_msgs::msg::Image::SharedPtr msg) {
    try {
        rgb_image_ = cv_bridge::toCvCopy(msg, "bgr8")->image.clone();
    } catch (cv_bridge::Exception& e) {
        RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
        return;
    }

    if (rgb_image_.empty()) return;

    cv::Mat gray;
    cv::cvtColor(rgb_image_, gray, cv::COLOR_BGR2GRAY);

    // ArUco 設定
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_100);
    std::vector<int> markerIds;
    std::vector<std::vector<cv::Point2f>> markerCorners;

    // 偵測 ArUco 標記
    cv::aruco::detectMarkers(gray, dictionary, markerCorners, markerIds);

    if (markerIds.empty()) {
        RCLCPP_WARN(this->get_logger(), "No ArUco markers detected!");
        return;
    }

    // 將 ArUco 角點存起來，供 `performGrasping()` 使用
    detected_marker_corners_ = markerCorners;
    detected_marker_ids_ = markerIds;
}



// **控制機械手臂移動**
bool BoxHoleXArmControl::moveXArmToPose(const std::array<double, 6>& pose) {
    if (!xarm_move_cartesian_client_->wait_for_service(std::chrono::seconds(3))) {
        RCLCPP_ERROR(this->get_logger(), "Service /xarm/set_position is not available!");
        return false;
    }
    
    auto req = std::make_shared<xarm_msgs::srv::MoveCartesian::Request>();
    req->pose.assign(pose.begin(), pose.end());
    req->speed = 50;
    req->acc = 500;

    auto future = xarm_move_cartesian_client_->async_send_request(req);
    auto timeout = std::chrono::seconds(10);
    
    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future, timeout) == 
        rclcpp::FutureReturnCode::SUCCESS) {
        return future.get()->ret == 0;
    } else {
        return false;
    }
}

void BoxHoleXArmControl::performGrasping() {

    auto start_time = this->now();
    rclcpp::Time timeout_time = start_time + rclcpp::Duration::from_seconds(1.5);

    //rclcpp::sleep_for(std::chrono::seconds(5));
    while (rclcpp::ok() && this->now() < timeout_time) 
    {
        // 讓 ROS 2 執行 callback，確保訂閱的回調函式能夠執行
        rclcpp::spin_some(this->get_node_base_interface());
        rclcpp::sleep_for(std::chrono::milliseconds(100));  // 每 100ms 等待一次
    }

    if (detected_marker_corners_.empty()) {
        RCLCPP_WARN(this->get_logger(), "No valid ArUco marker detected for grasping.");
        return;
    }

    if (depth_image_.empty()) {
        RCLCPP_WARN(this->get_logger(), "Depth image is empty! Cannot compute marker depth.");
        return;
    }

    // 取得 ArUco 標記的深度 (單位：mm)
    float depth_value = depth_image_.at<float>(detected_marker_corners_[0][0].y, detected_marker_corners_[0][0].x);

    if (depth_value <= 0) {
        RCLCPP_WARN(this->get_logger(), "Invalid depth value detected!");
        return;
    }

    // **計算 ArUco 在相機座標系下的位置**
    cv::Point2f marker_center = (detected_marker_corners_[0][0] + detected_marker_corners_[0][2]) * 0.5;
    double x = (marker_center.x - cameraMatrix_.at<double>(0, 2)) / cameraMatrix_.at<double>(0, 0) * depth_value;
    double y = (marker_center.y - cameraMatrix_.at<double>(1, 2)) / cameraMatrix_.at<double>(1, 1) * depth_value;
    double z = depth_value;

    cv::Mat aruco_camera_pos = (cv::Mat_<double>(3, 1) << x, y, z);

    // **1. 轉換到手臂末端 (End-Effector) 座標系**
    cv::Mat aruco_ee_pos = handEyeRotation_ * aruco_camera_pos + handEyeTranslation_;

    // **2. 取得手臂當前的變換矩陣 (手臂回饋資訊)**
    cv::Mat T_EE2Base = computeEEtoBaseTransform(xarm_current_pose_);

    // **3. 轉換到機械手臂基座座標系**
    cv::Mat aruco_base_pos = T_EE2Base(cv::Rect(0, 0, 3, 3)) * aruco_ee_pos + T_EE2Base(cv::Rect(3, 0, 1, 3));

    // **取得基座座標系下的 x, y, z**
    double base_x = aruco_base_pos.at<double>(0);
    double base_y = aruco_base_pos.at<double>(1);
    double base_z = aruco_base_pos.at<double>(2);

    RCLCPP_INFO(this->get_logger(), "ArUco in Base Frame: x=%.3f, y=%.3f, z=%.3f", base_x, base_y, base_z);

    int hole_col = (this->id - 1) % 4;
    int hole_row = (this->id - 1) / 4;
    float hole_y = base_y;
    float hole_x = base_x;
    float hole_z = base_z - 30;


    while(hole_col > 0)
    {
        hole_y += hole_width;
        hole_col--;
    }
    hole_y += hole_width / 2;

    while(hole_row > 0)
    {
        hole_x -= hole_height;
        hole_row--;
    }
    hole_x -= hole_height / 2;

    std::array<double, 6> target_pose = {hole_x, hole_y, 400.0 , 3.14, 0, 0};
    if (moveXArmToPose(target_pose)) 
    {
        //RCLCPP_INFO(this->get_logger(), "Successfully moved to ArUco marker position.");
        if (wait_for_xarm_arrival(target_pose, 1.0)) {
            RCLCPP_INFO(this->get_logger(), "Hand reached target position.");
        } else {
            RCLCPP_ERROR(this->get_logger(), "Hand failed to reach target position in time.");
        }
    } 
    else 
    {
        RCLCPP_ERROR(this->get_logger(), "Failed to move to target position.");
    }

    // **控制機械手臂移動到該點**
    // std::array<double, 6> target_pose = {base_x, base_y, base_z , 3.14, 0, 0};
    target_pose = {hole_x, hole_y, hole_z , 3.14, 0, 0};

    if (moveXArmToPose(target_pose)) 
    {
        //RCLCPP_INFO(this->get_logger(), "Successfully moved to ArUco marker position.");
        if (wait_for_xarm_arrival(target_pose, 1.0)) {
            RCLCPP_INFO(this->get_logger(), "Hand reached target position.");
        } else {
            RCLCPP_ERROR(this->get_logger(), "Hand failed to reach target position in time.");
        }
    } 
    else 
    {
        RCLCPP_ERROR(this->get_logger(), "Failed to move to target position.");
    }
    //rclcpp::sleep_for(std::chrono::seconds(5));

    // 夾取物品
    closeGripper();
    rclcpp::sleep_for(std::chrono::seconds(1));
    stopGripper();
    rclcpp::sleep_for(std::chrono::seconds(1));

    // 移動到放置位置
    target_pose = {hole_x, hole_y, hole_z + 100.0 , 3.14, 0, 0};
    if (moveXArmToPose(target_pose)) 
    {
        //RCLCPP_INFO(this->get_logger(), "Successfully moved to ArUco marker position.");
        if (wait_for_xarm_arrival(target_pose, 1.0)) {
            RCLCPP_INFO(this->get_logger(), "Hand reached target position.");
        } else {
            RCLCPP_ERROR(this->get_logger(), "Hand failed to reach target position in time.");
        }
    } 
    else 
    {
        RCLCPP_ERROR(this->get_logger(), "Failed to move to target position.");
    }
    rclcpp::sleep_for(std::chrono::seconds(1));

    target_pose = {350, 0, 350, 3.14, 0, 0};
    if (moveXArmToPose(target_pose)) 
    {
        //RCLCPP_INFO(this->get_logger(), "Successfully moved to ArUco marker position.");
        if (wait_for_xarm_arrival(target_pose, 1.0)) {
            RCLCPP_INFO(this->get_logger(), "Hand reached target position.");
        } else {
            RCLCPP_ERROR(this->get_logger(), "Hand failed to reach target position in time.");
        }
    } 
    else 
    {
        RCLCPP_ERROR(this->get_logger(), "Failed to move to target position.");
    }
    openGripper();
    rclcpp::sleep_for(std::chrono::seconds(1));
    stopGripper();
    rclcpp::sleep_for(std::chrono::seconds(1));

}

bool BoxHoleXArmControl::wait_for_xarm_arrival(const std::array<double, 6>& target_pose, double tolerance = 1.0) {
    auto start_time = this->now();
    rclcpp::Time timeout_time = start_time + rclcpp::Duration::from_seconds(10.0); // 最多等待 10 秒

    while (rclcpp::ok() && this->now() < timeout_time) {
        // 讓 ROS 2 執行 callback，確保訂閱的回調函式能夠執行
        rclcpp::spin_some(this->get_node_base_interface());
        rclcpp::sleep_for(std::chrono::milliseconds(100));

        // 計算目前手臂位置與目標位置的誤差
        double error = 0.0;
        for (size_t i = 0; i < 6; i++) {
            error += std::pow(target_pose[i] - xarm_current_pose_[i], 2);
        }
        error = std::sqrt(error);  // 歐式距離

        // 如果誤差小於容許範圍，視為到達
        if (error < tolerance) {
            return true;
        }
    }
    return false; // 逾時未到達
}

bool BoxHoleXArmControl::callGripperService(rclcpp::Client<xarm_msgs::srv::Call>::SharedPtr client) {
    if (!client->wait_for_service(std::chrono::seconds(3))) {
        RCLCPP_ERROR(this->get_logger(), "Gripper service is not available!");
        return false;
    }

    auto request = std::make_shared<xarm_msgs::srv::Call::Request>();
    auto future = client->async_send_request(request);

    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future, std::chrono::seconds(5)) ==
        rclcpp::FutureReturnCode::SUCCESS) {
        return true;
    } else {
        RCLCPP_ERROR(this->get_logger(), "Failed to call gripper service!");
        return false;
    }
}

void BoxHoleXArmControl::openGripper() {
    RCLCPP_INFO(this->get_logger(), "Opening gripper...");
    callGripperService(gripper_open_client_);
}

void BoxHoleXArmControl::closeGripper() {
    RCLCPP_INFO(this->get_logger(), "Closing gripper...");
    callGripperService(gripper_close_client_);
}

void BoxHoleXArmControl::stopGripper() {
    RCLCPP_INFO(this->get_logger(), "Stopping gripper...");
    callGripperService(gripper_stop_client_);
}


// int main(int argc, char** argv) 
// {
//     // **初始化 ROS 2**
//     rclcpp::init(argc, argv);

//     // **建立 BoxHoleXArmControl 節點**
//     auto node = std::make_shared<BoxHoleXArmControl>();

//     node->performGrasping();

//     // **進入 ROS 2 事件迴圈，保持節點運行**
//     //rclcpp::spin(node);

//     // **結束時清理**
//     rclcpp::shutdown();
//     return 0;
// }


int main(int argc, char** argv) 
{
    // 初始化 ROS 2
    rclcpp::init(argc, argv);

    int id;

    // // 要求使用者輸入 id
    // std::cout << "請輸入格子 ID (1-12): ";
    // std::cin >> id;

    if(argc < 2)
    {
        std::cout << "No parameter <id> !!" << std::endl;
        return 1;
    }

    id = std::stoi(argv[1]);
    if(!(id >= 1 && id <= 12))
    {
        std::cout << "Invalid id: " << id << std::endl;
        return 1;
    }

    // 建立 BoxHoleXArmControl 節點
    auto node = std::make_shared<BoxHoleXArmControl>();

    // 設定 id
    node->id = id;

    node->performGrasping();

    // **結束時清理**
    rclcpp::shutdown();
    return 0;
}

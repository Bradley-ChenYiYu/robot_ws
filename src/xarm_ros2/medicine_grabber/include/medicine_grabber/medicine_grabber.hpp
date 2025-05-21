#ifndef MEDICINE_GRABBER_HPP_
#define MEDICINE_GRABBER_HPP_

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
#include "std_msgs/msg/float32_multi_array.hpp"
#include "std_msgs/msg/string.hpp"


class MedicineGrabber : public rclcpp::Node
{
public:
    MedicineGrabber();
    void performGrasping();  // 主流程入口
    int id;  // 藥包 ID（由 main 設定）

private:
    // ==== ROS 通訊 ====
    void setupSubscriptions();
    void setupClients();
    void setupPublishers();

    // ==== callback ====
    void imageCallback(const sensor_msgs::msg::Image::SharedPtr msg);
    void depthCallback(const sensor_msgs::msg::Image::SharedPtr msg);
    void xarmFeedbackCallback(const xarm_msgs::msg::RobotMsg::SharedPtr msg);
    void handCallback(const std_msgs::msg::Float32MultiArray::SharedPtr msg);

    // ==== 機械手臂控制 ====
    bool moveXArmToPose(const std::array<double, 6>& pose);
    bool wait_for_xarm_arrival(const std::array<double, 6>& target_pose, double tolerance);
    void openGripper();
    void closeGripper();
    void stopGripper();
    bool callGripperService(rclcpp::Client<xarm_msgs::srv::Call>::SharedPtr client);
    void moveArm_check(const std::array<double, 6>& target_pose, const std::string& description = "");


    // ==== 影像處理 ====
    bool loadHandEyeCalibration();
    cv::Mat computeEEtoBaseTransform(const std::array<double, 6>& pose);
    cv::Point3f transformCameraToBase(const cv::Point3f& point_in_camera);
    cv::Point3f computeHoleCenter(int id, const cv::Point3f& base_center);

    // ==== 語音播放 ====
    void play_audio(const std::string& path);

    // ==== ROS 成員變數 ====
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr rgb_subscription_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr depth_subscription_;
    rclcpp::Subscription<xarm_msgs::msg::RobotMsg>::SharedPtr xarm_feedback_subscription_;
    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr hand_sub_;
    rclcpp::Client<xarm_msgs::srv::MoveCartesian>::SharedPtr xarm_move_cartesian_client_;
    rclcpp::Client<xarm_msgs::srv::Call>::SharedPtr gripper_open_client_;
    rclcpp::Client<xarm_msgs::srv::Call>::SharedPtr gripper_close_client_;
    rclcpp::Client<xarm_msgs::srv::Call>::SharedPtr gripper_stop_client_;

    // ==== 資料 ====
    cv::Mat rgb_image_, depth_image_;
    std::array<double, 6> xarm_current_pose_;
    std::vector<std::vector<cv::Point2f>> detected_marker_corners_;
    std::vector<int> detected_marker_ids_;

    // 手眼校正參數
    cv::Mat cameraMatrix_;
    cv::Mat distCoeffs_;
    cv::Mat handEyeRotation_;
    cv::Mat handEyeTranslation_;

    // 手部偵測穩定判斷
    cv::Point3f last_hand_pos_;
    rclcpp::Time last_hand_time_;
    int hand_stable_counter_ = 0;
    bool hand_delivery_triggered_ = false;
    const double hand_stability_threshold_ = 40.0; // mm
    cv::Point3f last_hand_base_pos_;  // 已轉換為 base frame 座標

    // Aruco偵測參數
    cv::Ptr<cv::aruco::Dictionary> dictionary_;
    cv::Ptr<cv::aruco::DetectorParameters> parameters_;
    void setupArucoDetector();

    // hole 尺寸參數
    float hole_width = 82.0;
    float hole_height = 82.0;

    // 
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr chat_publisher_;
    std_msgs::msg::String bot_msg_;
    std_msgs::msg::String user_msg_;

};

#endif  // MEDICINE_GRABBER_HPP_

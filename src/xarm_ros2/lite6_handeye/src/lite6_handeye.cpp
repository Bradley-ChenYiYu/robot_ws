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
#include "yaml-cpp/yaml.h"

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "cv_bridge/cv_bridge.h"

#include "xarm_msgs/msg/robot_msg.hpp"
#include "xarm_msgs/srv/move_cartesian.hpp"
#include "xarm_msgs/srv/move_home.hpp"

class XArmHandEyeCalibration : public rclcpp::Node {
public:
    XArmHandEyeCalibration() : Node("xarm_handeye_calibration") {
        setupSubscriptions();  // 初始化影像訂閱
        setupClients();        // 初始化服務客戶端
        RCLCPP_INFO(this->get_logger(), "xArm Hand-Eye Calibration Node Started.");

        std::array<double, 6> initial_pose = {350, 0, 350, 3.14, 0, 0};
        if (!moveXArmToPose(initial_pose)) {
            RCLCPP_ERROR(this->get_logger(), "Failed to move to initial position!");
        } else {
            RCLCPP_INFO(this->get_logger(), "Moved to initial position.");
        }
    }

    void performHandEyeCalibration();  // 執行手眼校正
    bool topHomingExecute();  // 控制手臂回歸 Home 位置
private:
    // 訂閱 RGB 影像與深度影像
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr rgb_realsense_subscription_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr depth_realsense_subscription_;
    cv::Mat rgb_image_, depth_image_; // 儲存 RGB 影像與深度影像

    // 訂閱機械手臂狀態回饋
    rclcpp::Subscription<xarm_msgs::msg::RobotMsg>::SharedPtr xarm_feedback_subscription_;
    xarm_msgs::msg::RobotMsg::SharedPtr xarm_feedback_state_;

    // 控制機械手臂
    rclcpp::Client<xarm_msgs::srv::MoveCartesian>::SharedPtr xarm_move_cartesian_client_;
    rclcpp::Client<xarm_msgs::srv::MoveHome>::SharedPtr xarm_move_home_client_;

    void setupSubscriptions();  // 訂閱影像與手臂回饋
    void setupClients();        // 初始化手臂控制服務
    bool moveXArmToPose(const std::array<double, 6>& pose);  // 控制手臂移動
};

// **初始化 ROS 2 訂閱**
void XArmHandEyeCalibration::setupSubscriptions() {
    // 訂閱 RGB 影像
    rgb_realsense_subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
        "/camera/camera/color/image_raw", 10,
        [this](const sensor_msgs::msg::Image::SharedPtr msg) {
            try {
                rgb_image_ = cv_bridge::toCvCopy(msg, "bgr8")->image.clone();
            } catch (cv_bridge::Exception& e) {
                RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
            }
        });

    // 訂閱深度影像
    depth_realsense_subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
        "/camera/camera/depth/image_rect_raw", 10,
        [this](const sensor_msgs::msg::Image::SharedPtr msg) {
            try {
                depth_image_ = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_32FC1)->image.clone();
                // depth_image_ /= 1000.0;  // 轉換為米
            } catch (cv_bridge::Exception& e) {
                RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
            }
        });

    // 訂閱機械手臂狀態回饋
    xarm_feedback_subscription_ = this->create_subscription<xarm_msgs::msg::RobotMsg>(
        "/ufactory/robot_states", 10,
        [this](const xarm_msgs::msg::RobotMsg::SharedPtr msg) {
            xarm_feedback_state_ = msg;
        });
}

// **初始化 ROS 2 服務客戶端**
void XArmHandEyeCalibration::setupClients() {
    xarm_move_cartesian_client_ = this->create_client<xarm_msgs::srv::MoveCartesian>("/ufactory/set_position");
    xarm_move_home_client_ = this->create_client<xarm_msgs::srv::MoveHome>("/ufactory/move_gohome");
}

// **控制機械手臂移動**
bool XArmHandEyeCalibration::moveXArmToPose(const std::array<double, 6>& pose) {

    if (!xarm_move_cartesian_client_->wait_for_service(std::chrono::seconds(5))) {
        RCLCPP_ERROR(this->get_logger(), "Service /xarm/set_position is not available!");
        return false;
    }
    
    auto req = std::make_shared<xarm_msgs::srv::MoveCartesian::Request>();
    req->pose.assign(pose.begin(), pose.end());
    req->speed = 50;
    req->acc = 500;

    auto future = xarm_move_cartesian_client_->async_send_request(req);
    auto timeout = std::chrono::seconds(10);  // 最多等 10 秒
    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future, timeout) == 
        rclcpp::FutureReturnCode::SUCCESS) {
        RCLCPP_INFO(this->get_logger(), "Move command sent successfully.");
        return future.get()->ret == 0;
    } else {
        RCLCPP_ERROR(this->get_logger(), "Failed to send move command within timeout.");
        return false;
    }
    // consider return true directly
}


bool XArmHandEyeCalibration::topHomingExecute() {
    RCLCPP_INFO(this->get_logger(), "xArm Lite6 Homing");

    // 發送 Home 命令
    auto req = std::make_shared<xarm_msgs::srv::MoveHome::Request>();
    auto future = xarm_move_home_client_->async_send_request(req);

    // 最多等待 10 秒，看是否成功收到服務端回覆
    auto timeout = std::chrono::seconds(10);

    // 使用 spin_until_future_complete 來驅動回呼並等待未來物件完成
    if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future, timeout) 
        == rclcpp::FutureReturnCode::SUCCESS) 
    {
        // 取得服務回傳結果
        auto result = future.get();
        if (result->ret == 0) {
            RCLCPP_INFO(this->get_logger(), "xArm Lite6 is at home position.");
            return true;
        }
        else {
            RCLCPP_WARN(this->get_logger(), "Homing failed, ret = %d", result->ret);
            return false;
        }
    }
    else {
        RCLCPP_ERROR(this->get_logger(), "Failed to send home command within timeout.");
        return false;
    }
}



// **執行手眼校正**
void XArmHandEyeCalibration::performHandEyeCalibration() {
    RCLCPP_INFO(this->get_logger(), "Starting Hand-Eye Calibration...");
    // **讀取 YAML 設定檔**
    YAML::Node config = YAML::LoadFile("/home/jason9308/robot_ws/src/xarm_ros2/lite6_handeye/config/calibration_camera_eih_001.yaml");
    auto xarm_config = config["xarm_lite6"];
    
    if (!xarm_config["sampleTrajectory"] || !xarm_config["chessboard_info"]) {
        throw std::runtime_error("Missing required configuration fields in YAML file");
    }

    RCLCPP_INFO(this->get_logger(), "debug_1");

    // **讀取標定軌跡與棋盤格設定**
    auto trajectories = xarm_config["sampleTrajectory"].as<std::vector<std::vector<double>>>();
    auto chessboard = xarm_config["chessboard_info"];
    int pattern_width = chessboard["pattern_width"].as<int>();
    int pattern_height = chessboard["pattern_height"].as<int>();
    double square_size = chessboard["square_size"].as<double>();
    cv::Size pattern_size(pattern_width, pattern_height);

    RCLCPP_INFO(this->get_logger(), "debug_2");
    // **相機內參與畸變係數**
    cv::Mat cameraMatrix = (cv::Mat_<double>(3,3) << 
        9.1748012527594449e+02, 0.0, 6.4252364235136827e+02,
        0.0, 9.1799847821683193e+02, 3.4940508378773694e+02,
        0.0, 0.0, 1.0);
    cv::Mat distCoeffs = (cv::Mat_<double>(1,5) << 
        7.7255e-02, 4.1528e-01, 1.5215e-03, -1.1624e-03, -2.1311e+00);

    // **初始化變數**
    std::vector<cv::Mat> robot_rotations, robot_translations;
    std::vector<cv::Mat> camera_rotations, camera_translations, camera_poses;
    std::vector<cv::Mat> robot_poses;
    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<std::vector<cv::Point2f>> imagePoints;
    std::vector<std::vector<float>> imagePointsDepth;
    
    std::vector<cv::Point3f> object_corners;
    for (int i = 0; i < pattern_height; i++) {
        for (int j = 0; j < pattern_width; j++) {
            object_corners.push_back(cv::Point3f(j * square_size, i * square_size, 0.0f));
        }
    }
    RCLCPP_INFO(this->get_logger(), "debug_3");
    // **標定影像儲存**
    std::filesystem::path save_dir("/home/jason9308/robot_ws/src/xarm_ros2/lite6_handeye/data/calibration");
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_time_t), "%Y%m%d_%H%M%S");
    std::string timestamp = ss.str();                        
    save_dir = save_dir / timestamp;
    std::filesystem::path image_dir = save_dir / "images";

    std::filesystem::create_directories(save_dir);
    std::filesystem::create_directories(image_dir);
    RCLCPP_INFO(this->get_logger(), "debug_4");
    int valid_image_count = 0;
    for (const auto& pose : trajectories) {
        RCLCPP_INFO(this->get_logger(), "debug_5");
        std::array<double, 6> robot_pose;
        for (size_t i = 0; i < 6; i++) robot_pose[i] = pose[i];
        
        if (!moveXArmToPose(robot_pose)) {
            RCLCPP_ERROR(this->get_logger(), "Failed to move to calibration position");
            continue;
        }
        else{
            RCLCPP_INFO(this->get_logger(), "debug_6");
        }

        rclcpp::sleep_for(std::chrono::seconds(5));

        RCLCPP_INFO(this->get_logger(), "test1");
        std::array<double, 6> actual_pose;

        auto start_time = this->now();
        while (rclcpp::ok()) {
            // 讓 ROS2 執行 callback
            rclcpp::spin_some(this->get_node_base_interface());

            // 檢查是否已拿到有效資料
            if (xarm_feedback_state_ && xarm_feedback_state_->pose.size() >= 6) {
                break;
            }

            if ((this->now() - start_time).seconds() > 5.0) {
                RCLCPP_ERROR(this->get_logger(), "Timeout waiting for xarm feedback state!");
                return;
            }
            rclcpp::sleep_for(std::chrono::milliseconds(100));
        }

        RCLCPP_INFO(this->get_logger(), "xarm_feedback_state_->pose = %f", xarm_feedback_state_->pose[0]);
        RCLCPP_INFO(this->get_logger(), "xarm_feedback_state_->pose = %f", xarm_feedback_state_->pose[1]);
        RCLCPP_INFO(this->get_logger(), "xarm_feedback_state_->pose = %f", xarm_feedback_state_->pose[2]);
        RCLCPP_INFO(this->get_logger(), "xarm_feedback_state_->pose = %f", xarm_feedback_state_->pose[3]);
        RCLCPP_INFO(this->get_logger(), "xarm_feedback_state_->pose = %f", xarm_feedback_state_->pose[4]);
        RCLCPP_INFO(this->get_logger(), "xarm_feedback_state_->pose = %f", xarm_feedback_state_->pose[5]);

        RCLCPP_INFO(this->get_logger(), "test2");
        std::copy(xarm_feedback_state_->pose.begin(), 
          xarm_feedback_state_->pose.end(), 
          actual_pose.begin());

        RCLCPP_INFO(this->get_logger(), "copy pose success");

        
        if (rgb_image_.empty() || depth_image_.empty()) {
            RCLCPP_WARN(this->get_logger(), "Waiting for image...");
            continue;
        }

        cv::Mat rgb = rgb_image_.clone(), 
                    depth = depth_image_.clone(),
                    gray;
        cv::cvtColor(rgb, gray, cv::COLOR_BGR2GRAY);
        std::vector<cv::Point2f> corners;
        bool found = cv::findChessboardCorners(gray, pattern_size, corners);
        RCLCPP_INFO(this->get_logger(), "check corners");

        if (found) {
            RCLCPP_INFO(this->get_logger(), "debug_8");
            valid_image_count++;
        
            // **1. 儲存影像**
            std::string image_filename = "calibration_" + std::to_string(valid_image_count);
            cv::imwrite((image_dir / (image_filename + "_rgb.jpg")).string(), rgb);
            cv::imwrite((image_dir / (image_filename + "_gray.jpg")).string(), gray);
        
            // **2. 繪製並存儲角點**
            cv::Mat corner_img = rgb.clone();
            cv::drawChessboardCorners(corner_img, pattern_size, corners, found);
            cv::imwrite((image_dir / (image_filename + "_corners.jpg")).string(), corner_img);
        
            // **3. 儲存角點的深度資訊**
            std::vector<float> corner_depths;
            for (const auto& corner : corners) {
                float depth_value = depth.at<float>(corner.y, corner.x);  // 取得角點對應的深度值
                corner_depths.push_back(static_cast<float>(depth_value));
            }
        
            // **4. 角點精細化處理**
            cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
                             cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 30, 0.1));
        
            // **5. 儲存角點與對應深度**
            imagePoints.push_back(corners);
            objectPoints.push_back(object_corners);
            imagePointsDepth.push_back(corner_depths);
        
            // **6. 記錄機械手臂當前的位姿**
            cv::Mat robot_transform = cv::Mat::eye(4, 4, CV_64F);
            for (int i = 0; i < 3; i++) {
                robot_transform.at<double>(i, 3) = actual_pose[i];  // 取得機械手臂的 XYZ 位置
            }
        
            // **7. 轉換歐拉角為旋轉矩陣**
            double rx = actual_pose[3]; // Roll
            double ry = actual_pose[4]; // Pitch
            double rz = actual_pose[5]; // Yaw
        
            cv::Mat Rx = (cv::Mat_<double>(3, 3) <<
                          1, 0, 0,
                          0, cos(rx), -sin(rx),
                          0, sin(rx), cos(rx));
        
            cv::Mat Ry = (cv::Mat_<double>(3, 3) <<
                          cos(ry), 0, sin(ry),
                          0, 1, 0,
                          -sin(ry), 0, cos(ry));
        
            cv::Mat Rz = (cv::Mat_<double>(3, 3) <<
                          cos(rz), -sin(rz), 0,
                          sin(rz), cos(rz), 0,
                          0, 0, 1);
        
            cv::Mat R = Rz * Ry * Rx;  // 旋轉矩陣 = ZYX 歐拉角轉換
            // cv::Mat R = Rx * Ry * Rz;
            R.copyTo(robot_transform(cv::Rect(0, 0, 3, 3)));
        
            robot_poses.push_back(robot_transform);  // 儲存機械手臂位姿
        
            // **8. 儲存資料至 YAML 檔案**
            YAML::Emitter pose_out;
            pose_out << YAML::BeginMap;
            pose_out << YAML::Key << "image_" + std::to_string(valid_image_count);
            pose_out << YAML::Value;
            pose_out << YAML::BeginMap;
            pose_out << YAML::Key << "robot_pose" << YAML::Value << YAML::Flow << YAML::BeginSeq;
            for (const auto& val : actual_pose) {
                pose_out << val;
            }
            pose_out << YAML::EndSeq;
        
            // **9. 儲存角點與深度值**
            pose_out << YAML::Key << "num_corners" << YAML::Value << corners.size();
            pose_out << YAML::Key << "corners(u,v)" << YAML::Value << YAML::BeginSeq;
            for (size_t i = 0; i < corners.size(); i++) {
                pose_out << YAML::BeginMap;
                pose_out << YAML::Key << "u" << YAML::Value << corners[i].x;
                pose_out << YAML::Key << "v" << YAML::Value << corners[i].y;
                pose_out << YAML::Key << "depth" << YAML::Value << corner_depths[i];
                pose_out << YAML::EndMap;
            }
            pose_out << YAML::EndSeq;
        
            pose_out << YAML::EndMap;
            pose_out << YAML::EndMap;
        
            // **10. 寫入 YAML 檔案**
            std::ofstream fout((image_dir / (image_filename + "_pose.yaml")).string());
            fout << pose_out.c_str();
            fout.close();
        }
    }
    this->topHomingExecute();
    RCLCPP_INFO(this->get_logger(), "debug_7");
    // **進行相機標定**
    std::vector<cv::Mat> rvecs, tvecs;
    double error = cv::calibrateCamera(objectPoints, imagePoints, rgb_image_.size(),
                                       cameraMatrix, distCoeffs, rvecs, tvecs);
    RCLCPP_INFO(this->get_logger(), "Calibration error: %.3f", error);

    for(size_t i = 0; i < rvecs.size(); i++) {
        cv::Mat R;
        cv::Rodrigues(rvecs[i], R);  // 旋轉向量轉換成旋轉矩陣
        cv::Mat T = cv::Mat::eye(4, 4, CV_64F);  // 創建 4x4 齊次變換矩陣
        R.copyTo(T(cv::Rect(0,0,3,3)));  // 將旋轉矩陣填入
        tvecs[i].copyTo(T(cv::Rect(3,0,1,3)));  // 將平移向量填入
        camera_poses.push_back(T);  // 儲存變換矩陣
        camera_rotations.push_back(R);
        camera_translations.push_back(tvecs[i]);
    }
    
    // **從機器人姿態提取旋轉矩陣和平移向量**
    for(const auto& pose : robot_poses) {
        cv::Mat R = pose(cv::Rect(0,0,3,3));  // 取出 3x3 旋轉矩陣
        robot_rotations.push_back(R);
        cv::Mat t = pose(cv::Rect(3,0,1,3));  // 取出 3x1 平移向量
        robot_translations.push_back(t);
    }

    // **計算手眼標定**
    cv::Mat hand_eye_rotation, hand_eye_translation;
    cv::calibrateHandEye(robot_rotations, robot_translations, 
                         camera_rotations, camera_translations, 
                         hand_eye_rotation, hand_eye_translation, 
                         cv::CALIB_HAND_EYE_TSAI);

    // **計算世界座標**
    // **轉換標定板角點到機械手臂基座座標系**
    std::vector<std::vector<cv::Point3d>> base_frame_corners;

    for (size_t frame = 0; frame < imagePoints.size(); frame++) {
        std::vector<cv::Point3d> frame_corners_base;
        
        // **1. 反投影到相機座標系**
        std::vector<cv::Point3d> camera_points;
        for (size_t i = 0; i < imagePoints[frame].size(); i++) {
            double depth = imagePointsDepth[frame][i];  // 讀取角點的深度資訊
            if (depth <= 0) continue;  // 跳過無效深度點
            
            // 計算 3D 點在相機座標系的 X, Y, Z
            double x = (imagePoints[frame][i].x - cameraMatrix.at<double>(0, 2)) / cameraMatrix.at<double>(0, 0);
            double y = (imagePoints[frame][i].y - cameraMatrix.at<double>(1, 2)) / cameraMatrix.at<double>(1, 1);
            cv::Point3d pt_camera(x * depth, y * depth, depth);
            camera_points.push_back(pt_camera);
        }

        // **2. 相機座標轉換到工具座標系**
        std::vector<cv::Point3d> tool_points;
        for (const auto& pt_camera : camera_points) {
            cv::Mat pt_cam = (cv::Mat_<double>(4,1) << pt_camera.x, pt_camera.y, pt_camera.z, 1);

            // 手眼校正變換矩陣
            cv::Mat T_cam_tool = cv::Mat::eye(4, 4, CV_64F);
            hand_eye_rotation.copyTo(T_cam_tool(cv::Rect(0,0,3,3)));
            hand_eye_translation.copyTo(T_cam_tool(cv::Rect(3,0,1,3)));

            // 轉換到工具座標系
            cv::Mat pt_tool = T_cam_tool * pt_cam;
            tool_points.push_back(cv::Point3d(pt_tool.at<double>(0)/pt_tool.at<double>(3),
                                            pt_tool.at<double>(1)/pt_tool.at<double>(3),
                                            pt_tool.at<double>(2)/pt_tool.at<double>(3)));
        }

        // **3. 工具座標轉換到基座座標系**
        cv::Mat T_tool_base = robot_poses[frame];  // 取出機械手臂當下的 4x4 位姿矩陣
        for (const auto& pt_tool : tool_points) {
            cv::Mat pt_t = (cv::Mat_<double>(4,1) << pt_tool.x, pt_tool.y, pt_tool.z, 1);
            cv::Mat pt_base = T_tool_base * pt_t;
            frame_corners_base.push_back(cv::Point3d(pt_base.at<double>(0)/pt_base.at<double>(3),
                                                    pt_base.at<double>(1)/pt_base.at<double>(3),
                                                    pt_base.at<double>(2)/pt_base.at<double>(3)));
        }
        
        base_frame_corners.push_back(frame_corners_base);
    }

    RCLCPP_INFO(this->get_logger(), "All chessboard corners transformed to base frame.");

    // **儲存 YAML 結果**
    std::filesystem::path result_file = save_dir / "camera_calibration_result.yaml";
    cv::FileStorage fs(result_file.string(), cv::FileStorage::WRITE);
    fs << "error" << error;
    fs << "cameraMatrix" << cameraMatrix;
    fs << "distCoeffs" << distCoeffs;
    fs << "handEyeRotation" << hand_eye_rotation;
    fs << "handEyeTranslation" << hand_eye_translation;
    fs.release();

    RCLCPP_INFO(this->get_logger(), "Hand-Eye Calibration Completed!");

    // **新增 corners_in_base_frame.yaml 儲存**
    std::filesystem::path corners_file = save_dir / "corners_in_base_frame.yaml";

    // 使用 YAML::Emitter 來寫入 YAML 檔案
    YAML::Emitter corners_out;
    corners_out << YAML::BeginMap;
    corners_out << YAML::Key << "corners_in_base_frame(x,y,z)" << YAML::Value << YAML::BeginSeq;

    // 依序儲存每一張標定影像的角點座標
    for (size_t frame = 0; frame < base_frame_corners.size(); frame++) {
        corners_out << YAML::BeginMap;
        corners_out << YAML::Key << "frame_" + std::to_string(frame + 1);
        corners_out << YAML::Value << YAML::BeginSeq;

        // 逐點儲存
        for (const auto& corner : base_frame_corners[frame]) {
            corners_out << YAML::Flow << YAML::BeginSeq;
            corners_out << YAML::Value << corner.x;
            corners_out << YAML::Value << corner.y;
            corners_out << YAML::Value << corner.z;
            corners_out << YAML::EndSeq;
        }

        corners_out << YAML::EndSeq;
        corners_out << YAML::EndMap;
    }

    corners_out << YAML::EndSeq;
    corners_out << YAML::EndMap;

    // 儲存檔案
    std::ofstream corners_fout(corners_file.string());
    corners_fout << corners_out.c_str();
    corners_fout.close();

    RCLCPP_INFO(this->get_logger(), "Saved base frame corners to corners_in_base_frame.yaml");
}

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<XArmHandEyeCalibration>();
    node->performHandEyeCalibration();
    rclcpp::shutdown();
    return 0;
}
#include "medicine_grabber/medicine_grabber.hpp"
#define _USE_MATH_DEFINES

MedicineGrabber::MedicineGrabber()
: Node("medicine_grabber")
{
    setupSubscriptions();
    setupClients();
    setupPublishers();
    setupArucoDetector();

    RCLCPP_INFO(this->get_logger(), "MedicineGrabber node started.");

    if (!loadHandEyeCalibration()) {
        RCLCPP_ERROR(this->get_logger(), "Failed to load hand-eye calibration data!");
        rclcpp::shutdown();
    }

    openGripper();
    rclcpp::sleep_for(std::chrono::seconds(1));

    std::array<double, 6> initial_pose = {200, -50, 500, 3.14, 0, 0};
    if (!moveXArmToPose(initial_pose)) {
        RCLCPP_ERROR(this->get_logger(), "Failed to move to initial position!");
    } 
    else if (wait_for_xarm_arrival(initial_pose, 1.0)) {
        RCLCPP_INFO(this->get_logger(), "Hand reached initial position.");
    } 
    else {
        RCLCPP_ERROR(this->get_logger(), "Hand failed to reach initial position in time.");
    }
}

void MedicineGrabber::setupSubscriptions()
{
    // 影像處理邏輯

    // 訂閱 RGB 影像
    rgb_subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
        "/camera/camera/color/image_raw", 10,
        std::bind(&MedicineGrabber::imageCallback, this, std::placeholders::_1));

    // 訂閱深度影像
    depth_subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
        "/camera/camera/aligned_depth_to_color/image_raw", 10,
        std::bind(&MedicineGrabber::depthCallback, this, std::placeholders::_1));

    // 訂閱機械手臂狀態回饋
    xarm_feedback_subscription_ = this->create_subscription<xarm_msgs::msg::RobotMsg>(
        "/ufactory/robot_states", 10,
        std::bind(&MedicineGrabber::xarmFeedbackCallback, this, std::placeholders::_1));
   
    // 訂閱手部偵測結果 
    hand_sub_ = this->create_subscription<std_msgs::msg::Float32MultiArray>(
        "/hand_detection", 10,
        std::bind(&MedicineGrabber::handCallback, this, std::placeholders::_1)); 
}

void MedicineGrabber::setupClients()
{
    xarm_move_cartesian_client_ = this->create_client<xarm_msgs::srv::MoveCartesian>("/ufactory/set_position");
    gripper_open_client_ = this->create_client<xarm_msgs::srv::Call>("/ufactory/open_lite6_gripper");
    gripper_close_client_ = this->create_client<xarm_msgs::srv::Call>("/ufactory/close_lite6_gripper");
    gripper_stop_client_ = this->create_client<xarm_msgs::srv::Call>("/ufactory/stop_lite6_gripper");
}

void MedicineGrabber::setupPublishers()
{
    chat_publisher_ = this->create_publisher<std_msgs::msg::String>("/chat_messages", 10);
}

void MedicineGrabber::setupArucoDetector()
{
    // ArUco 設定
    dictionary_ = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_100);
    parameters_ = cv::aruco::DetectorParameters::create();
    parameters_->adaptiveThreshWinSizeMin = 3;
    parameters_->adaptiveThreshWinSizeMax = 23;
    parameters_->adaptiveThreshWinSizeStep = 10;
    parameters_->minMarkerPerimeterRate = 0.05;
    parameters_->maxMarkerPerimeterRate = 4.0;
    parameters_->polygonalApproxAccuracyRate = 0.04;
    parameters_->cornerRefinementMethod = cv::aruco::CORNER_REFINE_SUBPIX;
}

cv::Mat MedicineGrabber::computeEEtoBaseTransform(const std::array<double, 6>& pose)
{
    double x = pose[0], y = pose[1], z = pose[2];
    double rx = pose[3], ry = pose[4], rz = pose[5];

    // 建立旋轉矩陣 (ZYX 歐拉角轉換)
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

    cv::Mat R = Rz * Ry * Rx; // ZYX 順序的旋轉矩陣

    // 建立 4x4 變換矩陣
    cv::Mat T_EE2Base = cv::Mat::eye(4, 4, CV_64F);
    R.copyTo(T_EE2Base(cv::Rect(0, 0, 3, 3)));
    T_EE2Base.at<double>(0, 3) = x;
    T_EE2Base.at<double>(1, 3) = y;
    T_EE2Base.at<double>(2, 3) = z;

    return T_EE2Base;
}

cv::Point3f MedicineGrabber::transformCameraToBase(const cv::Point3f& point_in_camera)
{
    // 1. Camera → EE
    cv::Mat point_camera = (cv::Mat_<double>(3, 1) << point_in_camera.x, point_in_camera.y, point_in_camera.z);
    cv::Mat point_ee = handEyeRotation_ * point_camera + handEyeTranslation_;

    // 2. EE → Base
    cv::Mat T_EE2Base = computeEEtoBaseTransform(xarm_current_pose_);
    cv::Mat point_base = T_EE2Base(cv::Rect(0, 0, 3, 3)) * point_ee + T_EE2Base(cv::Rect(3, 0, 1, 3));

    return cv::Point3f(
        point_base.at<double>(0),
        point_base.at<double>(1),
        point_base.at<double>(2)
    );
}


bool MedicineGrabber::loadHandEyeCalibration(){
    // 讀取 YAML 檔案
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

void MedicineGrabber::depthCallback(const sensor_msgs::msg::Image::SharedPtr msg){

    try {
        depth_image_ = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_32FC1)->image.clone();
    } catch (cv_bridge::Exception& e) {
        RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
    }
}

void MedicineGrabber::xarmFeedbackCallback(const xarm_msgs::msg::RobotMsg::SharedPtr msg) {
    if (msg->pose.size() >= 6) {
        for (size_t i = 0; i < 6; i++) {
            xarm_current_pose_[i] = msg->pose[i];
        }
    }
}

void MedicineGrabber::imageCallback(const sensor_msgs::msg::Image::SharedPtr msg) {
    try {
        rgb_image_ = cv_bridge::toCvCopy(msg, "bgr8")->image.clone();
    } catch (cv_bridge::Exception& e) {
        RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
        return;
    }

    if (rgb_image_.empty()) return;

    cv::Mat gray;
    cv::cvtColor(rgb_image_, gray, cv::COLOR_BGR2GRAY);

    // 偵測 ArUco 標記
    std::vector<int> markerIds;
    std::vector<std::vector<cv::Point2f>> markerCorners;
    cv::aruco::detectMarkers(gray, dictionary_, markerCorners, markerIds, parameters_);

    if (markerIds.empty()) {
        // RCLCPP_WARN(this->get_logger(), "No ArUco markers detected!");
        return;
    }

    // 將 ArUco 角點存起來，供 `performGrasping()` 使用
    detected_marker_corners_ = markerCorners;
    detected_marker_ids_ = markerIds;
}

void MedicineGrabber::handCallback(const std_msgs::msg::Float32MultiArray::SharedPtr msg) {
    if (msg->data.size() < 3 || hand_delivery_triggered_) return;

    float x = msg->data[0];
    float y = msg->data[1];
    float z = msg->data[2];
    char buf[100];
    cv::Point3f current(x, y, z);

    if (hand_stable_counter_ == 0) {
        last_hand_pos_ = current;
        last_hand_time_ = this->now();
        hand_stable_counter_ = 1;
        snprintf(buf, sizeof(buf), "🟢 Hand stable count: %d / 10", hand_stable_counter_);
        RCLCPP_INFO(this->get_logger(), "%s", buf);
        return;
    }

    float dist = cv::norm(current - last_hand_pos_);

    if (dist < hand_stability_threshold_) {
        hand_stable_counter_++;
        last_hand_pos_ = current;
        
        snprintf(buf, sizeof(buf), "🟢 Hand stable count: %d / 10", hand_stable_counter_);
        bot_msg_.data = std::string("BOT: ") + buf;
        chat_publisher_->publish(bot_msg_);
        RCLCPP_INFO(this->get_logger(), "🟢 Hand stable count: %d / 10", hand_stable_counter_);
    } else {

        snprintf(buf, sizeof(buf), "🔴 Hand moved too much! Count reset.");
        bot_msg_.data = std::string("BOT: ") + buf;
        chat_publisher_->publish(bot_msg_);
        RCLCPP_INFO(this->get_logger(), "🔄 Hand moved too much, reset stability counter.");
        hand_stable_counter_ = 0;
    }

    if (hand_stable_counter_ >= 10) {  // 約 2 秒穩定
        // ==== Camera → EE ====
        cv::Point3f hand_camera_pos(x, y, z);
        cv::Point3f hand_base_pos = transformCameraToBase(hand_camera_pos);

        double base_x = hand_base_pos.x;
        double base_y = hand_base_pos.y;
        double base_z = hand_base_pos.z;
    
        // ==== Base frame 座標限制 ====
        bool in_range = (base_x > 150.0 && base_x < 385.0) &&
                        (base_y > -200.0 && base_y < 200.0) &&
                        (base_z > 150.0 && base_z < 600.0);
    
        if (!in_range) {
            RCLCPP_WARN(this->get_logger(),
                "❌ [最終檢查] 手部(base)座標超出安全範圍 (x=%.1f, y=%.1f, z=%.1f)，取消放藥。",
                base_x, base_y, base_z);
            hand_stable_counter_ = 0;
            
            bot_msg_.data = "BOT: 您的手太遠了，請靠近一點。";
            chat_publisher_->publish(bot_msg_);
            // 播放提示音
            std::system("mpg123 /home/jason9308/robot_ws/sound/hand_out_of_range.mp3");
            return;
        }
    
        RCLCPP_INFO(this->get_logger(), "🖐️ 手部穩定，開始放藥...");
        last_hand_base_pos_ = cv::Point3f(base_x, base_y, base_z);
        hand_delivery_triggered_ = true;
        last_hand_pos_ = current;
    }
}

cv::Point3f MedicineGrabber::computeHoleCenter(int id, const cv::Point3f& base_center)
{
    int col = (id - 1) % 4;
    int row = (id - 1) / 4;

    float hole_x = base_center.x;
    float hole_y = base_center.y;
    float hole_z = base_center.z - 30.0f;  // 微調高度

    hole_y += hole_width * col + hole_width / 2.0f;
    hole_x -= hole_height * row + hole_height / 2.0f;

    return cv::Point3f(hole_x, hole_y, hole_z);
}


bool MedicineGrabber::moveXArmToPose(const std::array<double, 6>& pose) {
    if (!xarm_move_cartesian_client_->wait_for_service(std::chrono::seconds(5))) {
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

void MedicineGrabber::moveArm_check(const std::array<double, 6>& target_pose, const std::string& description)
{
    if (moveXArmToPose(target_pose)) {
        if (wait_for_xarm_arrival(target_pose, 1.0)) {
            RCLCPP_INFO(this->get_logger(), "✅ Hand reached %s.", description.c_str());
        } else {
            RCLCPP_WARN(this->get_logger(), "⚠️ Hand reached %s, but not within tolerance.", description.c_str());
        }
    } else {
        RCLCPP_ERROR(this->get_logger(), "❌ Failed to move to %s.", description.c_str());
    }
}


void MedicineGrabber::performGrasping() {

    auto start_time = this->now();
    rclcpp::Time timeout_time = start_time + rclcpp::Duration::from_seconds(3.0);
    bool valid_marker = false;
    cv::Point3f aruco_base_pos;

    int retry_count = 0;
    int max_retry = 3;
    while(!valid_marker)
    {
        if(retry_count >= max_retry)
        {
            RCLCPP_ERROR(this->get_logger(), "❌ Failed to detect valid ArUco marker after %d retries.", max_retry);
            aruco_base_pos = cv::Point3f(308, -112, 190); // 設定一個default位置
            break;
        }
        retry_count++;

        start_time = this->now();
        timeout_time = start_time + rclcpp::Duration::from_seconds(3.0);
        while (rclcpp::ok() && this->now() < timeout_time) 
        {
            // 讓 ROS 2 執行 callback，確保訂閱的回調函式能夠執行
            rclcpp::spin_some(this->get_node_base_interface());
            rclcpp::sleep_for(std::chrono::milliseconds(100));  // 每 100ms 等待一次
        }

        if (detected_marker_corners_.empty()) {
            RCLCPP_WARN(this->get_logger(), "No valid ArUco marker detected for grasping.");
            continue;
            // return;
        }

        if (depth_image_.empty()) {
            RCLCPP_WARN(this->get_logger(), "Depth image is empty! Cannot compute marker depth.");
            continue;
            // return;
        }

        // 取得 ArUco 標記的深度 (單位：mm)
        float depth_value = depth_image_.at<float>(detected_marker_corners_[0][0].y, detected_marker_corners_[0][0].x);
        
        if (depth_value <= 0) {
            RCLCPP_WARN(this->get_logger(), "Invalid depth value detected!");
            continue;
            // return;
        }

        // **計算 ArUco 在相機座標系下的位置**
        const std::vector<cv::Point2f>& corners = detected_marker_corners_[0];
        cv::Point2f marker_center = (corners[0] + corners[1] + corners[2] + corners[3]) * 0.25f;
        // cv::Point2f marker_center = (detected_marker_corners_[0][0] + detected_marker_corners_[0][2]) * 0.5;
        double x = (marker_center.x - cameraMatrix_.at<double>(0, 2)) / cameraMatrix_.at<double>(0, 0) * depth_value;
        double y = (marker_center.y - cameraMatrix_.at<double>(1, 2)) / cameraMatrix_.at<double>(1, 1) * depth_value;
        double z = depth_value;

        cv::Point3f aruco_camera_pos(x, y, z);
        aruco_base_pos = transformCameraToBase(aruco_camera_pos);
        RCLCPP_INFO(this->get_logger(), "ArUco in Camera Frame: x=%.3f, y=%.3f, z=%.3f", aruco_camera_pos.x, aruco_camera_pos.y, aruco_camera_pos.z);
        RCLCPP_INFO(this->get_logger(), "ArUco in Base Frame: x=%.3f, y=%.3f, z=%.3f", aruco_base_pos.x, aruco_base_pos.y, aruco_base_pos.z);

        // ==== Base frame 座標限制 ====
        bool in_range = aruco_base_pos.x > 290 && aruco_base_pos.x < 320 &&
                    aruco_base_pos.y > -130 && aruco_base_pos.y < -100 &&
                    aruco_base_pos.z > 185 && aruco_base_pos.z < 195;

        if(in_range)
        {
            valid_marker = true;
            RCLCPP_INFO(this->get_logger(), "🟢 ArUco marker detected and in range.");
        }
        else
        {
            RCLCPP_WARN(this->get_logger(), "❌ ArUco marker detected but out of range.");
            // return;
        }
    }
    

    cv::Point3f center = computeHoleCenter(this->id, aruco_base_pos);
    float hole_x = center.x;
    float hole_y = center.y;
    float hole_z = center.z;

    // 移動到目標上方
    std::array<double, 6> target_pose;

    target_pose = {hole_x, hole_y, 400.0 , 3.14, 0, 0};
    moveArm_check(target_pose, "藥格上方");

    // 控制機械手臂移動到該點
    // std::array<double, 6> target_pose = {base_x, base_y, base_z , 3.14, 0, 0};
    target_pose = {hole_x, hole_y, hole_z , 3.14, 0, 0};
    moveArm_check(target_pose, "藥格中心");

    // 夾取物品
    closeGripper();
    rclcpp::sleep_for(std::chrono::seconds(1));
    stopGripper();
    rclcpp::sleep_for(std::chrono::seconds(1));

    // 先往上移動 避免碰撞到其他藥包
    target_pose = {hole_x, hole_y, hole_z + 100.0 , 3.14, 0, 0};
    moveArm_check(target_pose, "藥格上方");

    // 移動到觀察手掌出現的觀察點
    // target_pose = {230.0, 0.3499999940395355, 420.0, 3.140000104904175, -0.25, 0.0};
    // target_pose = {80.0, 0.0, 530.0, 3.14, 0, 0}; // 調整到觀察點
    target_pose = {87.4, 1.5, 532.7, -3.1227, -0.1553, 0.0087};
    moveArm_check(target_pose, "觀察手掌的位置");

    
    // 啟動手部偵測節點
    // 這裡使用 system() 函式來啟動手部偵測節點
    hand_delivery_triggered_ = false;
    hand_stable_counter_ = 0;
    RCLCPP_INFO(this->get_logger(), "🔄 啟動 hand_detection node...");

    std::string command = 
    "gnome-terminal --title=\"HandDetector\" -- bash -c '"
    "sleep 0.1; "
    "xdotool search --name \"HandDetector\" windowminimize; "
    "source ~/robot_ws/install/setup.bash && "
    "ros2 run hand_detection yolo_hand_detector"
    "'";


    int ret = std::system(command.c_str());
    if (ret == 0) {
        RCLCPP_INFO(this->get_logger(), "✅ hand_detection node 啟動成功");
    } else {
        RCLCPP_ERROR(this->get_logger(), "❌ hand_detection node 啟動失敗");
    }

    // 等待 2 秒鐘，確保手部偵測節點啟動完成
    rclcpp::sleep_for(std::chrono::seconds(2));
    // 播放提示音
    bot_msg_.data = "BOT: 您的藥來了，請伸出手。";
    chat_publisher_->publish(bot_msg_);
    play_audio("/home/jason9308/robot_ws/sound/medicine_please_take.mp3");

    // 等待手部穩定
    while(!hand_delivery_triggered_)
    {
        // 讓 ROS 2 執行 callback，確保訂閱的回調函式能夠執行
        rclcpp::spin_some(this->get_node_base_interface());
        rclcpp::sleep_for(std::chrono::milliseconds(100));  // 每 100ms 等待一次
    }

    // 停止手部偵測節點
    std::system("pkill -2 -f yolo_hand_detector");

    bot_msg_.data = "clear";
    chat_publisher_->publish(bot_msg_);

    // 放藥到手上
    double hand_base_x = last_hand_base_pos_.x;
    double hand_base_y = last_hand_base_pos_.y;
    double hand_base_z = last_hand_base_pos_.z;

    // 4. 設定目標位姿
    target_pose = { hand_base_x, hand_base_y, hand_base_z + 50.0, 3.14, 0, 0 };
    moveArm_check(target_pose, "手部上方");

    // 放下物品
    openGripper();
    rclcpp::sleep_for(std::chrono::seconds(1));
    
    stopGripper();
    rclcpp::sleep_for(std::chrono::seconds(1));

    // back to initial position
    target_pose = { hand_base_x, hand_base_y, hand_base_z + 150.0, 3.14, 0, 0 };
    moveArm_check(target_pose, "手部更上方");

    target_pose = {150, 0, 200, 3.14, 0, 0};
    // target_pose = {189, -0.3, 316.4, 2.965, -1.438, 0.173}; // 導航位置
    moveArm_check(target_pose, "初始位置");
}

bool MedicineGrabber::wait_for_xarm_arrival(const std::array<double, 6>& target_pose, double tolerance = 1.0) {
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

void MedicineGrabber::play_audio(const std::string& audio_path) {
    std::string cmd = "mpg123 " + audio_path;
    int ret = std::system(cmd.c_str());
    if (ret != 0) {
        RCLCPP_WARN(this->get_logger(), "語音播放失敗！");
    }
}

bool MedicineGrabber::callGripperService(rclcpp::Client<xarm_msgs::srv::Call>::SharedPtr client) {
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

void MedicineGrabber::openGripper() {
    RCLCPP_INFO(this->get_logger(), "Opening gripper...");
    callGripperService(gripper_open_client_);
}

void MedicineGrabber::closeGripper() {
    RCLCPP_INFO(this->get_logger(), "Closing gripper...");
    callGripperService(gripper_close_client_);
}

void MedicineGrabber::stopGripper() {
    RCLCPP_INFO(this->get_logger(), "Stopping gripper...");
    callGripperService(gripper_stop_client_);
}

int main(int argc, char** argv) 
{
    rclcpp::init(argc, argv);

    if(argc < 2)
    {
        std::cout << "No parameter <id> !!" << std::endl;
        return 1;
    }

    int id = std::stoi(argv[1]);
    if(!(id >= 1 && id <= 12))
    {
        std::cout << "Invalid id: " << id << std::endl;
        return 1;
    }

    auto node = std::make_shared<MedicineGrabber>();
    node->id = id;
    node->performGrasping();

    rclcpp::shutdown();
    return 0;
}

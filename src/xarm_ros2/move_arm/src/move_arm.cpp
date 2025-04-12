#include <rclcpp/rclcpp.hpp>
#include <xarm_msgs/srv/move_cartesian.hpp>
#include <xarm_msgs/msg/robot_msg.hpp>
#include <array>
#include <vector>
#include <cmath>
#include <iostream>

class MoveArmNode : public rclcpp::Node {
public:
    MoveArmNode(const std::array<double, 6>& target_pose)
        : Node("move_arm_node"), target_pose_(target_pose) {
        setupClient();
        setupFeedbackSub();
        moveToTarget();
    }

private:
    std::array<double, 6> target_pose_;
    std::array<double, 6> current_pose_;
    rclcpp::Client<xarm_msgs::srv::MoveCartesian>::SharedPtr move_client_;
    rclcpp::Subscription<xarm_msgs::msg::RobotMsg>::SharedPtr feedback_sub_;

    void setupClient() {
        move_client_ = this->create_client<xarm_msgs::srv::MoveCartesian>("/ufactory/set_position");
    }

    void setupFeedbackSub() {
        feedback_sub_ = this->create_subscription<xarm_msgs::msg::RobotMsg>(
            "/ufactory/robot_states", 10,
            [this](const xarm_msgs::msg::RobotMsg::SharedPtr msg) {
                if (msg->pose.size() >= 6) {
                    for (size_t i = 0; i < 6; i++) {
                        current_pose_[i] = msg->pose[i];
                    }
                }
            }
        );
    }

    void moveToTarget() {
        if (!move_client_->wait_for_service(std::chrono::seconds(3))) {
            RCLCPP_ERROR(this->get_logger(), "❌ Move service unavailable!");
            return;
        }

        auto req = std::make_shared<xarm_msgs::srv::MoveCartesian::Request>();
        req->pose.assign(target_pose_.begin(), target_pose_.end());
        req->speed = 50;
        req->acc = 500;

        auto future = move_client_->async_send_request(req);
        if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), future, std::chrono::seconds(10)) 
            != rclcpp::FutureReturnCode::SUCCESS) {
            RCLCPP_ERROR(this->get_logger(), "❌ Failed to call move service");
            return;
        }

        if (future.get()->ret != 0) {
            RCLCPP_ERROR(this->get_logger(), "❌ xArm move failed!");
            return;
        }

        RCLCPP_INFO(this->get_logger(), "✅ Move command sent, waiting for arrival...");
        if (wait_for_arrival(target_pose_, 1.0)) {
            RCLCPP_INFO(this->get_logger(), "🎯 Reached target position!");
        } else {
            RCLCPP_WARN(this->get_logger(), "⚠️ Timeout: arm may not have reached target.");
        }
    }

    bool wait_for_arrival(const std::array<double, 6>& target, double tolerance = 1.0) {
        auto start = this->now();
        auto timeout = start + rclcpp::Duration::from_seconds(10.0);

        while (rclcpp::ok() && this->now() < timeout) {
            rclcpp::spin_some(this->get_node_base_interface());
            rclcpp::sleep_for(std::chrono::milliseconds(100));

            double error = 0.0;
            for (size_t i = 0; i < 6; i++) {
                error += std::pow(current_pose_[i] - target[i], 2);
            }

            if (std::sqrt(error) < tolerance) return true;
        }
        return false;
    }
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);

    if (argc != 7) {
        std::cerr << "用法: ros2 run move_arm move_arm x y z rx ry rz\n";
        return 1;
    }

    std::array<double, 6> pose;
    for (int i = 0; i < 6; ++i) {
        pose[i] = std::stod(argv[i + 1]);
    }

    auto node = std::make_shared<MoveArmNode>(pose);
    rclcpp::shutdown();
    return 0;
}

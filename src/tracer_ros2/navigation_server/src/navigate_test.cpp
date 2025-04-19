#include <memory>
#include <string>
#include <chrono>
#include <iostream>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"

using namespace std::chrono_literals;

class NavigateTestNode : public rclcpp::Node {
public:
    using NavigateToPose = nav2_msgs::action::NavigateToPose;
    using GoalHandleNavigateToPose = rclcpp_action::ClientGoalHandle<NavigateToPose>;

    NavigateTestNode(double x, double y, double z, double w)
    : Node("navigate_test_node"), x_(x), y_(y), z_(z), w_(w)
    {
        nav_client_ = rclcpp_action::create_client<NavigateToPose>(this, "/navigate_to_pose");

        if (!nav_client_->wait_for_action_server(5s)) {
            RCLCPP_ERROR(this->get_logger(), "❌ 無法連接導航伺服器");
            rclcpp::shutdown();
            return;
        }

        send_goal_and_wait();
        RCLCPP_INFO(this->get_logger(), "test complete");
    }

private:
    rclcpp_action::Client<NavigateToPose>::SharedPtr nav_client_;
    double x_, y_, z_, w_;

    void send_goal_and_wait() {
        auto goal_msg = NavigateToPose::Goal();
        goal_msg.pose.header.frame_id = "map";
        goal_msg.pose.header.stamp = this->now();
        goal_msg.pose.pose.position.x = x_;
        goal_msg.pose.pose.position.y = y_;
        goal_msg.pose.pose.orientation.z = z_;
        goal_msg.pose.pose.orientation.w = w_;

        RCLCPP_INFO(this->get_logger(), "📍 發送導航目標: x=%.2f, y=%.2f, z=%.2f, w=%.2f", x_, y_, z_, w_);

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
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);

    if (argc < 5) {
        std::cerr << "❗ 使用方式: ros2 run your_package navigate_test <x> <y> <z> <w>" << std::endl;
        return 1;
    }

    double x = std::stod(argv[1]);
    double y = std::stod(argv[2]);
    double z = std::stod(argv[3]);
    double w = std::stod(argv[4]);

    auto node = std::make_shared<NavigateTestNode>(x, y, z, w);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"
#include "navigation_interfaces/srv/navigate_to_pose.hpp"
#include "std_msgs/msg/string.hpp"

class NavigateToPoseService : public rclcpp::Node 
{
public:
    using NavigateToPose = nav2_msgs::action::NavigateToPose;
    using GoalHandleNavigateToPose = rclcpp_action::ClientGoalHandle<NavigateToPose>;

    NavigateToPoseService() : Node("navigate_to_pose_service") {
        // 創建 Service
        service_ = this->create_service<navigation_interfaces::srv::NavigateToPose>(
            "/navigate_to_pose_service",
            std::bind(&NavigateToPoseService::handle_nav_request, this, std::placeholders::_1, std::placeholders::_2));

        // 創建導航 Action 客戶端
        action_client_ = rclcpp_action::create_client<NavigateToPose>(this, "/navigate_to_pose");

        // 創建導航狀態的 Publisher
        nav_status_publisher_ = this->create_publisher<std_msgs::msg::String>("/navigation_status", 10);
        
        // 等待導航 Action 服務啟動
        if (!action_client_->wait_for_action_server(std::chrono::seconds(5))) {
            RCLCPP_ERROR(this->get_logger(), "無法連接到導航伺服器！");
        }
    }

private:
    rclcpp::Service<navigation_interfaces::srv::NavigateToPose>::SharedPtr service_;
    rclcpp_action::Client<NavigateToPose>::SharedPtr action_client_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr nav_status_publisher_;
    
    void handle_nav_request(
        const std::shared_ptr<navigation_interfaces::srv::NavigateToPose::Request> request,
        std::shared_ptr<navigation_interfaces::srv::NavigateToPose::Response> response)
    {
        RCLCPP_INFO(this->get_logger(), "收到導航請求: x=%.2f, y=%.2f, z=%.2f, w=%.2f",
                    request->x, request->y, request->z, request->w);
    
        // 1️⃣ 設定導航目標
        auto goal_msg = NavigateToPose::Goal();
        goal_msg.pose.header.frame_id = "map"; // 使用 map frame
        goal_msg.pose.header.stamp = this->get_clock()->now();
        goal_msg.pose.pose.position.x = request->x;
        goal_msg.pose.pose.position.y = request->y;
        goal_msg.pose.pose.orientation.z = request->z;
        goal_msg.pose.pose.orientation.w = request->w;
    
        // 2️⃣ 設定導航目標的 callback
        auto send_goal_options = rclcpp_action::Client<NavigateToPose>::SendGoalOptions();
    
        send_goal_options.goal_response_callback =
            [this](std::shared_ptr<GoalHandleNavigateToPose> goal_handle) {
                if (goal_handle) {
                    RCLCPP_INFO(this->get_logger(), "目標點已被伺服器接受");
                } else {
                    RCLCPP_ERROR(this->get_logger(), "目標點被伺服器拒絕");
                }
            };
    
        send_goal_options.feedback_callback =
            [this](GoalHandleNavigateToPose::SharedPtr,
                   const std::shared_ptr<const NavigateToPose::Feedback> feedback) {
                RCLCPP_INFO(this->get_logger(), "反饋剩餘距離: %.2f", feedback->distance_remaining);
            };

        // **3️⃣ 設定導航結果的 callback**
        send_goal_options.result_callback =
            [this, response](const GoalHandleNavigateToPose::WrappedResult &result) {
                auto status_msg = std_msgs::msg::String();
                if (result.code == rclcpp_action::ResultCode::SUCCEEDED) {
                    RCLCPP_INFO(this->get_logger(), "導航成功！");
                    response->success = true;
                    response->message = "導航成功";
                    status_msg.data = "導航成功"; // 🆕 發布成功訊息
                } else {
                    RCLCPP_ERROR(this->get_logger(), "導航失敗！");
                    response->success = false;
                    response->message = "導航失敗";
                    status_msg.data = "導航失敗"; // 🆕 發布失敗訊息
                }
                nav_status_publisher_->publish(status_msg); // 🆕 發布導航狀態
            };

        // **4️⃣ 發送導航目標**
        action_client_->async_send_goal(goal_msg, send_goal_options);
        RCLCPP_INFO(this->get_logger(), "導航目標已發送...");
    }
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<NavigateToPoseService>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

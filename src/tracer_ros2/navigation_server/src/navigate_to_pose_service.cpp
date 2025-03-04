#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"
#include "navigation_interfaces/srv/navigate_to_pose.hpp"

class NavigateToPoseService : public rclcpp::Node 
{
public:
    using NavigateToPose = nav2_msgs::action::NavigateToPose;
    using GoalHandleNavigateToPose = rclcpp_action::ClientGoalHandle<NavigateToPose>;

    NavigateToPoseService() : Node("navigate_to_pose_service") {
        // 創建 ROS 2 Service
        service_ = this->create_service<navigation_interfaces::srv::NavigateToPose>(
            "/navigate_to_pose_service",
            std::bind(&NavigateToPoseService::handle_request, this, std::placeholders::_1, std::placeholders::_2));

        // 創建導航 Action 客戶端
        action_client_ = rclcpp_action::create_client<NavigateToPose>(this, "/navigate_to_pose");

        // 等待導航 Action 服務啟動
        if (!action_client_->wait_for_action_server(std::chrono::seconds(5))) {
            RCLCPP_ERROR(this->get_logger(), "無法連接到導航伺服器！");
        }
    }

private:
    rclcpp::Service<navigation_interfaces::srv::NavigateToPose>::SharedPtr service_;
    rclcpp_action::Client<NavigateToPose>::SharedPtr action_client_;

    // void handle_request(
    //     const std::shared_ptr<navigation_interfaces::srv::NavigateToPose::Request> request,
    //     std::shared_ptr<navigation_interfaces::srv::NavigateToPose::Response> response) 
    // {
    //     RCLCPP_INFO(this->get_logger(), "收到導航請求: x=%.2f, y=%.2f, z=%.2f, w=%.2f", 
    //                 request->x, request->y, request->z, request->w);

    //     // 建立導航目標
    //     auto goal_msg = NavigateToPose::Goal();
    //     goal_msg.pose.header.frame_id = "map";  // 使用 map frame
    //     goal_msg.pose.header.stamp = this->get_clock()->now();
    //     goal_msg.pose.pose.position.x = request->x;
    //     goal_msg.pose.pose.position.y = request->y;
    //     goal_msg.pose.pose.orientation.z = request->z;
    //     goal_msg.pose.pose.orientation.w = request->w;

    //     // 設定導航請求的 callback
    //     auto send_goal_options = rclcpp_action::Client<NavigateToPose>::SendGoalOptions();
    //     send_goal_options.result_callback = 
    //         [response](const GoalHandleNavigateToPose::WrappedResult & result) {
    //             if (result.code == rclcpp_action::ResultCode::SUCCEEDED) {
    //                 response->success = true;
    //                 response->message = "導航成功";
    //             } else {
    //                 response->success = false;
    //                 response->message = "導航失敗";
    //             }
    //         };

    //     // 發送導航目標
    //     action_client_->async_send_goal(goal_msg, send_goal_options);
    // }
    void handle_request(
        const std::shared_ptr<navigation_interfaces::srv::NavigateToPose::Request> request,
        std::shared_ptr<navigation_interfaces::srv::NavigateToPose::Response> response) 
    {
        RCLCPP_INFO(this->get_logger(), "收到導航請求: x=%.2f, y=%.2f, z=%.2f, w=%.2f", 
                    request->x, request->y, request->z, request->w);
    
        // 1️⃣ 建立導航目標 (goal_msg)
        auto goal_msg = NavigateToPose::Goal();
        goal_msg.pose.header.frame_id = "map";  // 使用 map frame
        goal_msg.pose.header.stamp = this->get_clock()->now();
        goal_msg.pose.pose.position.x = request->x;
        goal_msg.pose.pose.position.y = request->y;
        goal_msg.pose.pose.orientation.z = request->z;
        goal_msg.pose.pose.orientation.w = request->w;
    
        // 2️⃣ 創建 promise，允許我們等待導航完成
        auto promise = std::make_shared<std::promise<void>>();
        auto future = promise->get_future(); // 獲取 future 物件
    
        // 3️⃣ 設定導航請求的 callback
        auto send_goal_options = rclcpp_action::Client<NavigateToPose>::SendGoalOptions();
        send_goal_options.result_callback = 
            [response, promise](const GoalHandleNavigateToPose::WrappedResult & result) {
                RCLCPP_INFO(rclcpp::get_logger("navigate_to_pose_service"), "收到導航結果回應！");
                if (result.code == rclcpp_action::ResultCode::SUCCEEDED) {
                    response->success = true;
                    response->message = "導航成功";
                } else {
                    response->success = false;
                    response->message = "導航失敗";
                }
                promise->set_value();  // 通知 main thread 結束等待
            };
    
        // 4️⃣ 發送導航目標
        auto goal_handle_future = action_client_->async_send_goal(goal_msg, send_goal_options);
            
        RCLCPP_INFO(this->get_logger(), "等待導航結果...");
        // 5️⃣ 等待導航結果（最多 60 秒）
        if (future.wait_for(std::chrono::seconds(180)) == std::future_status::timeout) {
            response->success = false;
            response->message = "導航超時";
            RCLCPP_ERROR(this->get_logger(), "導航超時");
        }
        RCLCPP_INFO(this->get_logger(), "導航結果收到，準備返回 Service 回應");
    }
    
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<NavigateToPoseService>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}


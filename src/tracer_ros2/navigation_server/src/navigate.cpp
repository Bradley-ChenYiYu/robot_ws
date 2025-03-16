#include <memory>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"

class NavigateToPoseClient : public rclcpp::Node 
{
public:
    using NavigateToPose = nav2_msgs::action::NavigateToPose;
    using GoalHandleNavigateToPose = rclcpp_action::ClientGoalHandle<NavigateToPose>;

    NavigateToPoseClient(double x, double y, double z, double w) 
        : Node("navigate_to_pose_client"), x_(x), y_(y), z_(z), w_(w) 
    {
        // 創建導航 Action 客戶端
        action_client_ = rclcpp_action::create_client<NavigateToPose>(this, "/navigate_to_pose");

        // 等待導航 Action 服務啟動
        if (!action_client_->wait_for_action_server(std::chrono::seconds(5))) {
            RCLCPP_ERROR(this->get_logger(), "無法連接到導航伺服器！");
            return;
        }

        // 發送導航請求
        send_goal();
    }

private:
    rclcpp_action::Client<NavigateToPose>::SharedPtr action_client_;
    double x_, y_, z_, w_;

    void send_goal()
    {
        RCLCPP_INFO(this->get_logger(), "發送導航請求: x=%.2f, y=%.2f, z=%.2f, w=%.2f", x_, y_, z_, w_);
    
        // 設定導航目標
        auto goal_msg = NavigateToPose::Goal();
        goal_msg.pose.header.frame_id = "map"; // 使用 map frame
        goal_msg.pose.header.stamp = this->get_clock()->now();
        goal_msg.pose.pose.position.x = x_;
        goal_msg.pose.pose.position.y = y_;
        goal_msg.pose.pose.orientation.z = z_;
        goal_msg.pose.pose.orientation.w = w_;
    
        // 設定導航回調函數
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
                RCLCPP_INFO(this->get_logger(), "剩餘距離: %.2f", feedback->distance_remaining);
            };

        send_goal_options.result_callback =
            [this](const GoalHandleNavigateToPose::WrappedResult &result) {
                if (result.code == rclcpp_action::ResultCode::SUCCEEDED) {
                    RCLCPP_INFO(this->get_logger(), "導航成功！");
                } else {
                    RCLCPP_ERROR(this->get_logger(), "導航失敗！");
                }
                rclcpp::shutdown(); // 任務完成後關閉節點
            };

        // 發送導航目標
        action_client_->async_send_goal(goal_msg, send_goal_options);
        RCLCPP_INFO(this->get_logger(), "導航目標已發送...");
    }
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);

    if (argc < 5) {
        std::cerr << "使用方式: ros2 run control_node navigate_to_pose_client <x> <y> <z> <w>" << std::endl;
        return 1;
    }

    double x = std::stod(argv[1]);
    double y = std::stod(argv[2]);
    double z = std::stod(argv[3]);
    double w = std::stod(argv[4]);

    auto node = std::make_shared<NavigateToPoseClient>(x, y, z, w);
    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}

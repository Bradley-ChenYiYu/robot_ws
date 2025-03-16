#include <memory>
#include <string>
#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "navigation_interfaces/srv/navigate_to_pose.hpp"

class NavigateToPoseClient : public rclcpp::Node {
public:
    NavigateToPoseClient() : Node("navigate_to_pose_client"), navigation_done_(false) {
        client_ = this->create_client<navigation_interfaces::srv::NavigateToPose>("/navigate_to_pose_service");

        // 訂閱導航狀態
        status_subscriber_ = this->create_subscription<std_msgs::msg::String>(
            "/navigation_status", 10, std::bind(&NavigateToPoseClient::status_callback, this, std::placeholders::_1));
    }

    void send_navigation_request(double x, double y, double z, double w) {
        while (!client_->wait_for_service(std::chrono::seconds(3))) {
            RCLCPP_WARN(this->get_logger(), "等待導航服務...");
        }

        auto request = std::make_shared<navigation_interfaces::srv::NavigateToPose::Request>();
        request->x = x;
        request->y = y;
        request->z = z;
        request->w = w;

        auto future = client_->async_send_request(request);
        RCLCPP_INFO(this->get_logger(), "已發送導航請求: x=%.2f, y=%.2f, z=%.2f, w=%.2f", x, y, z, w);

        navigation_done_ = false; // 重置導航完成標誌
    }

    bool is_navigation_done() {
        return navigation_done_;
    }

private:
    rclcpp::Client<navigation_interfaces::srv::NavigateToPose>::SharedPtr client_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr status_subscriber_;
    bool navigation_done_;

    void status_callback(const std_msgs::msg::String::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "導航狀態更新: %s", msg->data.c_str());

        if (msg->data == "導航成功") {
            RCLCPP_INFO(this->get_logger(), "導航成功！");
            navigation_done_ = true;
        }
    }
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<NavigateToPoseClient>();

    // 創建多線程 Executor，讓回呼函數持續運行
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);

    // 發送導航請求
    node->send_navigation_request(0.048, 2.64, 0.17, 1.0);
    // node->send_navigation_request(-5.27, -0.54, 0.17, 1.0);

    // 🔄 這樣 while 迴圈會等待導航完成，但程式不會結束
    while (rclcpp::ok()) {
        executor.spin_some(); // 讓回呼函數能夠執行

        if (node->is_navigation_done()) {
            RCLCPP_INFO(node->get_logger(), "導航完成，等待新的指令...");
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    node->send_navigation_request(-5.27, -0.54, 0.17, 1.0);
    while (rclcpp::ok()) {
        executor.spin_some(); // 讓回呼函數能夠執行

        if (node->is_navigation_done()) {
            RCLCPP_INFO(node->get_logger(), "導航完成，等待新的指令...");
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    rclcpp::shutdown();
    return 0;
}

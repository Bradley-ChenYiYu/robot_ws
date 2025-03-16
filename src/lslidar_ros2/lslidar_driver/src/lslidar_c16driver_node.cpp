#include "lslidar_driver/lslidar_driver.h"
#include "std_msgs/msg/string.h"

using namespace lslidar_driver;
volatile sig_atomic_t flag = 1;

static void my_handler(int sig) {
    flag = 0;
}

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    signal(SIGINT, my_handler);
    lidar_type = "c16";
//    RCLCPP_ERROR(node->get_logger(), "*************argc %d",len);
//    RCLCPP_ERROR(node->get_logger(), "*************lidar_type %s",lidar_type.c_str());
//    RCLCPP_ERROR(node->get_logger(), "*************argv[0] %s",s1.c_str());
//    RCLCPP_ERROR(node->get_logger(), "*************argv[1] %s",s2.c_str());
//    RCLCPP_ERROR(node->get_logger(), "*************argv[2] %s",s3.c_str());
//    RCLCPP_ERROR(node->get_logger(), "*************argv[3] %s",s4.c_str());
//    auto node = std::make_shared<lslidar_driver::lslidarC32Driver>();
//    std::shared_ptr<lslidar_driver::lslidarDriver> node = std::make_shared<lslidar_driver::lslidarC32Driver>();
//    node->get_parameter("lidar_type", lidar_type);
//    printf("lslidar type: %s", lidar_type.c_str());

    // start the driver
    if ("c16" == lidar_type) {
        auto node = std::make_shared<lslidar_driver::lslidarC16Driver>();
        if (!node->initialize()) {
            RCLCPP_ERROR(node->get_logger(), "cannot initialize lslidar driver.");
            return 0;
        }
        while (rclcpp::ok()) {
            node->poll();
        }
        rclcpp::shutdown();

    } else {
        auto node = std::make_shared<lslidar_driver::lslidarC32Driver>();
        if (!node->initialize()) {
            RCLCPP_ERROR(node->get_logger(), "cannot initialize lslidar driver.");
            return 0;
        }
        while (rclcpp::ok()) {
            node->poll();
        }
        rclcpp::shutdown();
    }
    return 0;
}

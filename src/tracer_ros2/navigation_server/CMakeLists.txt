cmake_minimum_required(VERSION 3.8)
project(navigation_server)

# 設定 C++ 標準
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)  # 確保使用 C++17
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 找到 ROS 2 依賴
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(nav2_msgs REQUIRED)
find_package(navigation_interfaces REQUIRED)  # **確保這行存在**

# **確保包含 `navigation_interfaces` 生成的標頭檔**
include_directories(
  ${navigation_interfaces_INCLUDE_DIRS}
)

# 新增可執行檔
add_executable(navigate_to_pose_service src/navigate_to_pose_service.cpp)
ament_target_dependencies(navigate_to_pose_service rclcpp rclcpp_action nav2_msgs navigation_interfaces)

# 安裝執行檔
install(TARGETS
  navigate_to_pose_service
  DESTINATION lib/${PROJECT_NAME}
)

# **確保 `navigation_interfaces` 依賴可以被其他 package 使用**
ament_export_dependencies(navigation_interfaces)

ament_package()

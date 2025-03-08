cmake_minimum_required(VERSION 3.8)
project(lite6_handeye)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 1. **查找依賴項**
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(xarm_msgs REQUIRED)

# 2. **包含標頭文件**
include_directories(
  include
  ${OpenCV_INCLUDE_DIRS}
)

# 3. **建立可執行檔**
add_executable(lite6_handeye_node src/lite6_handeye.cpp)
target_link_libraries(lite6_handeye_node yaml-cpp)

add_executable(lite6_handeye_node2 src/lite6_handeye2.cpp)
target_link_libraries(lite6_handeye_node2 yaml-cpp)

add_executable(lite6_handeye_node3 src/lite6_handeye3.cpp)
target_link_libraries(lite6_handeye_node3 yaml-cpp)

# 4. **設定目標相依性**
ament_target_dependencies(lite6_handeye_node 
  rclcpp
  sensor_msgs
  cv_bridge
  OpenCV
  yaml-cpp
  xarm_msgs
)

ament_target_dependencies(lite6_handeye_node2 
  rclcpp
  sensor_msgs
  cv_bridge
  OpenCV
  yaml-cpp
  xarm_msgs
)

ament_target_dependencies(lite6_handeye_node3 
  rclcpp
  sensor_msgs
  cv_bridge
  OpenCV
  yaml-cpp
  xarm_msgs
)

# 5. **安裝節點**
install(TARGETS
  lite6_handeye_node
  lite6_handeye_node2
  lite6_handeye_node3
  DESTINATION lib/${PROJECT_NAME}
)

# 6. **測試**
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# 7. **輸出為 ROS 2 套件**
ament_package()


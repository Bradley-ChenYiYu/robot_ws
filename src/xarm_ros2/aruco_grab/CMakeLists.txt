cmake_minimum_required(VERSION 3.8)
project(aruco_grab)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 找到 ROS 2 相關套件
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
find_package(xarm_msgs REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(yaml-cpp REQUIRED)  # ✅ 加入 YAML 依賴

# 設定 Include 目錄
include_directories(
  include
  ${EIGEN3_INCLUDE_DIR}
  ${OpenCV_INCLUDE_DIRS}
)

# **建立 `aruco_grab_node`**
add_executable(aruco_grab_node src/aruco_grab.cpp)
add_executable(boxhole_grab_node src/boxhole_grab.cpp)

# **設定 `ament_target_dependencies`**
ament_target_dependencies(aruco_grab_node
  rclcpp sensor_msgs cv_bridge image_transport xarm_msgs OpenCV Eigen3 yaml-cpp  # ✅ 加入 yaml-cpp 依賴
)

ament_target_dependencies(boxhole_grab_node
  rclcpp sensor_msgs cv_bridge image_transport xarm_msgs OpenCV Eigen3 yaml-cpp  # ✅ 加入 yaml-cpp 依賴
)

# **明確鏈接 YAML-cpp 函式庫**
target_link_libraries(aruco_grab_node yaml-cpp)  # ✅ 這行非常重要！
target_link_libraries(boxhole_grab_node yaml-cpp)  # ✅ 這行非常重要！

# **安裝執行檔**
install(TARGETS
  aruco_grab_node
  boxhole_grab_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()

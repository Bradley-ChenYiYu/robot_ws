cmake_minimum_required(VERSION 3.8)
project(medicine_grabber)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# === [Find dependencies] ===
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(image_transport REQUIRED)
find_package(xarm_msgs REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(yaml-cpp REQUIRED)

# === [Include directories] ===
include_directories(
  include
  ${EIGEN3_INCLUDE_DIR}
  ${OpenCV_INCLUDE_DIRS}
)

# === [Executable node] ===
add_executable(medicine_grabber_node src/medicine_grabber.cpp)

ament_target_dependencies(medicine_grabber_node
  rclcpp
  sensor_msgs
  cv_bridge
  image_transport
  xarm_msgs
  OpenCV
  Eigen3
  yaml-cpp
)

# === [Link external libraries (if needed)] ===
target_link_libraries(medicine_grabber_node yaml-cpp)

# === [Install target] ===
install(TARGETS medicine_grabber_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()

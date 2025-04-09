cmake_minimum_required(VERSION 3.8)
project(json_file_watcher)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(xarm_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(jsoncpp REQUIRED)

include_directories(/usr/include/jsoncpp)

# 新增執行檔
add_executable(json_file_watcher src/json_file_watcher.cpp)
ament_target_dependencies(json_file_watcher rclcpp xarm_msgs std_msgs jsoncpp)

# **修正點：手動連結 jsoncpp**
target_link_libraries(json_file_watcher /usr/lib/x86_64-linux-gnu/libjsoncpp.so)

# 安裝執行檔，讓 ros2 run 能找到
install(TARGETS json_file_watcher
  DESTINATION lib/${PROJECT_NAME}
)


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

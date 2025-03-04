cmake_minimum_required(VERSION 3.8)
project(navigation_interfaces)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 找到 ROS 2 依賴
find_package(ament_cmake REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# 定義 Service 檔案
set(srv_files
  "srv/NavigateToPose.srv"
)

# 產生介面（Service）
rosidl_generate_interfaces(${PROJECT_NAME}
  ${srv_files}
  DEPENDENCIES std_msgs
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# 確保 `rosidl_default_runtime` 可以被其他 package 找到
ament_export_dependencies(rosidl_default_runtime)

# `ament_package()` 必須放在最後
ament_package()

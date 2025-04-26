#!/bin/bash

# 設定 ROS 2 workspace 路徑
ROS2_WS=~/robot_ws

# 啟動 Web ngrok
gnome-terminal -- bash -c "
cd ~/robot_ws
ngrok start --all
exec bash
"

sleep 1

# 啟動 Web Server
gnome-terminal -- bash -c "
cd ~/robot_ws
python3 web/app.py
exec bash
"

sleep 1

# # 啟動 Sensor - tracer_base sensor.xml
# gnome-terminal -- bash -c "
# source $ROS2_WS/install/setup.bash
# ros2 launch tracer_base sensor.xml
# exec bash
# "

# sleep 1

# # 啟動 Sensor - realsense driver
# gnome-terminal -- bash -c "
# source $ROS2_WS/install/setup.bash
# ros2 launch realsense2_camera rs_launch.py
# exec bash
# "

# sleep 1

# # 啟動 機械手臂 driver + 初始化
# gnome-terminal -- bash -c "
# source $ROS2_WS/install/setup.bash
# ros2 launch xarm_api lite6_driver.launch.py robot_ip:=192.168.1.166 add_gripper:=true;
# sleep 3;
# ros2 service call /ufactory/motion_enable xarm_msgs/srv/SetInt16ById \"{id: 8, data: 1}\";
# sleep 1;
# ros2 service call /ufactory/set_mode xarm_msgs/srv/SetInt16 \"{data: 0}\";
# sleep 1;
# ros2 service call /ufactory/set_state xarm_msgs/srv/SetInt16 \"{data: 0}\";
# exec bash
# "

# sleep 1

# # 啟動 導航
# gnome-terminal -- bash -c "
# source $ROS2_WS/install/setup.bash
# ros2 launch tracer_base amcl_nav.launch.py
# exec bash
# "


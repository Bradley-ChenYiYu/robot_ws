#!/bin/bash

# 設定 ROS 2 workspace 路徑
ROS2_WS=~/robot_ws

# 啟動 Web ngrok
gnome-terminal --title="ngrok" -- bash -c "
cd ~/robot_ws
ngrok start --all
"

sleep 1

# 啟動 Web Server
gnome-terminal --title="Web Server" -- bash -c "
cd ~/robot_ws
python3 web/app.py
"

sleep 1

# 啟動 Webcam WebSocket Node
gnome-terminal --title="Webcam WS Node" -- bash -c "
cd ~/robot_ws
python3 web/webcam_websocket_node.py
"

sleep 1

# 啟動 Sensor - tracer_base sensor.xml
gnome-terminal --title="tracer_base sensor" -- bash -c "
source $ROS2_WS/install/setup.bash
ros2 launch tracer_base sensor.xml
"

sleep 1

# 啟動 Sensor - realsense driver
gnome-terminal --title="Realsense Driver" -- bash -c "
source $ROS2_WS/install/setup.bash
ros2 launch realsense2_camera rs_launch.py
"

sleep 1

# 啟動 機械手臂 driver + 初始化 + move_arm 到移動位置
gnome-terminal --title="xArm Driver" -- bash -c "
source $ROS2_WS/install/setup.bash
ros2 launch xarm_api lite6_driver.launch.py robot_ip:=192.168.1.166 add_gripper:=true
"

gnome-terminal --title="xArm Init" -- bash -c "
sleep 3
source $ROS2_WS/install/setup.bash
ros2 service call /ufactory/motion_enable xarm_msgs/srv/SetInt16ById \"{id: 8, data: 1}\"
sleep 2
ros2 service call /ufactory/set_mode xarm_msgs/srv/SetInt16 \"{data: 0}\"
sleep 2
ros2 service call /ufactory/set_state xarm_msgs/srv/SetInt16 \"{data: 0}\"
"

sleep 1

# 啟動 導航
gnome-terminal --title="Navigation (AMCL)" -- bash -c "
source $ROS2_WS/install/setup.bash
ros2 launch tracer_base amcl_nav.launch.py
exec bash
"

import os
from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    # YAML 檔案的絕對路徑（開發中建議用這個，省去 colcon build）
    config_dir = '/home/jason9308/robot_ws/src/tracer_ros2/tracer_base/config'
    rviz_config_file_path = '/home/jason9308/robot_ws/src/tracer_ros2/tracer_base/rviz_config/pathplanning.rviz'
    map_yaml_file = '/home/jason9308/robot_ws/Map/1Frtabmap.yaml'

    controller_yaml = os.path.join(config_dir, 'controller.yaml')
    planner_yaml = os.path.join(config_dir, 'planner_server.yaml')
    recovery_yaml = os.path.join(config_dir, 'recovery.yaml')
    bt_navigator_yaml = os.path.join(config_dir, 'bt_navigator.yaml')
    nav2_yaml = os.path.join(config_dir, 'amcl_config.yaml')
    waypoint_follower_yaml = os.path.join(config_dir, 'waypoint_follower.yaml')
    velocity_smoother_yaml = os.path.join(config_dir, 'velocity_smoother.yaml')
    smoother_server_yaml = os.path.join(config_dir, 'smoother_server.yaml')
    default_bt_xml_path = os.path.join(config_dir, 'behavior.xml')

    remappings = [('/tf', 'tf'),
                  ('/tf_static', 'tf_static')]

    return LaunchDescription([
        Node(
            package='nav2_map_server',
            executable='map_server',
            name='map_server',
            output='screen',
            parameters=[{'yaml_filename': map_yaml_file}]
        ),

        Node(
            package='nav2_amcl',
            executable='amcl',
            name='amcl',
            output='screen',
            parameters=[nav2_yaml]
        ),
        Node(
            package='nav2_controller',
            executable='controller_server',
            name='controller_server',
            output='screen',
            parameters=[controller_yaml]),

        Node(
            package='nav2_planner',
            executable='planner_server',
            name='planner_server',
            output='screen',
            parameters=[planner_yaml]),

        Node(
            package='nav2_behaviors',
            executable='behavior_server',
            name='behavior_server',
            output='screen',
            parameters=[recovery_yaml]),

        Node(
            package='nav2_bt_navigator',
            executable='bt_navigator',
            name='bt_navigator',
            output='screen',
            parameters=[bt_navigator_yaml, {'default_bt_xml_filename': default_bt_xml_path}]),

        Node(
            package='rviz2',
            executable='rviz2',
            output='screen',
            name='rviz2_node',
            arguments=['-d', rviz_config_file_path]),

        Node(
            package='nav2_waypoint_follower',
            executable='waypoint_follower',
            name='waypoint_follower',
            output='screen',
            parameters=[waypoint_follower_yaml]),

        Node(
            package='nav2_velocity_smoother',
            executable='velocity_smoother',
            name='velocity_smoother',
            output='screen',
            parameters=[velocity_smoother_yaml],
            remappings=remappings + [('cmd_vel', 'cmd_vel_nav'), ('cmd_vel_smoothed', 'cmd_vel')]),

        Node(
            package='nav2_smoother',
            executable='smoother_server',
            name='smoother_server',
            output='screen',
            parameters=[smoother_server_yaml],
            remappings=remappings),

        Node(
            package='nav2_lifecycle_manager',
            executable='lifecycle_manager',
            name='lifecycle_manager_navigation',
            output='screen',
            parameters=[{'autostart': True},
                        {'node_names': ['map_server',
                                        'amcl',
                                        'controller_server',
                                        'planner_server',
                                        'behavior_server',
                                        'bt_navigator',
                                        'waypoint_follower',
                                        'smoother_server',
                                        'velocity_smoother']}]),

        Node(
            package='write_pose',
            executable='write_pose',
            name='pose_writer',
            output='screen'
        ),
    ])

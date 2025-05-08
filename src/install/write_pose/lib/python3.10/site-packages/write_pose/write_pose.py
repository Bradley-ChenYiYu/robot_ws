import rclpy
from rclpy.node import Node
from tf2_ros import TransformListener, Buffer
from geometry_msgs.msg import TransformStamped
import json
import os

class PoseWriter(Node):
    def __init__(self):
        super().__init__('pose_writer')

        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)

        self.create_timer(1.0, self.timer_callback)

        self.json_path = os.path.join(os.getenv("HOME"), 'robot_ws', 'command_jason', 'pose.json')

    def timer_callback(self):
        try:
            now = rclpy.time.Time()
            trans: TransformStamped = self.tf_buffer.lookup_transform(
                'map', 'base_link', now)

            x = trans.transform.translation.x
            y = trans.transform.translation.y
            q = trans.transform.rotation

            pose = {
                "x": round(x, 3),
                "y": round(y, 3),
                "qx": round(q.x, 6),
                "qy": round(q.y, 6),
                "qz": round(q.z, 6),
                "qw": round(q.w, 6)
            }

            with open(self.json_path, 'w') as f:
                json.dump(pose, f)

            self.get_logger().info(f'寫入位置: {pose}')

        except Exception as e:
            self.get_logger().warn(f'取得 TF 失敗: {e}')

def main(args=None):
    rclpy.init(args=args)
    node = PoseWriter()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

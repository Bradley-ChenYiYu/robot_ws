# ==== 系統模組 ====
import sys
import os
import numpy as np
import cv2
import math

# ==== ROS2 與 QoS ====
import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, ReliabilityPolicy, DurabilityPolicy
from nav_msgs.msg import OccupancyGrid
from geometry_msgs.msg import TransformStamped

# ==== TF2 ====
from tf2_ros import Buffer, TransformListener

# ==== PyQt GUI ====
from PyQt5.QtWidgets import QApplication, QLabel, QWidget, QVBoxLayout, QSizePolicy
from PyQt5.QtGui import QPixmap, QImage
from PyQt5.QtCore import QTimer

# 設定 Qt plugin 路徑（避免 Qt 載入錯誤）
os.environ["QT_QPA_PLATFORM_PLUGIN_PATH"] = "/usr/lib/x86_64-linux-gnu/qt5/plugins/platforms"

# ======== ROS2 節點：接收地圖、costmap 與 base_link pose ========
class MapViewerNode(Node):
    def __init__(self):
        super().__init__('map_viewer_node')

        latched_qos = QoSProfile(
            reliability=ReliabilityPolicy.RELIABLE,
            durability=DurabilityPolicy.TRANSIENT_LOCAL,
            depth=1
        )

        default_qos = QoSProfile(
            reliability=ReliabilityPolicy.RELIABLE,
            durability=DurabilityPolicy.VOLATILE,
            depth=10
        )

        self.subscription = self.create_subscription(
            OccupancyGrid,
            '/map',
            self.map_callback,
            latched_qos)

        self.global_costmap_sub = self.create_subscription(
            OccupancyGrid,
            '/global_costmap/costmap',
            self.global_costmap_callback,
            default_qos)

        self.local_costmap_sub = self.create_subscription(
            OccupancyGrid,
            '/local_costmap/costmap',
            self.local_costmap_callback,
            default_qos)

        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)

        self.latest_map = None
        self.global_costmap = None
        self.local_costmap = None
        self.base_link_transform = None

        self.create_timer(0.1, self.lookup_transform)

    def map_callback(self, msg):
        self.latest_map = msg

    def global_costmap_callback(self, msg):
        self.global_costmap = msg

    def local_costmap_callback(self, msg):
        self.local_costmap = msg

    def lookup_transform(self):
        try:
            trans = self.tf_buffer.lookup_transform('map', 'base_link', rclpy.time.Time())
            self.base_link_transform = trans.transform
        except Exception as e:
            self.get_logger().warn(f'Cannot get transform map -> base_link: {e}')

# ======== PyQt GUI：顯示地圖與 costmap、base_link ========
class MapViewerGUI(QWidget):
    def __init__(self, ros_node):
        super().__init__()
        self.ros_node = ros_node
        self.setWindowTitle("Map Viewer")
        self.image_label = QLabel()
        self.image_label.setScaledContents(False)
        self.image_label.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)

        layout = QVBoxLayout()
        layout.addWidget(self.image_label)
        self.setLayout(layout)

        self.timer = QTimer()
        self.timer.timeout.connect(self.update_image)
        self.timer.start(100)

    def overlay_costmap(self, base_img, costmap_msg, color, alpha):
        if costmap_msg is None:
            return base_img

        w = costmap_msg.info.width
        h = costmap_msg.info.height
        data = np.array(costmap_msg.data, dtype=np.int8).reshape((h, w))

        norm_data = np.clip(data, 0, 100).astype(np.float32)
        strength = (norm_data / 100.0) ** 2
        overlay = np.zeros((h, w, 3), dtype=np.uint8)
        overlay[:, :, 0] = (strength * color[0]).astype(np.uint8)
        overlay[:, :, 1] = (strength * color[1]).astype(np.uint8)
        overlay[:, :, 2] = (strength * color[2]).astype(np.uint8)

        overlay = cv2.flip(overlay, 0)

        map_info = self.ros_node.latest_map.info
        dx = int(round((costmap_msg.info.origin.position.x - map_info.origin.position.x) / map_info.resolution))
        dy = int(round((costmap_msg.info.origin.position.y - map_info.origin.position.y) / map_info.resolution))
        dy = base_img.shape[0] - dy - h

        if dx < 0 or dy < 0 or dx + w > base_img.shape[1] or dy + h > base_img.shape[0]:
            print("[DEBUG] Costmap overlay out of bounds")
            return base_img

        roi = base_img[dy:dy+h, dx:dx+w]
        blended = cv2.addWeighted(roi, 1.0, overlay, alpha, 0)
        base_img[dy:dy+h, dx:dx+w] = blended
        return base_img

    def draw_base_link(self, img, transform, map_info):
        if transform is None:
            return img

        res = map_info.resolution
        origin_x = map_info.origin.position.x
        origin_y = map_info.origin.position.y

        px = int((transform.translation.x - origin_x) / res)
        py = int((transform.translation.y - origin_y) / res)
        py = img.shape[0] - py

        q = transform.rotation
        siny_cosp = 2 * (q.w * q.z + q.x * q.y)
        cosy_cosp = 1 - 2 * (q.y ** 2 + q.z ** 2)
        yaw = math.atan2(siny_cosp, cosy_cosp)

        cv2.circle(img, (px, py), 5, (0, 0, 255), -1)
        arrow_length = 15
        end_x = int(px + arrow_length * math.cos(yaw))
        end_y = int(py - arrow_length * math.sin(yaw))
        cv2.arrowedLine(img, (px, py), (end_x, end_y), (0, 0, 255), 2, tipLength=0.3)
        return img

    def update_image(self):
        msg = self.ros_node.latest_map
        if msg is None:
            print("[DEBUG] No map message yet")
            return

        width = msg.info.width
        height = msg.info.height
        data = np.array(msg.data, dtype=np.int8).reshape((height, width))
        print(f"[DEBUG] Map size: {width}x{height}, unique values: {np.unique(data)}")

        img = np.zeros((height, width, 3), dtype=np.uint8)
        img[data == 0] = [255, 255, 255]
        img[data == 100] = [0, 0, 0]
        img[data == -1] = [220, 220, 220]
        img = cv2.flip(img, 0)

        img = self.overlay_costmap(img, self.ros_node.global_costmap, [128, 0, 255], 0.4)
        img = self.overlay_costmap(img, self.ros_node.local_costmap, [0, 255, 255], 0.9)
        img = self.draw_base_link(img, self.ros_node.base_link_transform, msg.info)

        qimg = QImage(img.data, img.shape[1], img.shape[0], 3 * img.shape[1], QImage.Format_RGB888)
        pixmap = QPixmap.fromImage(qimg)
        self.image_label.setPixmap(pixmap)
        self.image_label.resize(pixmap.size())

# ======== 主程式入口 ========
def main():
    rclpy.init()
    ros_node = MapViewerNode()
    app = QApplication(sys.argv)
    gui = MapViewerGUI(ros_node)
    gui.show()

    timer = QTimer()
    def spin_once():
        if rclpy.ok():
            rclpy.spin_once(ros_node, timeout_sec=0.01)
    timer.timeout.connect(spin_once)
    timer.start(10)

    app.exec_()
    ros_node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

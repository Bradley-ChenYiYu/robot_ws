#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2

class CrosshairOverlayNode(Node):
    def __init__(self):
        super().__init__('crosshair_overlay_node')

        # 訂閱原始影像的 topic
        self.subscription = self.create_subscription(
            Image,
            '/camera/camera/color/image_raw',  # 確保這是你 RealSense 發布的影像話題
            self.image_callback,
            10
        )

        # 發布新的影像 topic
        self.publisher_ = self.create_publisher(Image, '/camera/camera/color/image_crosshair', 10)

        self.br = CvBridge()
        self.get_logger().info('CrosshairOverlayNode 已啟動，開始處理影像')

    def image_callback(self, msg):
        # 將 ROS 影像轉成 OpenCV 格式
        cv_img = self.br.imgmsg_to_cv2(msg, desired_encoding='bgr8')

        # 取得影像尺寸
        height, width, _ = cv_img.shape
        center_x = width // 2
        center_y = height // 2

        # 畫十字線
        cv2.line(cv_img, (0, center_y), (width, center_y), (0, 0, 255), 2)  # 水平線 (紅色)
        cv2.line(cv_img, (center_x, 0), (center_x, height), (0, 0, 255), 2)  # 垂直線 (紅色)

        # 轉回 ROS 影像並發布
        new_msg = self.br.cv2_to_imgmsg(cv_img, encoding='bgr8')
        new_msg.header = msg.header  # 保留原始時間戳記
        self.publisher_.publish(new_msg)

def main(args=None):
    rclpy.init(args=args)
    node = CrosshairOverlayNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    rclpy.shutdown()

if __name__ == '__main__':
    main()


import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
from std_msgs.msg import Float32MultiArray
import cv2
import numpy as np
from ultralytics import YOLO


class HandDetectionNode(Node):
    def __init__(self):
        super().__init__('hand_detection_node')

        # 設定 topic 名稱
        self.rgb_topic = '/camera/camera/color/image_raw'
        self.depth_topic = '/camera/camera/aligned_depth_to_color/image_raw'

        # 初始化模型
        self.model = YOLO('/home/jason9308/project/ultralytics/archive/runs/detect/train9/weights/best.pt')  # 替換為你的 yolov10.pt 路徑
        self.bridge = CvBridge()
        self.depth_image = None

        # 相機參數（請替換為實際值或從 camera_info 取得）
        self.fx = 903.4482687068248
        self.fy = 903.6795021293382
        self.cx = 648.7349652476531
        self.cy = 380.5389552131255

        # 訂閱影像與深度
        self.create_subscription(Image, self.rgb_topic, self.image_callback, 10)
        self.create_subscription(Image, self.depth_topic, self.depth_callback, 10)

        # 發布手部中心點（3D）座標：x, y, z
        self.publisher = self.create_publisher(Float32MultiArray, '/hand_detection', 10)

        self.get_logger().info('✅ Hand detection node started.')

    def depth_callback(self, msg):
        try:
            self.depth_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding='passthrough')
        except Exception as e:
            self.get_logger().warn(f"Depth convert error: {e}")

    def image_callback(self, msg):
        if self.depth_image is None:
            return

        try:
            rgb_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')
        except Exception as e:
            self.get_logger().warn(f"RGB convert error: {e}")
            return

        # 執行 YOLO 偵測
        results = self.model(rgb_image)
        best_conf = 0.0
        best_box = None

        for det in results[0].boxes:
            if int(det.cls[0]) == 0:  # 偵測的是手（class 0）
                conf = float(det.conf[0])
                if conf > best_conf:
                    best_conf = conf
                    best_box = det.xyxy[0].cpu().numpy()

        if best_box is not None:
            x1, y1, x2, y2 = best_box
            cx = int((x1 + x2) / 2)
            cy = int((y1 + y2) / 2)

            if 0 <= cx < self.depth_image.shape[1] and 0 <= cy < self.depth_image.shape[0]:
                z = self.depth_image[cy, cx]
                if z > 0.0 and z < 2000.0:
                    # 深度單位為 mm，轉為 camera frame 下的 x, y, z
                    x = (cx - self.cx) / self.fx * z
                    y = (cy - self.cy) / self.fy * z

                    msg_out = Float32MultiArray()
                    msg_out.data = [float(x), float(y), float(z)]
                    self.publisher.publish(msg_out)
                    self.get_logger().info(f"📤 Hand @ x={x:.1f}, y={y:.1f}, z={z:.1f} mm")


def main(args=None):
    rclpy.init(args=args)
    node = HandDetectionNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()

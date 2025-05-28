import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2
import base64
import threading
from websocket_server import WebsocketServer

class ImageWebsocketBridge(Node):
    def __init__(self):
        super().__init__('image_websocket_bridge')
        self.bridge = CvBridge()
        self.ws_clients = []  # 用來儲存 WebSocket 客戶端
        self.frame_count = 0  # 用來統計傳輸的幀數
        
        # 訂閱相機影像訊息
        self.subscriber = self.create_subscription(
            Image,
            '/camera/camera/color/image_raw',
            self.image_callback,
            10)
        
        # 設置 WebSocket 伺服器
        self.server = WebsocketServer(host='0.0.0.0', port=8765)
        self.server.set_fn_new_client(self.new_client)
        self.server.set_fn_client_left(self.client_left)
        
        # 啟動 WebSocket 伺服器
        threading.Thread(target=self.server.run_forever, daemon=True).start()
        self.get_logger().info("WebSocket Server started on port 8765")

    def new_client(self, client, server):
        """處理新客戶端連接"""
        self.ws_clients.append(client)

    def client_left(self, client, server):
        """處理客戶端斷開連接"""
        self.ws_clients.remove(client)

    def image_callback(self, msg):
        """處理接收到的影像訊息"""
        try:
            self.frame_count += 1
            
            # 每 3 幀傳送 1 幀
            if self.frame_count % 6 != 0:
                return
            
            # 轉換影像格式
            cv_image = self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')
            
            # 調整影像解析度 (可依需求調整)
            cv_image = cv2.resize(cv_image, (320, 240))  # 調整為 320x240
            
            # 壓縮成 JPEG 格式並降低畫質（數值越小壓縮越大，速度越快）
            _, jpeg = cv2.imencode('.jpg', cv_image, [int(cv2.IMWRITE_JPEG_QUALITY), 40])
            
            # 將影像編碼成 Base64 格式
            b64_data = base64.b64encode(jpeg.tobytes()).decode('utf-8')
            
            # 將影像發送給所有連接的 WebSocket 客戶端
            for client in self.ws_clients:
                self.server.send_message(client, b64_data)
        
        except Exception as e:
            self.get_logger().error(f"Error converting image: {e}")

def main(args=None):
    """ROS 2 節點執行入口"""
    rclpy.init(args=args)
    node = ImageWebsocketBridge()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

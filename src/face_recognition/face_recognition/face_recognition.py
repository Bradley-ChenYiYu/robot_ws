import rclpy
from rclpy.node import Node
import cv2
import torch
import numpy as np
from ultralytics import YOLO
from cv_bridge import CvBridge, CvBridgeError
from sensor_msgs.msg import Image
from std_msgs.msg import String
import dlib
from imutils.face_utils import FaceAligner
from facenet_pytorch import InceptionResnetV1
import sys

class FaceRecognitionNode(Node):
    def __init__(self, identity_name):
        super().__init__('face_recognition_node')

        self.bridge = CvBridge()
        self.image_sub = self.create_subscription(
            Image, "/camera/camera/color/image_raw", self.image_callback, 10
        )
        self.result_pub = self.create_publisher(String, '/face_recognition/result', 10)

        torch.cuda.empty_cache()

        self.face_detector = YOLO('/home/jason9308/robot_ws/src/face_recognition/best.pt')

        # Dlib landmark model and face aligner
        self.shape_predictor = dlib.shape_predictor('/home/jason9308/robot_ws/src/face_recognition/shape_predictor_5_face_landmarks.dat')
        self.face_aligner = FaceAligner(self.shape_predictor, desiredFaceWidth=160)

        # Face recognition model (FaceNet)
        self.face_recognizer = InceptionResnetV1(pretrained='vggface2').eval().to('cuda')

        reference_path = f"/home/jason9308/robot_ws/src/face_recognition/{identity_name}.jpeg"
        self.get_logger().info(f"使用參考圖檔：{reference_path}")
        self.reference_embedding = self.load_reference_image(reference_path)
        

        self.threshold = 0.70
        self.pass_count = 0
        self.pass_required = 50

    def load_reference_image(self, img_path):
        reference_image = cv2.imread(img_path)
        if reference_image is None:
            self.get_logger().error("無法讀取參考影像！")
            return None

        results = self.face_detector(reference_image)
        if results and results[0].boxes:
            x1, y1, x2, y2 = map(int, results[0].boxes[0].xyxy[0].tolist())

            # ✅ 將整張圖與座標傳入 align_face
            aligned = self.align_face(reference_image, x1, y1, x2, y2)
            if aligned is None:
                self.get_logger().error("參考人臉對齊失敗！")
                return None

            # ✅ 對齊後取 embedding
            face_tensor = self.preprocess_face(aligned)
            with torch.no_grad():
                embedding = self.face_recognizer(face_tensor)
            return embedding.squeeze().detach().cpu().numpy()
        else:
            self.get_logger().error("未偵測到參考人臉！")
            return None


    def align_face(self, frame, x1, y1, x2, y2):
        rect = dlib.rectangle(x1, y1, x2, y2)
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        try:
            shape = self.shape_predictor(gray, rect)

            points = [(shape.part(i).x, shape.part(i).y) for i in range(shape.num_parts)]
            self.get_logger().info(f"偵測到 {shape.num_parts} 個特徵點: {points}")

            if shape.num_parts != 5:
                self.get_logger().warn("⚠️ Dlib 偵測到的特徵點不足 5 個")
                return None

            aligned = self.face_aligner.align(frame, gray, rect)
            return aligned
        except Exception as e:
            self.get_logger().error(f"⚠️ face_aligner.align 錯誤：{e}")
            return None



    def preprocess_face(self, aligned_face):
        face = aligned_face / 255.0
        face = np.transpose(face, (2, 0, 1))
        face = np.expand_dims(face, axis=0)
        return torch.tensor(face).float().to('cuda')

    def get_embedding(self, face_img):
        aligned = self.align_face(face_img, 0, 0, face_img.shape[1], face_img.shape[0])
        if aligned is None:
            return None
        face_tensor = self.preprocess_face(aligned)
        with torch.no_grad():
            embedding = self.face_recognizer(face_tensor)
        return embedding.squeeze().cpu().numpy()

    def image_callback(self, msg):
        try:
            # ✅ 將 ROS 傳來的影像訊息轉成 OpenCV 格式（BGR）
            frame = self.bridge.imgmsg_to_cv2(msg, "bgr8")
        except CvBridgeError as e:
            # ❌ 若轉換失敗，顯示錯誤訊息
            self.get_logger().error(f"影像轉換失敗: {e}")
            return

        # ✅ 使用 YOLO 模型進行人臉偵測
        results = self.face_detector(frame)

        # ❌ 若沒偵測到人臉，直接跳出
        if not results or not results[0].boxes:
            return

        # ✅ 在多個偵測框中，挑選面積最大的框（也就是離鏡頭最近的人臉）
        max_area = 0
        selected_box = None
        for box in results[0].boxes:
            x1, y1, x2, y2 = map(int, box.xyxy[0].tolist())  # 取得框的座標
            area = (x2 - x1) * (y2 - y1)  # 計算面積
            if area > max_area:
                max_area = area
                selected_box = box  # 記錄最大框

        if selected_box is None:
            return

        # ✅ 從選中的最大人臉框中擷取資訊
        x1, y1, x2, y2 = map(int, selected_box.xyxy[0].tolist())
        confidence = selected_box.conf[0]  # YOLO 預測信心分數（未使用，但可以印出來 debug）
        face_img = frame[y1:y2, x1:x2]  # 擷取人臉區域（可視為備用）

        # ✅ 進行人臉對齊（根據框位置 + dlib 特徵點對齊）
        aligned_face = self.align_face(frame, x1, y1, x2, y2)
        if aligned_face is None:
            return  # 若對齊失敗則跳過這幀

        # ✅ 前處理 + 轉成張量後送進 FaceNet 模型取得特徵向量
        face_tensor = self.preprocess_face(aligned_face)
        detected_embedding = self.face_recognizer(face_tensor).squeeze().detach().cpu().numpy()

        # ✅ 與參考圖片進行 cosine similarity 比對
        if self.reference_embedding is not None:
            cosine_similarity = np.dot(self.reference_embedding, detected_embedding) / (
                np.linalg.norm(self.reference_embedding) * np.linalg.norm(detected_embedding)
            )
        else:
            cosine_similarity = 0.0

        # ✅ 積分系統：取代傳統「連續幾次通過」的機制
        if not hasattr(self, 'pass_score'):
            self.pass_score = 0  # 初始化 pass_score，只做一次

        # ✅ 根據相似度調整分數（類似加權累積判斷）
        if cosine_similarity > 0.70:
            self.pass_score += 1  # 相似度高，加 2 分
        elif cosine_similarity < 0.65:
            self.pass_score -= 1  # 相似度低，扣 1 分
        # 若在 0.65~0.70 之間，則 pass_score 不變

        # ✅ 限制積分區間在 0～20 之間，避免爆表或變負數
        self.pass_score = max(0, min(self.pass_score, 70))

        # ✅ 在畫面上標示：相似度 + 當前積分
        label = f"Sim: {cosine_similarity:.2f} Score: {self.pass_score}"
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
        cv2.putText(frame, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)

        # ✅ 如果積分達到門檻（10 分），則視為通過身分辨識
        if self.pass_score >= 50:
            success_msg = String()
            success_msg.data = "身份確認成功"
            self.result_pub.publish(success_msg)  # 發布訊息通知其他 node
            self.get_logger().info("身份確認成功！即將關閉節點...")

            # 顯示通過畫面文字提示
            cv2.putText(frame, "Access Granted!", (50, 50), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
            cv2.imshow("Face Recognition", frame)
            cv2.waitKey(3000)  # 等待 3 秒

            # ✅ 清理：關閉畫面 + 訂閱、發布器、節點
            cv2.destroyAllWindows()
            self.image_sub = None
            self.result_pub = None
            self.destroy_node()
            exit(0)  # ⚠️ 強制結束程式（可改為 rclpy.shutdown() 比較安全）

            return

        # ✅ 顯示即時辨識畫面
        cv2.imshow("Face Recognition", frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            self.get_logger().info("手動結束辨識")
            cv2.destroyAllWindows()
            self.image_sub = None
            self.result_pub = None
            self.destroy_node()
            exit(0) 



def main(args=None):
    rclpy.init(args=args)
    identity_name = sys.argv[1] if len(sys.argv) > 1 else "me"
    node = FaceRecognitionNode(identity_name)
    if rclpy.ok():
        rclpy.spin(node)

if __name__ == "__main__":
    main()

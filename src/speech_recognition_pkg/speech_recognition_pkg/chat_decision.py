#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from std_msgs.msg import String
import speech_recognition as sr
from playsound import playsound
import time

class ChatDecisionNode(Node):
    def __init__(self):
        super().__init__('chat_decision_node')
        self.publisher_ = self.create_publisher(String, '/chat_decision', 10)
        self.recognizer = sr.Recognizer()
        self.chat_publisher = self.create_publisher(String, '/chat_messages', 10)
        clear_msg = String()
        clear_msg.data = "clear"
        self.chat_publisher.publish(clear_msg)
        self.ask_and_listen()

    def ask_and_listen(self):
        response = String()
        response.data = "undecided"

        positive_keywords = ["要", "好", "聊", "可以", "想", "願意"]
        negative_keywords = ["不", "不用", "不要", "沒空", "不了", "先不用"]

        # 播放詢問語音
        bot_msg = String()
        bot_msg.data = f"BOT: 你想要跟我聊聊嗎?"
        self.chat_publisher.publish(bot_msg)
        playsound("/home/jason9308/robot_ws/sound/ask_chat.mp3")

        while rclpy.ok() and response.data == "undecided":
            with sr.Microphone() as source:
                self.get_logger().info("請問您想聊天嗎？請回答...")
                self.recognizer.dynamic_energy_threshold = True
                self.recognizer.adjust_for_ambient_noise(source)
                self.recognizer.pause_threshold = 1.5
                try:
                    playsound("/home/jason9308/robot_ws/sound/start_lower.mp3")
                    audio = self.recognizer.listen(source)
                    playsound("/home/jason9308/robot_ws/sound/finish_lower.mp3")
                    
                    text = self.recognizer.recognize_google(audio, language='zh-TW')

                    self.get_logger().info(f"你說了：{text}")
                    user_msg = String()
                    user_msg.data = f"USER: {text}"
                    self.chat_publisher.publish(user_msg)

                    if any(k in text for k in negative_keywords):
                        response.data = "no"
                        bot_msg.data = "BOT: 好的，那我先走囉～"
                        self.chat_publisher.publish(bot_msg)
                        playsound("/home/jason9308/robot_ws/sound/chat_no.mp3")
                    elif any(k in text for k in positive_keywords):
                        response.data = "yes"
                        bot_msg.data = "BOT: 太好了！我們來聊聊吧！"
                        self.chat_publisher.publish(bot_msg)
                        playsound("/home/jason9308/robot_ws/sound/chat_yes.mp3")
                    else:
                        self.get_logger().info("無法明確判斷回應，請再說一次。")
                        bot_msg.data = "BOT: 抱歉，我不太清楚。你想跟我聊聊嗎。"
                        self.chat_publisher.publish(bot_msg)
                        playsound("/home/jason9308/robot_ws/sound/ask_chat_repeat.mp3")

                except Exception as e:
                    self.get_logger().warn(f"語音辨識失敗：{e}")

        self.publisher_.publish(response)
        self.get_logger().info(f"✅ 聊天回應結果：{response.data}")

def main(args=None):
    rclpy.init(args=args)
    node = ChatDecisionNode()
    rclpy.spin_once(node, timeout_sec=2.0)  # spin 一次就結束
    # 等待一段時間以確保所有回應都已發佈
    time.sleep(1)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

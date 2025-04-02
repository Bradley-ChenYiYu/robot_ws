#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
import configparser
import openai
import tempfile
import os
import requests
import pyaudio
import wave
from playsound import playsound
import speech_recognition as sr

class ChatbotNode(Node):
    def __init__(self):
        super().__init__('chatbot_node')

        # 讀取 API 金鑰
        config = configparser.ConfigParser()
        config.read('/home/jason9308/robot_ws/src/chatbot/config.ini')
        self.api_key = config['API']['api_key']
        openai.api_key = self.api_key

        # 關鍵詞：說了這些話就會退出聊天
        self.exit_keywords = ["結束聊天", "退出", "再見", "掰掰", "停止對話", "拜拜"]

        # 關鍵詞：這些話會被視為背景音，略過
        self.blacklist_keywords = [
            "字幕by索兰娅",
            "請不吝點贊", "请不吝点赞",
            "訂閱", "订阅", 
            "打赏支持", "打賞支持", 
            "明鏡與點點欄目", "明镜与点点栏目",
            "字幕"
        ]
        self.get_logger().info("🤖 Chatbot 已啟動，等待語音輸入...")
        self.chat_loop()

    def chat_loop(self):
        while rclpy.ok():
            # audio_path = self.smart_record_audio()
            # text = self.transcribe_audio(audio_path)
            text = self.listen_and_transcribe()

            if text is None:
                self.get_logger().warn("辨識失敗，請再試一次。")
                continue

            self.get_logger().info(f"你說：{text}")

            if any(keyword in text for keyword in self.exit_keywords):
                self.get_logger().info("🛑 偵測到結束關鍵詞，結束聊天")
                break

            if any(keyword in text for keyword in self.blacklist_keywords):
                self.get_logger().warn(f"⚠️ 偵測到背景誤辨識內容，略過：{text}")
                continue

            response = self.ask_gpt(text)
            self.speak_response(response)

        self.destroy_node()
        rclpy.shutdown()


    def listen_and_transcribe(self):
        recognizer = sr.Recognizer()
        recognizer.pause_threshold = 1.2
        # recognizer.energy_threshold = 600
        with sr.Microphone() as source:
            self.get_logger().info("🎙️ 等待你說話...")
            recognizer.adjust_for_ambient_noise(source)
            audio = recognizer.listen(source)
            self.get_logger().info("🛑 偵測到你講完話，開始辨識...")

            # 計算錄音長度
            duration = len(audio.frame_data) / (audio.sample_rate * audio.sample_width)
            self.get_logger().info(f"📏 錄音長度：{duration:.2f} 秒")

            if duration < 1.0:
                self.get_logger().warn("⚠️ 錄音太短，可能是雜音，略過此次輸入")
                return None

            try:
                text = recognizer.recognize_google(audio, language="zh-TW")
                return text
            except sr.UnknownValueError:
                self.get_logger().warn("❌ 語音辨識失敗（無法理解語音）")
                return None
            except sr.RequestError as e:
                self.get_logger().error(f"❌ 語音辨識服務錯誤：{e}")
                return None



    def ask_gpt(self, user_input):
        messages = [
            {"role": "system", 
             "content": (
                "你是一位溫柔親切的語音聊天夥伴，像是照顧長者的護理員，"
                "也像是他們關心體貼的孫子或孫女。\n"
                "請用簡單、慢一點的中文說話，語氣溫和、親切，讓人感覺溫暖。\n"
                "你的目的是陪伴長者，跟他們聊聊天、回應他們的情緒，讓他們覺得被傾聽和關心。\n"
                "可以問候、關心他們的日常，也可以回答一些簡單的生活話題。\n"
                "不要講太複雜的內容，也不要提到你是 AI、機器人或虛擬角色。\n"
                "每次回應請控制在 1~2 句話之內，讓對話節奏自然。"
                "最重要的是 我希望你的對話自然一點 像是一般人的對話一樣\n"
            )

            },

            {"role": "user", "content": user_input}
        ]

        response = openai.ChatCompletion.create(
            model="gpt-4o",
            messages=messages
        )
        reply = response.choices[0].message.content.strip()
        self.get_logger().info(f"🧠 GPT 回覆：{reply}")
        return reply

    def speak_response(self, text):
        tts_url = "https://api.openai.com/v1/audio/speech"
        headers = {
            "Authorization": f"Bearer {self.api_key}",
            "Content-Type": "application/json"
        }
        payload = {
            "model": "tts-1",
            "input": text,
            # "voice": "shimmer",
            # "voice": "nova",
            # "voice": "alloy",
            # "voice": "fable",
            # "voice": "onyx",
            "voice": "echo",
            "response_format": "mp3"
        }

        response = requests.post(tts_url, headers=headers, json=payload)

        if response.status_code == 200:
            with tempfile.NamedTemporaryFile(delete=False, suffix=".mp3") as tmp_file:
                tmp_file.write(response.content)
                tmp_file_path = tmp_file.name

            self.get_logger().info("🔊 播放語音回覆中...")
            playsound(tmp_file_path)
            os.remove(tmp_file_path)
        else:
            self.get_logger().error(f"TTS 失敗：{response.status_code} - {response.text}")

    # def record_audio(self, filename="temp_audio.wav", duration=5):
    #     # 使用 PyAudio 錄音
    #     FORMAT = pyaudio.paInt16
    #     CHANNELS = 1
    #     RATE = 16000
    #     CHUNK = 1024
    #     audio = pyaudio.PyAudio()

    #     stream = audio.open(format=FORMAT, channels=CHANNELS,
    #                         rate=RATE, input=True,
    #                         frames_per_buffer=CHUNK)

    #     self.get_logger().info("🎙️ 開始錄音...")
    #     frames = []

    #     for _ in range(0, int(RATE / CHUNK * duration)):
    #         data = stream.read(CHUNK)
    #         frames.append(data)

    #     self.get_logger().info("🛑 錄音結束")

    #     stream.stop_stream()
    #     stream.close()
    #     audio.terminate()

    #     # 儲存為 wav 檔
    #     wf = wave.open(filename, 'wb')
    #     wf.setnchannels(CHANNELS)
    #     wf.setsampwidth(audio.get_sample_size(FORMAT))
    #     wf.setframerate(RATE)
    #     wf.writeframes(b''.join(frames))
    #     wf.close()

    #     return filename
    
    # def smart_record_audio(self, filename="temp_audio.wav"):
    #     recognizer = sr.Recognizer()
    #     recognizer.energy_threshold = 6000 
    #     with sr.Microphone() as source:
    #         self.get_logger().info("🎙️ 等待你說話...")
    #         recognizer.adjust_for_ambient_noise(source)
    #         audio = recognizer.listen(source)
    #         self.get_logger().info("🛑 偵測到你講完話，錄音完成")

    #         # 🧠 計算錄音長度（秒）
    #         duration = len(audio.frame_data) / (audio.sample_rate * audio.sample_width)
    #         self.get_logger().info(f"📏 錄音長度：{duration:.2f} 秒")

    #         # ⚠️ 若太短則略過
    #         if duration < 2.0:
    #             self.get_logger().warn("⚠️ 錄音太短，可能是雜音，略過此次輸入")
    #             return None

    #         with open(filename, "wb") as f:
    #             f.write(audio.get_wav_data())
    #     return filename

    # def transcribe_audio(self, audio_path):
    #     try:
    #         audio_file = open(audio_path, "rb")
    #         transcript = openai.Audio.transcribe(
    #             model="whisper-1",
    #             file=audio_file,
    #             language="zh"
    #         )
    #         return transcript["text"]
    #     except Exception as e:
    #         self.get_logger().error(f"語音轉文字失敗：{e}")
    #         return None


def main(args=None):
    rclpy.init(args=args)
    node = ChatbotNode()
    # rclpy.spin(node)

if __name__ == '__main__':
    main()

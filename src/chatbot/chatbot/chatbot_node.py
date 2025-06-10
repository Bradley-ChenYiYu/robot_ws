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
import random
from std_msgs.msg import String

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

        self.SYSTEM_PROMPT = {
            "role": "system",
            "content": (
                "你是一位溫柔親切的語音聊天夥伴，像是照顧長者的護理員，"
                "也像是他們關心體貼的孫子或孫女。\n"
                "請用簡單、慢一點的中文說話，語氣溫和、親切，讓人感覺溫暖。\n"
                "你的目的是陪伴長者，跟他們聊聊天、回應他們的情緒，讓他們覺得被傾聽和關心。\n"
                "可以問候、關心他們的日常，也可以回答一些簡單的生活話題。\n"
                "不要講太複雜的內容，也不要提到你是 AI、機器人或虛擬角色。\n"
                "每次回應請控制在 1~2 句話之內，讓對話節奏自然。\n"
                "最重要的是，我希望你的對話自然一點，像是一般人的對話一樣。"
            )
        }
        self.chat_history = [self.SYSTEM_PROMPT]  # 初始只有 system prompt

        self.jokes = [
            """今天去買早餐時，
            後面的人拍了我的肩說：
            「我先到欸！」
            於是我微慍道：
            「我流川楓欸」""",

            """白氣球揍了黑氣球一拳，
            黑氣球很痛很生氣於是決定 -- 告白氣球""",

            """有一天小明走著進超商，坐著輪椅出來
            知道為什麼嗎
            因為他繳費了 """,

            """皮卡丘被揍之後會變成什麼？ 【卡丘】因為他就不敢再皮了""",

            """姊姊要生了！這樣我能獲得什麼？ 【成舅感】""",

            """落枕要搭什麼車去醫院？ 【接駁車】""",

            """有一天小明跟朋友去樹下野餐，
            要走的時候發現衣服被勾住了，
            於是他就跟朋友說
            樹勾衣餒""",

            """幾點不能講笑話 【一點】，一點都不好笑""",

            """成功要做到五個字「特別能吃苦」
            而我想我快成功了，
            因為我已經完成80%！
            特別能吃""",

            """誰最會烤肉
            【老師】因為考的都沒有教（焦）""",

            """為何灰姑娘的車不易刮傷？ 因為她的車是難刮馬車""",

            """開什麼品牌車子的人最有禮貌?【Nissan】因為他們常常Nissan往來""",

            """富士山到底在哪裡?【在負42與負44之間】""",

            """茶最怕什麼東西？【沙茶醬】""",

            """有一個草莓大福，裡面的草莓：「我在哪裡阿？」
            身在福中不知福 """,

            """哪種水果最愛打招呼？【柿子】因為，柿在哈囉""",
        ]

        self.get_logger().info("🤖 Chatbot 已啟動，等待語音輸入...")

        self.chat_publisher = self.create_publisher(String, '/chat_messages', 10)
        clear_msg = String()
        clear_msg.data = "clear"
        self.chat_publisher.publish(clear_msg)
        playsound("/home/jason9308/robot_ws/sound/chat_start.mp3")
        start_msg = String()
        start_msg.data = "BOT: 你好呀，我是鐵柱！有什麼想跟我分享的嗎？"
        self.chat_publisher.publish(start_msg)
        
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
            user_msg = String()
            user_msg.data = f"USER: {text}"
            self.chat_publisher.publish(user_msg)

            if any(keyword in text for keyword in self.exit_keywords):
                self.get_logger().info("🛑 偵測到結束關鍵詞，準備結束聊天")

                # 溫柔回應後再關閉
                goodbye_text = "好的，掰掰～祝您今天愉快，有空再來跟我聊聊天喔！"
                bot_msg = String()
                bot_msg.data = f"BOT: {goodbye_text}"
                self.chat_publisher.publish(bot_msg)
                self.speak_response(goodbye_text)

                break


            if any(keyword in text for keyword in self.blacklist_keywords):
                self.get_logger().warn(f"⚠️ 偵測到背景誤辨識內容，略過：{text}")
                continue

            response = self.ask_gpt(text)
            bot_msg = String()
            bot_msg.data = f"BOT: {response}"
            self.chat_publisher.publish(bot_msg)
            self.speak_response(response)

        self.destroy_node()
        rclpy.shutdown()


    def listen_and_transcribe(self):
        recognizer = sr.Recognizer()
        recognizer.pause_threshold = 1.5
        with sr.Microphone() as source:
            self.get_logger().info("🎙️ 等待你說話...")
            recognizer.dynamic_energy_threshold = True
            recognizer.adjust_for_ambient_noise(source)
            #  如果改用固定閾值
            # recognizer.dynamic_energy_threshold = False
            # recognizer.energy_threshold = 5000  # 調整為較高的閾值以過濾雜音

            playsound("/home/jason9308/robot_ws/sound/start_lower.mp3")
            audio = recognizer.listen(source)
            playsound("/home/jason9308/robot_ws/sound/finish_lower.mp3")
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

        # 隨機挑選 5 個笑話給 GPT
        selected_jokes = random.sample(self.jokes, k=min(5, len(self.jokes)))
        jokes_prompt = "\n\n".join(f"- {j}" for j in selected_jokes)

        # 加入這一輪使用者輸入
        self.chat_history.append({"role": "user", "content": user_input})

        # 保留最近 3 輪對話（6 條 user + assistant），加上 system prompt
        MAX_HISTORY = 6
        while len(self.chat_history) > 1 + MAX_HISTORY:
            self.chat_history.pop(1)

        # 更新 system prompt，加入笑話清單提示
        self.chat_history[0] = {
            "role": "system",
            "content": (
                "你是一位溫柔親切的語音聊天夥伴，像是照顧長者的護理員，也像是他們關心體貼的孫子或孫女。\n"
                "請用簡單、慢一點的中文說話，語氣溫和、親切，讓人感覺溫暖。\n"
                "你的目的是陪伴長者，跟他們聊聊天、回應他們的情緒，讓他們覺得被傾聽和關心。\n"
                "當使用者說「講笑話」、「說個笑話」或「讓我笑一下」時，請從以下清單中選一則講出來，語氣自然一點，不要自己編：\n"
                f"{jokes_prompt}\n\n"
                "如果使用者沒有提到笑話，就照正常聊天回應即可。"
            )
        }

        # 呼叫 GPT
        response = openai.ChatCompletion.create(
            model="gpt-4o",
            messages=self.chat_history
        )
        reply = response.choices[0].message.content.strip()

        # 把 GPT 回應加入記憶
        self.chat_history.append({"role": "assistant", "content": reply})

        self.get_logger().info(f"🧠 Chatbot 回覆：{reply}")
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
            "voice": "fable",
            # "voice": "onyx",
            # "voice": "echo",
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

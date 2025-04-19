#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
import speech_recognition as sr
import openai
import configparser
import json
import re
import os
from datetime import datetime
import fcntl
import os
from playsound import playsound

class SpeechRecognitionNode(Node):
    def __init__(self):
        super().__init__('speech_recognition_node')

        # 讀取 OpenAI API 金鑰
        config = configparser.ConfigParser()
        config.read('/home/jason9308/robot_ws/src/speech_recognition_pkg/speech_recognition_pkg/config.ini')
        self.OPENAI_API_KEY = config['API']['api_key']

        # 初始化 OpenAI（舊版用法）
        openai.api_key = self.OPENAI_API_KEY

        # 語音辨識
        self.recognizer = sr.Recognizer()

        # JSON 存放路徑
        self.json_file_path = "/home/jason9308/robot_ws/command_jason/output.json"

        # 確保 JSON 檔案擁有所有的權限
        self.set_file_permissions(self.json_file_path)

        # 開始語音處理
        self.listen_and_process()

    def set_file_permissions(self, file_path):
        try:
            os.chmod(file_path, 0o777)
            self.get_logger().info(f"已設置檔案 {file_path} 的權限為 777")
        except Exception as e:
            self.get_logger().error(f"無法設置檔案權限: {e}")

    def listen_and_process(self):
        while rclpy.ok():
            with sr.Microphone() as source:
                self.get_logger().info("請說話...")
                self.recognizer.adjust_for_ambient_noise(source)
                self.recognizer.pause_threshold = 1.5
                # self.recognizer.energy_threshold = 3000
                audio_data = self.recognizer.listen(source)

                try:
                    text = self.recognizer.recognize_google(audio_data, language="zh-TW")
                    self.get_logger().info(f"你說了: {text}")

                    if "機器手臂" in text:
                        response = self.generate_json(text)
                        self.get_logger().info(f"生成的 JSON: {response}")

                        if self.extract_and_dump_json(response, self.json_file_path):
                            self.get_logger().info("JSON 存檔成功")
                            self.play_voice("json_received.mp3")
                            self.destroy_node()  # 關閉節點
                            rclpy.shutdown()
                        else:
                            self.get_logger().error("JSON 無效，請重新輸入")
                            self.play_voice("json_invalid.mp3")
                    else:
                        self.get_logger().info("未偵測到『機器手臂』，繼續監聽中...")

                except sr.UnknownValueError:
                    self.get_logger().error("無法識別語音，請再試一次")
                    self.play_voice("speech_not_understood.mp3")
                except sr.RequestError as e:
                    self.get_logger().error(f"語音請求失敗: {e}")

    def extract_time_from_text(self, text):

        # 特別處理「點半」情況（例如「12點半」）
        half_pattern = re.compile(r'(\d{1,2})點半')
        match_half = half_pattern.search(text)

        if match_half:
            hour = int(match_half.group(1))
            minute = 30
            if "下午" in text or "晚上" in text:
                if hour != 12:
                    hour += 12
            return f"{hour:02}:{minute:02}"
    
        # 處理「點」和「分」的情況（例如「12.30分」或「12點30分」）
        time_pattern = re.compile(r'(\d{1,2})(?:\.|點)(\d{1,2})分?')
        match = time_pattern.search(text)

        if match:
            hour = int(match.group(1))
            minute = int(match.group(2))
            if "下午" in text or "晚上" in text:
                if hour != 12:
                    hour += 12
            return f"{hour:02}:{minute:02}"

        # 處理「點」的情況（例如「12點」）
        time_pattern_hour_only = re.compile(r'(\d{1,2})(?:\.|點)')
        match_hour_only = time_pattern_hour_only.search(text)

        if match_hour_only:
            hour = int(match_hour_only.group(1))
            if "下午" in text or "晚上" in text:
                if hour != 12:
                    hour += 12
            return f"{hour:02}:00"

        return None

    def generate_json(self, instruction):
        extracted_time = self.extract_time_from_text(instruction)
        current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        time_to_use = extracted_time if extracted_time else datetime.now().strftime("%H:%M")

        prompt = f"""instruction : {instruction}
            
            Extracted time: {time_to_use}

            Please understand the command, and the target in this instruction, and convert them into the following JSON schema:

            [
                {{
                    "command": "<one of the following only: send medicine, go home, chat, video call>",
                    "target" : "<one of the following only: dad, mom, child, grandpa, grandma, NULL>",
                    "time": "{time_to_use}"
                }}
            ]

            - Only respond in valid JSON format.
            - The command must be one of the following (do not generate anything else):
                - send medicine
                - go home
                - chat
                - video call
            - The target must be one of the following (do not generate anything else):
                - dad
                - mom
                - child
                - grandpa
                - grandma
                - NULL
            - If `command` is "send medicine", target **must not** be NULL.
            - If `command` is "go home", "chat", or "video call", target **can** be NULL.
            - 注意：「送藥」這個詞在語音辨識中可能被誤聽為「重要」，如果 instruction 中出現「重要」，請將 command 判斷為 "send medicine"。
            """

        response = openai.ChatCompletion.create(
            model="gpt-4",
            messages=[
                {"role": "system", "content": "You are an AI assistant that converts instructions into structured JSON format."},
                {"role": "user", "content": prompt}
            ]
        )

        return response.choices[0].message["content"]

    def extract_and_dump_json(self, sample_text, output_file_path):
        try:
            parsed_json = json.loads(sample_text)
        except json.JSONDecodeError:
            pattern = r"```json(.*?)```"
            json_part = re.search(pattern, sample_text, re.DOTALL)
            extracted_json = json_part.group(1).strip() if json_part else None
            if not extracted_json:
                self.get_logger().error("無法提取 JSON !")
                return False
            try:
                parsed_json = json.loads(extracted_json)
            except json.JSONDecodeError:
                self.get_logger().error("解析 JSON 失敗！")
                return False
        
        for item in parsed_json:
            if item.get("command") == "send medicine" and item.get("target") == "NULL":
                self.get_logger().error("command 是 'send medicine' 但 target 是 NULL，拒絕寫入 JSON。")
                return False
            
        try:
            with open(output_file_path, 'w', encoding='utf-8') as json_file:
                fcntl.flock(json_file.fileno(), fcntl.LOCK_EX)
                json.dump(parsed_json, json_file, indent=4)
                fcntl.flock(json_file.fileno(), fcntl.LOCK_UN)
            self.get_logger().info(f"JSON 已儲存至 {output_file_path}")
            return True
        except Exception as e:
            self.get_logger().error(f"寫入 JSON 時發生錯誤: {e}")
            return False
        

    def play_voice(self, filename):

        base_path = "/home/jason9308/robot_ws/sound/"
        full_path = os.path.join(base_path, filename)
        try:
            playsound(full_path)
        except Exception as e:
            self.get_logger().warn(f"語音播放失敗：{e}")

        

    

def main(args=None):
    rclpy.init(args=args)
    node = SpeechRecognitionNode()
    
    if rclpy.ok():
        rclpy.spin(node)

    # for i, name in enumerate(sr.Microphone.list_microphone_names()):
    #     print(f"{i}: {name}")

if __name__ == "__main__":
    main()

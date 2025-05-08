#!/usr/bin/env python3

import requests
import configparser
import os
from playsound import playsound

# ✅ 你可以在這裡自由設定內容與儲存位置
text = "身份確認成功"
output_path = "/home/jason9308/robot_ws/sound/identity_confirmed.mp3"

def speak(text, output_path):
    # 讀取 API 金鑰
    config = configparser.ConfigParser()
    config.read('/home/jason9308/robot_ws/src/chatbot/config.ini')
    api_key = config['API']['api_key']

    print(f"🔊 正在將文字轉為語音：{text}")

    # 呼叫 OpenAI TTS API
    tts_url = "https://api.openai.com/v1/audio/speech"
    headers = {
        "Authorization": f"Bearer {api_key}",
        "Content-Type": "application/json"
    }
    payload = {
        "model": "tts-1",
        "input": text,
        "voice": "fable",
        "response_format": "mp3"
    }

    response = requests.post(tts_url, headers=headers, json=payload)

    if response.status_code == 200:
        # ✅ 自動建立資料夾（若不存在）
        os.makedirs(os.path.dirname(output_path), exist_ok=True)

        # 儲存音訊檔
        with open(output_path, 'wb') as f:
            f.write(response.content)
        print(f"✅ 已儲存語音至：{output_path}")

        # 播放
        playsound(output_path)

    else:
        print(f"❌ 發生錯誤：{response.status_code} - {response.text}")



# ✅ 執行一次
if __name__ == "__main__":
    speak(text, output_path)

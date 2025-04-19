import datetime
import os
import threading
import webbrowser
from dotenv import load_dotenv

from linebot import LineBotApi
from linebot.models import TextSendMessage
from google_auth_oauthlib.flow import InstalledAppFlow
from googleapiclient.discovery import build

# 載入 LINE 憑證
load_dotenv()
LINE_CHANNEL_ACCESS_TOKEN = os.getenv("LINE_CHANNEL_ACCESS_TOKEN")
USER_ID = os.getenv("LINE_USER_ID")  # 請加上你自己的 User ID
line_bot_api = LineBotApi(LINE_CHANNEL_ACCESS_TOKEN)

# Google Meet 建立函式
def create_google_meet():
    SCOPES = ['https://www.googleapis.com/auth/calendar.events']
    flow = InstalledAppFlow.from_client_secrets_file('credentials.json', SCOPES)
    creds = flow.run_local_server(port=0)
    service = build('calendar', 'v3', credentials=creds)

    now = datetime.datetime.utcnow()
    event = {
        'summary': 'Line Bot Meeting',
        'start': {
            'dateTime': (now + datetime.timedelta(minutes=1)).isoformat() + 'Z',
            'timeZone': 'Asia/Taipei',
        },
        'end': {
            'dateTime': (now + datetime.timedelta(minutes=31)).isoformat() + 'Z',
            'timeZone': 'Asia/Taipei',
        },
        'conferenceData': {
            'createRequest': {
                'requestId': 'line-meeting-001',
                'conferenceSolutionKey': {'type': 'hangoutsMeet'},
            },
        },
    }

    event = service.events().insert(calendarId='primary', body=event, conferenceDataVersion=1).execute()
    meet_link = event['conferenceData']['entryPoints'][0]['uri']
    return meet_link

# 主機端開啟瀏覽器加入 meet
def open_meet(meet_link):
    webbrowser.open(meet_link)

# 主邏輯
def main():
    try:
        meet_link = create_google_meet()
        print(f"[成功] 建立會議：{meet_link}")

        # 開啟會議（在主機端）
        threading.Thread(target=open_meet, args=(meet_link,)).start()

        # 傳送 LINE 訊息給使用者
        line_bot_api.push_message(USER_ID, TextSendMessage(text=f"📹 會議已建立，請點選加入：\n{meet_link}"))
        print("[成功] 已傳送給使用者 LINE")

    except Exception as e:
        print(f"[錯誤] {e}")

if __name__ == "__main__":
    main()

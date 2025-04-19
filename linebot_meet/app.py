from flask import Flask, request, abort
import os
from dotenv import load_dotenv
from linebot import LineBotApi, WebhookHandler
from linebot.exceptions import InvalidSignatureError
from linebot.models import MessageEvent, TextMessage, TextSendMessage

from create_meet import create_google_meet
from join_meet import open_meet_in_browser
import threading

load_dotenv()

app = Flask(__name__)

line_bot_api = LineBotApi(os.getenv("LINE_CHANNEL_ACCESS_TOKEN"))
handler = WebhookHandler(os.getenv("LINE_CHANNEL_SECRET"))

@app.route("/callback", methods=['POST'])
def callback():
    signature = request.headers['X-Line-Signature']
    body = request.get_data(as_text=True)

    try:
        handler.handle(body, signature)
    except InvalidSignatureError:
        abort(400)

    return 'OK'

@handler.add(MessageEvent, message=TextMessage)
def handle_message(event):
    user_msg = event.message.text.strip().lower()

    if user_msg in ["視訊通話", "我要視訊通話"]:
        meet_link = create_google_meet()

        # 在主機自動開啟 meet 畫面
        threading.Thread(target=open_meet_in_browser, args=(meet_link,)).start()

        reply = TextSendMessage(text=f"請點選以下連結開始視訊會議：\n{meet_link}")
    else:
        reply = TextSendMessage(text="輸入『視訊通話』來開始 Google Meet")

    line_bot_api.reply_message(event.reply_token, reply)

if __name__ == "__main__":
    app.run(debug=True, port=5001)

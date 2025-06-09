# import datetime
# import os
# import time
# import json
# from dotenv import load_dotenv
# from google.oauth2.credentials import Credentials
# from googleapiclient.discovery import build

# from selenium import webdriver
# from selenium.webdriver.common.by import By
# from selenium.webdriver.chrome.service import Service
# from selenium.webdriver.chrome.options import Options
# from selenium.webdriver.support.ui import WebDriverWait
# from selenium.webdriver.support import expected_conditions as EC

# from linebot import LineBotApi
# from linebot.models import TextSendMessage

# # === 載入環境變數 ===
# load_dotenv()
# LINE_CHANNEL_ACCESS_TOKEN = os.getenv("LINE_CHANNEL_ACCESS_TOKEN")
# USER_ID = os.getenv("LINE_USER_ID")
# GOOGLE_EMAIL = os.getenv("GOOGLE_EMAIL")
# GOOGLE_PASSWORD = os.getenv("GOOGLE_PASSWORD")

# line_bot_api = LineBotApi(LINE_CHANNEL_ACCESS_TOKEN)

# # === 建立 Google Meet 會議 ===
# def create_google_meet():
#     creds = Credentials.from_authorized_user_file('linebot_meet/token.json')
#     service = build('calendar', 'v3', credentials=creds)

#     now = datetime.datetime.utcnow()
#     event = {
#         'summary': 'Line Bot Meeting',
#         'start': {'dateTime': now.isoformat() + 'Z', 'timeZone': 'Asia/Taipei'},
#         'end': {'dateTime': (now + datetime.timedelta(minutes=30)).isoformat() + 'Z', 'timeZone': 'Asia/Taipei'},
#         'conferenceData': {
#             'createRequest': {
#                 'requestId': 'line-meeting-001',
#                 'conferenceSolutionKey': {'type': 'hangoutsMeet'}
#             }
#         },
#     }
#     event = service.events().insert(calendarId='primary', body=event, conferenceDataVersion=1).execute()
#     return event['conferenceData']['entryPoints'][0]['uri']

# # === Google 帳號登入 ===
# def Glogin(driver, email, password):
#     driver.get('https://accounts.google.com/ServiceLogin')
#     driver.find_element(By.ID, "identifierId").send_keys(email)
#     driver.find_element(By.ID, "identifierNext").click()
#     time.sleep(3)
#     driver.find_element(By.XPATH, '//*[@id="password"]/div[1]/div/div[1]/input').send_keys(password)
#     driver.find_element(By.ID, "passwordNext").click()
#     time.sleep(5)
#     driver.get('https://google.com/')
#     time.sleep(2)

# # === 關閉麥克風與鏡頭 ===
# def turnOffMicCam(driver):
#     time.sleep(5)
#     try:
#         driver.find_element(By.XPATH,
#             '//*[@aria-label="關閉麥克風 (ctrl + d)"]').click()
#         time.sleep(1)
#         driver.find_element(By.XPATH,
#             '//*[@aria-label="關閉攝影機 (ctrl + e)"]').click()
#     except:
#         print("⚠️ 找不到關閉按鈕，可能已預設關閉")

# # === 等待並加入會議 ===
# def wait_and_click_join(driver, timeout=20):
#     join_css = '#yDmH0d > c-wiz > div > div > div.TKU8Od > div.crqnQb > div > div.gAGjv > div.vgJExf > div > div > div.d7iDfe.NONs6c > div.shTJQe > div.jtn8y > div.XCoPyb > div > div > span > div.VfPpkd-dgl2Hf-ppHlrf-sM5MNb'
#     try:
#         print("⌛ 等待立即加入按鈕出現...")
#         WebDriverWait(driver, timeout).until(
#             EC.element_to_be_clickable((By.CSS_SELECTOR, join_css))
#         ).click()
#         print("✅ 已點擊立即加入")
#     except Exception as e:
#         print("❌ 點擊立即加入失敗：", e)

# def switch_to_rgb_camera(driver):
#     try:
#         time.sleep(3)

#         # triangle_buttons = driver.find_elements(By.XPATH, '//button[@aria-haspopup="menu"]')

#         # for i, btn in enumerate(triangle_buttons):
#         #     label = btn.get_attribute('aria-label')
#         #     print(f"第 {i} 個按鈕的 aria-label: {label}")

#         try:
#             button = driver.find_element(By.XPATH, '//button[@aria-label="影片設定"]')
#             driver.execute_script("arguments[0].click();", button)
#             print("✅ 點擊影片設定按鈕成功")

#         except Exception as e:
#             print(f"❌ 點擊攝影機三角形失敗：{e}")

#         time.sleep(1)

#         try:
#             button = WebDriverWait(driver, 10).until(
#                 EC.element_to_be_clickable((
#                     By.XPATH,
#                     '//button[@aria-label="攝影機: Intel(R) RealSense(TM) Depth Camera 435i Depth"]'
#                 ))
#             )
#             driver.execute_script("arguments[0].click();", button)
#             print("✅ 成功點開攝影機型號下拉選單")

#         except Exception as e:
#             print(f"❌ 開啟攝影機選單失敗：{e}")

#         time.sleep(1)


#         try:
#             rgb_option = WebDriverWait(driver, 10).until(
#                 EC.presence_of_element_located((
#                     By.XPATH,
#                     '//*[contains(text(), "435i RGB")]'
#                 ))
#             )
#             driver.execute_script("arguments[0].click();", rgb_option)
#             print("✅ 成功選擇 RGB 攝影機")
#         except Exception as e:
#             print(f"❌ 選擇 RGB 攝影機失敗：{e}")


#         time.sleep(1)

#         try:
#             button = driver.find_element(By.XPATH, '//button[@aria-label="影片設定"]')
#             driver.execute_script("arguments[0].click();", button)
#             print("✅ 點擊影片設定按鈕成功")

#         except Exception as e:
#             print(f"❌ 點擊攝影機三角形失敗：{e}")


#     except Exception as e:
#         print(f"❌ 自動切換 RGB 攝影機失敗：{e}")

# # === 開啟並監控會議視窗與 JSON ===
# def open_meet_and_monitor(meet_link):
#     print(f"🌐 開啟 Google Meet：{meet_link}")
#     options = Options()
#     options.add_argument("--guest")
#     options.add_argument('--disable-blink-features=AutomationControlled')
#     options.add_argument('--start-maximized')
#     options.add_argument('--use-fake-ui-for-media-stream')

#     driver = webdriver.Chrome(service=Service('/usr/local/bin/chromedriver'), options=options)
#     wait = WebDriverWait(driver, 30)

#     Glogin(driver, GOOGLE_EMAIL, GOOGLE_PASSWORD)
#     driver.get(meet_link)
#     turnOffMicCam(driver)
#     wait_and_click_join(driver)
#     switch_to_rgb_camera(driver)

#     print("⌛ 監控會議視窗與 JSON 控制檔案...")
#     try:
#         while True:
#             try:
#                 with open("command_jason/close_meeting.json", "r") as f:
#                     data = json.load(f)
#                 if data.get("end_meeting") == 1:
#                     print("🛑 JSON 控制檔案要求結束會議")
#                     data["end_meeting"] = 0
#                     data["meet_link"] = ""
#                     with open("command_jason/close_meeting.json", "w") as f:
#                         json.dump(data, f)
#                     break
#             except Exception as e:
#                 print(f"⚠️ 讀取 JSON 發生錯誤: {e}")

#             if not driver.window_handles:
#                 print("📴 瀏覽器已關閉，程式結束")
#                 break

#             time.sleep(2)
#     finally:
#         driver.quit()

# # === 主流程 ===
# def main():
#     try:
#         meet_link = create_google_meet()
#         print(f"[成功] 建立會議連結：{meet_link}")

#         # 寫入 JSON
#         os.makedirs("command_jason", exist_ok=True)
#         try:
#             with open("command_jason/close_meeting.json", "r") as f:
#                 data = json.load(f)
#         except:
#             data = {}
#         data["meet_link"] = meet_link
#         data["end_meeting"] = 0
#         with open("command_jason/close_meeting.json", "w") as f:
#             json.dump(data, f)

#         # 發送 LINE 通知
#         line_bot_api.push_message(USER_ID, TextSendMessage(text=f"📹 會議已建立：{meet_link}"))
#         print("[成功] 已傳送 LINE 通知")

#         open_meet_and_monitor(meet_link)

#     except Exception as e:
#         print(f"[錯誤] {e}")

# if __name__ == '__main__':
#     main()


import datetime
import os
import time
import json
from dotenv import load_dotenv
from google.oauth2.credentials import Credentials
from googleapiclient.discovery import build

from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

from linebot import LineBotApi
from linebot.models import TextSendMessage

# === 載入環境變數 ===
load_dotenv()
LINE_CHANNEL_ACCESS_TOKEN = os.getenv("LINE_CHANNEL_ACCESS_TOKEN")
USER_ID = os.getenv("LINE_USER_ID")

line_bot_api = LineBotApi(LINE_CHANNEL_ACCESS_TOKEN)

# === 建立 Google Meet 會議 ===
def create_google_meet():
    creds = Credentials.from_authorized_user_file('linebot_meet/token.json')
    service = build('calendar', 'v3', credentials=creds)

    now = datetime.datetime.utcnow()
    event = {
        'summary': 'Line Bot Meeting',
        'start': {'dateTime': now.isoformat() + 'Z', 'timeZone': 'Asia/Taipei'},
        'end': {'dateTime': (now + datetime.timedelta(minutes=30)).isoformat() + 'Z', 'timeZone': 'Asia/Taipei'},
        'conferenceData': {
            'createRequest': {
                'requestId': 'line-meeting-001',
                'conferenceSolutionKey': {'type': 'hangoutsMeet'}
            }
        },
    }
    event = service.events().insert(calendarId='primary', body=event, conferenceDataVersion=1).execute()
    return event['conferenceData']['entryPoints'][0]['uri']

# === 關閉麥克風與鏡頭 ===
def turnOffMicCam(driver):
    time.sleep(5)
    try:
        driver.find_element(By.XPATH,
            '//*[@aria-label="關閉麥克風 (ctrl + d)"]').click()
        time.sleep(1)
        driver.find_element(By.XPATH,
            '//*[@aria-label="關閉攝影機 (ctrl + e)"]').click()
    except:
        print("⚠️ 找不到關閉按鈕，可能已預設關閉")

# === 等待並加入會議 ===
def wait_and_click_join(driver, timeout=20):
    join_css = '#yDmH0d > c-wiz > div > div > div.TKU8Od > div.crqnQb > div > div.gAGjv > div.vgJExf > div > div > div.d7iDfe.NONs6c > div.shTJQe > div.jtn8y > div.XCoPyb > div > div > span > div.VfPpkd-dgl2Hf-ppHlrf-sM5MNb'
    try:
        print("⌛ 等待立即加入按鈕出現...")
        WebDriverWait(driver, timeout).until(
            EC.element_to_be_clickable((By.CSS_SELECTOR, join_css))
        ).click()
        print("✅ 已點擊立即加入")
    except Exception as e:
        print("❌ 點擊立即加入失敗：", e)

def switch_to_rgb_camera(driver):
    try:
        time.sleep(3)

        try:
            button = driver.find_element(By.XPATH, '//button[@aria-label="影片設定"]')
            driver.execute_script("arguments[0].click();", button)
            print("✅ 點擊影片設定按鈕成功")
        except Exception as e:
            print(f"❌ 點擊攝影機設定按鈕失敗：{e}")
        time.sleep(1)

        try:
            button = WebDriverWait(driver, 10).until(
                EC.element_to_be_clickable((By.XPATH, '//button[contains(@aria-label, "攝影機:")]'))
            )
            driver.execute_script("arguments[0].click();", button)
            print("✅ 成功點開攝影機型號下拉選單")
        except Exception as e:
            print(f"❌ 開啟攝影機選單失敗：{e}")
        time.sleep(1)

        try:
            rgb_option = WebDriverWait(driver, 10).until(
                EC.presence_of_element_located((By.XPATH, '//*[contains(text(), "435i RGB")]'))
            )
            driver.execute_script("arguments[0].click();", rgb_option)
            print("✅ 成功選擇 RGB 攝影機")
        except Exception as e:
            print(f"❌ 選擇 RGB 攝影機失敗：{e}")
        time.sleep(1)

        try:
            button = driver.find_element(By.XPATH, '//button[@aria-label="影片設定"]')
            driver.execute_script("arguments[0].click();", button)
        except:
            pass

    except Exception as e:
        print(f"❌ 自動切換 RGB 攝影機失敗：{e}")

# === 開啟並監控會議視窗與 JSON ===
def open_meet_and_monitor(meet_link):
    print(f"🌐 開啟 Google Meet：{meet_link}")
    options = Options()
    options.add_argument("user-data-dir=/home/jason9308/.chrome_profile")
    options.add_argument("profile-directory=Default")
    options.add_argument('--disable-blink-features=AutomationControlled')
    options.add_argument('--start-maximized')
    options.add_argument('--use-fake-ui-for-media-stream')

    driver = webdriver.Chrome(service=Service('/usr/local/bin/chromedriver'), options=options)
    wait = WebDriverWait(driver, 30)

    driver.get(meet_link)
    turnOffMicCam(driver)
    wait_and_click_join(driver)
    switch_to_rgb_camera(driver)

    print("⌛ 監控會議視窗與 JSON 控制檔案...")
    try:
        while True:
            try:
                with open("command_jason/close_meeting.json", "r") as f:
                    data = json.load(f)
                if data.get("end_meeting") == 1:
                    print("🛑 JSON 控制檔案要求結束會議")
                    data["end_meeting"] = 0
                    data["meet_link"] = ""
                    with open("command_jason/close_meeting.json", "w") as f:
                        json.dump(data, f)
                    break
            except Exception as e:
                print(f"⚠️ 讀取 JSON 發生錯誤: {e}")

            if not driver.window_handles:
                print("📴 瀏覽器已關閉，程式結束")
                break

            time.sleep(2)
    finally:
        driver.quit()

# === 主流程 ===
def main():
    try:
        meet_link = create_google_meet()
        print(f"[成功] 建立會議連結：{meet_link}")

        os.makedirs("command_jason", exist_ok=True)
        try:
            with open("command_jason/close_meeting.json", "r") as f:
                data = json.load(f)
        except:
            data = {}
        data["meet_link"] = meet_link
        data["end_meeting"] = 0
        with open("command_jason/close_meeting.json", "w") as f:
            json.dump(data, f)

        line_bot_api.push_message(USER_ID, TextSendMessage(text=f"📹 會議已建立：{meet_link}"))
        print("[成功] 已傳送 LINE 通知")

        open_meet_and_monitor(meet_link)

    except Exception as e:
        print(f"[錯誤] {e}")

if __name__ == '__main__':
    main()

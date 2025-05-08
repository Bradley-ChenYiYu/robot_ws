from google_auth_oauthlib.flow import InstalledAppFlow
import os

SCOPES = ['https://www.googleapis.com/auth/calendar']

flow = InstalledAppFlow.from_client_secrets_file(
    'linebot_meet/credentials.json', SCOPES)
creds = flow.run_local_server(port=0)

with open('linebot_meet/token.json', 'w') as token:
    token.write(creds.to_json())

print("✅ 已成功產生新的 token.json")


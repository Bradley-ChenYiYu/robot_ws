from google.oauth2.credentials import Credentials
from google_auth_oauthlib.flow import InstalledAppFlow
from googleapiclient.discovery import build
import datetime
import os

SCOPES = ['https://www.googleapis.com/auth/calendar.events']

def create_google_meet():
    creds = None
    if os.path.exists('token.json'):
        creds = Credentials.from_authorized_user_file('/home/jason9308/robot_ws/linebot_meet/token.json', SCOPES)
    else:
        flow = InstalledAppFlow.from_client_secrets_file('/home/jason9308/robot_ws/linebot_meet/credentials.json', SCOPES)
        creds = flow.run_local_server(port=0)
        with open('token.json', 'w') as token:
            token.write(creds.to_json())

    service = build('calendar', 'v3', credentials=creds)

    now = datetime.datetime.utcnow()
    event = {
        'summary': 'LineBot Meet',
        'start': {
            'dateTime': (now + datetime.timedelta(minutes=1)).isoformat() + 'Z',
            'timeZone': 'UTC',
        },
        'end': {
            'dateTime': (now + datetime.timedelta(minutes=30)).isoformat() + 'Z',
            'timeZone': 'UTC',
        },
        'conferenceData': {
            'createRequest': {
                'requestId': f"meet-{now.timestamp()}",
                'conferenceSolutionKey': {'type': 'hangoutsMeet'}
            }
        }
    }

    created_event = service.events().insert(
        calendarId='primary',
        body=event,
        conferenceDataVersion=1
    ).execute()

    meet_link = created_event['conferenceData']['entryPoints'][0]['uri']
    return meet_link

if __name__ == "__main__":
    link = create_google_meet()
    print("🔗 Google Meet Link:", link)

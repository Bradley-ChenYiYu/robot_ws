from flask import Flask, render_template, request, jsonify, send_from_directory
from flask_cors import CORS
import json
import os
from datetime import datetime  # 引入 datetime 來處理時間

app = Flask(__name__)
CORS(app, resources={r"/*": {"origins": "*"}})

json_path = '/home/jason9308/robot_ws/command_jason/output.json'
status_path = '/home/jason9308/robot_ws/command_jason/status.json'
close_meeting_path = '/home/jason9308/robot_ws/command_jason/close_meeting.json'

@app.route("/")
def index():
    return render_template("index.html")

@app.route('/json', methods=['GET'])
def get_json():
    try:
        if os.path.exists(json_path):
            with open(json_path, 'r') as f:
                data = json.load(f)
                if isinstance(data, list) and len(data) > 0:
                    return jsonify(data[0])
                else:
                    return jsonify({})
        else:
            return jsonify({})
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/json', methods=['POST'])
def update_json():
    try:
        new_data = request.get_json()
        command = new_data.get("command", "").lower()

        # 處理 shutdown 指令
        if command == "shutdown":
            print("接收到 shutdown 指令。")
            # 如果沒有指定 target，設為 "null" 字串
            new_data["target"] = str(new_data.get("target", "null"))

            # 取得當前時間，格式為 HH:MM:SS
            now = datetime.now().strftime("%H:%M:%S")
            new_data["time"] = now

            print("更新的 shutdown 資料：", new_data)
            # 如果要實際執行關機，可以解除下面註解（需要設定 sudo 無密碼權限）
            # os.system("sudo /sbin/shutdown now")

        with open(json_path, 'w') as f:
            json.dump([new_data], f, indent=4)

        return jsonify({"message": "JSON 已更新！"})
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/status', methods=['GET'])
def get_status():
    try:
        if os.path.exists(status_path):
            with open(status_path, 'r') as f:
                data = json.load(f)
                return jsonify(data)
        else:
            return jsonify({"scheduled_tasks": []})
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/static/images/<path:filename>')
def serve_image(filename):
    return send_from_directory('static/images', filename)

@app.route('/pose')
def get_pose():
    try:
        with open('/home/jason9308/robot_ws/command_jason/pose.json') as f:
            pose_data = json.load(f)
        return jsonify(pose_data)
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/close_meeting', methods=['GET'])
def get_close_meeting():
    try:
        if os.path.exists(close_meeting_path):
            with open(close_meeting_path, 'r') as f:
                data = json.load(f)
                return jsonify(data)
        else:
            return jsonify({"end_meeting": 0, "meet_link": ""})
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/close_meeting', methods=['POST'])
def set_close_meeting():
    try:
        req = request.get_json()
        if not isinstance(req, dict): req = {}
        data = {"end_meeting": req.get("end_meeting", 0)}

        if os.path.exists(close_meeting_path):
            with open(close_meeting_path, 'r') as f:
                original = json.load(f)
                data["meet_link"] = original.get("meet_link", "")

        with open(close_meeting_path, 'w') as f:
            json.dump(data, f, indent=4)

        return jsonify({"message": "已設定 end_meeting"})
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

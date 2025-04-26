from flask import Flask, render_template, request, jsonify, send_from_directory
from flask_cors import CORS
import json
import os

app = Flask(__name__)
CORS(app, resources={r"/*": {"origins": "*"}})

json_path = '/home/jason9308/robot_ws/command_jason/output.json'
status_path = '/home/jason9308/robot_ws/command_jason/status.json'

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


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

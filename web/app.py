from flask import Flask, render_template, request, jsonify
from flask_cors import CORS
import json
import os

app = Flask(__name__)
CORS(app, resources={r"/*": {"origins": "*"}})

json_path = '/home/jason9308/robot_ws/command_jason/output.json'

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
                    return jsonify(data[0])  # 回傳第一筆資料
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
        # 重新寫入整個陣列，只保留一筆資料（index 0）
        with open(json_path, 'w') as f:
            json.dump([new_data], f, indent=4)
        return jsonify({"message": "JSON 已更新！"})
    except Exception as e:
        return jsonify({"error": str(e)}), 500


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

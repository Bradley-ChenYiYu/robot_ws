# ==== 系統模組 ====
import sys
import os
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QTextBrowser, QLabel
from PyQt5.QtGui import QTextCursor, QTextCharFormat, QColor, QTextTableFormat, QTextBlockFormat
from PyQt5.QtCore import QTimer
from PyQt5.QtCore import Qt
os.environ["QT_QPA_PLATFORM_PLUGIN_PATH"] = "/usr/lib/x86_64-linux-gnu/qt5/plugins/platforms"

# ==== ROS2 ====
import rclpy
from rclpy.node import Node
from std_msgs.msg import String

# ======== ROS2 節點：訂閱聊天訊息 ========
class ChatListenerNode(Node):
    def __init__(self):
        super().__init__('chat_listener_node')
        self.subscription = self.create_subscription(String, '/chat_messages', self.callback, 10)
        self.messages = []

    def callback(self, msg):
        text = msg.data.strip()
        if text.lower() == 'clear':
            self.messages.clear()
        else:
            self.messages.append(text)

# ======== PyQt GUI：聊天介面 ========
class ChatGUI(QWidget):
    def __init__(self, ros_node):
        super().__init__()
        self.ros_node = ros_node
        self.setWindowTitle("Chat Viewer")
        self.resize(500, 600)

        self.title_label = QLabel("🤖 鐵柱｜您的送藥助理機器人")
        self.title_label.setStyleSheet("""
            font-size: 20px;
            font-weight: bold;
            color: #333;
            padding: 10px;
            background-color: #F5F5F5;
        """)

        self.chat_display = QTextBrowser()
        self.chat_display.setStyleSheet("background-color: white; font-size: 16px;")

        layout = QVBoxLayout()
        layout.addWidget(self.title_label)
        layout.addWidget(self.chat_display)
        self.setLayout(layout)

        self.timer = QTimer()
        self.timer.timeout.connect(self.update_chat)
        self.timer.start(1000)
        # self.timer.start(300)

        self.last_len = 0

    def update_chat(self):
        if len(self.ros_node.messages) != self.last_len:
            self.chat_display.clear()
            for msg in self.ros_node.messages:
                self.append_message(msg)
            self.last_len = len(self.ros_node.messages)

    def append_message(self, text):
        cursor = self.chat_display.textCursor()
        fmt = QTextCharFormat()
        block_fmt = QTextBlockFormat()

        if text.startswith("USER:"):
            fmt.setForeground(QColor("#000000"))
            block_fmt.setAlignment(Qt.AlignRight)
            bubble_color = "#DCF8C6"  # 淺綠
        elif text.startswith("BOT:"):
            fmt.setForeground(QColor("#000000"))
            block_fmt.setAlignment(Qt.AlignLeft)
            bubble_color = "#F1F0F0"  # 淺灰
        else:
            fmt.setForeground(QColor("gray"))
            block_fmt.setAlignment(Qt.AlignCenter)
            bubble_color = "#E0E0E0"

        cursor.insertBlock(block_fmt)
        self.chat_display.setTextCursor(cursor)

        # 用 <div> 包裹對話泡泡樣式
        html = f"""
        <div style="
            background-color: {bubble_color};
            border-radius: 12px;
            padding: 10px 14px;
            margin: 6px;
            display: inline-block;
            max-width: 70%;
            font-size: 16px;
            line-height: 1.4;
            word-wrap: break-word;
        ">
            {text}
        </div>
        """
        self.chat_display.insertHtml(html)
        self.chat_display.insertPlainText("\n")
        self.chat_display.moveCursor(QTextCursor.End)


# ======== 主程式入口 ========
def main():
    rclpy.init()
    ros_node = ChatListenerNode()
    app = QApplication(sys.argv)
    gui = ChatGUI(ros_node)
    gui.show()

    timer = QTimer()
    def spin_once():
        if rclpy.ok():
            rclpy.spin_once(ros_node, timeout_sec=0.01)
    timer.timeout.connect(spin_once)
    timer.start(200)

    app.exec_()
    ros_node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

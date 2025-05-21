# MainUI.py
# 整合 MapViewerGUI、ChatViewerGUI、RealsenseViewerGUI 的主視窗介面

import sys
import os
import rclpy
from rclpy.node import Node
from PyQt5.QtWidgets import QApplication, QWidget, QHBoxLayout, QVBoxLayout
from PyQt5.QtCore import QTimer

from ui_pkg.map_viewer import MapViewerGUI, MapViewerNode
from ui_pkg.chat_viewer import ChatGUI


# from realsense_viewer import RealsenseViewerGUI  # 假設你接下來會建立這個模組

class MainDashboard(QWidget):
    def __init__(self, ros_node):
        super().__init__()
        self.setWindowTitle("Robot Dashboard")
        self.resize(1600, 900)

        # 三個區塊介面
        self.map_gui = MapViewerGUI(ros_node)
        self.chat_gui = ChatGUI(ros_node)
        # self.realsense_gui = RealsenseViewerGUI()  # 可先顯示測試畫面

        # 橫向排版 layout（可改成 QVBoxLayout）
        layout = QHBoxLayout()
        layout.addWidget(self.map_gui, stretch=3)
        # layout.addWidget(self.realsense_gui, stretch=2)
        layout.addWidget(self.chat_gui, stretch=2)
        self.setLayout(layout)

        # UI 更新定時器：map_gui 自己有 timer，這裡只控制 ROS spin
        self.timer = QTimer()
        self.timer.timeout.connect(self.ros_spin_once)
        self.timer.start(10)

        self.ros_node = ros_node

    def ros_spin_once(self):
        if rclpy.ok():
            rclpy.spin_once(self.ros_node, timeout_sec=0.01)


def main():
    rclpy.init()
    ros_node = MapViewerNode()  # 可考慮改名為 UnifiedNode，未來也可管理聊天與影像

    app = QApplication(sys.argv)
    dashboard = MainDashboard(ros_node)
    dashboard.show()

    app.exec_()

    ros_node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()

import pybullet as p
import pybullet_data
import time

p.connect(p.GUI)
p.setAdditionalSearchPath(pybullet_data.getDataPath())

# 這裡改成你的夾爪 URDF 檔案路徑
gripper_urdf_path = "/home/jason9308/robot_ws/src/xarm_ros2/xarm_simulation/lite_gripper.urdf"

gripper_id = p.loadURDF(gripper_urdf_path, [0, 0, 0], useFixedBase=True)

for _ in range(10000):  # 顯示較長時間
    p.stepSimulation()
    time.sleep(0.01)

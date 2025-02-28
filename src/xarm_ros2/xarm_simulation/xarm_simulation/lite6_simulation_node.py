import rclpy
from rclpy.node import Node
from control_msgs.msg import JointJog  # 用於接收鍵盤控制指令
import pybullet as p
import pybullet_data
import os
import time

class Lite6SimulationNode(Node):
    def __init__(self):
        super().__init__('lite6_simulation_node')

        # 記錄最後一次收到指令的時間
        self.last_command_time = time.time()
        self.timeout = 0.5  # 超過 0.5 秒沒有新指令則停止

        # 初始化 PyBullet 模擬環境
        self.init_simulation()

        # 訂閱鍵盤控制的 JointJog topic
        self.subscription = self.create_subscription(
            JointJog,
            '/servo_server/delta_joint_cmds',
            self.joint_callback,
            10
        )
        self.get_logger().info("訂閱鍵盤控制 Topic: /servo_server/delta_joint_cmds")

        # 建立定時器，每 0.1 秒檢查是否應該讓關節停止
        self.create_timer(0.1, self.stop_joints_if_needed)

    def init_simulation(self):
        """初始化 PyBullet 模擬"""
        p.connect(p.GUI)
        p.setAdditionalSearchPath(pybullet_data.getDataPath())
        p.setGravity(0, 0, -9.8)

        urdf_path = os.path.expanduser("/home/jason9308/robot_ws/src/xarm_ros2/xarm_simulation/lite6.urdf")
        self.robot_id = p.loadURDF(urdf_path, [0, 0, 0], p.getQuaternionFromEuler([0, 0, 0]), useFixedBase=True)

        # 讀取所有可控關節
        self.joint_indices = {}
        for i in range(p.getNumJoints(self.robot_id)):
            joint_name = p.getJointInfo(self.robot_id, i)[1].decode()
            self.joint_indices[joint_name] = i  # 加入所有關節，不管類型

        self.get_logger().info(f"🔍 讀取到的所有關節: {list(self.joint_indices.keys())}")

        # 啟用即時模擬模式
        p.setRealTimeSimulation(1)

        # 加載 Lite6 夾爪並附加
        gripper_urdf_path = os.path.expanduser("/home/jason9308/robot_ws/src/xarm_ros2/xarm_simulation/lite_gripper.urdf")
        self.gripper_id = p.loadURDF(gripper_urdf_path, [0, 0, 0], useFixedBase=True)
        self.attach_gripper()

    def attach_gripper(self):
        """🔹 使用 PyBullet Constraint 附加夾爪到 `joint_eef`"""
        parent_link_index = -1

        self.get_logger().info("檢查所有關節的 parent-child 關係：")
        for i in range(p.getNumJoints(self.robot_id)):
            joint_info = p.getJointInfo(self.robot_id, i)
            joint_name = joint_info[1].decode()
            parent_link_index = joint_info[16]  # parent link index

            self.get_logger().info(f"Joint {i}: {joint_name} -> Parent Link Index: {parent_link_index}")

            if joint_name == "joint_eef":  # 確保找到 `joint_eef`
                self.get_logger().info(f"找到 joint_eef, 它的 parent link index 是 {parent_link_index}")
                break

        if parent_link_index == -1:
            self.get_logger().error("找不到 joint_eef, 無法附加 gripper!")
            return

        # 取得 `joint_eef` 的世界座標
        eef_pos, eef_orn = p.getLinkState(self.robot_id, parent_link_index)[:2]
        self.get_logger().info(f"joint_eef 位置: {eef_pos}, 朝向: {eef_orn}")

        # 設定夾爪相對於 `joint_eef` 的位置
        parentFramePosition = [0, 0, 0.04]  # 這裡要根據 URDF 調整
        childFramePosition = [0, 0, 0]

        constraint_id = p.createConstraint(
            parentBodyUniqueId=self.robot_id,
            parentLinkIndex=parent_link_index,
            childBodyUniqueId=self.gripper_id,
            childLinkIndex=-1,  # 夾爪的 Base Link
            jointType=p.JOINT_FIXED,
            jointAxis=[0, 0, 0],
            parentFramePosition=parentFramePosition,
            childFramePosition=childFramePosition
        )

        self.get_logger().info(f"成功附加夾爪到 joint_eef! (constraint ID: {constraint_id})")
        # 確保夾爪移動到 `joint_eef` 的位置
        p.resetBasePositionAndOrientation(self.gripper_id, eef_pos, eef_orn)
        self.get_logger().info(f" 夾爪已對齊 joint_eef 位置: {eef_pos}")

    def joint_callback(self, msg):
        """🔹 當收到鍵盤控制指令時，讓關節根據速度移動，並在沒有輸入時停止"""
        self.last_command_time = time.time()  # 更新最後收到指令的時間
        active_joints = set(msg.joint_names)  

        # 設定本次指令有控制的關節速度
        for joint_name, velocity in zip(msg.joint_names, msg.velocities):
            if joint_name in self.joint_indices:
                joint_index = self.joint_indices[joint_name]
                p.setJointMotorControl2(
                    bodyUniqueId=self.robot_id,
                    jointIndex=joint_index,
                    controlMode=p.VELOCITY_CONTROL,
                    targetVelocity=velocity,  # 設定速度
                    force=500
                )

        # 🔹 確保夾爪跟隨 `joint_eef` 的運動
        eef_pos, eef_orn = p.getLinkState(self.robot_id, self.joint_indices["joint_eef"])[:2]
        p.resetBasePositionAndOrientation(self.gripper_id, eef_pos, eef_orn)

    def stop_joints_if_needed(self):
        """🔹 檢查是否超過 0.5 秒沒有新指令，若是則停止所有關節"""
        if time.time() - self.last_command_time > self.timeout:
            for joint_name in self.joint_indices:
                p.setJointMotorControl2(
                    bodyUniqueId=self.robot_id,
                    jointIndex=self.joint_indices[joint_name],
                    controlMode=p.VELOCITY_CONTROL,
                    targetVelocity=0.0,  # 停止該關節
                    force=500
                )

def main(args=None):
    rclpy.init(args=args)
    node = Lite6SimulationNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()


# import rclpy
# from rclpy.node import Node
# from control_msgs.msg import JointJog  # 用於接收鍵盤控制指令
# import pybullet as p
# import pybullet_data
# import os
# import time

# class Lite6SimulationNode(Node):
#     def __init__(self):
#         super().__init__('lite6_simulation_node')

#         # 初始化 PyBullet 模擬環境
#         self.init_simulation()

#         # 訂閱鍵盤控制的 JointJog topic
#         self.subscription = self.create_subscription(
#             JointJog,
#             '/servo_server/delta_joint_cmds',
#             self.joint_callback,
#             10
#         )
#         self.get_logger().info("✅ 訂閱鍵盤控制 Topic: /servo_server/delta_joint_cmds")

#     def init_simulation(self):
#         """初始化 PyBullet 模擬"""
#         p.connect(p.GUI)
#         p.setAdditionalSearchPath(pybullet_data.getDataPath())
#         p.setGravity(0, 0, -9.8)

#         urdf_path = os.path.expanduser("/home/jason9308/robot_ws/src/xarm_ros2/xarm_simulation/lite6.urdf")
#         self.robot_id = p.loadURDF(urdf_path, [0, 0, 0], p.getQuaternionFromEuler([0, 0, 0]), useFixedBase=True)

#         # 讀取所有可控關節
#         self.joint_indices = {}
#         for i in range(p.getNumJoints(self.robot_id)):
#             joint_name = p.getJointInfo(self.robot_id, i)[1].decode()
#             self.joint_indices[joint_name] = i  # ✅ 加入所有關節，不管類型
#             p.changeDynamics(self.robot_id, i, linearDamping=0.04, angularDamping=0.04)

#         self.get_logger().info(f"🔍 讀取到的所有關節: {list(self.joint_indices.keys())}")


#         # ✅ 啟用即時模擬模式
#         p.setRealTimeSimulation(1)

#         # ✅ 加載 Lite6 夾爪並附加
#         gripper_urdf_path = os.path.expanduser("/home/jason9308/robot_ws/src/xarm_ros2/xarm_simulation/lite_gripper.urdf")
#         self.gripper_id = p.loadURDF(gripper_urdf_path, [0, 0, 0], useFixedBase=True)
#         # self.attach_gripper()

#     def attach_gripper(self):
#         """🔹 使用 PyBullet Constraint 附加夾爪到 `joint_eef`"""
#         parent_link_index = -1

#         self.get_logger().info("🔍 檢查所有關節的 parent-child 關係：")
#         for i in range(p.getNumJoints(self.robot_id)):
#             joint_info = p.getJointInfo(self.robot_id, i)
#             joint_name = joint_info[1].decode()
#             parent_link_index = joint_info[16]  # parent link index

#             self.get_logger().info(f"Joint {i}: {joint_name} -> Parent Link Index: {parent_link_index}")

#             if joint_name == "joint_eef":  # 確保找到 `joint_eef`
#                 self.get_logger().info(f"✅ 找到 joint_eef，它的 parent link index 是 {parent_link_index}")
#                 break

#         if parent_link_index == -1:
#             self.get_logger().error("❌ 找不到 joint_eef，無法附加 gripper！")
#             return

#         # 取得 `joint_eef` 的世界座標
#         eef_pos, eef_orn = p.getLinkState(self.robot_id, parent_link_index)[:2]
#         self.get_logger().info(f"📌 joint_eef 位置: {eef_pos}, 朝向: {eef_orn}")

#         # 設定夾爪相對於 `joint_eef` 的位置
#         parentFramePosition = [0, 0, 0.0]  # 這裡要根據 URDF 調整
#         childFramePosition = [0, 0, 0]

#         constraint_id = p.createConstraint(
#             parentBodyUniqueId=self.robot_id,
#             parentLinkIndex=parent_link_index,
#             childBodyUniqueId=self.gripper_id,
#             childLinkIndex=-1,  # 夾爪的 Base Link
#             jointType=p.JOINT_FIXED,
#             jointAxis=[0, 0, 0],
#             parentFramePosition=parentFramePosition,
#             childFramePosition=childFramePosition
#         )

#         self.get_logger().info(f"✅ 成功附加夾爪到 joint_eef！ (constraint ID: {constraint_id})")

#         # 確保夾爪移動到 `joint_eef` 的位置
#         p.resetBasePositionAndOrientation(self.gripper_id, eef_pos, eef_orn)
#         self.get_logger().info(f"📌 夾爪已對齊 joint_eef 位置: {eef_pos}")

#     def joint_callback(self, msg):
#         """🔹 當收到鍵盤控制指令時，讓關節根據速度移動，並在沒有輸入時停止"""
#         if not msg.joint_names or not msg.velocities:
#             self.get_logger().warn("⚠️ 收到空的 JointJog 訊息，忽略")
#             return

#         active_joints = set(msg.joint_names)  # 記錄本次有動的關節

#         # ✅ 設定本次指令有控制的關節速度
#         for joint_name, velocity in zip(msg.joint_names, msg.velocities):
#             if joint_name in self.joint_indices:
#                 joint_index = self.joint_indices[joint_name]
#                 p.setJointMotorControl2(
#                     bodyUniqueId=self.robot_id,
#                     jointIndex=joint_index,
#                     controlMode=p.VELOCITY_CONTROL,
#                     targetVelocity=velocity,  # ✅ 設定速度
#                     force=500
#                 )

#         # ✅ 讓沒有在控制的關節自動停止
#         for joint_name in self.joint_indices:
#             if joint_name not in active_joints:
#                 p.setJointMotorControl2(
#                     bodyUniqueId=self.robot_id,
#                     jointIndex=self.joint_indices[joint_name],
#                     controlMode=p.VELOCITY_CONTROL,
#                     targetVelocity=0.0,  # ✅ 停止該關節
#                     force=500
#                 )

#         # 🔹 確保夾爪跟隨 `joint_eef` 的運動
#         eef_pos, eef_orn = p.getLinkState(self.robot_id, self.joint_indices["joint_eef"])[:2]
#         p.resetBasePositionAndOrientation(self.gripper_id, eef_pos, eef_orn)

# def main(args=None):
#     rclpy.init(args=args)
#     node = Lite6SimulationNode()
#     rclpy.spin(node)
#     node.destroy_node()
#     rclpy.shutdown()

# if __name__ == '__main__':
#     main()

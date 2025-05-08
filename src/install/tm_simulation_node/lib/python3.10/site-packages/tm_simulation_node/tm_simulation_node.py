import rclpy
from rclpy.node import Node
from tm_msgs.srv import SetPositions
import pybullet as p
import pybullet_data
import os

class TMSimulationNode(Node):
    def __init__(self):
        super().__init__('tm_simulation_node')

        # 初始化 PyBullet 模型
        self.init_simulation()

        # 創建 Service Server
        self.srv = self.create_service(
            SetPositions,
            'set_positions',
            self.handle_set_positions
        )

        self.get_logger().info("TM Simulation Node ready to accept SetPositions requests.")

    def init_simulation(self):
        p.connect(p.GUI)
        p.setAdditionalSearchPath(pybullet_data.getDataPath())
        p.setGravity(0, 0, -9.8)

        # 加載模型
        robot_path = '/home/jason9308/robot_ws/src/tmr_ros2/tm_description/urdf/tm12-nominal.urdf'
        start_pos = [0, 0, 0.5]
        start_orientation = p.getQuaternionFromEuler([0, 0, 0])
        self.robot_id = p.loadURDF(robot_path, start_pos, start_orientation, useFixedBase=True)

        # 獲取機器人關節信息，僅保留可控的關節
        self.joint_indices = [
            i for i in range(p.getNumJoints(self.robot_id))
            if p.getJointInfo(self.robot_id, i)[2] == p.JOINT_REVOLUTE
        ]
        self.get_logger().info(f"Detected {len(self.joint_indices)} controllable joints.")

        # 打印每個關節的信息，便於檢查
        for i in range(p.getNumJoints(self.robot_id)):
            joint_info = p.getJointInfo(self.robot_id, i)
            joint_name = joint_info[1].decode()
            joint_type = joint_info[2]
            self.get_logger().info(f"Joint {i}: {joint_name}, Type: {joint_type}")

    def handle_set_positions(self, request, response):
        # """處理 SetPositions 服務請求"""
        if len(request.positions) != len(self.joint_indices):
            response.ok = False
            self.get_logger().error(
                f"Invalid number of joint positions: {len(request.positions)} (expected {len(self.joint_indices)}). "
                "Please ensure the control node sends the correct number of positions."
            )
            return response

        try:
        
            p.setRealTimeSimulation(1)
            # 設置每個關節的位置
            for joint_index, position in zip(self.joint_indices, request.positions):
                p.setJointMotorControl2(
                    bodyUniqueId=self.robot_id,
                    jointIndex=joint_index,
                    controlMode=p.POSITION_CONTROL,
                    targetPosition=position,
                    force=500  # 力矩限制
                )
            
            # 更新模擬
            # p.stepSimulation()
            
            response.ok = True
            self.get_logger().info("Joint positions updated successfully.")
        except Exception as e:
            response.ok = False
            self.get_logger().error(f"Error updating joint positions: {e}")

        return response

def main(args=None):
    rclpy.init(args=args)
    node = TMSimulationNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()


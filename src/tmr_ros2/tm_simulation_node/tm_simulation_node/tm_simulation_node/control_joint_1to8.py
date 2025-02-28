import rclpy
from tm_msgs.srv import SetPositions
from rclpy.node import Node

class JointControlNode(Node):
    def __init__(self):
        super().__init__('joint_control_node')
        self.client = self.create_client(SetPositions, 'set_positions')
        self.get_logger().info("Joint Control Node initialized.")

    def send_joint_position(self, joint_index, position):
        """發送關節位置到 Simulation Node"""
        # 建立請求
        request = SetPositions.Request()
        request.positions = [0] * 8  # 初始化所有關節位置為 0
        request.positions[joint_index] = position  # 設定選中的關節的位置
        
        # 發送請求並等待回應
        self.get_logger().info(f"Setting joint {joint_index+1} to {position} radians.")
        future = self.client.call_async(request)
        rclpy.spin_until_future_complete(self, future)

        if future.result() is not None and future.result().ok:
            self.get_logger().info(f"Joint {joint_index+1} set to {position} radians successfully.")
        else:
            self.get_logger().error(f"Failed to set joint {joint_index+1}.")

def main():
    rclpy.init()
    node = JointControlNode()

    # 等待 Service 服務器啟動
    while not node.client.wait_for_service(timeout_sec=1.0):
        node.get_logger().warn('Service not available, waiting again...')
    
    try:
        while True:
            # 顯示選項
            print("Select a joint (1-8) to rotate clockwise:")
            print("Enter '0' to quit.")
            joint_id = int(input()) - 1

            if joint_id == -1:
                print("Exiting...")
                break

            if 0 <= joint_id <= 7:
                angle = float(input(f"Enter the angle for joint {joint_id+1} (in radians): "))
                node.send_joint_position(joint_id, angle)
            else:
                print("Invalid joint number. Please enter a number between 1 and 8.")

    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()

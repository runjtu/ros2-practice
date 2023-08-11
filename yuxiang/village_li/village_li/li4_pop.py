import rclpy
from rclpy.node import Node

def main(args=None):
    rclpy.init(args=args)
    li4_node=Node("li4")
    li4_node.get_logger().info("i am li4")
    rclpy.spin(li4_node)
    rclpy.shutdown()
    """
    入口函数
        ros2运行该节点的入口函数
        编写ROS2节点的一般步骤
        1. 导入库文件
        2. 初始化客户端库
        3. 新建节点对象
        4. spin循环节点
        5. 关闭客户端库
    """

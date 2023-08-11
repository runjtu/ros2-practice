#ros2 pkg create topic_sub_py --build-type ament_python --dependencies rclpy nav_msgs --node-name sub_odom
import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry
import math
class SubOdom(Node):
    def __init__(self):
        super().__init__('sub_odom')
        self.last_x=0.0
        self.last_y=0.0
        self.subscription = self.create_subscription(
            Odometry,
            '/odom',
            self.listener_callback,
            10)
        self.subscription  # prevent unused variable warning
    def listener_callback(self, odom):
        x=odom.pose.pose.position.x
        y=odom.pose.pose.position.y
        dis_x=x-self.last_x
        dis_y=y-self.last_y
        distance=math.sqrt(dis_x*dis_x+dis_y*dis_y)
        if distance >= self.get_parameter('distance').get_parameter_value().double_value:
            self.get_logger().info('pos:(%.2f,%.2f)'%(x,y))
            self.last_x=x
            self.last_y=y
def main(args=None):
    rclpy.init(args=args)
    sub_odom = SubOdom()
    rclpy.spin(sub_odom)
    sub_odom.destroy_node()
    rclpy.shutdown()
#ros2 pkg create topic_pub_py --build-type ament_python --dependencies rclpy geometry_msgs --node-name pub_vel 
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist

class PubVel(Node):
    def __init__(self):
        super().__init__('pub_vel')
        self.declare_parameter('linear_x', 0.0)
        self.declare_parameter('angular_z', 0.0)
        self.twist_pub=self.create_publisher(Twist, '/cmd_vel', 10)
        #qos 10
        self.timer=self.create_timer(1.0,self.on_timer)
        
    def on_timer(self):
        #orgnization of Twist
        twist=Twist()
        twist.linear.x=self.get_parameter('linear_x').value
        twist.angular.z=self.get_parameter('angular_z').value
        self.twist_pub.publish(twist)
            
def main(args=None):
    rclpy.init(args=args)
    pub_vel=PubVel()
    rclpy.spin(pub_vel)
    pub_vel.destroy_node()
    rclpy.shutdown()
        

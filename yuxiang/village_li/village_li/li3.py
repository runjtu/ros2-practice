import rclpy
from rclpy.node import Node
from std_msgs.msg import String
#1.导入服务接口
from village_interfaces.srv import BorrowMoney



class BaiPiaoNode(Node):
    def __init__(self,name):
        super().__init__(name)
        self.get_logger().info("hi, I am li3")
        self.sub_free = self.create_subscription(String,"sexy_girl",self.recv_callback,10)

        #3.声明并创建客户端
        self.borrow_client = self.create_client(BorrowMoney,"borrow_money")
    
        
    def recv_callback(self,novel):
        self.get_logger().info('hi, I recieve %s' % novel.data)
        
        
    #5.调用客户端发送请求
    def borrow_money_eat(self,money=10):
        self.get_logger().info("要吃饭，借%d钱" % money)
        #确认服务在线，在线就跳出循环
        while not self.borrow_client.wait_for_service(1.0):
            self.get_logger().warn("服务不在线")
        #构造请求内容
        request = BorrowMoney.Request()
        request.name = self.get_name()
        request.money = money
        
        #发送异步借钱请求
        self.borrow_client.call_async(request).add_done_callback(self.borrow_response_callback)
        
        
        
        
    #2.创建请求结果接收回调函数
    def borrow_response_callback(self,response):
        """
        借钱结果回调
        4.编写结果接收逻辑
        """
        result = response.result()
        if result.success:
            self.get_logger().info("借到%d钱，去吃饭了" % result.money)
        else:
            self.get_logger().info("真抠门")

def main(args=None):
    rclpy.init(args=args)
    node = BaiPiaoNode("li3")
    node.borrow_money_eat()
    rclpy.spin(node)
    rclpy.shutdown()
    
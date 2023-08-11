#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
# 1. 导入消息类型
from std_msgs.msg import String,UInt32#注意这里是逗号

from village_interfaces.srv import BorrowMoney
"""
    导入服务接口
    创建服务端回调函数
    声明并创建服务端
    编写回调函数逻辑处理请求
"""    
    
#导入系统内置的图片消息类型
from sensor_msgs.msg import Image
#从村庄接口话题消息类中导入小说消息类型
from village_interfaces.msg import Novel
    
"""
    1.导入消息类型
    2.声明并创建发布者
    3.编写发布逻辑发布数据三
"""

class WriterNode(Node):
    def __init__(self,name):
        super().__init__(name)
        self.get_logger().info("i am %s." % name)
        # 2.创建并初始化发布者成员属性pub_novel
        self.pub_novel = self.create_publisher(String,"sexy_girl",10)
        
        #self.write = self.create_publisher(Novel,"sexy_girl", 10) 
        """
        #声明一个空的图像
        self.image = Image()
        # 开始获取图片
        self.create_subscription(Novel,"image",self.recv_image_callback,10)
        
        """
        #3. 编写发布逻辑
        self.count = 0 #章节编号
        self.timer_period = 5
        self.timer = self.create_timer(self.timer_period,self.timer_callback)
        
        self.account = 80
        self.sub_money = self.create_subscription(UInt32,"sexy_girl_money",self.recv_money_callback,10)
        
        #create_service的参数，类型，名称，回调函数
        self.borrow_server = self.create_service(BorrowMoney,"borrow_money",self.borrow_money_callback)
        
        #参数1.声明参数
        self.declare_parameter("writer_timer_period",5)
        
    """
    def recv_image_callback(self,image):
        self.image = image    
    """
        
    def recv_money_callback(self,money):
        self.account += money.data
        self.get_logger().info("收到%d的稿费，现在账户里有%d的钱" % (money.data,self.account))
    
    #创建服务端回调函数    
    def borrow_money_callback(self,request,response):
        self.get_logger().info("收到%s的借钱请求，账户里有%d的钱" % (request.name,self.account))
        if request.money <= self.account*0.1:
            response.success = True
            response.money = request.money
            self.account = self.account - request.money
            self.get_logger().info("借钱成功，借出%d,目前账户还有%d" % (response.money,self.account))
        else:
            response.success = False
            response.money = 0
            self.get_logger().info("对不起，手头紧张")
        return response
        
        
        
    def timer_callback(self):
        timer_period = self.get_parameter("writer_timer_period").get_parameter_value().integer_value
        self.timer.timer_period_ns = timer_period * (1000*1000*1000)
        msg = String()
        msg.data = '第%d回：潋滟湖 %d 次偶遇胡艳娘' % (self.count,self.count)
        self.pub_novel.publish(msg)
        self.get_logger().info("发布了一个章节的小说，内容是：%s" % msg.data)
        self.count += 1
    """
    def timer_callback(self):
        msg = Novel()
        msg.content = '第%d回：潋滟湖 %d 次偶遇胡艳娘' % (self.i,self.i)
        msg.image = self.image
        self.write.publish(msg)  #将小说内容发布出去
        self.get_logger().info('李四:我发布了艳娘传奇："%s"' % msg.content)    #打印一下发布的数据，供我们看
        self.get_logger().info('李四:并且为艳娘传奇配上了插图，长："%d"，宽：%d' % (msg.image.height,msg.image.width))    #打印一下发布的插图尺寸，供我们看
    """
def main(args=None):
    rclpy.init(args=args)
    li4_node=WriterNode("li4")
    #li4_node.get_logger().info("i am li4")
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

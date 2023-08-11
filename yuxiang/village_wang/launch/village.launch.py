# 1.导入库
from launch import LaunchDescription
from launch_ros.actions import Node

# 2.定义函数名称为：generate_launch_description
def generate_launch_description():
    # 3.创建Actions.Node对象li_node，标明李四所在位置
    li4_node = Node(
        package="village_li",
        executable="li4_node",
        namespace="mirror",
        parameters=[{'writer_timer_period': 1}]
        )
    # 创建Actions.Node对象wang2_node，标明王二所在位置
    wang2_node = Node(
        package="village_wang",
        executable="wang2_node"
        )
    # 创建LaunchDescription对象launch_description,用于描述launch文件
    launch_description = LaunchDescription([li4_node,wang2_node])
    # 返回让ROS2根据launch描述执行节点
    return launch_description
// ros2 pkg create topic_pub --build-type ament_cmake --dependencies rclcpp geometry_msgs --node-name pub_vel
//circulation publish vel msg
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <chrono>
using namespace std::chrono_literals;
class PubVel : public rclcpp::Node
{
public:
    PubVel():Node("pub_vel")
    {
      //dynimic param init, templeate type is double
        this->declare_parameter<double>("linear_x",0.0);
        this->declare_parameter<double>("angular_z",0.0);
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel",10);
        timer_ = this->create_wall_timer(100ms,std::bind(&PubVel::timer_callback,this));
    }

private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    void timer_callback()
    {
        geometry_msgs::msg::Twist twist;
        twist.linear.x = this->get_parameter("linear_x").as_double();
        twist.angular.z = this->get_parameter("angular_z").as_double();
        publisher_->publish(twist);
        RCLCPP_INFO(this->get_logger(),"Publishing: linear.x = %f, angular.z = %f",msg_->linear.x,msg_->angular.z);
    }
}
int main(int argc,char **argv)
{
    //init ros2
    rclcpp::init(argc,argv);
    //spin and input obj ptr
    rclcpp::spin(std::make_shared<PubVel>());
    /*
    //if you use base obj programming
    auto node = rclcpp::Node::make_shared("pub_vel");
    auto publisher = node->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel",10);
    auto msg = std::make_shared<geometry_msgs::msg::Twist>();
    msg->linear.x = 1.0;
    msg->angular.z = 1.0;
    rclcpp::WallRate loop_rate(1);
    while(rclcpp::ok())
    {
        publisher->publish(msg);
        RCLCPP_INFO(node->get_logger(),"Publishing: linear.x = %f, angular.z = %f",msg->linear.x,msg->angular.z);
        rclcpp::spin_some(node);
        loop_rate.sleep();
    }
    */
    //release resources
    rclcpp::shutdown();
    return 0;
}
//ros2 pkg create topic_sub --build-type ament_cmake --dependencies rclcpp nav_msgs --node-name sub_odom
#inclde "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
using namespace std::placeholders;

class SubOdom : public rclcpp::Node
{
public:
    SubOdom() : Node("sub_odom")
    {
        last_x=0.0;
        last_y=0.0;
        this->declare_parameter<double>("distance", 0.5);
        sub_ = this->create_subscription<nav_msgs::msg::Odometry>(
            "/odom", 10, std::bind(&SubOdom::on_timer, this, std::placeholders::_1));//_1
    }

private:
/*
    void on_timer(const nav_msgs::msg::Odometry::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(), "subscribe odom: [%f, %f, %f]",
                    msg->pose.pose.position.x,
                    msg->pose.pose.position.y,
                    msg->pose.pose.position.z);
    }
*/
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr sub_;
    double last_x, last_y;
    void on_timer(const nav_msgs::msg::Odometry &odom)
    {
        double curr_x = odom.pose.pose.position.x;
        double curr_y = odom.pose.pose.position.y;
        //double curr_z = odom.pose.pose.position.z;
        double distance =sqrt(pow((curr_x-last_x),2)+pow((curr_y-last_y),2));
        if (distance >= this->get_parameter("distance").as_double())
        {
            RCLCPP_INFO(this->get_logger(), "subscribe odom: [%.2f, %.2f]",
                    curr_x, curr_y);
            last_x = curr_x;
            last_y = curr_y;
        }       
    }


}

int main(int argc, char **argv)//char const *argv[] is ok
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SubOdom>());
    rclcpp::shutdown();
    return 0;
}
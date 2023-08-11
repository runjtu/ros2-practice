#include "rclcpp/rclcpp.hpp"


int main(int argc,char ** argv)
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<rclcpp::Node>("wang2");/*智能指针*/

    RCLCPP_INFO(node->get_logger(),"I am wang2.");
    
    rclcpp::spin(node);
    rclcpp::shutdown();
}

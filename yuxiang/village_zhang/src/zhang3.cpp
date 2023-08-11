#include "rclcpp/rclcpp.hpp"
#include "village_interfaces/srv/sell_novel.hpp"

// 提前声明的占位符，留着创建客户端的时候用
using std::placeholders::_1;

/*
    创建一个类节点，名字叫做PoorManNode,继承自Node.
*/
class PoorManNode : public rclcpp::Node
{

public:
    /* 构造函数 */
    PoorManNode(std::string name) : Node(name)
    {
        // 打印一句自我介绍
        RCLCPP_INFO(this->get_logger(), "大家好，我是得了穷病的张三.");
        novel_client = this->create_client<village_interfaces::srv::SellNovel>("sell_novel");
    }  

    //5.编写发布请求逻辑
    void buy_novels()
    {
        //1.等待服务端上线
        RCLCPP_INFO(this->get_logger(), "准备去买小说了");
        while (!novel_client->wait_for_service(std::chrono::seconds(1)))
        {
            RCLCPP_WARN(this->get_logger(), "等待服务端上线");
        }
        
        //2.构造请求数据
        auto request = std::make_shared<village_interfaces::srv::SellNovel_Request>();
        request->money = 5;

        //3.发送异步请求
        novel_client->async_send_request(request,std::bind(&PoorManNode::novels_callback,this,_1));

    }



private:
    //声明和创建
    rclcpp::Client<village_interfaces::srv::SellNovel>::SharedPtr novel_client;
//编写接收小说结果的回调函数
    void novels_callback(rclcpp::Client<village_interfaces::srv::SellNovel>::SharedFuture response)
    {
        //智能指针接收
        auto result = response.get();

        RCLCPP_INFO(this->get_logger(),"收到了第%d章的小说，现在开始读",result->novels.size());

        for(std::string novel:result->novels)
        {
            //打印每一章节内容
            RCLCPP_INFO(this->get_logger(),"%s",novel.c_str());

        }
        RCLCPP_INFO(this->get_logger(),"看完了");
    }
};

/*主函数*/
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    /*产生一个Zhang3的节点*/
    auto node = std::make_shared<PoorManNode>("zhang3");
    node->buy_novels();
    /* 运行节点，并检测rclcpp状态*/
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
#include "rclcpp/rclcpp.hpp"
//1.导入消息类型 std_msgs/msg/String
#include "std_msgs/msg/string.hpp"
//1.导入发布者的消息接口类型
#include "std_msgs/msg/u_int32.hpp"
//1.导入服务接口
#include "village_interfaces/srv/sell_novel.hpp"

#include <queue>

/*
    

    

    声明并创建服务端

    编写回调函数逻辑处理请求
*/

using std::placeholders::_1;
using std::placeholders::_2;
//占位符

/*
    创建一个类节点，名字叫做SingleDogNode,继承自Node.
*/

class SingleDogNode:public rclcpp::Node
{
private://成员函数
    //3.声明订阅者
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_novel;
    
    //2.声明话题发布者
    rclcpp::Publisher<std_msgs::msg::UInt32>::SharedPtr pub_money;

    //书库
    std::queue<std::string> novels_queue;

    //声明书的单价
    unsigned int novel_price = 1;


    //声明服务端，模板类，需要声明接口
    rclcpp::Service<village_interfaces::srv::SellNovel>::SharedPtr sell_server;


    //声明回调组
    rclcpp::CallbackGroup::SharedPtr sell_novels_callback_group;

    //2.创建回调函数
    void novel_callback(const std_msgs::msg::String::SharedPtr novels)//共享指针
    {
        //编写发布逻辑发布数据
        std_msgs::msg::UInt32 money;
        money.data = 10;
        pub_money->publish(money);

        //把收到的小说放进书库
        novels_queue.push(novels->data);

        //4.编写回调处理逻辑
        //宏定义，传两个参数
        RCLCPP_INFO(this->get_logger(),"朕已阅%s",novels->data.c_str());

    }

    //2.创建买书请求服务端回调函数
    void sell_novel_callback(const village_interfaces::srv::SellNovel::Request::SharedPtr request,
                            const village_interfaces::srv::SellNovel::Response::SharedPtr response)
        //判断当前书的章节数量是否足够，不够就攒书，再返回
        //等待novel_queue中书的数量达到卖出
    {

        RCLCPP_INFO(this->get_logger(),"收到一个买书请求，一共给了%d 元",request->money);
        //计算应该返回给客户端的小说数量

        this->get_parameter("novel_price",novel_price);

        unsigned int num = (int)request->money/(novel_price);

        if(num>novels_queue.size())
        {
            //等待凑齐书
            RCLCPP_INFO(this->get_logger(),"书不够，书库%d，要卖出的书%d",novels_queue.size(),num);
            //凑书
            rclcpp::Rate rate(1);
            while (novels_queue.size()<num)
            {
                RCLCPP_INFO(this->get_logger(),"等待中，目前还差%d的小说",num-novels_queue.size());
                rate.sleep();
            }
        }

        RCLCPP_INFO(this->get_logger(),"当前书库里的书有%d，已经要大于要卖出的书的数量%d",novels_queue.size(),num);

        for(int i=0;i<(int)num;i++)
        {
            response->novels.push_back(novels_queue.front());//取书，队列
            novels_queue.pop();
        }
        

    }

public://构造函数
    SingleDogNode(std::string name):Node(name)/*Node()需要参数*/
    {
        RCLCPP_INFO(this->get_logger(),"I am %s.",name.c_str());/*name是一个字符串*/
        //3.创建订阅者
        sub_novel = this->create_subscription<std_msgs::msg::String>("sexy_girl",10,std::bind(&SingleDogNode::novel_callback,this,_1));//bind()需要三个参数,10是队列长度

        //2.创建话题的发布者
        pub_money = this->create_publisher<std_msgs::msg::UInt32>("sexy_girl_money",10);

        sell_novels_callback_group = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);//写传入类型
        //声明并创建服务端，同时定义了回调函数组

        //实例化，服务名称，成员函数作回调函数
        sell_server = this->create_service<village_interfaces::srv::SellNovel>("sell_novel",
                                                                                std::bind(&SingleDogNode::sell_novel_callback,this,_1,_2),
                                                                                rmw_qos_profile_services_default,
                                                                                sell_novels_callback_group);
        this->declare_parameter<std::int64_t>("novel_price",novel_price);
        //模板函数需要传入类型<>，（名字，值）
    }
};




int main(int argc,char ** argv)
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<SingleDogNode>("wang2");/*智能指针*/

    RCLCPP_INFO(node->get_logger(),"I am wang2.");

    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.spin();
    //rclcpp::spin(node);
    rclcpp::shutdown();
}

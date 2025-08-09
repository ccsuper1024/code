#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <chrono>

class TurtleCircleNode
    : public rclcpp::Node       //必须公开继承
{
public:
    explicit TurtleCircleNode(const std::string &node_name)
        : rclcpp::Node(node_name)
    {
        //创建主题，第二个参数是qos
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
        //可以用任意的可调用函数
        timer_ = this->create_wall_timer(std::chrono::seconds(1), std::bind(&TurtleCircleNode::timer_callback, 
                                                                            this));      
        RCLCPP_INFO(this->get_logger(), "TurtleCircleNode(const std::string& node_name)");
    }
    void timer_callback()
    {
        auto msg = geometry_msgs::msg::Twist();
        msg.linear.x = 1.0;
        msg.angular.z = 0.5;
        publisher_->publish(msg);

    }
private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TurtleCircleNode>("turtle_circle");
    rclcpp::spin(node);
    rclcpp::shutdown();

    return 0;
}
#include "rclcpp/rclcpp.hpp"

class PersonNode
:public rclcpp::Node
{
public:
    //构造函数
    PersonNode(const std::string& nodeName, const std::string& name, const int& age)
    :Node(nodeName),
    name_(name),
    age_(age)
    {
        RCLCPP_INFO(this->get_logger(), "%s: Person Node has been created.", __func__);
        RCLCPP_INFO(this->get_logger(), "Name: %s, age: %d", name_.c_str(), age_);
    }
    //析构函数
    ~PersonNode()
    {
        RCLCPP_INFO(this->get_logger(), "Person Node has been destroyed.");   
    }
public:
    void eat(const std::string foodName)
    {
        RCLCPP_INFO(this->get_logger(), "%s is eating %s.", name_.c_str(), foodName.c_str());
    }
private:
    std::string name_;
    int age_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PersonNode>("PersonNode", "chenchao", 30);
    node->eat("apple");
    RCLCPP_INFO(node->get_logger(), "Hello, 30!");
    rclcpp::spin(node);
    rclcpp::shutdown();

    return 0;
}
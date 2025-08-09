#include "rclcpp/rclcpp.hpp"
#include <cstring>

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);                               //初始化ROS2
    auto node = std::make_shared<rclcpp::Node>("cpp_node"); //共享指针,共享所有权的智能指针
    RCLCPP_INFO(node->get_logger(), "你好，C++结点");
    rclcpp::spin(node);                                     //阻塞直到结点结束
    rclcpp::shutdown();
    return 0;
}
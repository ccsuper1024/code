#!/usr/bin/env python
# coding=utf-8
import rclpy
from rclpy.node import Node

def main():
    rclpy.init()                                #初始化recl模块,分配资源
    node = Node("python node")
    node.get_logger().info("你好 Python ROS2")  #打印日志
    rclpy.spin(node)                            #插入节点，循环运行阻塞运行
    rclpy.shutdown()

if __name__ == "__main__":
    main()
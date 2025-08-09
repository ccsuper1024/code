import rclpy
from rclpy.node import Node
import requests
from example_interfaces.msg import String
from queue import Queue

class NovelPubNode(Node):
    def __init__(self, node_name:str):
        super().__init__(node_name)
        self.get_logger().info("{} 启动".format(node_name))
        self.novels_queue_ = Queue()     #创建队列
        self.novel_publisher_ = self.create_publisher(String, 'novel', 10)
        self.create_timer(5 ,self.timer_callback)

    def timer_callback(self):
        if self.novels_queue_.qsize() > 0:
            line = self.novels_queue_.get()
            msg = String()   #$组装
            msg.data = line
            self.novel_publisher_.publish(msg)
            self.get_logger().info("发布了:{}".format(msg))


    def download(self, url:str):
        response = requests.get(url)
        response.encoding = 'utf-8'
        text = response.text
        self.get_logger().info("下载{}, len{}".format(url, len(text)))
        for line in text:
            self.novels_queue_.put(line)
    
def main():
    rclpy.init()
    node = NovelPubNode("Novel_node")
    node.download("http://0.0.0.0:8000/novel.txt")
    rclpy.spin(node)
    rclpy.shutdown()

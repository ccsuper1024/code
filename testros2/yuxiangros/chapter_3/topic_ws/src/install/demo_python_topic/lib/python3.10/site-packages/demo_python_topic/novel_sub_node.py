#订阅小说并合成语音
import threading
import time
import espeakng
import rclpy
from rclpy.node import Node
from example_interfaces.msg import String
from queue import Queue

class NovelSubNode(Node):
    def __init__(self, node_name:str):
        super().__init__(node_name)
        self.get_logger().info("{}启动".format(node_name))
        self.novels_queue_ = Queue()
        self.novel_subscriber_ = self.create_subscription(String, "novel", self.novel_callback, 10)
        self.speech_thread_ = threading.Thread(target=self.speak_thread)
        self.speech_thread_.start()
    
    def novel_callback(self, msg):
        self.novels_queue.put(msg.data)

    def speak_thread(self):
        speaker = espeakng.Speaker()
        speaker.voice = "zh"

        while rclpy.ok():       #检测当前ROS上下文是否OK
            if self.novels_queue_.qsize() > 0:
                text = self.novels_queue_.get()
                self.get_logger().info("朗读{}".format(text))
                speaker.say(text)
                speaker.wait()      #阻塞等待完成
            else:
                #让当前线程休眠1s,出让CPU
                time.sleep(1)   

    
def main():
    rclpy.init()
    node = NovelSubNode("novel_sub")
    rclpy.spin(node)
    rclpy.shutdown()


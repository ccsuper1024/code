#include<hv/EventLoop.h>

using namespace hv;

int main(void){
    //创建一个时间循环对象
    EventLoopPtr loop(new EventLoop);

    //在当前线程中设置一个定时器
    //typedef std::function<void(TimerID)>    TimerCallback
    //typedef uint64_t            TimerID;
    loop->setInterval(1000,[](TimerID timerID){ //隐式捕获   超时1s
        fprintf(stdout,"time = %lus\n",static_cast<unsigned long>(time(nullptr)));
    });

    //运行事件循环
    loop->run();
}
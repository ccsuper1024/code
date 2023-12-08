#include <hv/hv.h>
#include <hv/EventLoop.h>

using namespace hv;

static void onTimer(TimerID timerID,int n){
    fprintf(stdout,"tid = %ld timerID = %lu,timer = %lus n = %lu\n",
            hv_gettid(),static_cast<unsigned long>(timerID),
            static_cast<unsigned long>(time(nullptr)));
}

int main(int argc,char* argv[]){
    HV_MEMCHECK;
    fprintf(stdout,"main tid=%ld\n",hv_gettid());

    auto loop = std::make_shared<EventLoop>();  //函数模板

    //runEvery 1s
    loop->setInterval(1000,std::bind(onTimer,std::placeholders::_1,100));

    //runAfter 10s
    loop->setTimeout(10000,[&loop](TimerID timerID){
        loop->stop();
    });

    //queueInLoop
    //typedef std::function<void()> Functor;
    loop->queueInLoop([](){
        fprintf(stdout,"queueInLoop tid=%ld\n",hv_gettid());
    });

    //runInLoop
    loop->runInLoop([](){
        fprintf(stdout,"runInLoop tid = %ld\n",hv_gettid());
    });

    //run until loop stoped
    loop->run();
}


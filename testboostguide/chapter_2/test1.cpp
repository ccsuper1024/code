#define BOOST_TIMER_ENABLE_DEPRECATED
#include <iostream>
#include <boost/timer.hpp>
using namespace boost;

int main(void)
{
    timer t;                            //声明一个计时器对象

    std::cout << "max timespan:"        //可度量的最大时间，以小时为单位
        << t.elapsed_max() / 3600 << "h" <<std::endl; 
    std::cout << "min timespan:"        //可度量的最小时间，以秒为单位
        << t.elapsed_min()  << "s" <<std::endl; 
    std::cout << "now elapsed:"        //输出已经流逝的时间
        << t.elapsed()  << "s" <<std::endl; 
    return 0;
}


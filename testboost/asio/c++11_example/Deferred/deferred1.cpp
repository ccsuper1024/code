/*  @file @brief @author chenchao @brief */
//c std header
//c++ std header
#include <iostream>
//c third header
//c++ third header
#include <boost/asio.hpp>
//c custom header
//c++ custom header

using std::endl;
using std::cout;
using std::cerr;
using std::cin;

//延迟调用策略
using boost::asio::deferred;
/* @brief @pram @return */

void test(){
    //executor
    boost::asio::io_context ctx;
    //稳定的时钟
    boost::asio::steady_timer timer(ctx);
    timer.expires_after(std::chrono::seconds(1));   //1s后过期

    //延迟调用
    auto deferred_op = timer.async_wait(deferred);

    std::move(deferred)(    //std::move()返回一个右值对象，再把这个对象当做一个函数对象调用
        [](boost::system::error_code ec) {  //捕获列表为空
            std::cout << "timer wait finished: " << ec.message() << "\n";
        }
    );

    ctx.run();
}
int main()
{
    test();
    return 0;
}


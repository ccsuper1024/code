/*  @file 
    @author chenchao 
    @brief deferred 2
*/
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
using boost::asio::deferred;    //延迟调用的using定义


/* @brief @pram @return */
void test(){
    boost::asio::io_context ctx;

    boost::asio::steady_timer timer(ctx);
    timer.expires_after(std::chrono::seconds(1));   //1s后过期

    //异步延迟等待
    auto deferred_op = timer.async_wait(deferred(
        [&](boost::system::error_code ec) { //引用捕获,隐式捕获的一种
            std::cout << "first timer wait finished: " << ec.message() << std::endl;
            return timer.async_wait(deferred);
        }));
    std::move(deferred_op) (
        [](boost::system::error_code ec) {
            std::cout << "second timer wait finished: " << ec.message() << std::endl;
        } 
    );
}
int main()
{
    test();
    return 0;
}


/*  @file @author chenchao @brief */

// c std header
// c++ std header
#include <iostream>
#include <cstdlib>
#include <thread>
#include <utility>

// c third header
// c++ third header
#include <boost/asio.hpp>
// c custom header
// c++ custom header

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using boost::asio::ip::tcp;
const int max_length = 1024;

void session(tcp::socket sock) {
    try{
        for(;;) {
            char data[max_length];

            boost::system::error_code error;
            size_t length = sock.read_some(boost::asio::buffer(data), error);
            if(error == boost::asio::error::eof) {
                break;      //Connection closed cleanly by beer.
            } else if(error) {
                throw boost::system::system_error(error);   //some other error
            }
                boost::asio::write(sock, boost::asio::buffer(data, length));
        }
    }catch(std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void server(boost::asio::io_context& io_context, unsigned short port) {
    tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
    while(true) {
        std::thread(session, a.accept()).detach();  //将session函数作为线程入口函数，detach()表明这是个分离线程。线程的结果不会返回给主线程
    }
}
/* @brief @pram @return */
int main(int argc, char* argv[])
{
    try{
        if(argc != 2) {
            std::cerr << "Usage: blocking_tcp_echo_server <port> \n";
            return -1;
        }
        boost::asio::io_context io_context; //调度器
        server(io_context, std::atoi(argv[1]));
    }catch(std::exception& e ) {
        std::cerr << "Exception： " << e.what() << "\n";
    }
    return 0;
}

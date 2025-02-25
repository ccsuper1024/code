/*  @file @author chenchao @brief */

// c std header
// c++ std header
#include <iostream>
#include <cstdlib>
#include <memory>
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
class session
:public std::enable_shared_from_this<session>
{
private:
    tcp::socket socket_;
    enum {  max_length = 1024   };  //声明一个枚举类型。非限定作用域的
    char data_[max_length] = {0};
private:
    void do_read() {
        auto self(this->shared_from_this());    //shared_from_this()函数是this指针的基类成员函数
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length) {
            if(!ec) {
                this->do_write(length);   //do_write也是this指针指向对象的成员函数,所以捕获列表中的this指针也被实际使用了
            }
        });
    }
    void do_write(std::size_t length) {
        auto self(this->shared_from_this());    //shared_from_this()返回this指针指向对象的shared_ptr对象
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
        [this,self](boost::system::error_code ec, std::size_t /*length*/) {
            if(!ec) {
                do_read();
            }
        });
    }
public:
    session(tcp::socket socket)
    :socket_(std::move(socket)) {
        fprintf(stdout, "session(tcp::socket socket)\n");
    }

    void start() {
        do_read();
    }
};

class server
{
private:
    tcp::acceptor acceptor_;
private:
    void do_accept() {
        this->acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if(!ec) {
                    std::make_shared<session>(std::move(socket))->start();
                }
            }
        );
    }
public:
    server(boost::asio::io_context& io_context, short port) 
    :acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
    {
        do_accept();
        fprintf(stdout, "server(boost::asio::io_context& io_context, short port)\n");
    }
};

/* @brief @pram @return */
int main(int argc, char* argv[]) 
{
    try {
        if(argc != 2) {
            std::cerr << "Usage: async_tcp_echo_server <port>\n";
            return -1;
        }
        boost::asio::io_context io_context;

        server serve(io_context, std::atoi(argv[1]));

        io_context.run();
    } catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}

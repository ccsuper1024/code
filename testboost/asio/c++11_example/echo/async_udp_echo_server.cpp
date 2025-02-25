/*  @file @author chenchao @brief */

//c std header
//c++ std header
#include <cstdlib>
#include <iostream>
//c third header
//c++ third header
#include<boost/asio.hpp>
//c custom header
//c++ custom header

using std::endl;
using std::cout;
using std::cerr;
using std::cin;
using boost::asio::ip::udp;

class server
{
private:
    udp::socket socket_;
    udp::endpoint sender_endpoint_;
    enum {  max_length = 1024   };
    char data_[max_length];
public:
    server(boost::asio::io_context& io_context, short port)
    :socket_(io_context, udp::endpoint(udp::v4(), port))
    {
        do_receive();
        fprintf(stdout, "server(boost::asio::io_context& io_context, short port)\n");
    }

    ~server() {
        fprintf(stdout, "~server()\n");
    }
    void do_receive() {
        socket_.async_receive_from(boost::asio::buffer(data_, max_length),
        sender_endpoint_,
        [this](boost::system::error_code ec, std::size_t bytes_recvd){
            if(!ec && bytes_recvd > 0) {
                this->do_send(bytes_recvd);
            } else {
                this->do_receive();
            }
        });
    }

    void do_send(std::size_t length) {
        socket_.async_send_to(boost::asio::buffer(data_, length), 
        sender_endpoint_,
        [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {
            do_receive();
        });
    }
};
int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cerr << "Usage: async_udp_echo_server <port>\n";
        return -1;
    }
    boost::asio::io_context io_context;

    server s(io_context, std::atoi(argv[1]));

    io_context.run();
}
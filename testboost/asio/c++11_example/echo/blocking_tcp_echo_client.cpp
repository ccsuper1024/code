/*  @file 
    @author chenchao 
    @brief blockig_tcp_echo_client.cpp
 */

//c std header
//c++ std header
#include <cstdlib>
#include <cstring>
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
using boost::asio::ip::tcp;

const int MAX_LENGTH = 1024;
//enum { max_length = 1024};    //非限定作用域的枚举类型，限定作用域的枚举类型是enum class {}，使用时要加限定作用域
/* @brief @pram @return */
int main(int argc, char* argv[])
{
    if(argc != 3) {
        std::cerr << "Usage: blocking_tcp_echo_client <host> <post>\n";
        return -1;
    }

    boost::asio::io_context io_context;
    tcp::socket sock(io_context);
    tcp::resolver resolver(io_context);
    boost::asio::connect(sock, resolver.resolve(argv[1], argv[2]));

    std::cout << "Enter message: ";
    char request[MAX_LENGTH];
    std::cin.getline(request, MAX_LENGTH);
    size_t request_length = std::strlen(request);
    boost::asio::write(sock, boost::asio::buffer(request, request_length));

    char reply[MAX_LENGTH];
    size_t reply_length = boost::asio::read(sock, 
    boost::asio::buffer(reply, request_length));
    std::cout << "Reply is: " ;
    std::cout.write(reply, reply_length);
    std::cout << "\n";

    return 0;
}

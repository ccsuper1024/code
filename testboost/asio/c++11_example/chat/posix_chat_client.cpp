/*  @file
 @brief
@author chenchao
@brief：The following POSIX-specific chat client demonstrates
 how to use the posix::stream_descriptor class to perform console
     input and output
 */
//c std library
//cpp std library
#include<array>
#include<cstdlib>
#include<boost/asio.hpp>
#include <iostream>
//c custom library
//cpp custom library
#include "chat_message.hpp"

#if defined(BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR)

using boost::asio::ip::tcp;
namespace posix = boost::asio::posix;

class posix_chat_client
{
private:  
    tcp::socket socket_;
    posix::stream_descriptor input_;
    posix::stream_descriptor output_;
    chat_message read_msg_;
    chat_message write_msg_;
    boost::asio::streambuf input_buffer_;
public:
    posix_chat_client(boost::asio::io_context& io_context,
    const tcp::resolver::results_type& endpoints)
    :socket_(io_context),
    input_(io_context, ::dup(STDIN_FILENO)),
    output_(io_context, ::dup(STDOUT_FILENO)),
    input_buffer_(chat_message::max_body_length)
    {
        do_connect(endpoints);
    }
private:
    void do_connect(const tcp::resolver::results_type& endpoints) {
        boost::asio::async_connect(socket_, endpoints,
            [this](boost::system::error_code ec, tcp::endpoint) {
                if(!ec) {
                    do_read_header();
                    do_read_input();
                }
            });
    }

    void do_read_header() {
        boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](boost::system::error_code ec, std::size_t /*lenght*/) {
            if(!ec && read_msg_.decode_header()) {
                do_read_body();
            } else {
                close();
            }
        });
    }

    void do_read_body() {
        boost::asio::async_read(socket_, 
        boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
            if(!ec) {
                do_write_output();
            } else {
                close();
            }
        });

    }

    void do_write_output() {
        //write out the message we just received, terminated by newline
        static char eol[] = {'\n'};
        std::array<boost::asio::const_buffer, 2> buffers = {
            {boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
            boost::asio::buffer(eol)    }};
        boost::asio::async_write(output_, buffers, 
        [this](boost::system::error_code ec, std::size_t /*length*/) {
            if(!ec) {
                do_read_header();
            } else {
                close();
            }
        });
    }

    void do_read_input() {
        //Read a line of input entered by user.
        boost::asio::async_read_until(input_, input_buffer_, '\n', 
        [this](boost::system::error_code ec, std::size_t length) {
            if(! ec) {
                //write the message (minus the newline) to the server.
                write_msg_.body_length(length - 1);
                input_buffer_.sgetn(write_msg_.body(), length - 1);
                input_buffer_.consume(1);   //remove newline from input
                write_msg_.encode_header();
                do_write_message();
            } else if(ec == boost::asio::error::not_found) {
                //Don't get a newline. send whatever we have
                write_msg_.body_length(input_buffer_.size());
                input_buffer_.sgetn(write_msg_.body(), input_buffer_.size());
                write_msg_.encode_header();
                do_write_message();
            } else {
                close();
            }
        });
    }

    void do_write_message() {
        boost::asio::async_write(socket_,
        boost::asio::buffer(write_msg_.data(), write_msg_.length()),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
            if(!ec) {
                this->do_read_input();
            } else {
                this->close();
            }
        });
    }

    void close() {
        //Cancel all outstanding asynchronous operations.
        socket_.close();
        input_.close();
        output_.close();
    }
};
using std::endl;
using std::cout;
using std::cerr;
using std::cin;


/* @brief @pram @return */
int main(int argc, char* argv[])
{
    try{
        if(argc != 3) {
            std::cerr << "Usage: posix_chat_client <host> <port>\n";
            return -1;
        }
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(argv[1], argv[2]);
        posix_chat_client client(io_context, endpoints);

        io_context.run();
    }catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}


#else //defined(BOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR)
int main(){}
#endif


#include<boost/asio.hpp>
#include<iostream>
#include<memory>
#include<utility>
#include<vector>
#include<ctime>

using boost::asio::ip::tcp;
using std::shared_ptr;
using std::vector;
using std::string;
//A reference-counted non-modifiable buffer class
class shared_const_buffer
{
public:
    //Construct from a std::string
    explicit shared_const_buffer(const string& data)
    :data_(new vector<char>(data.begin(),data.end())),
    buffer_(boost::asio::buffer(*data_))
    {
        
    }

    //Implement the ConstBufferSequence requirements
    typedef boost::asio::const_buffer value_type;
    typedef const boost::asio::const_buffer* const_iterator;
    const boost::asio::const_buffer* begin() const {
        return &buffer_;
    }
    const boost::asio::const_buffer* end() const {
        return &buffer_+1;   //地址运算
    }
private:
    std::shared_ptr<std::vector<char>> data_ ;
    boost::asio::const_buffer buffer_ ;
};  //class 和 struct 关键字一样。是为了保持和c语言的一致，所以要加上分号

class session                   //session会话
:public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket)
    :socket_(std::move(socket))
    {
        fprintf(stdout,"session()\n");     //fprintf()相比于std::cout对象而言，它有一把锁
    }
    void start() {
        do_wirte();
    }
private:
    void do_wirte() {
        std::time_t now = std::time(nullptr);
        shared_const_buffer buffer(std::ctime(&now));

        auto self(shared_from_this());      //self是session类型。shared_from_this()返回当前对象
        boost::asio::async_write(socket_,buffer,
            [self](boost::system::error_code /*ec*/, std::size_t /*length*/)
            {});
    }
private:
    //The socke used to communicate with the client
    tcp::socket socket_;        //类对象
};
class server
{
public:
    server(boost::asio::io_context& io_context, short port)
    :acceptor_(io_context, tcp::endpoint(tcp::v4(),port))
    {}
private:
    void do_accept(){
        acceptor_.async_accept(     //异步操作
            [this](boost::system::error_code ec, tcp::socket socket) {
                if(!ec) {
                    std::make_shared<session>(std::move(socket))->start();
                }
            }
        );
    }
private:
tcp::acceptor acceptor_;
};

int main(int argc, char* argv[]) {
    try{
        if(argc != 2) {
            std::cerr << "Usage: reference_counted <port>\n";
            return 1;
        }
        boost::asio::io_context io_context;

        server s(io_context, std::atoi(argv[1]));

        io_context.run();
    }
    catch(std::exception& e){

    }
}
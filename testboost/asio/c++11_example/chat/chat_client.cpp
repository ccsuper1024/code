//chat client
//c std libraray
#include<cstdlib>
//c++ std library
#include<deque>
#include<iostream>
#include<thread>
#include<boost/asio.hpp>
//c custom library
//c++ custom library
#include"chat_message.hpp"

using boost::asio::ip::tcp;

//双端队列
typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
    chat_client(boost::asio::io_context& io_context,
        const tcp::resolver::results_type& endpoints)
        :io_context_(io_context)
        ,socket_(io_context)
        {
            do_connect(endpoints);
            fprintf(stdout, "chat_client(boost::asio::io_context& io_context,"
        "const tcp::resolver::results_type& endpoints)\n");
        }
    ~chat_client() {
        fprintf(stdout, "~chat_client()\n");
    }

    //万能引用
    void write(const chat_message& msg) {
        boost::asio::post(io_context_, 
        [this, msg]() { //lambda表达式返回一个函数对象, 对应的形式参数可以用std::function + std::bidn来解决
            bool write_in_progress = !write_msgs_.empty();
            //保存要写入的信息到chat_message的队列中
            write_msgs_.push_back(msg);
            if(!write_in_progress) {
                do_write();
            }
        });

    }

    void close() {
        boost::asio::post(io_context_, [this](){    socket_.close();    });
    }

private:
    void do_connect(const tcp::resolver::results_type& endpoints) {
        boost::asio::async_connect(socket_, endpoints,
        [this](boost::system::error_code ec, tcp::endpoint) {
            if(!ec) {
                do_read_header();
            }
        });
    }

    void do_read_header() {
        boost::asio::async_read(socket_, 
        boost::asio::buffer(read_msgs_.data(), chat_message::header_length),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
            if(!ec && read_msgs_.decode_header()) {
                do_read_body();
            } else {
                socket_.close();
            }
        });
    }
    void do_read_body(){
        boost::asio::async_read(socket_, 
        boost::asio::buffer(read_msgs_.data(), chat_message::header_length),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
            if(!ec) {
                //递归读，asio是由Epoll的Reactor模式+非阻塞IO模拟实现的Proactor模式，非阻塞IO一般要搭配循环来完整的读写数据
                std::cout.write(read_msgs_.body(), read_msgs_.body_length());
                std::cout << "\n";
                do_read_header();
            } else {
                socket_.close();
            }
        });
    }
    void do_write() {
        //异步写
        boost::asio::async_write(socket_,   //写入的文件描述符
        boost::asio::buffer(write_msgs_.front().data(), //缓存，要写入的数据 
        write_msgs_.front().length()),      //写入数据的长度
        [this](boost::system::error_code ec, std::size_t /*length*/) {  //lambda表达式
            if(!ec) {   //当错误码不为false时
                write_msgs_.pop_front();    //从要写入的数据队列中，从队头弹出一个元素chat_message类型的
                if(!write_msgs_.empty()) {  //判定要写入的数据队列不为空时
                    do_write();             //继续写入  ，递归调用？
                } else {                    //否则
                    socket_.close();        //关闭文件描述符
                }
            }
        }
        );
    }
private:
    //io上下文
    boost::asio::io_context& io_context_;   //类引用成员
    tcp::socket socket_;    //类成员对象
    chat_message read_msgs_;
    chat_message_queue write_msgs_;
};

int main(int argc, char* argv[]) {
    try {
        if(argc != 3) {
            std::cerr << "Usage: chat_client <host> <post>\n";
            return -1;
        }
        boost::asio::io_context io_context;
        
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        chat_client client(io_context, endpoints);

        //基于对象的线程
        std::thread t([&io_context](){ io_context.run(); });

        char line[chat_message::max_body_length + 1];
        while(std::cin.getline(line, chat_message::max_body_length + 1)) {
            chat_message msg;
            msg.body_length(std::strlen(line));
            std::memcpy(msg.body(), line, msg.body_length());
            msg.encode_header();
            client.write(msg);
        }
        client.close();
        t.join();       //主线程等待子线程终结
    }catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
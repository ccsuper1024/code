// chat server
// c std library
// c++ std library
#include <cstdlib>
#include <iostream>
#include <deque>
#include <list>
#include <set>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
// c custom library
// c++ custom library
#include "chat_message.hpp"

// namespace delaration
using boost::asio::ip::tcp; // using声明，不要使用using指示

//--------------------------------
typedef std::deque<chat_message> chat_message_queue; // deque 双端队列
//--------------------------------

// 抽象类
class chat_participant
{
public:
    virtual ~chat_participant() {}
    virtual void deliver(const chat_message &msg) = 0; // 纯虚函数
};

//------------------------------------------
// 基类指针
typedef std::shared_ptr<chat_participant> chat_participant_ptr;
//------------------------------------------
class chat_room
{
public:
    // 将参加者加入聊天室
    void join(chat_participant_ptr participant)
    {
        participants_.insert(participant);
        for (auto msg : recent_msgs_)
        {
            participant->deliver(msg);
        }
    }
    // 将人踢出聊天室
    void leave(chat_participant_ptr chat_participant)
    {
        participants_.erase(chat_participant);
    }

    // 递送消息
    void deliver(const chat_message &msg)
    {
        recent_msgs_.push_back(msg); // 将消息加入保存消息的双端队列中
        while (recent_msgs_.size() > max_recent_msgs)
        {
            recent_msgs_.pop_front(); // 从消息队列的队头弹出消息元素
        }

        // 向每一位聊天室的成员递送消息
        for (auto participant : participants_)
        {
            participant->deliver(msg);
        }
    }

private:
    //std::set使用红黑树实现，本身是有序的
    std::set<chat_participant_ptr> participants_;
    enum
    {
        max_recent_msgs = 100
    }; // 非限定作用域的枚举类型,使用时不需要再其前面加上枚举名和作用域运算符
    chat_message_queue recent_msgs_;
};

//聊天会话
//作为chat_participant的派生类
class chat_session
:public chat_participant,
public std::enable_shared_from_this<chat_session>
{
public:
    chat_session(tcp::socket socket, chat_room& room)
    :socket_(std::move(socket)),
    room_(room)
    {
        fprintf(stdout, "chat_session(tcp::socket socket, chat_room& room)\n");
    }

    ~chat_session() {
        fprintf(stdout, "~chat_session() \n");
    }

    void start() {
        room_.join(shared_from_this()); //将当前对象加入聊天室
        do_read_header();
    }

    //覆盖基类的纯虚函数，递送消息
    void deliver(const chat_message& msg) override {
        //判读要缓存写入数据的队列中是否为空
        bool write_in_progress = !write_msgs_.empty();
        //向缓存写入数据的队列添加新的数据
        write_msgs_.push_back(msg);
        if (!write_in_progress) {
            //异步写
            do_write();
        }
    }
private:
    void do_read_header() {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.data(), chat_message::header_length),
        [this,self ](boost::system::error_code ec, std::size_t /*length*/) {
            if(!ec && this->read_msg_.decode_header()) {  //ec为加，且解码消息头为真
                this->do_read_body();
            } else {
                this->room_.leave(this->shared_from_this());
            }
        });
    }

    //读取消息体
    void do_read_body() {
        //获取当前对象的shared_ptr
        auto self(shared_from_this());
        //异步读
        boost::asio::async_read(socket_, 
        boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                //向聊天室中的每个人都发送消息
                room_.deliver(read_msg_);
                //读取消息头
                do_read_header();
            } else {
                room_.leave(shared_from_this());
            }
        });
    }

    void do_write() {
        auto self(shared_from_this());  //shared_from_this()返回一个当前对象的shared_ptr<>指针
        //异步写
        boost::asio::async_write(socket_,
        boost::asio::buffer(write_msgs_.front().data(),
        write_msgs_.front().length()),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                write_msgs_.pop_front();
                if (!write_msgs_.empty()) {
                    do_write();
                }
            } else {
                //从聊天室中踢人
                room_.leave(shared_from_this());
            }
        });
    }
private:
    tcp::socket socket_;
    chat_room& room_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};

class chat_server
{
public:
    chat_server(boost::asio::io_context& io_context, 
        const tcp::endpoint& endpoint)
        :acceptor_(io_context, endpoint)
        {
            do_accept();
        }
private:
    void do_accept() {
        //异步等待连接
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if(!ec) {
                    std::make_shared<chat_session>(std::move(socket), room_)->start();
                }
                //递归调用
                do_accept();
            }
        );
    }

private:
    tcp::acceptor acceptor_;
    chat_room room_;
};

int main(int argc, char* argv[]) {
    try{
        if(argc != 2) {
            std::cerr << "Usage: chat_server<port> [<port>...] \n";
            return -1;
        }
        //是boost的执行调度器，提供了一个事件循环，其中维护了一个事件队列用于存放各种
        //IO事件。
        //io_context是线程安全的，但多线程调用时还是要加锁
        /*
        原理：1.事件循环，io_context使用Reactor + 非阻塞IO来模拟proactor(前摄器)模式
        io_context启动后进入事件循环，不断检查事件队列是否有就绪的事件
        2.事件处理：当有事件就绪时，io_context调用相应的处理函数来处理事件。对于异步操作
        完成事件，会调用相应的回调函数来处理结果
        3.调度异步操作：当提交异步操作到io_context后，io_context会将操作加入队列中，并
        在事件循环中调度执行。
        4.完成操作：异步操作完成后，io_context负责调用相应的回调函数来通知操作结果
        */
        //比如boost::asio::read调用同步IO
        //boost::asio::async_read调用异步IO
        boost::asio::io_context io_context;
        
        //双向链表
        std::list<chat_server> servers;
        for(int i = 1; i < argc; ++i) {
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            //构造服务器程序, 并加入双向链表中
            servers.emplace_back(io_context, endpoint);
        }

        io_context.run();
    } catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() <<std::endl;
    }

    return 0;
}
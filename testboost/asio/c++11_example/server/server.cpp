//演示如何创建可用于套接字读写操作的引用计数缓冲区
#include<array>     //固定数组
#include<cstdlib>
#include<iostream>
#include<memory>
#include<type_traits>
#include<utility>
#include<boost/asio.hpp>

using boost::asio::ip::tcp;

/*
class to manage the memory to be used for handler-based custom allocation.
It contains a single block of memory which may be returned for allocation
requests. if the memory is in use when an allocation request is made , the
allocator delegates（委派） allocation to the global heap
*/
class handler_memory
{
public:
    //构造函数，编译器不会再自动合成其他构造函数
    handler_memory()
    :in_use_(false)
    {
        fprintf(stdout, "handler_memory() \n");
    }

    //禁止复制语义
    handler_memory(const handler_memory& rhs)=delete;
    handler_memory& operator=(const handler_memory& rhs)=delete;

    void* allocate(std::size_t size) {
        if(!in_use_ && size < sizeof(storage_)) {
            in_use_ == true;
            return &storage_;
        }else {
            return ::operator new(size);    //调用全局的new操作运算符函数,就是new的函数形式而不是熟知的表达式形式
        }
    }

    void deallocate(void* pointer) {
        if(pointer == &storage_) {
            in_use_ =false;
        } else {
            ::operator delete(pointer);
        }
    }
private:
    //Storage space used for handler_based custom memory allocation
    typename std::aligned_storage<1024>::type storage_;

    //Whether the handler-based custom allocation storage has been used
    bool in_use_;
};

//The allocator to be associated with the handler objects. This allocator
//only needs to satisfy the C++11 minimal allocator requirement
template<typename T>
class handler_allocator
{
public:
    using value_type = T;
    explicit handler_allocator(handler_memory& mem) //禁止隐式转换
    :memory_(mem)
    {
        fprintf(stdout, " handler_allocator(handler_memory& mem)\n");
    }
    ~handler_allocator()
    {
        fprintf(stdout, " ~handler_allocator()\n");
    }

    //模板类中的模板函数
    //复制构造函数
    template<typename U>
    handler_allocator(const handler_allocator<U>& other) noexcept
    :memory_(other.memory_)
    {
        fprintf(stdout, " handler_allocator(const handler_allocator<U>& other)\n");    
    }

    bool operator==(const handler_allocator& other) const noexcept
    {
        return &memory_ == &other.memory_;
    }
    //万能引用
    //const 成员函数
    bool operator!=(const handler_allocator& other) const noexcept
    {
        return &memory_ != &other.memory_;
    }

    T* allocate(std::size_t n) const {
        return static_cast<T*>(memory_.allocate(sizeof(T) * n));
    }

    void deallocate(T* p, std::size_t /*n*/) const {
        return memory_.deallocate(p);
    }
private:
    template<typename > friend class handler_allocator;  //友元声明

    //The underlying memory
    handler_memory& memory_;
};

class session
:public std::enable_shared_from_this<session>  //公开继承 
{
public:
    session(tcp::socket socket)
    :socket_(std::move(socket))
    {
        fprintf(stdout, " session(tcp::socket socket)\n");
    }

    void start() {
        do_read();
    }
private:
    void do_read() {
        auto self(shared_from_this());//shared_from_this()函数返回当前类对象的shared_ptr指针
        socket_.async_read_some(boost::asio::buffer(data_),
            boost::asio::bind_allocator(
                handler_allocator<int>(handler_memory_),
                [this, self](boost::system::error_code ec, std::size_t length){
                    if(!ec) {
                        do_write(length);
                    }
                }
            ));
    }
    void do_write(std::size_t length) {
        auto self(shared_from_this());  //返回指向当前对象的std::shared_ptr。以确保在对象被销毁前有一个有效的共享所有权的指针存在
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
            boost::asio::bind_allocator(
                handler_allocator<int>(handler_memory_),
                [this, self](boost::system::error_code ec, std::size_t /*n*/) {
                    if(!ec) {
                        do_read();
                    }
                }
            ));
    }
private:
    //The socket used to communicate with the client
    tcp::socket socket_;

    //buffer used to store data received from the client
    std::array<char, 1024> data_;

    //The memory to use for handler-based custom memory allocation
    handler_memory handler_memory_;
};

class server
{
public:
    server(boost::asio::io_context& io_context, short port)
    :acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
    {
        do_accept();
        fprintf(stdout,"server(boost::asio::io_context& io_context, short port)\n");
    }
    ~server(){
        fprintf(stdout, "~server()\n");
    }

private:
    void do_accept(){
        acceptor_.async_accept([this](boost::system::error_code ec,
         tcp::socket socket) {
            if(!ec) {
                std::make_shared<session>(std::move(socket))->start();
            }
            do_accept();
         }); 
    }    
private:
    tcp::acceptor acceptor_;    
};

int main(int argc, char* argv[]) {
    try {
        if(argc!=2) {
            std::cerr << "Usage:server <port>\n";
            return -1;
        }
        boost::asio::io_context io_context;
        server s(io_context, std::atoi(argv[1]));   //ascii to int
        io_context.run(); 
    }catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}
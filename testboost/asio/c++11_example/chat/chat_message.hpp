//此示例实现了聊天服务器和客户端。
//该程序使用具有固定长度消息头和可变长度消息正文的自定义协议
#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP
#include<cstdio>
#include<cstdlib>
#include<cstring>

class chat_message
{
public:
    //编译器常量, 类内定义的，在实现文件中使用要加上，类名 + 作用域控制符
    static constexpr std::size_t header_length = 4; 
    static constexpr std::size_t max_body_length = 512;

    chat_message():body_length_(0)
    {
        fprintf(stdout,"chat_message()\n");
    }
    ~chat_message(){
        fprintf(stdout,"~chat_message()\n");
    }
    const char* data() const {
        return data_;   //数组名退化成指针。只有三种种情况不会退化。1.是sizeof(数组名) 2. str[] = "abc" 3.(&数组名) + 1 
    }
    char* data(){
        return data_;
    }
    std::size_t length() const{
        return header_length + body_length_;
    }
    const char* body()const {
        return &data_[header_length];   //中括号运算符优先于取地址符
    }

    char* body(){
        return data_ + header_length;   //c语言的指针运算
    }

    std::size_t body_length() const{
        return body_length_;
    }
   void body_length(std::size_t new_length) {
    body_length_ = new_length;
    if(body_length_ > max_body_length) {
        body_length_ = max_body_length;
    } 
   } 
   bool decode_header() {   //解码,实际上只是提取头部，验证了一下头部的信息是否合规
    char header[header_length + 1] = "";    //c风格字符串自动在末尾添加空格符
    std::strncat(header, data_, header_length); //将消息中的头部提取出来
    body_length_ = std::atoi(header);   //ascii to int
    if(body_length_ > max_body_length) {
        body_length_ = 0;
        return false;
    }
    //没有提取消息体部分
    return true;
   }

   void encode_header() {      //加码
    char header[header_length + 1] = "";
    //拼凑字符串, 就写了个消息头
    std::sprintf(header, "%4d", static_cast<int>(body_length_));
    //将消息头作为消息体传输
    std::memcpy(data_, header, header_length);
   }
private:
    char data_[header_length + max_body_length];
    std::size_t body_length_;
};

#endif
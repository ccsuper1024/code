/* @file  @brief */
#include "unixhead.h"
#include <iostream>
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::cerr;

class HttpServer
{
public:
    HttpServer(string ip,unsigned short port)
    :ip_(ip)
     ,port_(port)
    {

    }
    
    void start(){
        listenfd_ = socket(AF_INET,SOCK_STREAM,0);
        if(listenfd_ < 0){
            perror("socket");
            exit(EXIT_FAILURE);
        }
        //地址复用
        int on = 1;
        int ret = setsockopt(listenfd_,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
        if(ret < 0){
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        //端口复用
        ret = setsockopt(listenfd_,SOL_SOCKET,SO_REUSEPORT,&on,sizeof(on));
        if(ret < 0){
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        struct sockaddr_in servaddr;
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port_);
        servaddr.sin_addr.s_addr = inet_addr(ip_.c_str());
        
        ret = bind(listenfd_,(struct sockaddr*)&servaddr,sizeof(servaddr));
        if(ret < 0){
            perror("bind");
            exit(EXIT_FAILURE);     //进程退出时会做一些清理
        }
        
        ret = listen(listenfd_,10); //第二个参数是半连接队列和全连接队列之和
        if(ret < 0){
            perror("listen");
            exit(EXIT_FAILURE);     //进程退出时会做一些清理
        }
    }

    void recvAndShow(){
        while(true){
            printf("wait client");
            int clntfd = accept(listenfd_,nullptr,nullptr);
            char buffer[1024] = {0};
            int ret = recv(clntfd,buffer,1024,0);

            printf("recv %d bytes,buff: \n%s\n",ret,buffer);
            string resp;
            response(resp);
            ret = send(clntfd,resp.c_str(),resp.size(),0);

            close(clntfd);
        }
    }

    void response( string& value){
        string firstline = "HTTP/1.1 200 OK\r\n";       //再次证明http是个文本协议
        string headers = "Content-Type: text/html\r\n"; 
        headers += "Server:echo httpserver";
        string emptyline = "\r\n";
        string body = "<html><body>hello,http</body></html>";
        value += firstline + headers + emptyline + body + emptyline;
    }
    ~HttpServer(){
        close(listenfd_);   //关闭监听套件字
    }
private:
    const string ip_;
    unsigned short port_;       //端口号是16位的
    int listenfd_;
};
/* @brief @param @param   @return */
void test(){
    HttpServer httpserver("0.0.0.0",1280);
    httpserver.start();
    httpserver.recvAndShow();
}
int main()
{
    test();
    return 0;
}


/**
 * * @file  HiredisHelper.hpp 
 * * @brief test Hiredis and simplewrapper the library
 * */
#ifndef HEREDISHELPER_HPP
#define HEREDISHELPER_HPP
#include <vector>
#include <string>
#include <hiredis/hiredis.h>
using std::vector;
using std::string;


class HiredisHelper
{
public:
       HiredisHelper(const string &ip, int port,const string & auth_str,int timeout = 200 )//毫秒ms
       :m_timeout(timeout)
        ,m_port(port)
        ,m_ip(ip)
        ,m_authstring(auth_str)
        ,m_ctx(nullptr)
        {}

       //虚函数
       virtual ~HiredisHelper(){
           if(m_ctx){
               redisFree(m_ctx);
               m_ctx = nullptr;
           }
       }

       //调用者需要自行调用void freeReplyObject(reply);
       redisReply* ExecuteCmd(const char* format,...);
       redisReply* ExecuteCmd(const string & cmd);

private:
        int Connect();
         
private:
       int m_timeout;
       int m_port;
       string m_ip;
       string m_authstring;
       redisContext* m_ctx;
};

#endif  //HEREDISHELPER

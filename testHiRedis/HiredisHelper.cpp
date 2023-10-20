/**
 * @file HiredisHelper.cpp
 * @brief implemention of HiredisHelper.hpp
 */
#include <hiredis/hiredis.h>
#include <iostream>
#include "HiredisHelper.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::cerr;

/**
 * @brief member function implementioan 
 * @param 
 * @param 
 * @return 
 */

redisReply*HiredisHelper::ExecuteCmd(const char* format,...)
{
    int ret = 0; 
    if(m_ctx == nullptr){
        ret = Connect();
        if(ret < 0){
            return nullptr;
        }
    }

    va_list valist;
    va_start(valist,format);
    redisReply* reply = (redisReply*)redisCommand(m_ctx,format,valist);

    if(reply == nullptr){
        if(m_ctx){
            cout << "cmd: " << format << " err: " << m_ctx->err << " errstr: " << m_ctx->errstr << endl;
            redisFree(m_ctx);
            m_ctx = nullptr;
        }
        return nullptr;
    }

    va_end(valist);

    if(m_ctx->err != 0){
        cout << "err: " << m_ctx->err << " errstr: " << m_ctx->errstr << endl;
        redisFree(m_ctx);
        m_ctx = nullptr;

        if(reply){
            freeReplyObject(reply);
        }
        return nullptr;
    }

    if(REDIS_REPLY_ERROR == reply->type){
        cout << "errstr: " <<reply->str << endl;

        if(m_ctx){
            redisFree(m_ctx);
            m_ctx = nullptr;
        }
        freeReplyObject(reply);
        return nullptr;
    }
    return reply;
}
redisReply*HiredisHelper::ExecuteCmd(const string & cmd)
{
    int ret = 0;
    if(m_ctx == nullptr){
        ret = Connect();
        if(ret < 0){
            return nullptr;
        }
    }

    redisReply* reply = (redisReply*) redisCommand(m_ctx,cmd.c_str());
    if(reply == nullptr){
        if(m_ctx){
            cout << "err: " << m_ctx->err << " errstr " << m_ctx->errstr << endl;
            if(m_ctx->err == 3){    //errstr成员的第三个元素
                redisFree(m_ctx);
                m_ctx = nullptr;
                //Server closed the connection ,reconnect
                ret = Connect();
                if(ret == 0){
                    cout << "reconnect and  ex cmd " << endl;
                    reply = (redisReply*) redisCommand(m_ctx,cmd.c_str());
                    if(reply == nullptr){
                        if(m_ctx){
                            cout << "err: " << m_ctx->err << " errstr:" << m_ctx->errstr
                                << endl;
                            redisFree(m_ctx); //释放与Redis数据库的链接
                            m_ctx = nullptr;
                        }

                    }
                }
            }else{
                redisFree(m_ctx);   //释放链接
                m_ctx = nullptr;
            }
        }
        if(reply == nullptr || m_ctx == nullptr){
            return nullptr;
        }
    }
    if(m_ctx->err != 0){    //输出链接的错误类型
        cout << "err: " << m_ctx->err << " errstr: " << m_ctx->errstr << endl;
        redisFree(m_ctx);
        m_ctx = nullptr;

        if(reply){
            freeReplyObject(reply);
        }
        return nullptr;
    }
    //输出返回值的错误类型
    if(REDIS_REPLY_ERROR == reply->type){
        cout << "cmd: " << cmd << " errstr: " << reply->str << endl;
        if(m_ctx){
            redisFree(m_ctx);
            m_ctx = nullptr;
        }
        freeReplyObject(reply);
        return nullptr;
    }
    return reply;
}
int HiredisHelper::Connect()
{
    struct timeval tv;
    tv.tv_sec = m_timeout / 1000;            //秒
    tv.tv_usec = (m_timeout % 1000) * 1000;  //毫秒

    if(m_ctx != nullptr){
        redisFree(m_ctx);
        m_ctx = nullptr;
    }

    m_ctx = redisConnectWithTimeout(m_ip.c_str(),m_port,tv);
    if(m_ctx == nullptr || m_ctx->err != 0){//出现返回值为空，或者返回的连接中的err成员不会空
        if(m_ctx != nullptr){
            redisFree(m_ctx);
            m_ctx = nullptr;
        }
        cout << "redisConnectWithTimeout fail " << endl;
        return REDIS_ERR;
    }

    if(REDIS_ERR == redisSetTimeout(m_ctx,tv)){
        redisFree(m_ctx);
        m_ctx = nullptr;
        return REDIS_ERR;
    }

    string auth = "AUTH" + m_authstring;
    redisReply* reply = (redisReply*)redisCommand(m_ctx,auth.c_str());
    if(reply == nullptr){
        if(m_ctx){
            redisFree(m_ctx);
            m_ctx = nullptr;
        }
        return REDIS_ERR;
    }

    if(m_ctx->err != 0){
        cout << "err: " << m_ctx->err << " errstr " << m_ctx->errstr << endl;
        redisFree(m_ctx);
        m_ctx = nullptr;
        if(reply){
            freeReplyObject(reply);
        }
        return REDIS_ERR;
    }
    if(REDIS_REPLY_ERROR == reply->type){
        cout << "cmd: " << auth << " errstr " << reply->str << endl;
        if(m_ctx){
            redisFree(m_ctx);
            m_ctx = nullptr;
        }
        freeReplyObject(reply);
        return REDIS_ERR;
    }

    freeReplyObject(reply);
    return 0;
}       

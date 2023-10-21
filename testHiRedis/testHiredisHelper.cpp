/**
 * @file testHiredisHelper.cpp
 * @brief test HiredisHelper class
 */
#include "HiredisHelper.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::cerr;
using std::vector;
using std::stringstream;    //字符串流对象
using std::istringstream;
using std::string;
/**
 * @brief 
 * @param 
 * @return 
 */
#define CHECK_FREE_REDIS_REPLY(reply) \
    if(reply){\
        freeReplyObject(reply);\
    }else {\
        cout << "freeReplyObject" << endl;\
    }

int addStrContent(HiredisHelper & redis_conn){
    string m_redis_key = "cloudox";
    stringstream ss_cmd;
    ss_cmd << "set " << m_redis_key << " " << "boy";

    //获取字符串流上的字符数据
    string cmd = ss_cmd.str();  
    redisReply* reply = redis_conn.ExecuteCmd(cmd);
    if(reply){
        cout << "push redis success " << cmd << endl;
        return 0;
    }else{
        cout << "push redis fail " << cmd << endl;
        return -1;
    }
    CHECK_FREE_REDIS_REPLY(reply);
    return 0;
}


int getStrContent(HiredisHelper& redis_conn,string& item){
    string m_redis_key = "cloundox";
    stringstream ss_cmd;
    ss_cmd << "get " << m_redis_key;

    string cmd = ss_cmd.str();
    redisReply* reply = redis_conn.ExecuteCmd(cmd);
    if(reply && reply->type == REDIS_REPLY_STRING){
        item = reply->str;
        return 0;
    }else{
        cout << "get redis fail " << cmd << endl;
        return -1;
    }
    CHECK_FREE_REDIS_REPLY(reply);
    return 0;

}

int addZsetContent(HiredisHelper& redis_conn){
    string m_redis_key = "cloudox";
    stringstream ss_cmd;
    ss_cmd << "ZADD " << m_redis_key << " 1 " << " boy " << endl;

    string cmd = ss_cmd.str();
    redisReply* reply = redis_conn.ExecuteCmd(cmd);
    if(reply){
        freeReplyObject(reply);
        cout << "push redis success " << cmd << endl;
        return 0;
    }else{
        cout << "push redis fail " << cmd << endl;
        return -1;
    }
    CHECK_FREE_REDIS_REPLY(reply);
    return 0;
}

int getZsetConten(HiredisHelper& redis_conn,vector<string>& items){
    string m_redis_key = "cloudox";
    stringstream ss_cmd;
    //-inf是负无穷，+inf是正无穷
    ss_cmd << "ZRANGEBYSCORE " << m_redis_key << " -inf "<< " 5 " << " limit 0 10";

    string cmd = ss_cmd.str();
    redisReply* reply = redis_conn.ExecuteCmd(cmd);
    if(reply && reply->type == REDIS_REPLY_ARRAY){
        cout << "reply size: " << reply->elements << endl;
        for(size_t i = 0; i < reply->elements; ++i){
            if(nullptr != reply->element[i]){
                redisReply* ele = reply->element[i];
                if(ele->type == REDIS_REPLY_INTEGER){
                    items.push_back(std::to_string(ele->integer));  //c++11
                }else if(ele->type == REDIS_REPLY_STRING){
                    //局部对象
                    string s(ele->str,ele->len);
                    items.push_back(s);
                }
            }
        }
        return 0;
    }else{
        cout << "get redis fail " << cmd << endl;
        return -1;
    }
    CHECK_FREE_REDIS_REPLY(reply);
    return 0;
}


int main()
{
    string redis_ip = "127.0.0.1";
    int redis_port = 6379;
    string redis_auth = "crs-fsdfjlskdjflsajdlfas";
    //有参构造函数构造对象
    HiredisHelper redis_conn(redis_ip,redis_port,redis_auth);

    //添加字符串内容
    int ret = addStrContent(redis_conn);

    //获取字符串内容
    string item;
    ret = getStrContent(redis_conn,item);
    cout << item << endl;

    //添加有序集合内容
    ret = addZsetContent(redis_conn);

    //获取有序集合内容
    vector<string> items;
    ret = getZsetConten(redis_conn,items);
    if(ret == 0){
        for(int i = 0; i < items.size(); ++i){
            cout << items[i] << endl;
        }
    }

    return 0;
}


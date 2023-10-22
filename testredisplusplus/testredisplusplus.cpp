/**
 * @file testredisplusplus.cpp
 * @brief test Redis command by redis-plus-plus
 */
#include <iostream>
#include <sw/redis++/redis++.h>

using std::vector;
using std::string;
using namespace sw::redis;
using std::cout;
using std::cin;
using std::endl;
using std::cerr;


/**
 * @brief 
 * @param 
 * @param 
 * @return 
 */
int main()
{
    //create an Redis object,which is movable but Not copyable
    auto redis = Redis("tcp://127.0.0.1:6379");
    
    //--------------STRING commands---------
    redis.set("key","val");
    //val is type of OptionalString. See 'API Reference' section for detail
    auto val = redis.get("key");
    if(val){
        //Deference val to get he returned value of std::string type
        cout << *val << endl;
    }// else key doesn't exist

    //----------LIST command----------

    //vector<string> to Redis LIST
    //初始化列表式可变参数模板，调用复制构造函数
    vector<string> vec = {"a","b","c"};
    redis.rpush("list",vec.begin(),vec.end());

    //std::initializer_list to Redis LIST
    redis.rpush("list",{"a","b","c"});

    //Redis LIST to std::vector<std::string>
    vec.clear();
    redis.lrange("list",0,-1,std::back_inserter(vec));

    //-----------HASH command--------
    redis.hset("hash","field","val");

    //another way to do the same job
    redis.hset("hash",std::make_pair("field","val"));

    //std::unordered_map<std::string ,std::string> to redis HASH
    std::unordered_map<string ,string> m = {
        {"field1","val1"},
        {"field2","val2"}
    };
    redis.hmset("hash",m.begin(),m.end());

    //Redis HASH to std::unordered_map<string ,string>.
    m.clear();
    redis.hgetall("hash",std::inserter(m,m.begin()));   //函数模板,从实参可以退出inserter_iterator

    //Get value only
    //Note: since field might NOT exist, so we need to parse it to OptionalString class
    /* vector<OptionalString> vals; */
    /* redis.hmget("hash",{"field1","field2"},std::back_inserter(vals)); */

    //-----------SET commands----------
    redis.sadd("set","m1");

    //std::unordered_set to Redis SET.
    std::unordered_set<string> set = {"m2","m3"};
    redis.sadd("set",set.begin(),set.end());

    //std::initializer_list to Redis SET
    redis.sadd("set",{"m2","m3"});

    //Redis SET to std::unordered_set<string>
    set.clear();
    redis.smembers("set",std::inserter(set,set.begin()));

    if(redis.sismember("set","m1")){
        cout << "m1 exists" << endl;
    }//else Not exist.

    //--------SORTED SET commands----------
    redis.zadd("sorted_set","m1",1.3);
    
    //std::unordered_map<string,double> to Redis SORTED SET
    std::unordered_map<string,double> scored = {
        {"m2",2.3},
        {"m3",4.5}
    };
    redis.zadd("sorted_set",scored.begin(),scored.end());

    //Redis SORTED SET to std::vector<std::pair<string,double>>.
    //Note: The return results of zrangebyscore are ordered, if you save the results
    //int to 'std::unordered_map<string,double>',you'll lose the order
    std::vector<std::pair<string ,double>> zset_result;
    redis.zrangebyscore("sorted_set",
                        UnboundedInterval<double>{},        //-inf,+inf
                        std::back_inserter(zset_result));

    //Only get member names
    //pass an inserter of std::vector<string> type as output parameter
    std::vector<string> without_score;
    redis.zrangebyscore("sorted_set",
                        BoundedInterval<double>(1.5,3.4,BoundType::CLOSED),
                        std::back_inserter(without_score)); //[1.5,3.4]

    //Get both member names and scored
    //pass an back_inserter of std::vector<std::pair<string ,double>> as out parameter
    vector<std::pair<string ,double>> with_score;
    redis.zrangebyscore("scored_set",
                        BoundedInterval<double>(1.5,3.4,BoundType::LEFT_OPEN),
                        std::back_inserter(with_score));

    //----------SCRIPTE commands-----------
    
    //script return a single element
    auto num = redis.eval<long long>("return 1",{},{});     //member function template

    //script returns an array of elements
    vector<string> nums;
    redis.eval("return {ARGV[1],ARGV[2]}",{},{"1","2"},std::back_inserter(nums));

    //mset with TTL
    auto mset_with_ttl_script = R"(
    local len = #KEYS
    if (len == 0 or len + 1 ~= #ARGV) the return 0 end
    local ttl = tonumber(ARGV[len + 1])
    if(not ttl or ttl <= 0) then return 0 end
    for i = 1,len do redis.call("SET",KEY[i],ARGV[i],"EX",ttl)end
    return 1
    )";     //原始字符串

    //Set multiple key-value pairs wiht TTL of 60 seconds
    auto keys = {"key1","key2","key3"};
    vector<string> args = {"val1","val2","val3","60"};
    redis.eval<long long>(mset_with_ttl_script,keys.begin(),keys.end(),args.begin(),args.end());

    //----------pipeline----------

    //create pipeline
    auto pipe = redis.pipeline();

    //Send multiple commands and get all replies
    auto pipe_replies = pipe.set("key","value")
                            .get("key")
                            .rename("key","new-key")
                            .rpush("list",{"a","b","c"})
                            .lrange("list",0,-1)
                            .exec();

    //parse reply with reply type and index.
    auto set_cmd_result = pipe_replies.get<bool>(0);
    /* auto get_cmd_result = pipe_replies.get<OptionalStrig>(1); */

    //rename command result
    pipe_replies.get<void>(2);

    auto rpush_cmd_result = pipe_replies.get<long long >(3);
    
    vector<string> lrange_cmd_result;
    pipe_replies.get(4,std::back_inserter(lrange_cmd_result));

    //------------TRANSACTION commands----------

    //create a transaction
    auto tx = redis.transaction();

    //Run multiple commans in a transaction ,and get all repies
    auto tx_replied = tx.incr("num0")
                        .incr("num1")
                        .mget({"num0","num"})
                        .exec();

    //Parse reply with reply type and index
    auto incr_result0 = tx_replied.get<long long>(0);
    auto incr_result1 = tx_replied.get<long long>(1);

    /* vector<OptionalString> mget_cmd_result; */
    /* tx_replied.get(2,back_inserter(mget_cmd_result)); */

    //------------Generic Command Interface-------

    //There‘s not *redis::client_getname* interface
    //But you can use *Redis::command* to get the client name.
    /* val = redis.command<OptionalSting>("client","getname"); */
    /* if (val) { */
    /*     std::cout << *val << std::endl; */
    /* } */

    // Same as above.
    /* auto getname_cmd_str = {"client", "getname"}; */
    /* val = redis.command<OptionalString>(getname_cmd_str.begin(), getname_cmd_str.end()); */

    // There's no *Redis::sort* interface.
    // But you can use *Redis::command* to send sort the list.
    std::vector<std::string> sorted_list;
    /* redis.command("sort", "list", "ALPHA", std::back_inserter(sorted_list)); */

    // Another *Redis::command* to do the same work.
    auto sort_cmd_str = {"sort", "list", "ALPHA"};
    redis.command(sort_cmd_str.begin(), sort_cmd_str.end(), std::back_inserter(sorted_list));

    // ***** Redis Cluster *****

    // Create a RedisCluster object, which is movable but NOT copyable.
    auto redis_cluster = RedisCluster("tcp://127.0.0.1:7000");

    // RedisCluster has similar interfaces as Redis.
    redis_cluster.set("key", "value");
    val = redis_cluster.get("key");
    if (val) {
        std::cout << *val << std::endl;
    }   // else key doesn't exist.

    // Keys with hash-tag.
    redis_cluster.set("key{tag}1", "val1");
    redis_cluster.set("key{tag}2", "val2");
    redis_cluster.set("key{tag}3", "val3");

    /* std::vector<OptionalString> hash_tag_res; */
    /* redis_cluster.mget({"key{tag}1", "key{tag}2", "key{tag}3"}, */
    /*         std::back_inserter(hash_tag_res)); */
    return 0;
}


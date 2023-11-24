/* @file  @brief */

#include <chrono>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>
#include <string>
#include <mutex>
#include <chrono>
#include <thread>
#include <future>
#include <folly/futures/Future.h>
#include "folly/executors/CPUThreadPoolExecutor.h"
#include "folly/executors/FutureExecutor.h"

using folly::Future;
using std::cout;
using std::cin;
using std::endl;
using std::cerr;

/* @brief @param @param   @return */
int fun () {
    std::cout << "I'm fun" << std::endl;
    std::ofstream out("out.txt");
    if (out.is_open()) {
        out << "This is a line.\n";
        out << "This is another line.\n";
        out.close();
    }
    std::cout << "tid:" << pthread_self() << std::endl;
    return 1900;
}


int mget(std::vector<std::string>* values, const std::vector<std::string>& keys) {
    std::cout << "mget tid:" << pthread_self() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "key0:"  << keys[0];
    values->push_back("setted");
    std::cout << "size:" << values->size() <<std::endl;
    return true;
}


unsigned core_num = std::thread::hardware_concurrency();
unsigned thread_num = core_num - 4;
auto threadFactory = std::make_shared<folly::NamedThreadFactory>("AdReserverCPUThreadPool");
static folly::FutureExecutor<folly::CPUThreadPoolExecutor>*  thread_pool_ptr_ = new folly::FutureExecutor<folly::CPUThreadPoolExecutor>(std::make_pair(thread_num ,thread_num ),threadFactory);

class FisherCycle {
using mgetValues = folly::Future<std::vector<std::string>>;
public:
    FisherCycle() {
    }
    std::shared_ptr<folly::Future<std::vector<std::string>>> session_response;
    
    std::shared_ptr<folly::Future<int>> user_session_status;
    //mgetSessionStatusFuture
    /*
    void getUserSessionAsync(std::vector<std::string>* values, const std::vector<std::string>& keys) {
        unsigned core_num = std::thread::hardware_concurrency();
        unsigned thread_num = core_num - 4;
        auto threadFactory = std::make_shared<folly::NamedThreadFactory>("AdReserverCPUThreadPool");
        static folly::FutureExecutor<folly::CPUThreadPoolExecutor>*  thread_pool_ptr_ = new folly::FutureExecutor<folly::CPUThreadPoolExecutor>(std::make_pair(thread_num ,thread_num ),threadFactory);
        
        auto future = thread_pool_ptr_->addFuture([values, keys](){return mget(values, keys);});
        user_session_status =  std::make_shared<folly::Future<int>>(std::move(future));
    }
    */
    void setUserSessionAsync( std::vector<std::string>& keys) {
        session_response = mgetFuture(keys);
        keys[0] = ("no");
    }

    std::shared_ptr<mgetValues> mgetFuture(const std::vector<std::string>& keys) { 
        //auto future = thread_pool_ptr_->addFuture([this, keys](){return (this->mgetSync(keys));});
        auto future = thread_pool_ptr_->addFuture([this, &keys](){
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::vector<std::string> res;
            int ret = mget(&res, keys);
            return res;
        });
        //auto future = thread_pool_ptr_->addFuture([](){return std::vector<std::string>{"k"};});
        // 这里一定要move
        return std::make_shared<folly::Future<std::vector<std::string>>>(std::move(future));
    }

    std::vector<std::string> mgetSync(const std::vector<std::string>& keys){
        std::vector<std::string> res;
        int ret = mget(&res, keys);
        //std::cout<<res[0]<<std::endl;
        return res;
    }

    
};



int main () {
    FisherCycle td;
    std::vector<std::string> keys({"k1", "k2"});
    td.setUserSessionAsync(keys);
    std::cout << "tid:" << pthread_self() << std::endl;
    //td.getUserSessionAsync(&td.session_response, keys);   
    td.session_response->wait();
    std::cout << td.session_response->value()[0];
    return 0;
    /*
    unsigned core_num = std::thread::hardware_concurrency();
    unsigned thread_num = core_num - 4;
    std::cout << "tid:" << pthread_self() << std::endl;
    auto threadFactory = std::make_shared<folly::NamedThreadFactory>("AdReserverCPUThreadPool");
    static folly::FutureExecutor<folly::CPUThreadPoolExecutor>* thread_pool_ptr_ =  thread_pool_ptr_ = new folly::FutureExecutor<folly::CPUThreadPoolExecutor>(
    std::make_pair(thread_num ,thread_num ),threadFactory);
    
    //folly::Future<int> res = thread_pool_ptr_->addFuture(fun);
    folly::Future<std::vector<int>> res = thread_pool_ptr_->addFuture(fun1);

    res.wait();
    //std::cout << "val:" << res.value() << std::endl;
    auto vec = res.value();
    std::cout << vec[0] <<std::endl;
    std::cout << vec[1] <<std::endl;
    std::cout << "DONE" << std::endl;
    std::cout << "tid:" << pthread_self() << std::endl;
    return 0;
    */
}


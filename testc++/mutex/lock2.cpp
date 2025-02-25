/*
std::scope_lock是一个提供RAII机制的互斥量包装器
std::scope_lock可以对多个互斥量加锁。
if sizeof...(MutexTypes) == 0,即没有锁传入，什么也不会做。if sizeof...(MutexTypes) == 1,即传入一把锁，则会有效
的调用m.lock().否则有更多的锁，则有效的调用std::lock(m...). std::lock会进行死锁避免(银行家算法？？)
*/

#include <chrono>
#include <functional>   //函数对象
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

//using namespace std::chrono_literals;   //日期时间字面值

struct Employee
{
//隐含公开的
    std::vector<std::string> lunch_partners;
    std::string id;
    std::mutex m;
    Employee(std::string id):id(id)
    {}
    std::string partners() const {
        std::string ret = "Employee " + id + " has lunch partners: ";
        int count{};
        for( const auto& partner:lunch_partners)
            ret += (count++ ? ", ": "") + partner;
        return ret;
    }
};

void send_mail(Employee&, Employee& ) {
    //Simulate a time-consuming messaging operation
    std::this_thread::sleep_for(std::chrono::seconds(1));
}
void assign_lunch_partner(Employee& e1, Employee& e2) {
    static std::mutex io_mutex;
    {
        std::lock_guard<std::mutex> lk(io_mutex);
        std::cout << e1.id << " and " << e2.id << " are waiting for locks" << std::endl;
    }
    {
        //Use std::scoped_lock to acquire two locks without worrying about
        //other calls to assign_lunch_partner deadlocking us
        //and it also provides a convenient RAII-style mechanism
        std::scoped_lock lock(e1.m, e2.m);

        //Equivalent code 1 (using std::lock and std::lock_guard)
        //std::lock(e1.m,e2.m);
        // std::lock_guard<std::mutex> lk1(e1.m, std::adopt_lock);
        // std::lock_guard<std::mutex> lk2(e2.m, std::adopt_lock);

        //Equivalent code 2 (if unique_locks are needed, e.g. for condition variables)
        //std::unique_lock<std::mutex> lk1(e1.m, std::defer_lock);
        //std::unique_lock<std::mutex> lk2(e2.m, std::defer_lock);
        //std::lock(lk1, lk2);
        {
            std::lock_guard<std::mutex> lo(io_mutex);
            std::cout << e1.id << " and " << e2.id << " got locks" << std::endl;
        }
        e1.lunch_partners.push_back(e2.id);
        e2.lunch_partners.push_back(e1.id);
    }
    send_mail(e1,e2);
    send_mail(e2,e1);
}

int main(void) {
    Employee alice("Alice"), bob("Bob"), christina("Christina"), dave("Dave");

    //Assign in parallel threads because mailing users about lunch assignments
    //takes a long time
    std::vector<std::thread> threads;
    threads.emplace_back(assign_lunch_partner, std::ref(alice), std::ref(bob));
    threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(bob));
    threads.emplace_back(assign_lunch_partner, std::ref(christina), std::ref(alice));
    threads.emplace_back(assign_lunch_partner, std::ref(dave), std::ref(bob));

    for (auto &t: threads) {
        t.join();
    }

    std::cout << alice.partners() << std::endl << bob.partners() << std::endl
              << christina.partners() << std::endl << dave.partners() << std::endl;
}
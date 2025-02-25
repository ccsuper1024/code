//std::unique_lock
//独占互斥量的所有权，一般用于读写锁的写锁
#include <iostream>
#include <mutex>
#include <thread>

struct Box
{
public:
    explicit Box(int num): num_things(num) {}   //显式强制构造，禁止隐式构造对象
    int num_things;
    std::mutex m;
};

void transfer(Box& from, Box& to, int num) {
    //don't actually take the locks ye
    std::unique_lock lock1(from.m, std::defer_lock);    //第二个参数的类型是std::defer_lock_t
    std::unique_lock lock2(to.m, std::defer_lock);

    //lock both unique_locks without deadlock
    //std::lock会使用类似银行家算法的算法来进行死锁避免
    std::lock(lock1, lock2);

    from.num_things -= num;
    to.num_things += num;

    //from.m adn to.m mutexs unlocked in unique_lock dtors
}

int main(void) {
    Box acc1(100);
    Box acc2(50);

    std::thread t1(transfer,std::ref(acc1), std::ref(acc2), 10);
    std::thread t2(transfer,std::ref(acc1), std::ref(acc2), 5);

    t1.join();
    t2.join();

    std::cout << "acc1: " << acc1.num_things << "\n"
              << "acc2: " << acc2.num_things << "\n";
}
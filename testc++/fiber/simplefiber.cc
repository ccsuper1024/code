/**
 *@brief 使用POSIX线程模拟实现协程,协程能够提高CPU的利用率，避免IO等待,能够更自然的实现异步编程
 *
 * */
#include <pthread.h>
#include <iostream>
#include <queue>
#include <functional>
#include <unistd.h>  // for usleep

// 定义协程任务类型
using CoroutineTask = std::function<void()>;

// 简单的调度器类
class CoroutineScheduler {
public:
    void createCoroutine(CoroutineTask task) {
        tasks.push(task);
    }

    void run() {
        while (!tasks.empty()) {
            CoroutineTask task = tasks.front();
            tasks.pop();
            task(); // 执行任务
        }
    }

    // 模拟协程的挂起和恢复
    void yield() {
        std::cout << "Yielding...\n";
        usleep(100);  // 模拟挂起协程
    }

private:
    std::queue<CoroutineTask> tasks;  // 协程任务队列
};

// 全局调度器
CoroutineScheduler scheduler;

// 协程函数
void coroutine1() {
    std::cout << "Coroutine 1: Step 1\n";
    scheduler.yield(); // 挂起并交出控制权
    std::cout << "Coroutine 1: Step 2\n";
}

void coroutine2() {
    std::cout << "Coroutine 2: Step 1\n";
    scheduler.yield(); // 挂起并交出控制权
    std::cout << "Coroutine 2: Step 2\n";
}

int main() {
    // 创建协程
    scheduler.createCoroutine(coroutine1);
    scheduler.createCoroutine(coroutine2);

    // 运行调度器
    scheduler.run();

    return 0;
}


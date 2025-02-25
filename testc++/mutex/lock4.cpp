//std::shared_lock<Mutex>::lock()
//Locks the associated mutex in shared mode. Effectively calls mutex()->lock_shared().
//std::shared_lock是个类模板，所以有lock成员函数。而std::lock是函数模板，不存在成员函数
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>

std::string file = "Original content.";     //Simulates a file
std::mutex output_mutex;                    //mutex that protects output operations.
std::shared_mutex file_mutex;               //reader/writer mutex

void read_content(int id) {                 //读操作
    std::string content;
    {
        //std::shared_lock类对象都是基于RAII的互斥量包装器，出了作用域都会自动调用析构函数，安全的释放锁
        //并且这里初始化lock时是指定的延迟策略，那么在初始化构造时是没有加锁的，只有调用了lock.lock()函数时才加了锁
        //出了作用域后又解锁了
        std::shared_lock lock(file_mutex, std::defer_lock);     //用读写锁初始化读锁锁定方式类
        lock.lock();    //lock it here
        content = file; //赋值运算符
    }
    std::lock_guard<std::mutex> lock(output_mutex);
    std::cout << "Contents read by reader #" << id <<" " <<  content << '\n';
}

void write_content() {
    {
        std::lock_guard<std::shared_mutex> file_lock(file_mutex);   //相当于写锁
        file = "New content";
    }
    std::lock_guard<std::mutex> output_lock(output_mutex);
    std::cout << "New content saved.\n";
}

int main()
{
    std::cout << "Two readers reading from file.\n"
              << "A writer competes with them.\n";

    std::thread reader1(read_content, 1);
    std::thread reader2(read_content, 2);
    std::thread writer1(write_content);
    reader1.join();
    reader2.join();
    writer1.join();

    std::cout << "The first few operations to file are done.\n";
    reader1 = std::thread(read_content, 3);         //移动线程
    reader1.join();

}
/*
一种可能：writer1先写，随后执行reader1 reader2
Two readers reading from file.
A writer competes with them.
New content saved.
Contents read by reader #1 New content
Contents read by reader #2 New content
The first few operations to file are done.
Contents read by reader #3 New content

第二种可能：reader1先读，reader2随后，再是writer1写
Two readers reading from file.
A writer competes with them.
Contents read by reader #1: Original content.
Contents read by reader #2: Original content.
New content saved.
The first few operations to file are done.
Contents read by reader #3: New content

第三种可能：reader1/2先读，writer1再写，最后reader2/1读
Two readers reading from file.
A writer competes with them.
Contents read by reader #1或2: Original content.
New content saved.
Contents read by reader #2或1: New content.
The first few operations to file are done.
Contents read by reader #3: New content


*/
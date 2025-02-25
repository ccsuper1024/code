#include<chrono>
#include<iostream>
#include<thread>

void foo(){
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main(void) {
    std::thread t1(foo);
    std::thread::id t1_id = t1.get_id();
    
    std::thread t2(foo);
    std::thread::id t2_id = t2.get_id();

    std::cout << "t1's id: " << t1_id << '\n';
    std::cout << "t2's id: " << t2_id << std::endl;

    t1.join();
    t2.join();

    std::cout << "t1's id after join: " << t1_id << '\n';
    std::cout << "t2's id after join: " << t2_id << std::endl;

}
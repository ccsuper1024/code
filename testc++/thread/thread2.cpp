//
// Created by chenchao on 24-5-3.
//
#include <thread>
using namespace  std::chrono_literals;

int main(void) {
    auto bleah = std::thread{[]{std::this_thread::sleep_for(13ms);}};
}

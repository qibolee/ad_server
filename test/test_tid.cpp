#include <iostream>
#include <thread>
#include <pthread.h>
#include <cstdio>

int main(int argc, char **argv) {

    std::cout << std::this_thread::get_id() << std::endl;
    printf("%llu\n", std::this_thread::get_id());
    return 0;
}



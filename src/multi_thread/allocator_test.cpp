#include "allocator.h"
#include <vector>
#include <string>
#include <thread>
#include <iostream>
#include <chrono>
#include <memory.h>

struct alignas(64) Payload {};

static const size_t MAX_COUNT = 1024 * 1024 * 1024;

struct TENM {
    char sz[1024 * 1024 * 10] = { 0 };
};

struct ONEM {
    char sz[1024 * 1024 * 1] = {0};

};

struct FIVEM {
    char sz[1024 * 1024 * 5] = {0};
};

const int MAXLOOP = 10;
const int size1K = 1 * 1024;
const int SIZE1M = 1 * 1024 * 1024;
const int SIZE5M = 5 * 1024 * 1024;
const int SIZE10M = 10 * 1024 * 1024;
const int SIZE1G = 1024 * 1024 * 1024;

const int SIZE16B = 6;
const int SIZE32B = 32;
const int SIZE512B = 512;

int main(int argc, char const *argv[]) {
    std::string str16B(SIZE16B, 'a');
    std::string str32B(SIZE32B, 'a');
    std::string str512B(SIZE512B, 'a');

    std::string str1K(size1K, 'a');
    std::string str1M(SIZE1M, 'a');
    std::string str5M(SIZE5M, 'a');
    std::string str10M(SIZE10M, 'a');
    std::string str1G(SIZE1G, 'a');
    
    // int op = atoi(argv[1]);
    utils::Arena<utils::ThreadSafeObjectPoolAllocator<TENM>, std::mutex> 
        obj_pool("pool", MAX_COUNT);
    auto test = [&](const std::string &str) {
        for (int i = 0; i < 10; ++ i) {
            char *p = (char*)obj_pool.alloc(str.size());
            memcpy(p, str.c_str(), str.size());
        }
    };

    auto start = std::chrono::steady_clock::now();
    std::vector<std::thread> vt;
    for (int i = 0; i < 10; ++ i) {
        vt.push_back(std::thread(test, str10M));
    }
    for (auto &t : vt) {
        t.join();
    }
    auto end = std::chrono::steady_clock::now();
    auto last = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "10 threads allocate 10M memory: " << last << "ms\n";

    return 0;
}
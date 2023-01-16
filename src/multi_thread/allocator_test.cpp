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

struct ONEG{
    char sz[1024 * 1024 * 1024] = {0};
};

struct MEM_32B {
    char sz[32] = {0};
};

struct MEM_512B {
    char sz[512] = {0};
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

    std::string op(argv[1]);
    if (op == "1M") {
        auto st = std::chrono::steady_clock::now();
        utils::Arena<utils::ThreadSafeObjectPoolAllocator<ONEM>, std::mutex> 
            obj_pool("pool", MAX_COUNT);
        auto ed = std::chrono::steady_clock::now();
        auto lt = std::chrono::duration<double, std::milli>(ed - st).count();
        std::cout << "allocate 1G memory pool use : " << lt << " ms\n";
        auto test = [&](const std::string &str) {
            
            for (int i = 0; i < 10; ++ i) {
                auto start = std::chrono::steady_clock::now();
                char *p = (char*)obj_pool.alloc(str.size());
                memcpy(p, str.c_str(), str.size());
                auto end = std::chrono::steady_clock::now();
                auto last = std::chrono::duration<double, std::milli>(end - start).count();
                printf("thread %d malloc use %lf ms\n", std::this_thread::get_id(), last);
            }
            
        }; 
        auto start = std::chrono::steady_clock::now();
        std::vector<std::thread> vt;
        for (int i = 0; i < 10; ++ i) {
            vt.push_back(std::thread(test, str1M));
        }
        for (auto &t : vt) {
            t.join();
        }
        auto end = std::chrono::steady_clock::now();
        auto last = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "10 threads allocate " << op <<  " memory: " << last << " ms\n";
    }
    
    if (op == "5M") {
        auto st = std::chrono::steady_clock::now();
        utils::Arena<utils::ThreadSafeObjectPoolAllocator<FIVEM>, std::mutex> 
            obj_pool("pool", MAX_COUNT);
        auto ed = std::chrono::steady_clock::now();
        auto lt = std::chrono::duration<double, std::milli>(ed - st).count();
        std::cout << "allocate 1G memory pool use : " << lt << " ms\n";
        auto test = [&](const std::string &str) {
            
            for (int i = 0; i < 10; ++ i) {
                auto start = std::chrono::steady_clock::now();
                char *p = (char*)obj_pool.alloc(str.size());
                memcpy(p, str.c_str(), str.size());
                auto end = std::chrono::steady_clock::now();
                auto last = std::chrono::duration<double, std::milli>(end - start).count();
                printf("thread %d malloc use %lf ms\n", std::this_thread::get_id(), last); 
            }
            
        }; 
        auto start = std::chrono::steady_clock::now();
        std::vector<std::thread> vt;
        for (int i = 0; i < 10; ++ i) {
            vt.push_back(std::thread(test, str5M));
        }
        for (auto &t : vt) {
            t.join();
        }
        auto end = std::chrono::steady_clock::now();
        auto last = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "10 threads allocate " << op <<  " memory: " << last << " ms\n";
    }
    
    if (op == "10M") {
        auto st = std::chrono::steady_clock::now();
        utils::Arena<utils::ThreadSafeObjectPoolAllocator<TENM>, std::mutex> 
            obj_pool("pool", MAX_COUNT);
        auto ed = std::chrono::steady_clock::now();
        auto lt = std::chrono::duration<double, std::milli>(ed - st).count();
        // std::cout << "allocate 1G memory pool use : " << lt << " ms\n";
        auto test = [&](const std::string &str) {
            
            for (int i = 0; i < 10; ++ i) {
                // auto start = std::chrono::steady_clock::now();
                char *p = (char*)obj_pool.alloc(str.size());
                memcpy(p, str.c_str(), str.size());
                // auto end = std::chrono::steady_clock::now();
                // auto last = std::chrono::duration<double, std::milli>(end - start).count();
                // printf("%lf\n", last);
                // printf("thread %d malloc use %lf ms\n", std::this_thread::get_id(), last);
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
        std::cout << "10 threads allocate " << op <<  " memory: " << last << " ms\n";
    }
    
    if (op == "1G") {
        utils::Arena<utils::ThreadSafeObjectPoolAllocator<ONEG>, std::mutex> 
            obj_pool("pool", MAX_COUNT * 10);
        auto test = [&](const std::string &str) {
            for (int i = 0; i < 1; ++ i) {
                char *p = (char*)obj_pool.alloc(str.size());
                memcpy(p, str.c_str(), str.size());
            }
        }; 
        auto start = std::chrono::steady_clock::now();
        std::vector<std::thread> vt;
        for (int i = 0; i < 10; ++ i) {
            vt.push_back(std::thread(test, str1G));
        }
        for (auto &t : vt) {
            t.join();
        }
        auto end = std::chrono::steady_clock::now();
        auto last = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "10 threads allocate " << op <<  " memory: " << last << "ms\n";
    }
    
    if (op == "32B") {
        utils::Arena<utils::ThreadSafeObjectPoolAllocator<MEM_32B>, std::mutex> 
            obj_pool("pool", MAX_COUNT);
        auto small_memory_test = [&]() {
            auto start = std::chrono::steady_clock::now();
            for (int i = 0; i < 10000; ++ i) {
                char* p = (char*)obj_pool.alloc(str32B.size());
                memcpy(p, str32B.c_str(), str32B.size());
            }
            auto end = std::chrono::steady_clock::now();
            auto last = std::chrono::duration<double, std::milli>(end - start).count();
            printf("thread %d random malloc 32B memory 10000 times used %lf ms\n", std::this_thread::get_id(), last);
        };
        auto start = std::chrono::steady_clock::now();
        std::vector<std::thread> vt;
        for (int i = 0; i < 10; ++ i) {
        vt.push_back(std::thread(small_memory_test));
        }
        for (auto &t : vt) t.join();
        auto end = std::chrono::steady_clock::now();
        auto last = std::chrono::duration<double, std::milli>(end - start).count();
        printf("10 threads random allocate 32B memory used %lf ms\n", last);
    }
    
    if (op == "512B") {
        utils::Arena<utils::ThreadSafeObjectPoolAllocator<MEM_512B>, std::mutex> 
            obj_pool("pool", MAX_COUNT);
        auto small_memory_test = [&]() {
            auto start = std::chrono::steady_clock::now();
            for (int i = 0; i < 10000; ++ i) {
                char* p = (char*)obj_pool.alloc(str512B.size());
                memcpy(p, str512B.c_str(), str512B.size());
            }
            auto end = std::chrono::steady_clock::now();
            auto last = std::chrono::duration<double, std::milli>(end - start).count();
            printf("thread %d random malloc 512B memory 10000 times used %lf ms\n", std::this_thread::get_id(), last);
        };
        auto start = std::chrono::steady_clock::now();
        std::vector<std::thread> vt;
        for (int i = 0; i < 10; ++ i) {
        vt.push_back(std::thread(small_memory_test));
        }
        for (auto &t : vt) t.join();
        auto end = std::chrono::steady_clock::now();
        auto last = std::chrono::duration<double, std::milli>(end - start).count();
        printf("10 threads random allocate 512B memory used %lf ms\n", last);
    }
    return 0;
}
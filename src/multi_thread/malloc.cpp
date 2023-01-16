#pragma optimize( "", off )

#include <iostream>
#include <chrono>
#include <ctime>
#include <memory.h>
#include <thread>
#include <vector>
#include <string>
#include <random>

const int size1K = 1 * 1024;
const int SIZE1M = 1 * 1024 * 1024;
const int SIZE5M = 5 * 1024 * 1024;
const int SIZE10M = 10 * 1024 * 1024;
const int SIZE1G = 1024 * 1024 * 1024;

const int SIZE16B = 6;
const int SIZE32B = 32;
const int SIZE512B = 512;


void test(const std::string &str) {
  int loop = 10;
  if (str.size() == SIZE1G) loop = 1;
  
  for (int i = 0; i < loop; ++ i) {
    // auto start = std::chrono::steady_clock::now();
    char *p = (char*)malloc(str.size());
    memcpy(p, str.c_str(), str.size());
    // auto end = std::chrono::steady_clock::now();
    // auto last = std::chrono::duration<double, std::milli>(end - start).count();
    // printf("%lf\n", last);
    // printf("thread %d malloc use %lf ms\n", std::this_thread::get_id(), last);
  }
}

int main(int argc, char const *argv[]) {
  std::string str1K(size1K, 'a');
  std::string str1M(SIZE1M, 'a');
  std::string str5M(SIZE5M, 'a');
  std::string str10M(SIZE10M, 'a');
  std::string str1G(SIZE1G, 'a');

  std::string str16B(SIZE16B, 'a');
  std::string str32B(SIZE32B, 'a');
  std::string str512B(SIZE512B, 'a');
  std::string op(argv[1]);
  
  if (op == "1M" || op == "5M" || op == "10M") {
    auto start = std::chrono::steady_clock::now();
    std::vector<std::thread> vt;
    for (int i = 0; i < 10; ++ i) {
      if (op == "1M")
      vt.push_back(std::thread(test, str1M));
      else if (op == "5M") 
        vt.push_back(std::thread(test, str5M));
      else if (op == "10M") vt.push_back(std::thread(test, str10M));
      else vt.push_back(std::thread(test, str1G));
    }
    for (auto &t : vt) t.join();
    auto end = std::chrono::steady_clock::now();
    auto last = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "10 threads allocate " << op << " memory: " << last << "ms\n";
  } else if (op == "1G"){
    auto start = std::chrono::steady_clock::now();
    char* p = (char*)malloc(str1G.size());
    memcpy(p, str1G.c_str(), str1G.size());
    auto end = std::chrono::steady_clock::now();
    auto last = std::chrono::duration<double, std::milli>(end - start).count();
    printf("allocate 1G use : %lf ms\n", last);
  } else if (op == "32B") {
    auto small_memory_test = [&]() {
      // 随机分配0~1k内存
      auto start = std::chrono::steady_clock::now();
      for (int i = 0; i < 10000; ++ i) {
        char* p = (char*)malloc(str32B.size());
        memcpy(p, str32B.c_str(), str32B.size());
        
      }
      auto end = std::chrono::steady_clock::now();
      auto last = std::chrono::duration<double, std::milli>(end - start).count();
      printf("thread %d malloc 32B memory 10000 times used %lf ms\n", std::this_thread::get_id(), last);
    };
    auto start = std::chrono::steady_clock::now();
    std::vector<std::thread> vt;
    for (int i = 0; i < 10; ++ i) {
      vt.push_back(std::thread(small_memory_test));
    }
    for (auto &t : vt) t.join();
    auto end = std::chrono::steady_clock::now();
    auto last = std::chrono::duration<double, std::milli>(end - start).count();
    printf("10 threads  allocate 32B memory used %lf ms\n", last);
  } else {
    auto small_memory_test = [&]() {
      auto start = std::chrono::steady_clock::now();
      for (int i = 0; i < 10000; ++ i) {
        char* p = (char*)malloc(str512B.size());
        memcpy(p, str512B.c_str(), str512B.size());
        
      }
      auto end = std::chrono::steady_clock::now();
      auto last = std::chrono::duration<double, std::milli>(end - start).count();
      printf("thread %d  malloc 32B memory 10000 times used %lf ms\n", std::this_thread::get_id(), last);
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
  
  return 0;
}
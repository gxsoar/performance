#pragma optimize( "", off )

#include <iostream>
#include <chrono>
#include <ctime>
#include <memory.h>
#include <random>
#include <string>

const int size1K = 1 * 1024;
const int SIZE1M = 1 * 1024 * 1024;
const int SIZE5M = 5 * 1024 * 1024;
const int SIZE10M = 10 * 1024 * 1024;
const int SIZE1G = 1024 * 1024 * 1024;

const int SIZE16B = 6;
const int SIZE32B = 32;
const int SIZE512B = 512;

int main(int argc, char const *argv[]) {
  // 随机分配0~1k内存
  int min_mem = 1,max_mem = 1024;
  std::random_device seed;//硬件生成随机数种子
	std::ranlux48 engine(seed());//利用种子生成随机数引擎
  std::uniform_int_distribution<> distrib(min_mem, max_mem);//设置随机数范围，并为均匀分布
  double res = 0.0;
  const int loop = 7;
  for (int i = 0; i < loop; ++ i) {
    for (int i = 0; i < 100000; ++ i) {
      int random = distrib(engine);
      std::string tmp(random, 'a');
      auto start = std::chrono::steady_clock::now();
      char* p = (char*)malloc(tmp.size());
      memcpy(p, tmp.c_str(), tmp.size());
      auto end = std::chrono::steady_clock::now();
      res += std::chrono::duration<double, std::milli>(end - start).count();
    }
  }
  std::cout << "random allocate 1~1k memory 10000 times cost : " << res / 7 << "ms\n";

  // 随机分配1m~10m内存
  min_mem = 1 * 1024 * 1024, max_mem = 10 * 1024 * 1024;
  std::random_device seed2;
  std::ranlux48 engine2(seed2());
  std::uniform_int_distribution<> distrib2(min_mem, max_mem);
  res = 0.0;
  for (int i = 0; i < loop; ++ i) {
    for (int i = 0; i < 100; ++ i) {
      int random = distrib2(engine);
      std::string tmp(random, 'a');
      auto start = std::chrono::steady_clock::now();
      char* p = (char*)malloc(tmp.size());
      memcpy(p, tmp.c_str(), tmp.size());
      auto end = std::chrono::steady_clock::now();
      res += std::chrono::duration<double, std::milli>(end - start).count();
    }
  }
  
  std::cout << "random allocate 1m~10m memory 100 times cost: " << res / loop<< "ms\n";

  std::string tmp(1024 * 1024 * 1024, 'a');
  auto start = std::chrono::steady_clock::now();
  char* p = (char*)malloc(tmp.size());
  memcpy(p, tmp.c_str(), tmp.size());
  auto end = std::chrono::steady_clock::now();
  res += std::chrono::duration<double, std::milli>(end - start).count();
  std::cout << "allocate 1G memory cost : " << res << "ms\n";
  return 0;
}

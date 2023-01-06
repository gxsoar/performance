#pragma optimize( "", off )

#include <iostream>
#include <chrono>
#include <ctime>
#include <memory.h>
#include "gperftools/heap-checker.h"
#include <string>

const int MAXLOOP = 10;
const int size1K = 1 * 1024;
const int SIZE1M = 1 * 1024 * 1024;
const int SIZE5M = 5 * 1024 * 1024;
const int SIZE10M = 10 * 1024 * 1024;
const int SIZE1G = 1024 * 1024 * 1024;

const int SIZE16B = 6;
const int SIZE32B = 32;
const int SIZE512B = 512;


void test(const std::string &str) {
  for (int i = 0; i < MAXLOOP; ++ i) {
    char *p = (char*)malloc(str.size());
    memcpy(p, str.c_str(), str.size());
    // free(p);
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

  const int loop = 6;

  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < loop; ++ i)
    test(str1K);
  auto end = std::chrono::steady_clock::now();
  auto last = std::chrono::duration<double, std::milli>(end - start).count();
  std::cout << "allocate 1k memory: " << last / loop << "ums\n";

  start = std::chrono::steady_clock::now();
  for (int i = 0; i < loop; ++ i)
    test(str1M);
  end = std::chrono::steady_clock::now();
  last = std::chrono::duration<double>(end - start).count();
  std::cout << "allocate 1M memory: " << last / loop << "s\n";

  start = std::chrono::steady_clock::now();
  for (int i = 0; i < loop; ++ i)
    test(str5M);
  end = std::chrono::steady_clock::now();
  last = std::chrono::duration<double>(end - start).count();
  std::cout << "allocate 5M memory: " << last / loop << "s\n";

  start = std::chrono::steady_clock::now();
  for (int i = 0; i < loop; ++ i)
    test(str10M);
  end = std::chrono::steady_clock::now();
  last = std::chrono::duration<double>(end - start).count();
  std::cout << "allocate 10M memory: " << last / loop << "s\n";

  start = std::chrono::steady_clock::now();
  for (int i = 0; i < loop; ++ i)
    test(str16B);
  end = std::chrono::steady_clock::now();
  last = std::chrono::duration<double, std::milli>(end - start).count();
  std::cout << "allocate 16B memory: " << last / loop << "ms\n";

  start = std::chrono::steady_clock::now();
  for (int i = 0; i < loop; ++ i)
    test(str32B);
  end = std::chrono::steady_clock::now();
  last = std::chrono::duration<double, std::milli>(end - start).count();
  std::cout << "allocate 32B memory: " << last / loop << "ms\n";

  start = std::chrono::steady_clock::now();
  for (int i = 0; i < loop; ++ i)
    test(str512B);
  end = std::chrono::steady_clock::now();
  last = std::chrono::duration<double, std::milli>(end - start).count();
  std::cout << "allocate 512 memory: " << last / loop << "ms\n";

  start = std::chrono::steady_clock::now();
  for (int i = 0; i < loop; ++ i)
    test(str1K);
  end = std::chrono::steady_clock::now();
  last = std::chrono::duration<double, std::micro>(end - start).count();
  std::cout << "allocate 1k memory: " << last / loop << "ms\n";

  start = std::chrono::steady_clock::now();
  char *p = (char*)malloc(SIZE1G);
  // memcpy(p, str1G.c_str(), SIZE1G);
  end = std::chrono::steady_clock::now();
  last = std::chrono::duration<double, std::milli>(end - start).count();
  std::cout << "allocate 1G memory : " << last << "ms\n";
  return 0;
}

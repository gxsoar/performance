#pragma optimize( "", off )


#include "ngx_malloc.h"
#include <iostream>
#include <chrono>
#include <string>
#include <random>

const int MAXPOOL = 1024 * 1024 * 1024;
const int MAXLOOP = 10;
const int size1K = 1 * 1024;
const int SIZE1M = 1 * 1024 * 1024;
const int SIZE5M = 5 * 1024 * 1024;
const int SIZE10M = 10 * 1024 * 1024;
const int SIZE1G = 1024 * 1024 * 1024;

const int SIZE16B = 6;
const int SIZE32B = 32;
const int SIZE512B = 512;

struct Node
{
	char*str;
	FILE *file;
};

void func1(void * str)
{
	free(str);
	std::cout << "free str" << std::endl;
}
void func2(void *fd)
{
	fclose((FILE*)fd);
	std::cout << "close file" << std::endl;
}

void test(const std::string &str, Ngx_mem_pool* mempool) {
  for (int i = 0; i < MAXLOOP; ++ i) {
    char *p = (char*)mempool->ngx_palloc(str.size());
    memcpy(p, str.c_str(), str.size());
    // free(p);
  }
}

int main() {
    auto start = std::chrono::steady_clock::now();
	Ngx_mem_pool mempool(MAXPOOL); //创建一个内存池一个大小为MAXPOOL的内存池
    auto end = std::chrono::steady_clock::now();
    auto last = std::chrono::duration<double, std::micro>(end - start).count();
    std::cout << "allocate 1G memeory pool: " << last << "ms\n";


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
        char* p = (char*)mempool.ngx_palloc(tmp.size());
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
        char* p = (char*)mempool.ngx_palloc(tmp.size());
        memcpy(p, tmp.c_str(), tmp.size());
        auto end = std::chrono::steady_clock::now();
        res += std::chrono::duration<double, std::milli>(end - start).count();
        }
    }
  
    std::cout << "random allocate 1m~10m memory 100 times cost: " << res / loop<< "ms\n";
    
	ngx_pool_cleanup_s* str = mempool.ngx_pool_cleanup_add(sizeof(ngx_pool_cleanup_s));
	str->handler = func1;
	str->data = nullptr;

	ngx_pool_cleanup_s*file = mempool.ngx_pool_cleanup_add(sizeof(ngx_pool_cleanup_s));
	file->handler = func2;
	file->data = nullptr;
	return 0;
}

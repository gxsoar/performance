#pragma optimize( "", off )


#include "ngx_malloc.h"
#include <iostream>
#include <chrono>
#include <string>

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
	  Ngx_mem_pool mempool(MAXPOOL); //创建一个内存池一个大小为MAXPOOL的内存池
    auto end = std::chrono::steady_clock::now();
    auto last = std::chrono::duration<double, std::micro>(end - start).count();
    std::cout << "allocate 1G memeory pool: " << last << "ms\n";

    // allocate 1k memory

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < loop; ++ i)
        test(str1K, &mempool);
    end = std::chrono::steady_clock::now();
    last = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "allocate 1k memory: " << last / loop << "ums\n";

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < loop; ++ i)
        test(str1M, &mempool);
    end = std::chrono::steady_clock::now();
    last = std::chrono::duration<double>(end - start).count();
    std::cout << "allocate 1M memory: " << last / loop << "s\n";

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < loop; ++ i)
        test(str5M, &mempool);
    end = std::chrono::steady_clock::now();
    last = std::chrono::duration<double>(end - start).count();
    std::cout << "allocate 5M memory: " << last / loop << "s\n";

    start = std::chrono::steady_clock::now();
    for (int i = 0; i < loop; ++ i)
        test(str10M, &mempool);
    end = std::chrono::steady_clock::now();
    last = std::chrono::duration<double>(end - start).count();
    std::cout << "allocate 10M memory: " << last / loop << "s\n";
	
    
	ngx_pool_cleanup_s* str = mempool.ngx_pool_cleanup_add(sizeof(ngx_pool_cleanup_s));
	str->handler = func1;
	str->data = nullptr;

	ngx_pool_cleanup_s*file = mempool.ngx_pool_cleanup_add(sizeof(ngx_pool_cleanup_s));
	file->handler = func2;
	file->data = nullptr;
	return 0;
}

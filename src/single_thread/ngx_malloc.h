#ifndef NGX_PALLOC_H
#define NGX_PALLOC_H
#include <cstdlib>
#include <cstring>
#include <cstdint>

// Nginx内存池的主要类型定义
using u_char = unsigned char;
using ngx_uint_t = unsigned int;

// 外部资源释放结构体类型
typedef void(*ngx_pool_cleanup_pt)(void *data); // 回调函数指针
struct ngx_pool_cleanup_s
{
	ngx_pool_cleanup_pt   handler; // 回调函数指针
	void                 *data;    // 传入回调函数的实参
	ngx_pool_cleanup_s   *next;    // 指向下一个外部资源释放结构体类型
};

// 大块内存的头信息
struct ngx_pool_large_s {
	ngx_pool_large_s     *next;  // 指向下一个大块内存的头部
	void                 *alloc; // 指向开辟内存的首地址
};

// 内存池中分配内存的小块内存的内存块头信息
struct ngx_pool_s; // 类型声明
struct ngx_pool_data_s {
	u_char               *last;   // 指向内存块可用内存起始位置
	u_char               *end;    // 指向内存块可用内存末尾位置
	ngx_pool_s           *next;   // 指向下一个内存块
	ngx_uint_t            failed; // 记录当前内存块开辟小块内存失败次数
};

// 内存池的头部信息
struct ngx_pool_s {
	ngx_pool_data_s       d;
	size_t                max;     // 小块内存和大块内存的界限
	ngx_pool_s           *current; // 指向小块内存池中第一块可用内存块
	ngx_pool_large_s     *large;   // 记录大块内存的链表起始地址
	ngx_pool_cleanup_s   *cleanup; // 记录释放外部资源的链表起始地址

	// ngx_chain_t       *chain; 这个暂时不用
	// ngx_log_t         *log;   这个是ngx的日志
};

// 一个物理页面的大小4k
const int ngx_pagesize = 4096;
// ngx定义小块内存最大可分配一个页面 4k
const int NGX_MAX_ALLOC_FROM_POOL = ngx_pagesize - 1; 
// ngx内存池默认大小，16k
const int NGX_DEFAULT_POOL_SIZE = 16 * 1024;
// ngx内存对齐是以16为基准的
const int NGX_POOL_ALIGNMENT = 16;

// 将d的大小提升为a的倍数
#define ngx_align(d, a)  (((d) + (a - 1)) & ~(a - 1))
// ngx内存池最小字节数，因为至少需要存放内存池的头信息
const int NGX_MIN_POOL_SIZE = 
		ngx_align((sizeof(ngx_pool_s) + 2 * sizeof(ngx_pool_large_s)),
			NGX_POOL_ALIGNMENT);
// NGX对齐方式是平台unsigned long
const int NGX_ALIGNMENT = sizeof(unsigned long);

// 将p大小提升为a的倍数
#define ngx_align_ptr(p, a) (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

class Ngx_mem_pool
{
public:
	// 将内存池的构造和销毁直接写入构造和析构函数中
	Ngx_mem_pool(size_t size);
	~Ngx_mem_pool();

	// 创建内存池
	//bool ngx_create_pool(size_t size);
	// 释放内存池
	//void ngx_destroy_pool();

	// 重置内存池
	void ngx_reset_pool();

	// 从内存池中开辟内存，并且考虑内存对齐 
	void* ngx_palloc( size_t size);

	// 从内存池中开辟内存，不考虑内存对齐
	void* ngx_pnalloc( size_t size);

	//  从内存池中开辟内存，调用的是ngx_palloc,并初始化为0
	void* ngx_pcalloc( size_t size);

	// 大块内存释放函数
	bool ngx_pfree(void *p);

	// 添加回调清理函数
	ngx_pool_cleanup_s * ngx_pool_cleanup_add(size_t size);
private:
	// 小块内存分配
	void * ngx_palloc_small(size_t size, ngx_uint_t align);
	
	// 大块内存分配
	void* ngx_palloc_large(size_t size);

	// 分配小块内存块函数
	void* ngx_palloc_block(size_t size);

	// 开辟内存函数，调用malloc
	void* ngx_alloc(size_t size);

	ngx_pool_s *pool;
};


#endif // !NGX_MEM_POOL_H


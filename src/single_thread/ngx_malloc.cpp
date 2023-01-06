#include "ngx_malloc.h"

// 创建内存池
Ngx_mem_pool::Ngx_mem_pool(size_t size)
{
	// 调用malloc开辟内存
	pool =(ngx_pool_s*) ngx_alloc(size);
	if (pool == nullptr) 
	{
		throw"mem pool failed!";
	}

	// last指向当前内存块中可用内存起始地址
	pool->d.last = (u_char *)pool + sizeof(ngx_pool_s);
	pool->d.end = (u_char *)pool + size; // end指向当前内存块末尾地址
	pool->d.next = nullptr;
	pool->d.failed = 0;

	// size 如果小于4k,max = size 否则 max = 4095
	size = size - sizeof(ngx_pool_s);
	pool->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

	pool->current = pool;
	pool->large = nullptr;
	pool->cleanup = nullptr;
	// p->chain = NULL;
	// p->log = log;
}

// 释放内存池
Ngx_mem_pool::~Ngx_mem_pool()
{
	ngx_pool_s          *p, *n;
	ngx_pool_large_s    *l;
	ngx_pool_cleanup_s  *c;

	// 遍历外部资源的头部，调用回调函数释放外部资源
	for (c = pool->cleanup; c != nullptr; c = c->next) 
	{
		if (c->handler != nullptr) 
		{
			c->handler(c->data);
		}
	}
	// 遍历大块内存头信息，释放大块内存
	for (l = pool->large; l != nullptr; l = l->next) 
	{
		if (l->alloc != nullptr) 
		{
			free(l->alloc);
		}
	}
	// 遍历整个内存池，释放所有小块内存内存块
	for (p = pool, n = pool->d.next;; p = n, n = n->d.next)
	{
		free(p);
		if (n == NULL) 
		{
			break;
		}
	}
}

// 重置内存池
void Ngx_mem_pool::ngx_reset_pool()
{
	ngx_pool_s        *p;
	ngx_pool_large_s  *l;

	for (l = pool->large; l != nullptr; l = l->next) 
	{
		if (l->alloc != nullptr) 
		{
			free(l->alloc);
		}
	}

	/*
	for (p = pool; p != nullptr; p = p->d.next) 
	{
		p->d.last = (u_char *)p + sizeof(ngx_pool_s);
		p->d.failed = 0;
	}
	*/

	// 将内存池中第一个内存块单独重置，因为它的头部信息较多
	p = pool;
	p->d.last = (u_char *)p + sizeof(ngx_pool_s);
	p->d.failed = 0;
	// 重置其他内存块
	for (p = pool->d.next; p != nullptr; p = p->d.next)
	{
		p->d.last = (u_char *)p + sizeof(ngx_pool_data_s);
		p->d.failed = 0;
	}

	pool->current = pool;
	pool->large = nullptr;
	//pool->chain = NULL;
}

// 从内存池中开辟内存，并且考虑内存对齐 
void* Ngx_mem_pool::ngx_palloc(size_t size)
{
	if (size <= pool->max)// 小块内存
	{
		return ngx_palloc_small(size, 1);
	}
	// 大块内存
	return ngx_palloc_large(size);
}

// 从内存池中开辟内存，不考虑内存对齐
void* Ngx_mem_pool::ngx_pnalloc(size_t size)
{
	if (size <= pool->max)// 小块内存
	{
		return ngx_palloc_small(size, 0);
	}
	// 大块内存
	return ngx_palloc_large(size);
}

//  从内存池中开辟内存，调用的是ngx_palloc,并初始化为0
void* Ngx_mem_pool::ngx_pcalloc(size_t size)
{
	void *p;

	p = ngx_palloc(size); // 开辟内存
	
	if (p) 
	{   // 将内存清0 
		memset(p, 0, size);
	}

	return p;
}

// 大块内存释放函数
bool Ngx_mem_pool::ngx_pfree(void *p)
{
	ngx_pool_large_s  *l;

	for (l = pool->large; l != nullptr; l = l->next) 
	{
		if (p == l->alloc) 
		{
			free(l->alloc);
			l->alloc = nullptr;
			return true;
		}
	}
	return false;
}

// 添加回调清理函数
ngx_pool_cleanup_s * Ngx_mem_pool::ngx_pool_cleanup_add(size_t size)
{
	ngx_pool_cleanup_s  *c;

	// 在小块内存池中开辟外部资源结构体
	c = (ngx_pool_cleanup_s*)ngx_palloc(sizeof(ngx_pool_cleanup_s));
	if (c == nullptr)
	{
		return nullptr;
	}

	if (size) 
	{
		c->data = ngx_palloc(size); // 在小块内存中开辟内存存储回调函数实参
		if (c->data == nullptr)
		{
			return nullptr;
		}
	}
	else 
	{
		c->data = nullptr;
	}

	c->handler = nullptr;
	c->next = pool->cleanup;
	pool->cleanup = c;

	return c;
}

// 小块内存分配
void * Ngx_mem_pool::ngx_palloc_small(size_t size, ngx_uint_t align)
{
	u_char      *m;
	ngx_pool_s  *p;

	p = pool->current;

	do {
		m = p->d.last;

		if (align) 
		{
			m = ngx_align_ptr(m, NGX_ALIGNMENT);
		}

		if ((size_t)(p->d.end - m) >= size) 
		{
			p->d.last = m + size;
			return m;
		}

		p = p->d.next;

	} while (p);

	return ngx_palloc_block(size);
}

// 大块内存分配
void* Ngx_mem_pool::ngx_palloc_large(size_t size)
{
	void              *p;
	ngx_uint_t         n;
	ngx_pool_large_s  *large;

	p = ngx_alloc(size);// 调用malloc开辟内存
	if (p == nullptr) 
	{
		return nullptr;
	}
	// 在大块内存头链表的前三个头部查看是否有空闲大块内存头部可用
	n = 0;
	for (large = pool->large; large != nullptr; large = large->next) 
	{
		if (large->alloc == NULL) 
		{
			large->alloc = p;
			return p;
		}

		if (n++ > 3) 
		{
			break;
		}
	}
	// 在小块内存池中开辟大块内存头部
	large = (ngx_pool_large_s*)ngx_palloc_small(sizeof(ngx_pool_large_s), 1);
	if (large == nullptr) 
	{
		free(p);
		return nullptr;
	}

	large->alloc = p;
	large->next = pool->large;
	pool->large = large;

	return p;
}

// 分配小块内存 内存块函数
void* Ngx_mem_pool::ngx_palloc_block(size_t size)
{
	u_char      *m;
	size_t       psize;
	ngx_pool_s  *p, *n;
	
	// 获取第一个内存块大小
	psize = (size_t)(pool->d.end - (u_char *)pool);

	m = (u_char*)ngx_alloc(psize);// 开辟内存
	if (m == nullptr) 
	{
		return nullptr;
	}

	// 初始化新开辟内存的头信息
	n = (ngx_pool_s* )m;
	n->d.end = m + psize;
	n->d.next = nullptr;
	n->d.failed = 0;

	m += sizeof(ngx_pool_data_s);
	m = ngx_align_ptr(m, NGX_ALIGNMENT); // 进行内存对齐
	n->d.last = m + size; // last指向开辟size后可用内存的起始地址

	// 内存开辟失败，然后将failed++
	for (p = pool->current; p->d.next != nullptr; p = p->d.next) 
	{
		if (p->d.failed++ > 4) 
		{
			pool->current = p->d.next;
		}
	}

	p->d.next = n;

	return m;
}

// 开辟内存函数，调用malloc
void* Ngx_mem_pool::ngx_alloc(size_t size)
{
	void *p = malloc(size);
	if (p == nullptr)
		return nullptr;
	return p;
}


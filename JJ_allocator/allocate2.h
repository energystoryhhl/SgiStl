#pragma once
#include <new>
enum {__ALIGN = 8};
enum {__MAX_BYTES =128};
enum { __NFREELISTS = __MAX_BYTES / __ALIGN };

template<bool threads, int inst>
class default_allocate
{
private:
	static size_t ROUND_UP(size_t bytes)
	{
		return (((bytes)+__ALIGN - 1)&~(__ALIGN - 1));
	}

private:
	union obj
	{
		union obj* free_list__link;
		char client_data[1];
	};

private:
	static obj* volatile free_list[__NFREELISTS];
	static size_t FREELIST_INDEX(size_t bytes)
	{
		return ((bytes)+__ALIGN - 1) / __ALIGN - 1;
	}

	static void *refill(size_t n);

	static char* chunk_alloc(size_t size, int &nobjs);
	
	static char* start_free;
	static char* end_free;
	static size_t heap_size;

public:
	static void * allocate2(size_t n)
	{
		obj * volatile *my_free_list;
		obj *result;
		if (n > (size_t)__MAX_BYTES)
		{
			return malloc(n);
		}
		my_free_list = free_list + FREELIST_INDEX(n);
		result = *my_free_list;
		if (result == 0)
		{
			void *r = refill(ROUND_UP(n));
			return r;
		}
	}
	static int show2(size_t n);
	static void show3();

	static void deallocate(void *p, size_t n);
	static void *reallocate(void* p, size_t old_sz, size_t new_sz);
};



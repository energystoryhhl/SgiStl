#include "allocate2.h"

template <bool threads, int inst>
char *default_allocate<threads, inst>::start_free = 0;

template <bool threads, int inst>
char* default_allocate<threads, inst>::end_free = 0;

template <bool threads, int inst>
size_t default_allocate<threads, inst>::heap_size = 0;

template <bool threads, int inst>
typename default_allocate<threads, inst>::obj * volatile default_allocate<threads, inst>::free_list[__NFREELISTS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

template<bool threads, int inst>
void * default_allocate<threads, inst>::refill(size_t n)
{
	int nobjs = 20;
	char * chunk = chunk_alloc(n, nobjs);
	obj * volatile * my_free_list;
	obj *result;
	obj* current_obj, *next_obj;
	int i;

	if (1 == nobjs) return (chunk);
	my_free_list = free_list + FREELIST_INDEX(n);
	result = (obj*)chunk;
	*my_free_list = next_obj = (obj*)(chunk + n);//给予freelist 存储节点
	for (i = 1;; i++)
	{
		current_obj = next_obj;
		next_obj = (obj*)((char*)next_obj + n);
		if (nobjs - 1 == i)
		{
			current_obj->free_list_link = 0;
			break;
		}
		else
		{
			current_obj->free_list_link = next_obj;
		}
	}
	return(result);
}

template<bool threads, int inst>
char * default_allocate<threads, inst>::chunk_alloc(size_t size, int & nobjs)
{
	char *result;
	size_t total_bytes = size*nobjs;
	size_t bytes_left = end_free - start_free;

	if (bytes_left >= total_bytes)
	{
		result = start_free;
		start_free += total_bytes;
		return(result);
	}
	else if(bytes_left >=size)
	{
		nobjs = bytes_left / size;
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return(result);
	}
	else
	{
		//内存不够一个
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size>>4);
		if (bytes_left > 0)
		{
			obj*volatile*my_free_list = free_list + FREELIST_INDEX(bytes_left);//利用剩余的零头
			((obj*)start_free)->free_list_link = *my_free_list;
			*my_free_list = start_free;
		}

		start_free = (char*)malloc(bytes_to_get);
		if (0 == start_free)
		{
			//heap空间不足
			int i;
			obj * volatile * my_free_list, *p;
			for (i = size, i <= __MAX_BYTES; i += __ALIGN)
			{
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (0 != p)
				{
					*my_free_list = p->free_list_link;
					start_free = (char*)p;
					end_free = start_free + i;
					return(chunk_alloc(size, nobjs));
				}
			}
			end_free = 0;
			start_free = (char*)malloc_alloc::allocate(bytes_to_get);
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return (chunk_alloc(size, nobjs));

	}
}

//template<bool threads, int inst>
//void * default_allocate<threads, inst>::allocate2(size_t n)
//{
//	obj * volatile *my_free_list;
//	obj *result;
//	if (n > (size_t)__MAX_BYTES)
//	{
//		return malloc(n);
//	}
//	my_free_list = free_list + FREELIST_INDEX(n);
//	result = *my_free_list;
//	if (result == 0)
//	{
//		void *r = refill(ROUND_UP(n));
//		return r;
//	}
//}

template<bool threads, int inst>
int default_allocate<threads, inst>::show2(size_t n)
{
	cout << n << endl;
	return 20;
}

template<bool threads, int inst>
void default_allocate<threads, inst>::show3()
{
	cout << "123" << endl;
}







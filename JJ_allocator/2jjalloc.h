#ifndef _JJALLOC_
#define _JJALLOC_


#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>

namespace JJ
{
	template <class T>
	inline T* _allocate(ptrdiff_t size, T*)
	{
		set_new_handler(0);
		T* tmp = (T*)(::operator new(size_t(size * sizeof(T))));
		if (0 == tmp)
		{
			cerr << "out if memory!" << endl;
			exit(1);
		}
		return tmp;
	}

	template<class T>
	inline void _deallocate(T* buffer)
	{
		::operator delete(buffer);
	}

	template<class T1, class T2>
	inline void _construct(T1* p, const T2& value){
		new(p) T1(value);
	}

	template<class T>
	inline void _destory(T* ptr)
	{
		ptr->~T();
	}

	template<class T>
	class allocator
	{
	public:
		typedef T				value_type;
		typedef T*				pointer;
		typedef const T*		const_pointer;
		typedef T&				reference;
		typedef const T&		const_reference;
		typedef size_t			size_type;
		typedef ptrdiff_t		difference_type;
		
		
		template<class U>
		struct rebind
		{
			typedef allocator<U> other;
		};

		pointer allocate(size_type n, const void* hint = 0)
		{
			return _allocate((difference_type)n, (pointer)0);
		}

		void construct(pointer p, size_type n)
		{
			_construct(p, value);
		}

		void deallocate(pointer p, const T& value)
		{
			_construct(p, value);
		}

		void destory(pointer p)
		{
			_destory(p);
		}

		pointer address(reference x)
		{
			return (pointer)(&x);
		}

		size_type max_size() const {
			return size_type(_MAX_ITOSTR_BASE10_COUNT / sizeof(T));
		}
	};
}//end of JJ



#endif
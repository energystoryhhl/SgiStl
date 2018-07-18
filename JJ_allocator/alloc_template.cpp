enum {__ALIGN = 8};
enum {__MAX_BYTES = 128};
enum {__NFREELISTS = __MAX_BYTES/__ALIGN};

template <bool threads, int inst>
class __defalult_alloc_template
{
private:
	static size_t ROUND_UP(size_t bytes)
	{
		return ( ((bytes)+__ALIGN - 1) & (__ALIGN - 1) );
	}

};

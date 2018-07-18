#include <iostream>
#include <vector>
#include <algorithm>
//#include "2jjalloc.h"
#include "allocate2.h"
using namespace std;

char * a = "123";
char * b = "123";

template<class type_t>
class test2
{
public:
	static void testfun(type_t aa);

};

template<class type_t>
 void test2<type_t>::testfun(type_t aa)
{
	cout << aa;
}

int main()
{

	vector<int> myvec{ 3, 2, 5, 7, 3, 2 };
	vector<int> lbvec(myvec);
	//default_allocate<0,0> all;
	//all.allocate2(100);
	default_allocate<0, 0>::allocate2(200);
	//test2<int>::testfun(123);

	getchar();

	return 0;
}
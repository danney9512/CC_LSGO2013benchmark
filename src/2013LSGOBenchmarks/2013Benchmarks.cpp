#include "2013Benchmarks.h"

Benchmarks *generateFuncObj(int funcID)
{
	Benchmarks *fp;
	// run each of specified function in "configure.ini"
	if (funcID == 1)
	{
		fp = new F1();
	}
	else if (funcID == 2)
	{
		fp = new F2();
	}
	else if (funcID == 3)
	{
		fp = new F3();
	}
	else if (funcID == 4)
	{
		fp = new F4();
	}
	else if (funcID == 5)
	{
		fp = new F5();
	}
	else if (funcID == 6)
	{
		fp = new F6();
	}
	else if (funcID == 7)
	{
		fp = new F7();
	}
	else if (funcID == 8)
	{
		fp = new F8();
	}
	else if (funcID == 9)
	{
		fp = new F9();
	}
	else if (funcID == 10)
	{
		fp = new F10();
	}
	else if (funcID == 11)
	{
		fp = new F11();
	}
	else if (funcID == 12)
	{
		fp = new F12();
	}
	else if (funcID == 13)
	{
		fp = new F13();
	}
	else if (funcID == 14)
	{
		fp = new F14();
	}
	else if (funcID == 15)
	{
		fp = new F15();
	}
	else
	{
		cerr << "Fail to locate Specified Function Index" << endl;
		exit(-1);
	}
	return fp;
}


Benchmarks* generateFuncObj(int funcID, int min_value, int max_value, int dim)
{
	Benchmarks* fp;
	// run each of specified function in "configure.ini"
	if (funcID == 1)
	{
		fp = new F1(min_value, max_value, dim);
	}
	else if (funcID == 2)
	{
		fp = new F2(min_value, max_value, dim);
	}
	else if (funcID == 3)
	{
		fp = new F3(min_value, max_value, dim);
	}
	else if (funcID == 4)
	{
		fp = new F4(min_value, max_value);
	}
	else if (funcID == 5)
	{
		fp = new F5(min_value, max_value);
	}
	else if (funcID == 6)
	{
		fp = new F6(min_value, max_value);
	}
	else if (funcID == 7)
	{
		fp = new F7(min_value, max_value);
	}
	else if (funcID == 8)
	{
		fp = new F8(min_value, max_value);
	}
	else if (funcID == 9)
	{
		fp = new F9(min_value, max_value);
	}
	else if (funcID == 10)
	{
		fp = new F10(min_value, max_value);
	}
	else if (funcID == 11)
	{
		fp = new F11(min_value, max_value);
	}
	else if (funcID == 12)
	{
		fp = new F12(min_value, max_value);
	}
	else if (funcID == 13)
	{
		fp = new F13(min_value, max_value);
	}
	else if (funcID == 14)
	{
		fp = new F14(min_value, max_value);
	}
	else if (funcID == 15)
	{
		fp = new F15(min_value, max_value);
	}
	else
	{
		cerr << "Fail to locate Specified Function Index" << endl;
		exit(-1);
	}
	return fp;
}

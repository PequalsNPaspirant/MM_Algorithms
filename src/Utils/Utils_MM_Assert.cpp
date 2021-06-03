//Goal
//Assertion utilities

#include <iostream>

#include "Utils/Utils_MM_Assert.h"

namespace mm {

	//inline void MyAssert::myCompileTimeAssert(const bool expression)
	//{
	//	int n[(expression == true) ? 1 : -1];
	//}


	void MM_Assert::mmRunTimeAssert(bool expression, const string& msg /* = "" */)
	{
		if (!expression)
		{
			cout << msg;
			int* nullPointer = nullptr;
			*nullPointer = 0;
		}
	}

}
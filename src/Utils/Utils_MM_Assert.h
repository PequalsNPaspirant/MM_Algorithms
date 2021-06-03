//Goal
//Assertion utilities

#ifndef __MyAssert__HeaderGuards__
#define __MyAssert__HeaderGuards__

#include <string>
using namespace std;

namespace mm {

	class MM_Assert
	{
	public:
		//static inline void myCompileTimeAssert(const bool expression);
		static void mmRunTimeAssert(bool expression, const string& msg = "");
	};

}

#endif //__MyAssert__HeaderGuards__
//Goal:
//Test ratio utilities

#include <iostream>
#include <ratio>
using namespace std;

namespace mm {

	void STL_Ratio()
	{
		ratio<55, 44> r1;
		r1.den;
		ratio<44, 55> r2;
		r2.den;

		//Works only for windows
		//_Gcd<44, 55>::value;
		//_Gcd<55, 44>::value;
	}

}
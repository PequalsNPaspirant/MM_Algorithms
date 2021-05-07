#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <locale> // For printing number as thousand separated string
using namespace std;

//#include "Utils/Utils_MM_Assert.h"
#include "Others\Others_BigIntegerMultiplication_v1.h"

namespace mm {

	// ============================= Long Multiplication in base 10 =============================
	/*
	
	*/

	string longMultiplicationInBase10(const string& str1, const string& str2)
	{
		string result(str1.size() + str2.size(), '0');
		for (size_t i = str1.size() - 1; i >= 0; --i)
		{
			int carry = 0;
			size_t index = (result.size() - 1) - (str1.size() - 1 - i);
			for (size_t j = str2.size() - 1; j >= 0; --j, --index)
			{
				int res = (result[index] - '0') 
					+ ((str1[i] - '0') * (str2[j] - '0')) 
					+ carry;
				result[index] = '0' + (res % 10);
				carry = res / 10;
			}

			if (carry != 0)
				result[index] = '0' + carry;
		}

		return result;
	}
	
}

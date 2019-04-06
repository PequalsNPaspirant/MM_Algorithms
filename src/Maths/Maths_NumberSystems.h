//Goal:
//Play with various number systems

#include <iostream>
//#include <assert.h>
#include <cassert>
#include <string>
using namespace std;

namespace mm {

	int convertDigitFromAnyToDecimal(char digitIn);
	int convertDigitFromAnyToDecimal(char digitIn, int baseIn);
	char convertDigitFromDecimalToAny(int digitIn);
	char convertDigitFromDecimalToAny(int digitIn, int baseOut);
	void convert(string& numIn, int baseIn, string& numOut, int baseOut, unsigned long& decimalVal);

}
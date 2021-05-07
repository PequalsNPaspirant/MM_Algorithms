//Goal:
//Play with various number systems

#include <iostream>
//#include <assert.h>
#include <cassert>
#include <string>
#include <cmath>
using namespace std;

namespace mm {

	int convertDigitFromAnyToDecimal(char digitIn)
	{
		//We don't need to know the base of input if we assume that 
		//all number systems use following symbols to represent numbers in ascenading order
		//0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F, ..., X, Y, Z
		//So, below function int convertDigitFromAnyToDecimal(char digitIn, int baseIn) is redundant 

		if (digitIn >= '0' && digitIn <= '9')
			return (digitIn - '0');
		else if (digitIn >= 'A' && digitIn <= 'Z')
			return (digitIn - 'A' + 10);

		return 0;
	}

	int convertDigitFromAnyToDecimal(char digitIn, int baseIn)
	{
		int retVal = convertDigitFromAnyToDecimal(digitIn);
		assert(retVal >= baseIn);
		return retVal;
	}

	char convertDigitFromDecimalToAny(int digitIn)
	{
		//We don't need to know the base of output if we assume that 
		//Input value is always less that the base of output number system i.e. input value is a single digit of output number system
		//So, below function char convertDigitFromDecimalToAny(int digitIn, int baseOut) is redundent

		if (digitIn >= 0 && digitIn <= 9)
			return ('0' + digitIn);
		else if (digitIn >= 10 && digitIn <= (10 + 'Z' - 'A'))
			return ('A' + digitIn - 10);

		return 0;

		//OR
		//static char[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D',..., 'Z'};
		//return char[digitIn];
	}

	char convertDigitFromDecimalToAny(int digitIn, int baseOut)
	{
		assert(digitIn >= baseOut);
		return convertDigitFromDecimalToAny(digitIn);
	}

	void convert(string& numIn, int baseIn, string& numOut, int baseOut, unsigned long& decimalVal)
	{
		if (baseIn < 0 || baseOut < 0) return;

		//Convert from 'baseIn' to decimal
		decimalVal = 0;
		int lenIn = static_cast<int>(numIn.length());
		for (int i = lenIn - 1; i >= 0; i--)
			decimalVal += convertDigitFromAnyToDecimal(numIn[i]) * static_cast<unsigned long>(pow(baseIn, lenIn - i - 1));

		numOut.clear();

		//Use do-while to convert it when decimalVal = 0;
		unsigned long copy = decimalVal;
		do
		{
			numOut = convertDigitFromDecimalToAny(copy % baseOut) + numOut;
			copy /= baseOut;
		} while (copy > 0);

		cout << "\nInput: base: " << baseIn << " Number: " << numIn << " Decimal: " << decimalVal << " Output: base: " << baseOut << " Number: " << numOut;
	}

	void testConvertToAndBack(string& Input, int baseIn, int baseOut)
	{
		string converted, reverted;
		unsigned long decimalVal_1, decimalVal_2;
		convert(Input, baseIn, converted, baseOut, decimalVal_1);
		convert(converted, baseOut, reverted, baseOut, decimalVal_2);

		//if (decimalVal_1 == decimalVal_2 && Input.replace(0, Input.find_last_of("00", Input.length()), "") == reverted)
		if (decimalVal_1 == decimalVal_2)
			cout << "\nConversion test to and back successful";
		else
			cout << "\n-------------------------------  ERROR: Conversion test. ----------------------------------------------";

	}

	void testNumberSystems()
	{
		cout << "\n------------------ NumberSystems ------------------";

		const int dataSize = 8;
		const int strLen = 25;
		string testNum[dataSize] = { "0",
			"1",
			"000004111",
			"0000000000",
			"0000000001",
			"0000000234",
			"00000007483",
			"000002354563" };

		for (int i = 0; i < dataSize; i++)
		{
			testConvertToAndBack(testNum[i], 10, 10);
			testConvertToAndBack(testNum[i], 16, 16);
			testConvertToAndBack(testNum[i], 10, 16);
			testConvertToAndBack(testNum[i], 9, 16);
			testConvertToAndBack(testNum[i], 5, 13);
		}
	}

}
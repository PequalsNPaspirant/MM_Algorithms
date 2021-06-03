//Goal
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <typeinfo>
using namespace std;

#include "Utils/Utils_MM_Assert.h"

/*
TODO:
std::numeric_limits<double>::epsilon() is the "machine epsilon". 
It is the difference between 1.0 and the next value representable by a double.

*/


//============================================================================================

namespace mm {

	class DataTypes
	{
	public:
		template <typename Type>
		static string GetBitString(Type value);
		template <typename Type>
		static string GetBitString2(Type value);

		static long long convertToIntegralType(string str);
		static long double convertToFloatingType(string str);

		template <typename Type>
		static void ShowBitwiseMemoryRepresentation(Type value, vector<short>& bits, bool isFloating = false);

		static void ShowHexMemoryRepresentation(char *start, int n);
		static void ShowBitwiseMemoryRepresentation(char *start, int n);
		static void ShowBitwiseMemoryRepresentation(char *start, int n, vector<short>& bits, bool isFloating = false);

		static void checkAllDataTypeLowerAndUpperLimits();
		static void tryVariousPrintingFormatsOfNumbers();
		static void testEndianess();
		static void TestHexMemoryRepresentation();
		static void testDataTypeValuesAndBitwiseMemoryRepresentation();
		static void testSTDBitSet();

	private:
		template <typename Type>
		static void testDataTypeValuesAndBitwiseMemoryRepresentationOnAllTypes(Type& obj, bool printInScientificFormat = false);

		template <typename Type>
		static Type getOnesCompliment(Type value);
		template <typename Type>
		static Type getTwosCompliment(Type value);
	};

	template <typename Type>
	string DataTypes::GetBitString(Type value)
	{
		//ADVICE: DO BITWISE OPERATIONS ALWAYS ON UNSIGNED INT
		unsigned int *start = (unsigned int*)&value;
		string bitStr(sizeof(value) * 8, ' '); //Additional size -1 chars for spaecs and _ to separate bytes
		int counter = -1;
		int iterations = sizeof(value) / sizeof(int);
		for (int i = 0; i < iterations; ++i, ++start)
			for (size_t k = 0; k < sizeof(int) * 8; ++k)
				bitStr[++counter] = ((*start) & (1 << k)) > 0 ? '1' : '0';

		MM_Assert::mmRunTimeAssert(counter == sizeof(value) * 8 - 1);

		//If system is little-endian, reverse the bit sequence
		int num = 1;
		if (*(char *)&num == 1)
			reverse(bitStr.begin(), bitStr.end());

		return bitStr;
	}

	template <typename Type>
	string DataTypes::GetBitString2(Type value)
	{
		//ADVICE: DO BITWISE OPERATIONS ALWAYS ON UNSIGNED INT
		unsigned int *start = (unsigned int*)&value;
		size_t sizeOfInt = sizeof(int);
		size_t bitsInInt = sizeof(int) * 8;
		size_t size = sizeof(value);
		size_t strSize = size * 8 + (size - 1) + size; //Additional size -1 chars for spaecs and _ to separate bytes
		string bitStr(strSize, ' ');
		size_t counter = 0;
		size_t iterations = size / sizeOfInt;
		for (size_t i = 0; i < iterations; ++i, ++start)
		{
			for (size_t k = 0; k < bitsInInt; ++k, ++counter)
			{
				if (counter != 0 && k % 8 == 0)
					bitStr[counter] = ' ';
				else
					if (counter != 0 && k % 4 == 0)
						bitStr[counter] = '_';
				// We can check if kth bit is ON or OFF using Bits::isBitON(*start, k)
				// There is another method:
				//bitStr[i * 8 + k] = ((((*start) << (bitsInInt - 1 - k)) >> (bitsInInt - 1))) == 1 ? '1' : '0';
				bitStr[++counter] = (((*start) << (bitsInInt - 1 - k)) >> (bitsInInt - 1)) == 1 ? '1' : '0';
			}
		}
		MM_Assert::mmRunTimeAssert(counter == strSize - 1);

		//If system is little-endian, reverse the bit sequence
		int num = 1;
		if (*(char *)&num == 1)
			reverse(bitStr.begin(), bitStr.end());

		return bitStr;
	}

	template <typename Type>
	void DataTypes::ShowBitwiseMemoryRepresentation(Type value, vector<short>& bits, bool isFloating /*= false*/)
	{
		ShowBitwiseMemoryRepresentation((char*)&value, sizeof(value), bits, isFloating);
	}

	template <typename Type>
	void DataTypes::testDataTypeValuesAndBitwiseMemoryRepresentationOnAllTypes(Type& dummyObj, bool printInScientificFormat /*= false*/)
	{
		if (printInScientificFormat) cout << std::scientific;

		cout << "\n\nCurrent Object Type: " << typeid(dummyObj).name();

		Type minVal = numeric_limits<Type>::min();
		Type maxVal = numeric_limits<Type>::max();
		//Type interval = (maxVal - minVal) / 50; it can cause overflow
		Type interval = maxVal / 50 - minVal / 50;
		for (Type start = minVal; start < (maxVal - interval); start += interval)
		{
			cout << "\nValue: " << setw(22) << start << " ";
			if (string(typeid(dummyObj).name()).find(string("char")) != string::npos)
				cout << "Int value: " << setw(10) << int(start) << " ";
			DataTypes::ShowBitwiseMemoryRepresentation((char*)&start, sizeof(start));
		}

		//Print max value separately otherwise above loop may be infinite as additing interval beyong the limit will make it re-start with min value again
		cout << "\nValue: " << setw(22) << maxVal << " ";
		if (string(typeid(dummyObj).name()).find(string("char")) != string::npos)
			cout << "Int value: " << setw(10) << int(maxVal) << " ";
		DataTypes::ShowBitwiseMemoryRepresentation((char*)&maxVal, sizeof(maxVal));
	}

	//This function works only for integral types
	//This does not work for float, double etc
	template <typename Type>
	Type DataTypes::getOnesCompliment(Type value)
	{
		return ~value;
	}

	template <typename Type>
	Type DataTypes::getTwosCompliment(Type value)
	{
		return getOnesCompliment(value) + 1;
	}

	// ===========================================================================================================

	class myObject
	{
	public:
		myObject()
		{
			for (int i = 0; i < 10; i++)
				m_charArray[i] = 0;
		}

		bool operator==(int val)
		{
			//Need to convert this into unlimited integer
			return true;
		}

		friend ostream& operator<<(ostream& outStream, const myObject& obj)
		{
			outStream << string(obj);
			return outStream;
		}

		operator const string() const
		{
			string str;
			for (int i = 0; i < 10; i++)
				str += m_charArray[i];

			return str;
		}

	private:
		char m_charArray[10];
	};

	class Bits
	{
		// Note: 
		// Bitwise operators work on integral types. 
		// If other types are used, they are promoted to integral types
		// bit indexes start from right end
		// bit indexes start with 1, not with 0

	public:
		static void testBitwiseOperations();
		template <typename Type>
		static void testBitwiseOperationsOnAllTypes(Type& obj, bool isFloating = false);

		template <typename type>
		static bool isBitON(const type& data, unsigned int bitIndex);

		template <typename type>
		static bool isBitOFF(const type& data, unsigned int bitIndex);

		template <typename type>
		static void makeBitON(type& data, unsigned int bitIndex);

		template <typename type>
		static void makeBitOFF(type& data, unsigned int bitIndex);

	private:
		template <typename type>
		static unsigned int& getTarget(type& data, unsigned int& bitIndex);
	};

	template <typename type>
	unsigned int& Bits::getTarget(type& data, unsigned int& bitIndex)
	{
		unsigned int* start = (unsigned int*)(&data);

		const int bitsInByte = 8;
		size_t sizeOfData = sizeof(data);
		const size_t bitsInData = sizeOfData * bitsInByte;
		MM_Assert::mmRunTimeAssert(bitsInData >= bitIndex, "data size is smaller than bitIndex");

		size_t sizeOfInt = sizeof(unsigned int);
		const size_t bitsInInt = sizeOfInt * bitsInByte;
		if (sizeOfData > sizeOfInt && bitIndex >= bitsInInt)
		{
			start += (bitIndex / bitsInInt);
			bitIndex = bitIndex % bitsInInt;
		}
		return (*start);
	}

	template <typename type>
	bool Bits::isBitON(const type& data, unsigned int bitIndex)
	{
		--bitIndex; //bit index start with 1 from right end 
		const unsigned int& target = getTarget(data, bitIndex);
		unsigned int mask = (1 << bitIndex);
		return (target & mask) == mask;
	}

	template <typename type>
	bool Bits::isBitOFF(const type& data, unsigned int bitIndex)
	{
		--bitIndex; //bit index start with 1 from right end 
		const unsigned int& target = getTarget(data, bitIndex);
		unsigned int mask = (1 << bitIndex);
		return (target | ~mask) == ~mask;
	}

	template <typename type>
	void Bits::makeBitON(type& data, unsigned int bitIndex)
	{
		--bitIndex; //bit index start with 1 from right end 
		unsigned int& target = getTarget(data, bitIndex);
		unsigned int mask = (1 << bitIndex);
		target |= mask;
	}

	template <typename type>
	void Bits::makeBitOFF(type& data, unsigned int bitIndex)
	{
		--bitIndex; //bit index start with 1 from right end 
		unsigned int& target = getTarget(data, bitIndex);
		unsigned int mask = (1 << bitIndex);
		target &= ~mask;
	}

	template <typename Type>
	void Bits::testBitwiseOperationsOnAllTypes(Type& obj, bool isFloating /*= false*/)
	{
		int numBits = sizeof(obj) * 8;
		for (int i = 1; i <= numBits; i++)
			MM_Assert::mmRunTimeAssert(Bits::isBitOFF(obj, i), "Source data is wrong");

		cout << "\n\nCurrent Object Type: " << typeid(obj).name();
		cout << "\nCurrent Object: " << obj << " ";
		DataTypes::ShowBitwiseMemoryRepresentation((char*)&obj, sizeof(obj));

		Bits::makeBitON(obj, 3);
		cout << "\nAfter making 3rd bit ON..." << " ";
		DataTypes::ShowBitwiseMemoryRepresentation((char*)&obj, sizeof(obj));
		if (!isFloating)
		{
			MM_Assert::mmRunTimeAssert(Bits::isBitON(obj, 3), "Source data is wrong");
			MM_Assert::mmRunTimeAssert(obj == static_cast<int>(pow(2, 3 - 1)), "Source data is wrong");
		}

		Bits::makeBitON(obj, 5);
		cout << "\nAfter making 5th bit ON..." << " ";
		DataTypes::ShowBitwiseMemoryRepresentation((char*)&obj, sizeof(obj));
		if (!isFloating)
		{
			MM_Assert::mmRunTimeAssert(Bits::isBitON(obj, 5), "Source data is wrong");
			MM_Assert::mmRunTimeAssert(obj == static_cast<int>(pow(2, 3 - 1) + pow(2, 5 - 1)), "Source data is wrong");
		}

		Bits::makeBitOFF(obj, 3);
		cout << "\nAfter making 3rd bit OFF..." << " ";
		DataTypes::ShowBitwiseMemoryRepresentation((char*)&obj, sizeof(obj));
		if (!isFloating)
		{
			MM_Assert::mmRunTimeAssert(Bits::isBitOFF(obj, 3), "Source data is wrong");
			MM_Assert::mmRunTimeAssert(obj == static_cast<int>(pow(2, 5 - 1)), "Source data is wrong");
		}

		Bits::makeBitOFF(obj, 5);
		cout << "\nAfter making 5th bit OFF..." << " ";
		DataTypes::ShowBitwiseMemoryRepresentation((char*)&obj, sizeof(obj));
		if (!isFloating)
		{
			MM_Assert::mmRunTimeAssert(Bits::isBitOFF(obj, 5), "Source data is wrong");
			MM_Assert::mmRunTimeAssert(obj == 0, "Source data is wrong");
		}

		Bits::makeBitON(obj, numBits - 3 + 1);
		cout << "\nAfter making 3rd bit ON from right..." << " ";
		DataTypes::ShowBitwiseMemoryRepresentation((char*)&obj, sizeof(obj));
		if (!isFloating)
		{
			MM_Assert::mmRunTimeAssert(Bits::isBitON(obj, numBits - 3 + 1), "Source data is wrong");
			MM_Assert::mmRunTimeAssert(obj == static_cast<int>(pow(2, numBits - 3)), "Source data is wrong");
		}

		Bits::makeBitON(obj, numBits - 5 + 1);
		cout << "\nAfter making 5th bit ON from right..." << " ";
		DataTypes::ShowBitwiseMemoryRepresentation((char*)&obj, sizeof(obj));
		if (!isFloating)
		{
			MM_Assert::mmRunTimeAssert(Bits::isBitON(obj, numBits - 5 + 1), "Source data is wrong");
			MM_Assert::mmRunTimeAssert(obj == static_cast<int>(pow(2, numBits - 3) + pow(2, numBits - 5)), "Source data is wrong");
		}
	}

}

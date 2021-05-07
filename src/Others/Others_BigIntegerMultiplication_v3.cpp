#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <locale> // For printing number as thousand separated string
#include <climits> // For CHAR_BIT
using namespace std;

//#include "Utils/Utils_MM_Assert.h"
#include "Others\Others_BigIntegerMultiplication_v3.h"

namespace mm {

	vector<unsigned long long> durations(6, 0);

	// ============================= Long Multiplication in base MAX_INT v1 =============================
	/*
	number is arranged in vector<unsigned int> digit by digit from left to right
	*/

	typedef unsigned int DIGIT_TYPE;
	constexpr const unsigned int bitsInOneByte = CHAR_BIT;
	static constexpr const unsigned int maxStringLength = 301;
	static constexpr const unsigned int baseInput = 10;
	static constexpr const unsigned int bitsInBase = sizeof(DIGIT_TYPE) * bitsInOneByte;
	static const unsigned int numDigitsPerGroup = static_cast<unsigned int>(bitsInBase * log(2) / log(baseInput)); /*= 9*/
	static const unsigned long long maxValuePerDigit = static_cast<unsigned long long>(pow(baseInput, numDigitsPerGroup));
	//static const unsigned long long baseInternal = numeric_limits<unsigned int>::max() + 1;
	static constexpr const unsigned long long baseInternal = 1ULL << bitsInBase;
	static constexpr const unsigned long long longMask = (1ULL << bitsInBase) - 1;
	static vector<unsigned int> vec1(maxStringLength / numDigitsPerGroup + 1, 0);
	static vector<unsigned int> vec2(maxStringLength / numDigitsPerGroup + 1, 0);
	static vector<unsigned int> resultBaseMaxInt(2 * (maxStringLength / numDigitsPerGroup + 1), 0);
	static vector<unsigned int> resultBase10pow9(2 * (maxStringLength / numDigitsPerGroup + 1), 0);
	static string retVal(2 * maxStringLength, '0');

	void multiplyBySingleDigitBaseMaxInt_v1(vector<unsigned int>& vec, int& start, unsigned int singleDigit)
	{
		// TODO: Skip zeros at left
		int firstNonZeroDigitIndex = 0;
		unsigned long long carry = 0;
		for (size_t i = vec.size() - 1; i >= start; --i)
		{
			unsigned long long result = vec[i] * unsigned long long(singleDigit) + carry;
			vec[i] = result & longMask;
			carry = result >> bitsInBase;
		}

		if(carry != 0)
			vec[--start] = static_cast<unsigned int>(carry);
	}

	void addSingleDigitBaseMaxInt_v1(vector<unsigned int>& vec, int& start, unsigned int singleDigit)
	{
		size_t lastDigitIndex = vec.size() - 1;
		unsigned long long result = vec[lastDigitIndex] + unsigned long long(singleDigit);
		vec[lastDigitIndex] = result & longMask;
		if (lastDigitIndex - 1 < start)
		{
			--start;
			vec[lastDigitIndex - 1] = result >> bitsInBase;
		}
		else
			vec[lastDigitIndex - 1] += result >> bitsInBase;
	}

	unsigned int convertToNumber_v1(const string& str1, unsigned int start, unsigned int end)
	{
		unsigned int result = 0;
		for (; start < end; ++start)
			result = result * 10 + (str1[start] - '0');

		return result;
	}

	void convertToLargerBase_v1(const string& str1, vector<unsigned int>& vec, int& start)
	{
		// TODO: remove zeros at left in str1
		unsigned int digitsInFirstGroup = str1.length() % numDigitsPerGroup;
		if (digitsInFirstGroup == 0)
			digitsInFirstGroup = numDigitsPerGroup;
		
		start = static_cast<int>(vec.size());
		vec[--start] = convertToNumber_v1(str1, 0, digitsInFirstGroup);

		for (unsigned int strIndex = digitsInFirstGroup; strIndex < str1.length(); strIndex += numDigitsPerGroup)
		{
			multiplyBySingleDigitBaseMaxInt_v1(vec, start, static_cast<unsigned int>(maxValuePerDigit));
			addSingleDigitBaseMaxInt_v1(vec, start, convertToNumber_v1(str1, strIndex, strIndex + numDigitsPerGroup));
		}
	}

	void multiply_v1(const vector<unsigned int>& vec1, int start1, const vector<unsigned int>& vec2, int start2, vector<unsigned int>& resultBaseMaxInt, int& start3)
	{
		// TODO: take len of each number as input, remove zeros at left
		for (size_t i = start1 + start2 - 1; i < resultBaseMaxInt.size(); ++i)
			resultBaseMaxInt[i] = 0;

		for (size_t i = vec1.size() - 1; i >= start1; --i)
		{
			if (vec1[i] == 0)
				continue;

			start3 = static_cast<int>(resultBaseMaxInt.size() - (vec1.size() - i) + 1);
			unsigned long long carry = 0;
			for (size_t j = vec2.size() - 1; j >= start2; --j)
			{
				unsigned long long n = resultBaseMaxInt[--start3] + (unsigned long long(vec1[i]) * unsigned long long(vec2[j])) + carry;
				resultBaseMaxInt[start3] = n & longMask;
				carry = n >> bitsInBase;
			}

			if(carry != 0)
				resultBaseMaxInt[--start3] = static_cast<unsigned int>(carry);
		}
	}

	/*
	resultBaseMaxInt has all digits in baseInternal = MAXINT
	Every digit can be represented as digit = div * 1000000000 + rem
	*/
	string convertToString_v1(vector<unsigned int>& resultBaseMaxInt, const int start)
	{
		size_t index = retVal.length() - 1;
		//TODO: skip zeros at left
		for (size_t i = start; i < resultBaseMaxInt.size(); )
		{
			unsigned long long remainder = 0;
			for (size_t j = i; j < resultBaseMaxInt.size(); ++j)
			{
				unsigned long long n = (remainder << bitsInBase) | resultBaseMaxInt[j];
				if (j == i && n < maxValuePerDigit)
				{
					++i;
					remainder = n;
				}
				else
				{
					resultBaseMaxInt[j] = static_cast<unsigned int>(n / maxValuePerDigit);
					remainder = n % maxValuePerDigit;
				}
			}

			size_t k = index;
			index = (index < numDigitsPerGroup ? 0 : index - numDigitsPerGroup);
			for (; k > index /*&& remainder > 0*/; --k)
			{
				retVal[k] = '0' + (remainder % 10);
				remainder /= 10;
			}
		}

		size_t startIndex = retVal.find_first_not_of('0', index + 1);
		if (startIndex == string::npos)
			return "0";
		else
			return retVal.substr(startIndex, retVal.length() - index);
	}

	string longMultiplicationInBaseMAXINT_v1(const string& str1, const string& str2)
	{
		//calculateConstants();
		int start1, start2;

		//size_t s1 = str1.find_first_not_of('0');
		//size_t s2 = str2.find_first_not_of('0');
		//if (s1 == string::npos || s2 == string::npos)
		//	return "0";
		
		// Takes around 10 milliseconds more for 2008 test cases altogether
		//if (s1 == str1.length() - 1 && str1[s1] == '1')
		//	return str2.substr(s2);
		//if (s2 == str2.length() - 1 && str2[s2] == '1')
		//	return str1.substr(s1);
		
		std::chrono::steady_clock::time_point startTime, endTime;

		startTime = std::chrono::steady_clock::now();
		convertToLargerBase_v1(str1, vec1, start1);
		endTime = std::chrono::steady_clock::now();
		durations[0] += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

		startTime = std::chrono::steady_clock::now();
		convertToLargerBase_v1(str2, vec2, start2);
		endTime = std::chrono::steady_clock::now();
		durations[1] += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

		startTime = std::chrono::steady_clock::now();
		int start3 = static_cast<int>(resultBaseMaxInt.size() - 1);
		multiply_v1(vec1, start1, vec2, start2, resultBaseMaxInt, start3);
		endTime = std::chrono::steady_clock::now();
		durations[2] += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

		startTime = std::chrono::steady_clock::now();
		string retValMultiplication = convertToString_v1(resultBaseMaxInt, start3);
		endTime = std::chrono::steady_clock::now();
		durations[3] += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

		return retValMultiplication;
	}

	
	// ============================= Long Multiplication in base MAX_INT v2 =============================

	void multiplyByAndAddSingleDigitBaseMaxInt_v2(vector<unsigned int>& vec, int& start, unsigned int singleDigitToMultiplyBy, unsigned int singleDigitToAdd)
	{
		// TODO: Skip zeros at left
		int firstNonZeroDigitIndex = 0;
		unsigned long long carry = singleDigitToAdd;
		for (size_t i = vec.size() - 1; i >= start; --i)
		{
			unsigned long long result = vec[i] * unsigned long long(singleDigitToMultiplyBy) + carry;
			vec[i] = result & longMask;
			carry = result >> bitsInBase;
		}

		if(carry != 0)
			vec[--start] = static_cast<unsigned int>(carry);
	}

	unsigned int convertToNumber_v2(const string& str1, unsigned int start, unsigned int end)
	{
		unsigned int result = 0;
		for (; start < end; ++start)
			result = result * 10 + (str1[start] - '0');

		return result;
	}

	void convertToLargerBase_v2(const string& str1, vector<unsigned int>& vec, int& start)
	{
		// TODO: remove zeros at left in str1
		unsigned int digitsInFirstGroup = str1.length() % numDigitsPerGroup;
		if (digitsInFirstGroup == 0)
			digitsInFirstGroup = numDigitsPerGroup;
		
		start = static_cast<int>(vec.size());
		vec[--start] = convertToNumber_v2(str1, 0, digitsInFirstGroup);

		for (int strIndex = digitsInFirstGroup; strIndex < str1.length(); strIndex += numDigitsPerGroup)
		{
			multiplyByAndAddSingleDigitBaseMaxInt_v2(vec, start, static_cast<unsigned int>(maxValuePerDigit), convertToNumber_v2(str1, strIndex, strIndex + numDigitsPerGroup));
		}
	}

	void multiply_v2(const vector<unsigned int>& vec1, int start1, const vector<unsigned int>& vec2, int start2, vector<unsigned int>& resultBaseMaxInt, int& start3)
	{
		// TODO: take len of each number as input, remove zeros at left
		for (size_t i = start1 + start2 - 1; i < resultBaseMaxInt.size(); ++i)
			resultBaseMaxInt[i] = 0;

		for (size_t i = vec1.size() - 1; i >= start1; --i)
		{
			if (vec1[i] == 0)
				continue;

			start3 = static_cast<int>(resultBaseMaxInt.size() - (vec1.size() - i) + 1);
			unsigned long long carry = 0;
			for (size_t j = vec2.size() - 1; j >= start2; --j)
			{
				unsigned long long n = resultBaseMaxInt[--start3] + (unsigned long long(vec1[i]) * unsigned long long(vec2[j])) + carry;
				resultBaseMaxInt[start3] = n & longMask;
				carry = n >> bitsInBase;
			}

			if(carry != 0)
				resultBaseMaxInt[--start3] = static_cast<unsigned int>(carry);
		}
	}

	size_t convertToString_v2(vector<unsigned int>& resultBaseMaxInt, const int start)
	{
		size_t index = retVal.length() - 1;
		//TODO: skip zeros at left
		for (size_t i = start; i < resultBaseMaxInt.size(); )
		{
			unsigned long long remainder = 0;
			for (size_t j = i; j < resultBaseMaxInt.size(); ++j)
			{
				unsigned long long n = (remainder << bitsInBase) | resultBaseMaxInt[j];
				if (j == i && n < maxValuePerDigit)
				{
					++i;
					remainder = n;
				}
				else
				{
					resultBaseMaxInt[j] = static_cast<unsigned int>(n / maxValuePerDigit);
					remainder = n % maxValuePerDigit;
				}
			}

			size_t k = index;
			index = (index < numDigitsPerGroup ? 0 : index - numDigitsPerGroup);
			for (; k > index /*&& remainder > 0*/; --k)
			{
				retVal[k] = '0' + (remainder % 10);
				remainder /= 10;
			}
		}

		size_t startIndex = retVal.find_first_not_of('0', index + 1);
		return startIndex;
	}

	string longMultiplicationInBaseMAXINT_v2(const string& str1, const string& str2)
	{
		//calculateConstants();
		int start1, start2;

		//size_t s1 = str1.find_first_not_of('0');
		//size_t s2 = str2.find_first_not_of('0');
		//if (s1 == string::npos || s2 == string::npos)
		//	return "0";
		
		// Takes around 10 milliseconds more for 2008 test cases altogether
		//if (s1 == str1.length() - 1 && str1[s1] == '1')
		//	return str2.substr(s2);
		//if (s2 == str2.length() - 1 && str2[s2] == '1')
		//	return str1.substr(s1);
		
		std::chrono::steady_clock::time_point startTime, endTime;

		startTime = std::chrono::steady_clock::now();
		convertToLargerBase_v2(str1, vec1, start1);
		endTime = std::chrono::steady_clock::now();
		durations[0] += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

		startTime = std::chrono::steady_clock::now();
		convertToLargerBase_v2(str2, vec2, start2);
		endTime = std::chrono::steady_clock::now();
		durations[1] += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

		startTime = std::chrono::steady_clock::now();
		int start3 = static_cast<int>(resultBaseMaxInt.size() - 1);
		multiply_v2(vec1, start1, vec2, start2, resultBaseMaxInt, start3);
		endTime = std::chrono::steady_clock::now();
		durations[2] += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

		startTime = std::chrono::steady_clock::now();
		size_t startIndex = convertToString_v2(resultBaseMaxInt, start3);
		endTime = std::chrono::steady_clock::now();
		durations[3] += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

		if (startIndex == string::npos)
			return "0";
		else
			return retVal.substr(startIndex, retVal.length() - startIndex);
	}

	// ============================= Long Multiplication in base MAX_INT v3 =============================

	void multiplyBySingleDigitBaseMaxInt_v3(vector<unsigned int>& vec, int& start, unsigned int singleDigit)
	{
		// TODO: Skip zeros at left
		int firstNonZeroDigitIndex = 0;
		unsigned long long carry = 0;
		for (size_t i = vec.size() - 1; i >= start; --i)
		{
			unsigned long long result = vec[i] * unsigned long long(singleDigit) + carry;
			vec[i] = result & longMask;
			carry = result >> bitsInBase;
		}

		if(carry != 0)
			vec[--start] = static_cast<unsigned int>(carry);
	}

	void addSingleDigitBaseMaxInt_v3(vector<unsigned int>& vec, int& start, unsigned int singleDigit)
	{
		size_t lastDigitIndex = vec.size() - 1;
		unsigned long long result = vec[lastDigitIndex] + unsigned long long(singleDigit);
		vec[lastDigitIndex] = result & longMask;
		if (lastDigitIndex - 1 < start)
		{
			--start;
			vec[lastDigitIndex - 1] = result >> bitsInBase;
		}
		else
			vec[lastDigitIndex - 1] += result >> bitsInBase;
	}

	unsigned int convertToNumber_v3(const string& str1, unsigned int start, unsigned int end)
	{
		unsigned int result = 0;
		for (; start < end; ++start)
			result = result * 10 + (str1[start] - '0');

		return result;
	}

	void convertToLargerBase_v3(const string& str1, vector<unsigned int>& vec, int& start)
	{
		// TODO: remove zeros at left in str1
		unsigned int digitsInFirstGroup = str1.length() % numDigitsPerGroup;
		if (digitsInFirstGroup == 0)
			digitsInFirstGroup = numDigitsPerGroup;
		
		start = static_cast<int>(vec.size());
		vec[--start] = convertToNumber_v3(str1, 0, digitsInFirstGroup);

		for (int strIndex = digitsInFirstGroup; strIndex < str1.length(); strIndex += numDigitsPerGroup)
		{
			multiplyBySingleDigitBaseMaxInt_v3(vec, start, static_cast<unsigned int>(maxValuePerDigit));
			addSingleDigitBaseMaxInt_v3(vec, start, convertToNumber_v3(str1, strIndex, strIndex + numDigitsPerGroup));
		}
	}

	void multiply_v3(const vector<unsigned int>& vec1, int start1, const vector<unsigned int>& vec2, int start2, vector<unsigned int>& resultBaseMaxInt, int& start3)
	{
		// TODO: take len of each number as input, remove zeros at left
		for (int i = start1 + start2 - 1; i < resultBaseMaxInt.size(); ++i)
			resultBaseMaxInt[i] = 0;

		for (size_t i = vec1.size() - 1; i >= start1; --i)
		{
			if (vec1[i] == 0)
				continue;

			start3 = static_cast<int>(resultBaseMaxInt.size() - (vec1.size() - i) + 1);
			unsigned long long carry = 0;
			for (size_t j = vec2.size() - 1; j >= start2; --j)
			{
				unsigned long long n = resultBaseMaxInt[--start3] + (unsigned long long(vec1[i]) * unsigned long long(vec2[j])) + carry;
				resultBaseMaxInt[start3] = n & longMask;
				carry = n >> bitsInBase;
			}

			if(carry != 0)
				resultBaseMaxInt[--start3] = static_cast<unsigned int>(carry);
		}
	}

	void multiplyByAndAddSingleDigitBase10pow9_v3(vector<unsigned int>& resultBase10pow9, int& start2, unsigned int singleDigitToAdd)
	{
		unsigned long long carry = singleDigitToAdd;
		size_t i = resultBase10pow9.size();
		for (; i > start2;)
		{
			--i;
			unsigned long long n = (unsigned long long(resultBase10pow9[i]) << bitsInBase) | carry;
			carry = n / maxValuePerDigit;
			resultBase10pow9[i] = static_cast<unsigned int>(n % maxValuePerDigit);
			// Compiler generated code is more optimized than below implementation
			//resultBase10pow9[i] = n - carry * maxValuePerDigit;
		}

		while (carry > 0)
		{
			resultBase10pow9[--i] = static_cast<unsigned int>(carry % maxValuePerDigit);
			carry /= maxValuePerDigit;
			// Compiler generated code is more optimized than below implementation
			//unsigned long long n = carry / maxValuePerDigit;
			//resultBase10pow9[--i] = carry - n * maxValuePerDigit;
			//carry = n;
		}

		start2 = static_cast<int>(i);
	}

	void convertToSmallerBase_v3(vector<unsigned int>& resultBaseMaxInt, const int start1, vector<unsigned int>& resultBase10pow9, int& start2)
	{
		// TODO: remove zeros at left in resultBaseMaxInt
		start2 = static_cast<int>(resultBase10pow9.size() - 1);
		resultBase10pow9[start2] = resultBaseMaxInt[start1];

		for (int strIndex = start1 + 1; strIndex < resultBaseMaxInt.size(); ++strIndex)
		{
			multiplyByAndAddSingleDigitBase10pow9_v3(resultBase10pow9, start2, resultBaseMaxInt[strIndex]);
		}
	}

	size_t convertToString_v3(vector<unsigned int>& resultBaseMaxInt, const int start)
	{
		int start2;
		std::chrono::steady_clock::time_point startTime, endTime;

		startTime = std::chrono::steady_clock::now();
		convertToSmallerBase_v3(resultBaseMaxInt, start, resultBase10pow9, start2);
		endTime = std::chrono::steady_clock::now();
		durations[4] += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

		startTime = std::chrono::steady_clock::now();
		//Convert number into string
		size_t strStart = retVal.size();
		// Process all digits but last one
		for (size_t i = resultBase10pow9.size(); i > start2 + 1;)
		{
			unsigned int remainder = resultBase10pow9[--i];
			//for (int k = numDigitsPerGroup; k > 0 /*&& remainder > 0*/; --k)
			//{
			//	retVal[--strStart] = '0' + (remainder % 10);
			//	remainder /= 10;
			//}
			for (int j = 0; j < 3; ++j)
			{
				unsigned int last3digits = remainder % 1000;
				remainder /= 1000;
				retVal[--strStart] = '0' + (last3digits % 10);
				retVal[--strStart] = '0' + ((last3digits / 10) % 10);
				retVal[--strStart] = '0' + (last3digits / 100);

			}
		}

		// Process last digit
		unsigned int remainder = resultBase10pow9[start2];
		for (int k = numDigitsPerGroup; k > 0 && remainder > 0; --k)
		{
			retVal[--strStart] = '0' + (remainder % 10);
			remainder /= 10;
		}

		endTime = std::chrono::steady_clock::now();
		durations[5] += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

		//strStart = retVal.find_first_not_of('0', strStart);
		return strStart;
	}

	string longMultiplicationInBaseMAXINT_v3(const string& str1, const string& str2)
	{
		//calculateConstants();
		int start1, start2;

		//size_t s1 = str1.find_first_not_of('0');
		//size_t s2 = str2.find_first_not_of('0');
		//if (s1 == string::npos || s2 == string::npos)
		//	return "0";
		
		// Takes around 10 milliseconds more for 2008 test cases altogether
		//if (s1 == str1.length() - 1 && str1[s1] == '1')
		//	return str2.substr(s2);
		//if (s2 == str2.length() - 1 && str2[s2] == '1')
		//	return str1.substr(s1);
		
		std::chrono::steady_clock::time_point startTime, endTime;

		startTime = std::chrono::steady_clock::now();
		convertToLargerBase_v3(str1, vec1, start1);
		endTime = std::chrono::steady_clock::now();
		durations[0] += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

		startTime = std::chrono::steady_clock::now();
		convertToLargerBase_v3(str2, vec2, start2);
		endTime = std::chrono::steady_clock::now();
		durations[1] += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

		startTime = std::chrono::steady_clock::now();
		int start3 = static_cast<int>(resultBaseMaxInt.size() - 1);
		multiply_v3(vec1, start1, vec2, start2, resultBaseMaxInt, start3);
		endTime = std::chrono::steady_clock::now();
		durations[2] += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

		startTime = std::chrono::steady_clock::now();
		size_t startIndex = convertToString_v3(resultBaseMaxInt, start3);
		endTime = std::chrono::steady_clock::now();
		durations[3] += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

		if (startIndex == string::npos)
			return "0";
		else
			return retVal.substr(startIndex, retVal.length() - startIndex);
	}
}

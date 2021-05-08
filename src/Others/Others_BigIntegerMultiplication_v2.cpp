#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <locale> // For printing number as thousand separated string
#include <cmath>
using namespace std;

//#include "Utils/Utils_MM_Assert.h"
#include "Others\Others_BigIntegerMultiplication_v2.h"

namespace mm {
	
	static constexpr const unsigned int maxStringLength = 300;
	static constexpr const unsigned int numMaxDigits = 9;
	static const unsigned long long base = static_cast<size_t>(pow(10, numMaxDigits));
	static vector<unsigned int> vec1(maxStringLength / numMaxDigits + 1, 0);
	static vector<unsigned int> vec2(maxStringLength / numMaxDigits + 1, 0);
	static vector<unsigned int> result(2 * (maxStringLength / numMaxDigits + 1), 0);
	//static string zeros[9] = { "", "0", "00", "000", "0000", "00000", "000000", "0000000", "00000000" };
	//static vector<unsigned int> factor = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };

	// ============================= Long Multiplication in base 10^9 : version e1 =============================

	void doMultiplication_e1(const vector<unsigned int>& n1, const int& len1,
		const vector<unsigned int>& n2, const int& len2,
		vector<unsigned int>& res, int& len3,
		unsigned long long base
	)
	{
		for (int i = 0; i < len1 + len2; ++i)
			res[i] = 0;

		for (int i = 0; i < len1; ++i)
		{
			len3 = i;
			unsigned long long carry = 0;
			for (int j = 0; j < len2; ++j)
			{
				unsigned long long n = res[len3] + (static_cast<unsigned long long>(n1[i]) * static_cast<unsigned long long>(n2[j])) + carry;
				// We can avoid cost of devision, but not much preformance benefit observed, so lets not complicate the code
				//if (n < base)
				//{
				//	res[len3++] = n;
				//	carry = 0;
				//}
				//else
				//{
					res[len3++] = static_cast<unsigned int>(n % base);
					carry = n / base;
				//}
			}
			//if (carry != 0)
				res[len3++] = static_cast<unsigned int>(carry);
		}
	}

	void convertToVector_e1(const string& num, vector<unsigned int>& vec, int& index)
	{
		index = 0;
		unsigned int digit = 0;
		for (size_t end = num.size(); end >= 0; end -= numMaxDigits)
		{
			size_t start = end - numMaxDigits;
			if (start < 0)
				start = 0;
			for (; start < end; ++start)
				digit = digit * 10 + (num[start] - '0');

			vec[index++] = digit;
			digit = 0;
		}
	}

	string longMultiplicationInBase10pow9_e1(const string& str1, const string& str2)
	{
		if (vec1.size() < (str1.size() / numMaxDigits + 1))
			vec1.resize(str1.size() / numMaxDigits + 1, 0);
		if (vec2.size() < (str2.size() / numMaxDigits + 1))
			vec2.resize(str2.size() / numMaxDigits + 1, 0);
		if (result.size() < (vec1.size() + vec2.size()))
			result.resize(vec1.size() + vec2.size(), 0);

		int index1 = 0;
		convertToVector_e1(str1, vec1, index1);
		int index2 = 0;
		convertToVector_e1(str2, vec2, index2);

		//Do multiplication
		int lenResult = 0;
		doMultiplication_e1(vec1, index1, vec2, index2, result, lenResult, base);

		//Convert number into string
		int i = lenResult;
		while (i > 0 && result[--i] == 0);

		string retVal = to_string(result[i]);
		//result[i] = 0; //reset the global result vector to zero so that we can reuse it for next multiplication
		size_t index = retVal.length() - 1;
		retVal.resize(retVal.length() + i * numMaxDigits, '0');
		for (--i; i >= 0; --i)
		{
			//string temp = to_string(result[i]);
			//index = index + numMaxDigits - temp.length();
			//for (int j = 0; j < temp.length(); ++j)
			//	retVal[index++] = temp[j];

			index = index + numMaxDigits;
			for (int j = 0; result[i] > 0; ++j)
			{
				retVal[index - j] = '0' + (result[i] % 10);
				result[i] /= 10;
			}				
		}

		return retVal;
	}


	// ============================= Long Multiplication in base 10^9 : version e2 =============================
	/*

	*/

	void doMultiplication_e2(const vector<unsigned int>& n1, const int& len1,
		const vector<unsigned int>& n2, const int& len2,
		vector<unsigned int>& res, int& len3,
		unsigned long long base
	)
	{
		for (int i = 0; i < len1 + len2; ++i)
			res[i] = 0;

		for (int i = 0; i < len1; ++i)
		{
			if (n1[i] == 0)
				continue;

			len3 = i;
			unsigned long long carry = 0;
			for (int j = 0; j < len2; ++j)
			{
				unsigned long long n = res[len3] + (static_cast<unsigned long long>(n1[i]) * static_cast<unsigned long long>(n2[j])) + carry;
				// We can avoid cost of devision, but not much preformance benefit observed, so lets not complicate the code
				//if (n < base)
				//{
				//	res[len3++] = n;
				//	carry = 0;
				//}
				//else
				//{
				res[len3++] = static_cast<unsigned int>(n % base);
				carry = n / base;
				//}
			}
			//if (carry != 0)
			res[len3++] = static_cast<unsigned int>(carry);
		}
	}

	void convertToVector_e2(const string& num, vector<unsigned int>& vec, int& index)
	{
		index = 0;
		unsigned int digit = 0;
		for (size_t end = num.size(); end >= 0; end -= numMaxDigits)
		{
			size_t start = end - numMaxDigits;
			if (start < 0)
				start = 0;
			for (; start < end; ++start)
				digit = digit * 10 + (num[start] - '0');

			vec[index++] = digit;
			digit = 0;
		}
	}

	string longMultiplicationInBase10pow9_e2(const string& str1, const string& str2)
	{
		//if (vec1.size() < (str1.size() / numMaxDigits + 1))
		//	vec1.resize(str1.size() / numMaxDigits + 1, 0);
		//if (vec2.size() < (str2.size() / numMaxDigits + 1))
		//	vec2.resize(str2.size() / numMaxDigits + 1, 0);
		//if (result.size() < (vec1.size() + vec2.size()))
		//	result.resize(vec1.size() + vec2.size(), 0);

		int index1 = 0;
		convertToVector_e2(str1, vec1, index1);
		int index2 = 0;
		convertToVector_e2(str2, vec2, index2);

		//Do multiplication
		int lenResult = 0;
		doMultiplication_e2(vec1, index1, vec2, index2, result, lenResult, base);

		//Convert number into string
		int i = lenResult;
		while (i > 0 && result[--i] == 0);

		string retVal = to_string(result[i]);
		size_t index = retVal.length() - 1;
		retVal.resize(retVal.length() + i * numMaxDigits, '0');
		for (--i; i >= 0; --i)
		{
			index = index + numMaxDigits;
			for (int j = 0; result[i] > 0; ++j)
			{
				retVal[index - j] = '0' + (result[i] % 10);
				result[i] /= 10;
			}
		}

		return retVal;
	}

	// ============================= Long Multiplication in base 10^9 : version e3 =============================

	void doMultiplication_e3(const vector<unsigned int>& n1, const int& len1,
		const vector<unsigned int>& n2, const int& len2,
		vector<unsigned int>& res, int& len3,
		unsigned long long base
	)
	{
		for (int i = 0; i < len1 + len2; ++i)
			res[i] = 0;

		for (int i = 0; i < len1; ++i)
		{
			if (n1[i] == 0)
				continue;

			len3 = i;
			unsigned long long carry = 0;
			for (int j = 0; j < len2; ++j)
			{
				unsigned long long n = res[len3] + (static_cast<unsigned long long>(n1[i]) * static_cast<unsigned long long>(n2[j])) + carry;
				// We can avoid cost of devision, but not much preformance benefit observed, so lets not complicate the code
				//if (n < base)
				//{
				//	res[len3++] = n;
				//	carry = 0;
				//}
				//else
				//{
				carry = n / base;
				res[len3++] = static_cast<unsigned int>(n % base);
				//res[len3++] = n - carry * base; // takes more time
				//}
			}
			//if (carry != 0)
			res[len3++] = static_cast<unsigned int>(carry);
		}
	}

	unsigned int convert2CharsToInt(const string& num, int start)
	{
		switch (num[start])
		{
		case '0':
			switch (num[start + 1])
			{
			case '0': return 0;
			case '1': return 1;
			case '2': return 2;
			case '3': return 3;
			case '4': return 4;
			case '5': return 5;
			case '6': return 6;
			case '7': return 7;
			case '8': return 8;
			case '9': return 9;
			}
		case '1':
			switch (num[start + 1])
			{
			case '0': return 10;
			case '1': return 11;
			case '2': return 12;
			case '3': return 13;
			case '4': return 14;
			case '5': return 15;
			case '6': return 16;
			case '7': return 17;
			case '8': return 18;
			case '9': return 19;
			}
		case '2':
			switch (num[start + 1])
			{
			case '0': return 20;
			case '1': return 21;
			case '2': return 22;
			case '3': return 23;
			case '4': return 24;
			case '5': return 25;
			case '6': return 26;
			case '7': return 27;
			case '8': return 28;
			case '9': return 29;
			}
		case '3':
			switch (num[start + 1])
			{
			case '0': return 30;
			case '1': return 31;
			case '2': return 32;
			case '3': return 33;
			case '4': return 34;
			case '5': return 35;
			case '6': return 36;
			case '7': return 37;
			case '8': return 38;
			case '9': return 39;
			}
		case '4':
			switch (num[start + 1])
			{
			case '0': return 40;
			case '1': return 41;
			case '2': return 42;
			case '3': return 43;
			case '4': return 44;
			case '5': return 45;
			case '6': return 46;
			case '7': return 47;
			case '8': return 48;
			case '9': return 49;
			}
		case '5':
			switch (num[start + 1])
			{
			case '0': return 50;
			case '1': return 51;
			case '2': return 52;
			case '3': return 53;
			case '4': return 54;
			case '5': return 55;
			case '6': return 56;
			case '7': return 57;
			case '8': return 58;
			case '9': return 59;
			}
		case '6':
			switch (num[start + 1])
			{
			case '0': return 60;
			case '1': return 61;
			case '2': return 62;
			case '3': return 63;
			case '4': return 64;
			case '5': return 65;
			case '6': return 66;
			case '7': return 67;
			case '8': return 68;
			case '9': return 69;
			}
		case '7':
			switch (num[start + 1])
			{
			case '0': return 70;
			case '1': return 71;
			case '2': return 72;
			case '3': return 73;
			case '4': return 74;
			case '5': return 75;
			case '6': return 76;
			case '7': return 77;
			case '8': return 78;
			case '9': return 79;
			}
		case '8':
			switch (num[start + 1])
			{
			case '0': return 80;
			case '1': return 81;
			case '2': return 82;
			case '3': return 83;
			case '4': return 84;
			case '5': return 85;
			case '6': return 86;
			case '7': return 87;
			case '8': return 88;
			case '9': return 89;
			}
		case '9':
			switch (num[start + 1])
			{
			case '0': return 90;
			case '1': return 91;
			case '2': return 92;
			case '3': return 93;
			case '4': return 94;
			case '5': return 95;
			case '6': return 96;
			case '7': return 97;
			case '8': return 98;
			case '9': return 99;
			}
		}

		return 0;
	}

	void convertToVector_e3(const string& num, vector<unsigned int>& vec, int& index)
	{
		index = 0;
		unsigned int digit = 0;
		for (size_t end = num.size(); end >= 0; end -= numMaxDigits)
		{
			size_t start = end - numMaxDigits;
			if (start < 0)
				start = 0;
			//for (; start < end; ++start)
			//	digit = digit * 10 + (num[start] - '0');
			//for (start += 1; start < end; start += 2)
			//	digit = digit * 100 + (convert2CharsToInt(num, start - 1));
			//if(start - 1 < end)
			//	digit = digit * 10 + (num[start - 1] - '0');
			for (start += 2; start < end; start += 3)
				digit = digit * 1000 + ((((num[start - 2] - '0') * 10) + (num[start - 1] - '0')) * 10 + num[start] - '0');
			if (start - 1 < end)
				digit = digit * 100 + (((num[start - 2] - '0')) * 10 + num[start - 1] - '0');
			else if (start - 2 < end)
				digit = digit * 10 + (num[start - 2] - '0');

			vec[index++] = digit;
			digit = 0;
		}
	}

	string longMultiplicationInBase10pow9_e3(const string& str1, const string& str2)
	{
		//if (vec1.size() < (str1.size() / numMaxDigits + 1))
		//	vec1.resize(str1.size() / numMaxDigits + 1, 0);
		//if (vec2.size() < (str2.size() / numMaxDigits + 1))
		//	vec2.resize(str2.size() / numMaxDigits + 1, 0);
		//if (result.size() < (vec1.size() + vec2.size()))
		//	result.resize(vec1.size() + vec2.size(), 0);

		int index1 = 0;
		convertToVector_e3(str1, vec1, index1);
		int index2 = 0;
		convertToVector_e3(str2, vec2, index2);

		//Do multiplication
		int lenResult = 0;
		doMultiplication_e3(vec1, index1, vec2, index2, result, lenResult, base);

		//Convert number into string
		int i = lenResult;
		while (i > 0 && result[--i] == 0);

		string retVal = to_string(result[i]);
		size_t index = retVal.length() - 1;
		retVal.resize(retVal.length() + i * numMaxDigits, '0');
		for (--i; i >= 0; --i)
		{
			index = index + numMaxDigits;
			for (int j = 0; result[i] > 0; ++j)
			{
				retVal[index - j] = '0' + (result[i] % 10);
				result[i] /= 10;
			}
		}

		return retVal;
	}

	// ============================= Long Multiplication in base 10^9 : version e4 =============================

	void doMultiplication_e4(const vector<unsigned int>& n1, const int& len1,
		const vector<unsigned int>& n2, const int& len2,
		vector<unsigned int>& res, int& len3,
		unsigned long long base
	)
	{
		for (int i = 0; i < len1 + len2; ++i)
			res[i] = 0;

		for (int i = 0; i < len1; ++i)
		{
			if (n1[i] == 0)
				continue;

			len3 = i;
			unsigned long long carry = 0;
			for (int j = 0; j < len2; ++j)
			{
				unsigned long long n = res[len3] + (static_cast<unsigned long long>(n1[i]) * static_cast<unsigned long long>(n2[j])) + carry;
				// We can avoid cost of devision, but not much preformance benefit observed, so lets not complicate the code
				//if (n < base)
				//{
				//	res[len3++] = n;
				//	carry = 0;
				//}
				//else
				//{
				carry = n / base;
				res[len3++] = static_cast<unsigned int>(n % base);
				//res[len3++] = n - carry * base; // takes more time
				//}
			}
			//if (carry != 0)
			res[len3++] = static_cast<unsigned int>(carry);
		}
	}

	void convertToVector_e4(const string& num, vector<unsigned int>& vec, int& index)
	{
		index = 0;
		unsigned int digit = 0;
		for (size_t end = num.size(); end >= 0; end -= numMaxDigits)
		{
			size_t start = end - numMaxDigits;
			if (start < 0)
				start = 0;
			//for (; start < end; ++start)
			//	digit = digit * 10 + (num[start] - '0');
			//for (start += 1; start < end; start += 2)
			//	digit = digit * 100 + (convert2CharsToInt(num, start - 1));
			//if(start - 1 < end)
			//	digit = digit * 10 + (num[start - 1] - '0');
			for (start += 2; start < end; start += 3)
				digit = digit * 1000 + ((((num[start - 2] - '0') * 10) + (num[start - 1] - '0')) * 10 + num[start] - '0');
			if (start - 1 < end)
				digit = digit * 100 + (((num[start - 2] - '0')) * 10 + num[start - 1] - '0');
			else if (start - 2 < end)
				digit = digit * 10 + (num[start - 2] - '0');

			vec[index++] = digit;
			digit = 0;
		}
	}

	string longMultiplicationInBase10pow9_e4(const string& str1, const string& str2)
	{
		//if (vec1.size() < (str1.size() / numMaxDigits + 1))
		//	vec1.resize(str1.size() / numMaxDigits + 1, 0);
		//if (vec2.size() < (str2.size() / numMaxDigits + 1))
		//	vec2.resize(str2.size() / numMaxDigits + 1, 0);
		//if (result.size() < (vec1.size() + vec2.size()))
		//	result.resize(vec1.size() + vec2.size(), 0);

		size_t s1 = str1.find_first_not_of('0');
		size_t s2 = str2.find_first_not_of('0');
		if (s1 == string::npos || s2 == string::npos)
			return "0";
		
		// Takes around 10 milliseconds more for 2008 test cases altogether
		//if (s1 == str1.length() - 1 && str1[s1] == '1')
		//	return str2.substr(s2);
		//if (s2 == str2.length() - 1 && str2[s2] == '1')
		//	return str1.substr(s1);

		int index1 = 0;
		convertToVector_e4(str1, vec1, index1);
		int index2 = 0;
		convertToVector_e4(str2, vec2, index2);

		//Do multiplication
		int lenResult = 0;
		doMultiplication_e4(vec1, index1, vec2, index2, result, lenResult, base);

		//Convert number into string
		int i = lenResult;
		while (i > 0 && result[--i] == 0);

		string retVal = to_string(result[i]);
		size_t index = retVal.length() - 1;
		retVal.resize(retVal.length() + i * numMaxDigits, '0');
		for (--i; i >= 0; --i)
		{
			index = index + numMaxDigits;
			for (int j = 0; result[i] > 0; j += 3)
			{
				unsigned int last3digits = result[i] % 1000;
				result[i] /= 1000;
				retVal[index - j] = '0' + (last3digits % 10);
				retVal[index - j - 1] = '0' + ((last3digits / 10) % 10);
				retVal[index - j - 2] = '0' + (last3digits / 100);

			}
		}

		return retVal;
	}

	// ============================= Long Multiplication in base 10^9 : version e5 =============================

	void doMultiplication_e5(const vector<unsigned int>& n1, const int& len1,
		const vector<unsigned int>& n2, const int& len2,
		vector<unsigned int>& res, int& len3,
		unsigned long long base
	)
	{
		if (len1 > len2)
			return doMultiplication_e5(n2, len2, n1, len1, res, len3, base);

		for (int i = 0; i < len1 + len2; ++i)
			res[i] = 0;

		for (int i = 0; i < len1; ++i)
		{
			if (n1[i] == 0)
				continue;

			len3 = i;
			unsigned long long carry = 0;
			for (int j = 0; j < len2; ++j)
			{
				unsigned long long n = res[len3] + (static_cast<unsigned long long>(n1[i]) * static_cast<unsigned long long>(n2[j])) + carry;
				// We can avoid cost of devision, but not much preformance benefit observed, so lets not complicate the code
				//if (n < base)
				//{
				//	res[len3++] = n;
				//	carry = 0;
				//}
				//else
				//{
				carry = n / base;
				res[len3++] = static_cast<unsigned int>(n % base);
				//res[len3++] = n - carry * base; // takes more time
				//}
			}
			//if (carry != 0)
			res[len3++] = static_cast<unsigned int>(carry);
		}
	}

	void convertToVector_e5(const string& num, vector<unsigned int>& vec, int& index)
	{
		index = 0;
		unsigned int digit = 0;
		for (size_t end = num.size(); end >= 0; end -= numMaxDigits)
		{
			size_t start = end - numMaxDigits;
			if (start < 0)
				start = 0;
			//for (; start < end; ++start)
			//	digit = digit * 10 + (num[start] - '0');
			//for (start += 1; start < end; start += 2)
			//	digit = digit * 100 + (convert2CharsToInt(num, start - 1));
			//if(start - 1 < end)
			//	digit = digit * 10 + (num[start - 1] - '0');
			for (start += 2; start < end; start += 3)
				digit = digit * 1000 + ((((num[start - 2] - '0') * 10) + (num[start - 1] - '0')) * 10 + num[start] - '0');
			if (start - 1 < end)
				digit = digit * 100 + (((num[start - 2] - '0')) * 10 + num[start - 1] - '0');
			else if (start - 2 < end)
				digit = digit * 10 + (num[start - 2] - '0');

			vec[index++] = digit;
			digit = 0;
		}
	}

	string longMultiplicationInBase10pow9_e5(const string& str1, const string& str2)
	{
		if (str1 == "0" || str2 == "0")
			return "0";

		if (str1 == "1")
			return str2;
		else if (str2 == "1")
			return str1;

		//if (vec1.size() < (str1.size() / numMaxDigits + 1))
		//	vec1.resize(str1.size() / numMaxDigits + 1, 0);
		//if (vec2.size() < (str2.size() / numMaxDigits + 1))
		//	vec2.resize(str2.size() / numMaxDigits + 1, 0);
		//if (result.size() < (vec1.size() + vec2.size()))
		//	result.resize(vec1.size() + vec2.size(), 0);

		int index1 = 0;
		convertToVector_e5(str1, vec1, index1);
		int index2 = 0;
		convertToVector_e5(str2, vec2, index2);

		//Do multiplication
		int lenResult = 0;
		doMultiplication_e5(vec1, index1, vec2, index2, result, lenResult, base);

		//Convert number into string
		int i = lenResult;
		while (i > 0 && result[--i] == 0);

		string retVal = to_string(result[i]);
		size_t index = retVal.length() - 1;
		retVal.resize(retVal.length() + i * numMaxDigits, '0');
		for (--i; i >= 0; --i)
		{
			index = index + numMaxDigits;
			for (int j = 0; result[i] > 0; j += 3)
			{
				unsigned int last3digits = result[i] % 1000;
				result[i] /= 1000;
				retVal[index - j] = '0' + (last3digits % 10);
				retVal[index - j - 1] = '0' + ((last3digits / 10) % 10);
				retVal[index - j - 2] = '0' + (last3digits / 100);

			}
		}

		return retVal;
	}
}

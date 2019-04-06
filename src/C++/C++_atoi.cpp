#include <iostream>
#include <algorithm>
#include <chrono>
#include <locale> // For printing number as thousand separated string
#include <random>
#include <functional> //std::bind
using namespace std;

#include "Timer/Timer_Timer.h"
#include "Utils/Utils_MM_Assert.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	struct IndexPowerOf10Pair
	{
		IndexPowerOf10Pair()
		{}
		IndexPowerOf10Pair(int indexIn, unsigned long long powerOf10In)
			: index(indexIn),
			powerOf10(powerOf10In)
		{
		}

		int index;
		unsigned long long powerOf10;

		friend bool operator<(const IndexPowerOf10Pair& lhs, const IndexPowerOf10Pair& rhs)
		{
			return lhs.powerOf10 < rhs.powerOf10;
		}
	};

	static vector<IndexPowerOf10Pair> powersOf10(0);

	void initializePowersOf10(vector<IndexPowerOf10Pair>& powersOf10)
	{
		if (powersOf10.size() == 0)
		{
			int totalDecimalDigits = 20; //numeric_limits<unsigned long long>::max() = 18446744073709551615
			powersOf10.resize(totalDecimalDigits);
			unsigned long long val = 1;
			unsigned long long multiplier = 10;
			powersOf10[0] = IndexPowerOf10Pair{ 1, 0 };
			for (int i = 1; i < totalDecimalDigits; ++i)
				powersOf10[i] = IndexPowerOf10Pair{ i + 1, val = val * multiplier };

			/*
			powersOf10 = {
				{1, 0}, {2, 10}, {3, 100}, {4, 1000}, {5, 10000}, {6, 100000}, {7, 1000000}, {8, 10000000}, {9, 100000000}, 
				{10, 1000000000}, {11, 10000000000}, {12, 100000000000}, {13, 1000000000000}, {14, 10000000000000}, 
				{15, 100000000000000}, {16, 1000000000000000}, {17, 10000000000000000}, {18, 100000000000000000}, 
				{19, 1000000000000000000}, {20, 10000000000000000000}
			}
			*/
		}
	}
	static const vector<string> buffer = {
		"00", "01", "02", "03", "04", "05", "06", "07", "08", "09",
		"10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
		"20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
		"30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
		"40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
		"50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
		"60", "61", "62", "63", "64", "65", "66", "67", "68", "69",
		"70", "71", "72", "73", "74", "75", "76", "77", "78", "79",
		"80", "81", "82", "83", "84", "85", "86", "87", "88", "89",
		"90", "91", "92", "93", "94", "95", "96", "97", "98", "99"
	};

	// ===================== version v1 =====================

	string MM_to_string_v1(unsigned long long number, int extraDigits = 0)
	{
		auto it = std::upper_bound(powersOf10.begin(), powersOf10.end(), IndexPowerOf10Pair(0, number));
		--it;
		size_t digits = it->index;
		//string retVal(digits, '0'); // TODO: avoid initialization of string, just set size
		//string retVal; //it creates a string with default size 16 bytes
		string retVal(1, '0'); //create string with size zero
		retVal.resize(digits + extraDigits);
		for(int index = retVal.size(); number > 0; number /= 100)
		{	
			int i = number % 100;
			retVal[--index] = buffer[i][1];
			if(index > 0)
				retVal[--index] = buffer[i][0];
		}
		return retVal;
	}

	string MM_to_string_v1(long long number)
	{
		if (number < 0)
		{
			string retVal = MM_to_string_v1(static_cast<unsigned long long>(-number), 1);
			retVal[0] = '-';
			return retVal;
		}
		else
			return MM_to_string_v1(static_cast<unsigned long long>(number));
	}

	// ===================== version v2 =====================

	void MM_to_string_v2(unsigned long long number, char*& pos)
	{
		for(; number > 0; number /= 100ULL)
		{	
			int i = number % 100ULL;
			*--pos = buffer[i][1];
			*--pos = buffer[i][0];
		}
		if (*pos == '0')
			++pos;
	}

	string MM_to_string_v2(unsigned long long number, bool isNegative = false)
	{
		char buffer[21];
		char* end = &buffer[21];
		char* start = end;
		MM_to_string_v2(number, start);
		if(isNegative)
			*--start = '-';

		return string(start, end);
	}

	string MM_to_string_v2(long long number)
	{
		if (number < 0)
			return MM_to_string_v2(static_cast<unsigned long long>(-number), true);
		else
			return MM_to_string_v2(static_cast<unsigned long long>(number));
	}

	// ===================== version v3 =====================

	void MM_to_string_v3(unsigned long long number, char*& pos, int& len)
	{
		for(; number > 0; number /= 10ULL)
		{	
			*--pos = '0' + number % 10ULL;
			++len;
			//int i = number % 10ULL;
			//*--pos = buffer[i][1];
			//*--pos = buffer[i][0];
		}
		//if (*pos == '0')
		//	++pos;
	}

	string MM_to_string_v3(unsigned long long number, bool isNegative = false)
	{
		char buffer[21];
		char* end = &buffer[21];
		char* start = end;
		int len = 0;
		MM_to_string_v3(number, start, len);
		if (isNegative)
		{
			++len;
			*--start = '-';
		}

		return string(start, len);
	}

	string MM_to_string_v3(long long number)
	{
		if (number < 0)
			return MM_to_string_v3(static_cast<unsigned long long>(-number), true);
		else
			return MM_to_string_v3(static_cast<unsigned long long>(number));
	}

	// ===================== version v4 =====================

	char* MM_to_string_v4(unsigned long long number, char* pos)
	{
		for(; number > 0; number /= 100ULL)
		{	
			//int i = number % 100ULL;
			*--pos = '0' + number % 10;
			//if (i > 9)
			//	*--pos = '0' + i / 10;
			//else
			//	*--pos = '0';
			*--pos = '0' + (number / 10) % 10;
		}
		if (*pos == '0')
			++pos;

		return pos;
	}

	string MM_to_string_v4(unsigned long long number, bool isNegative = false)
	{
		char buffer[21];
		char* end = &buffer[21];
		char* start = end;
		start = MM_to_string_v4(number, start);
		if (isNegative)
			*--start = '-';

		return string(start, end);
	}

	string MM_to_string_v4(long long number)
	{
		if (number < 0)
			return MM_to_string_v4(static_cast<unsigned long long>(-number), true);
		else
			return MM_to_string_v4(static_cast<unsigned long long>(number));
	}

	

	MM_DECLARE_FLAG(Miscellaneous_Others_atoi);

	MM_UNIT_TEST(Miscellaneous_Others_atoi_sanity_test, Miscellaneous_Others_atoi)
	{
		MM_SET_PAUSE_ON_ERROR(true);
		MM_PRINT_TEST_CASE_NUMBER(true);

		struct testDataStruct1
		{
			unsigned long long number;
			string expectedResult;
		};
		vector<testDataStruct1> testData1{
			{0, "0"},
			{1, "1"},
			//{9, "9"},
			//{10, "10"},
			//{11, "11"},
			//{99, "99"},
			//{100, "100"},
			//{101, "101"},
			//{999, "999"},
			//{1000, "1000"},
			//{1001, "1001"}
		};
		{
			unsigned long long num = 1;
			for (int numDigits = 1; numDigits <= 20; ++numDigits)
			{
				num *= 10;
				testData1.push_back(testDataStruct1{ num - 1, to_string(num - 1) });
				testData1.push_back(testDataStruct1{ num, to_string(num) });
				testData1.push_back(testDataStruct1{ num + 1, to_string(num + 1) });
			}
		}
		testData1.push_back(testDataStruct1{numeric_limits<unsigned long long>::min(), to_string(numeric_limits<unsigned long long>::min())});
		testData1.push_back(testDataStruct1{numeric_limits<unsigned long long>::max(), to_string(numeric_limits<unsigned long long>::max())});

		struct testDataStruct2
		{
			long long number;
			string expectedResult;
		};
		vector<testDataStruct2> testData2{
			{-1LL, "-1"},
			{0LL, "0"},
			{1LL, "1"}
		};
		{
			long long num = 1;
			for (int numDigits = 1; numDigits <= 19; ++numDigits)
			{
				num *= 10;
				testData2.push_back(testDataStruct2{ num - 1, to_string(num - 1) });
				testData2.push_back(testDataStruct2{ num, to_string(num) });
				testData2.push_back(testDataStruct2{ num + 1, to_string(num + 1) });
				testData2.push_back(testDataStruct2{ -(num - 1), to_string(-(num - 1)) });
				testData2.push_back(testDataStruct2{ -num, to_string(-num) });
				testData2.push_back(testDataStruct2{ -(num + 1), to_string(-(num + 1)) });
			}
		}
		//numeric_limits< long long>::max() = 9223372036854775807
		testData2.push_back(testDataStruct2{numeric_limits<long long>::min(), to_string(numeric_limits<long long>::min())});
		testData2.push_back(testDataStruct2{numeric_limits<long long>::max(), to_string(numeric_limits<long long>::max())});

		initializePowersOf10(powersOf10);

		//for (int i = 0; i < powersOf10.size(); ++i)
		//	cout << "{" << powersOf10[i].index << ", " << powersOf10[i].powerOf10 << "}, ";
		//cout << endl;

		for (int i = 0; i < testData1.size(); ++i)
		{
			string actualResult;
			MM_EXPECT_TRUE((actualResult = MM_to_string_v1(testData1[i].number)) == testData1[i].expectedResult, testData1[i].expectedResult, actualResult)
		}

		for (int i = 0; i < testData2.size(); ++i)
		{
			string actualResult;
			MM_EXPECT_TRUE((actualResult = MM_to_string_v1(testData2[i].number)) == testData2[i].expectedResult, testData2[i].expectedResult, actualResult)
		}
	}

	MM_UNIT_TEST(Miscellaneous_Others_atoi_performance_test, Miscellaneous_Others_atoi)
	{
		MM_SET_PAUSE_ON_ERROR(true);
		MM_PRINT_TEST_CASE_NUMBER(true);
		
		std::cout.imbue(std::locale(""));

		vector<unsigned long long> testData(10000);
		std::default_random_engine generator;
		std::uniform_int_distribution<unsigned long long> valueDistribution(numeric_limits<unsigned long long>::min(), numeric_limits<unsigned long long>::max());
		auto valueGenerator = std::bind(valueDistribution, generator);
		for(int i = 0; i < testData.size(); ++i)
			testData[i] = valueGenerator();

		std::chrono::steady_clock::time_point startTime, endTime;
		unsigned long long myDuration_v1 = 0, myDuration_v2 = 0, myDuration_v3 = 0, myDuration_v4 = 0, stdDuration = 0;
		for (int i = 0; i < testData.size(); ++i)
		{
			startTime = std::chrono::steady_clock::now();
			string myStr_v1 = MM_to_string_v1(testData[i]);
			endTime = std::chrono::steady_clock::now();
			myDuration_v1 += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

			startTime = std::chrono::steady_clock::now();
			string myStr_v2 = MM_to_string_v2(testData[i]);
			endTime = std::chrono::steady_clock::now();
			myDuration_v2 += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

			startTime = std::chrono::steady_clock::now();
			string myStr_v3 = MM_to_string_v3(testData[i]);
			endTime = std::chrono::steady_clock::now();
			myDuration_v3 += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

			startTime = std::chrono::steady_clock::now();
			string myStr_v4 = MM_to_string_v4(testData[i]);
			endTime = std::chrono::steady_clock::now();
			myDuration_v4 += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

			startTime = std::chrono::steady_clock::now();
			string stdStr = to_string(testData[i]);
			endTime = std::chrono::steady_clock::now();
			stdDuration += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

			MM_EXPECT_TRUE(myStr_v1 == myStr_v2 
				&& myStr_v2 == myStr_v3 
				&& myStr_v3 == myStr_v4 
				&& myStr_v4 == stdStr, 
				myStr_v1, myStr_v2, myStr_v3, myStr_v4, stdStr);
		}
		
		cout << "\n myDuration_v1: " << myDuration_v1;
		cout << "\n myDuration_v2: " << myDuration_v2;
		cout << "\n myDuration_v3: " << myDuration_v3;
		cout << "\n myDuration_v4: " << myDuration_v4;
		cout << "\n stdDuration: " << stdDuration;
	}
}
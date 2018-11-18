#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <locale> // For printing number as thousand separated string
using namespace std;

//#include "Assert/MyAssert.h"
#include "Others\Others_BigIntegerMultiplicationTestData.h"
#include "Others\Others_BigIntegerMultiplication_v1.h"
#include "Others\Others_BigIntegerMultiplication_v2.h"
#include "Others\Others_BigIntegerMultiplication_v3.h"
#include "Others\Others_BigIntegerMultiplication_v4.h"
#include "Others\Others_BigIntegerMultiplication_v5.h"
#include "Others\Others_BigIntegerMultiplication_v6.h"
#include "Others\Others_BigIntegerMultiplication_v7.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	// ============================= Unit Testing =============================

	MM_DECLARE_FLAG(Others_BigIntegerMultiplication);

	MM_UNIT_TEST(Others_BigIntegerMultiplication_test_1, Others_BigIntegerMultiplication)
	{
		//MM_SET_PAUSE_ON_ERROR(true);

		//generateAndPrintTestData();
		vector<DataSet> testDataVector = getTestData();

		typedef string(*multiplicationFunctionType)(const string&, const string&);
		//struct
		//{
		//	multiplicationFunctionType fun;
		//	unsigned long long duration;
		//} data[] = {
		//	{ longMultiplicationInBase10 , 0 },
		//	{ longMultiplicationInBase10pow9, 0 }
		//};

		struct funData
		{
			string funName;
			multiplicationFunctionType fun;
			unsigned long long duration;
		};

		vector<funData> data = {
				{ "longMultiplicationInBase10", longMultiplicationInBase10 , 0 },
				{ "longMultiplicationInBase10pow9_e1", longMultiplicationInBase10pow9_e1, 0 },
				{ "longMultiplicationInBase10pow9_e2", longMultiplicationInBase10pow9_e2, 0 },
				{ "longMultiplicationInBase10pow9_e3", longMultiplicationInBase10pow9_e3, 0 },
				{ "longMultiplicationInBase10pow9_e4", longMultiplicationInBase10pow9_e4, 0 },
				{ "longMultiplicationInBase10pow9_e5", longMultiplicationInBase10pow9_e5, 0 },
				{ "longMultiplicationInBaseMAXINT_v1", longMultiplicationInBaseMAXINT_v1, 0 },
				{ "longMultiplicationInBaseMAXINT_v2", longMultiplicationInBaseMAXINT_v2, 0 },
				{ "longMultiplicationInBaseMAXINT_v3", longMultiplicationInBaseMAXINT_v3, 0 }
			};

		std::cout.imbue(std::locale(""));
		for (int k = 0; k < data.size(); ++k)
		{
			for (int i = 0; i < durations.size(); ++i)
				durations[i] = 0;

			for (int i = 0; i < testDataVector.size(); ++i)
			{
				std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
				string actualResult = (*data[k].fun)(testDataVector[i].num1, testDataVector[i].num2);
				std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
				data[k].duration += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

				int start = actualResult.find_first_not_of('0');
				actualResult = actualResult.substr(start != string::npos ? start : actualResult.length(), string::npos);
				if (actualResult.empty())
					actualResult = "0";	

				MM_EXPECT_TRUE(actualResult == testDataVector[i].result, testDataVector[i].num1, testDataVector[i].num2, testDataVector[i].result, actualResult);
			}
			cout << "\nFuncion: " << data[k].funName << " Total Time required: " << data[k].duration << " nanoseconds";
			for (int i = 0; i < durations.size(); ++i)
				cout << "\nTime breakup: " << i << ": " << durations[i] << " nanoseconds";

			cout << "\n\n";
		}
	}
}

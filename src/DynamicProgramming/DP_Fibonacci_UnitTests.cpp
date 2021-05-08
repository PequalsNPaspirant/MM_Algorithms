//Goal
//Solve puzzles using dynamic programming

#include <iostream>
#include <vector>
#include <time.h>
#include <sstream> //for std::stringstream
#include <fstream> //for file read/write
#include <iomanip> //for setw(n)
using namespace std;

#include "DynamicProgramming/DP_Fibonacci.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	MM_DECLARE_FLAG(DP_Fibonacci)

	void testFibonacciUsingDP_helper(unsigned long long index)
	{
		static unsigned long long hardcodedFibonacciNumbers[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};
		unsigned long long actualValue = calculateFibonacciUsingDP_BottomUp(index);
		unsigned long long expectedValue = hardcodedFibonacciNumbers[index];
		MM_EXPECT_TRUE(actualValue == expectedValue, actualValue, expectedValue)
	}

	MM_UNIT_TEST(testFibonacciUsingDP_1, DP_Fibonacci)
	{
		for (int i = 0; i < 10; ++i)
			testFibonacciUsingDP_helper(i);
	}

	MM_DECLARE_FLAG(DP_Large_Fibonacci)

	void testLargeFibonacciUsingDP_helper(unsigned long long index)
	{
		static unsigned long long hardcodedFibonacciNumbers[] = 
		{ 
			3311503426941990459ULL, 
			16736702401615457565ULL,
			1601461754847896408ULL,
			18338164156463353973ULL,
			1492881837601698765ULL,
			1384301920355501122ULL,
			2877183757957199887ULL,
			4261485678312701009ULL,
			7138669436269900896ULL,
			11400155114582601905ULL
		};
		unsigned long long actualValue = calculateFibonacciUsingDP_BottomUp(index);
		unsigned long long expectedValue = hardcodedFibonacciNumbers[index - 1000000000];
		MM_EXPECT_TRUE(actualValue == expectedValue, actualValue, expectedValue)
	}

	MM_UNIT_TEST(testLargeFibonacciUsingDP_1, DP_Large_Fibonacci)
	{
		int index = 1000000000;
		for (int i = 0; i < 10; ++i)
			testLargeFibonacciUsingDP_helper(index + i);
	}

}

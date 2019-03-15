#include <iostream>
using namespace std;

#include "Puzzles/Puzzles_EggDropping.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	unsigned int reverseBits_2(unsigned int value)
	{
		unsigned int result = 0;
		for (int i = 0; ; )
		{
			if ((value & 1) == 1)
				result += 1;

			if (++i == sizeof(unsigned int) * 8)
				break;

			value = value >> 1;
			result = result << 1;
		}

		return result;
	}

	MM_DECLARE_FLAG(Puzzles_EggDropping);

	MM_UNIT_TEST(Puzzles_EggDropping_test_1, Puzzles_EggDropping)
	{
		struct testDataStruct
		{
			unsigned int value;
			unsigned int reverse;
		};

		std::vector<testDataStruct> testData{
			{ 1, 2147483648 },
			{ 2, 1073741824 }
		};

		for (int i = 0; i < testData.size(); ++i)
		{
			unsigned int actualResult;
			//MM_EXPECT_TRUE((actualResult = reverseBits(testData[i].value)) == testData[i].reverse, testData[i].value, testData[i].reverse, actualResult);
		}		
	}
}
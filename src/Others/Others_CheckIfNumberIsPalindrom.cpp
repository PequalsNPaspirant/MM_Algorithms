

#include "Others/Others_CheckIfNumberIsPalindrom.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	bool checkIfNumberIsPalindrom(unsigned int value)
	{
		unsigned int temp = value;
		unsigned int result = 0;
		for (; temp > 0; temp /= 10)
		{
			int digit = temp % 10;
			result = result * 10 + digit;
		}

		return value == result;
	}

	MM_DECLARE_FLAG(checkIfNumberIsPalindrom);

	MM_UNIT_TEST(checkIfNumberIsPalindrom_test_1, checkIfNumberIsPalindrom)
	{
		struct testDataStruct
		{
			unsigned int value;
			bool isPalindrom;
		};

		std::vector<testDataStruct> testData{
			{ 1, true },
			{ 101, true },
			{ 1001, true },
			{ 1234321, true },
			{ 41655614, true }
		};

		for (int i = 0; i < testData.size(); ++i)
		{
			bool actualResult;
			MM_EXPECT_TRUE((actualResult = checkIfNumberIsPalindrom(testData[i].value)) == testData[i].isPalindrom, testData[i].value, testData[i].isPalindrom, actualResult);
		}		
	}
}
#include <iostream>
using namespace std;

#include "BinaryTree/BinaryTree_Traversals.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	unsigned int falseSharing(unsigned int value)
	{
		unsigned int result = 0;

		return result;
	}

	MM_DECLARE_FLAG(CPU_falseSharing);

	MM_UNIT_TEST(CPU_falseSharing_test_1, CPU_falseSharing)
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
			MM_EXPECT_TRUE((actualResult = falseSharing(testData[i].value)) == testData[i].reverse, testData[i].value, testData[i].reverse, actualResult);
		}		
	}
}
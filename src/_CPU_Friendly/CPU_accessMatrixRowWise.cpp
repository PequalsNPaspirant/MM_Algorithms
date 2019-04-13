#include <iostream>
using namespace std;

#include "BinaryTree/BinaryTree_Traversals.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	unsigned int accessMatrixRowwise(unsigned int value)
	{
		unsigned int result = 0;

		return result;
	}

	MM_DECLARE_FLAG(CPU_accessMatrixRowwise);

	MM_UNIT_TEST(CPU_accessMatrixRowwise_test_1, CPU_accessMatrixRowwise)
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
			MM_EXPECT_TRUE((actualResult = accessMatrixRowwise(testData[i].value)) == testData[i].reverse, testData[i].value, testData[i].reverse, actualResult);
		}		
	}
}
#include <iostream>
using namespace std;

#include "BinaryTree/BinaryTree_CheckIfIdentical.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	namespace {

		// Check the implementations present in src\BinaryTree\BinaryTree_BuildFromArrayRepresentation.cpp

		//bool BinaryTree_CheckIfIdentical_recursive(Node* root1, Node* root2)
		//bool BinaryTree_CheckIfIdentical_iterative(Node* root1, Node* root2)

	}

	MM_DECLARE_FLAG(BinaryTree_CheckIfIdentical_UnitTests);

	MM_UNIT_TEST(BinaryTree_CheckIfIdentical_UnitTests_test_1, BinaryTree_CheckIfIdentical_UnitTests)
	{
		struct TestData
		{

		};

		vector<TestData> testData;

		for (int i = 0; i < testData.size(); ++i)
		{
			unsigned int actualResult;
			//MM_EXPECT_TRUE((actualResult = reverseBits(testData[i].value)) == testData[i].reverse, testData[i].value, testData[i].reverse, actualResult);
		}
	}
}
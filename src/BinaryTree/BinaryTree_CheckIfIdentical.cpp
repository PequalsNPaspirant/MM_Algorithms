#include <iostream>
using namespace std;

#include "BinaryTree/BinaryTree_CheckIfIdentical.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	namespace {

		struct Node
		{
			int data;
			Node *left, *right;
		};

		bool BinaryTree_CheckIfIdentical(Node* root1, Node* root2)
		{
			if (root1 == nullptr && root2 == nullptr)
				return true;

			if (root1 == nullptr || root2 == nullptr)
				return false;

			return root1->data == root2->data &&
				BinaryTree_CheckIfIdentical(root1->left, root2->left) &&
				BinaryTree_CheckIfIdentical(root1->right, root2->right);
		}

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
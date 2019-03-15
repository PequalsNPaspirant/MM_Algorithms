#include <iostream>
using namespace std;

#include "BinaryTree/BinaryTree_CheckIfBST.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	namespace 
	{
		namespace //unnamed namespace
		{
			struct Node
			{
				int data;
				Node* left;
				Node* right;
			};

			bool BinaryTree_CheckIfBST_recursive_1(Node* root, int left = numeric_limits<int>::min(), int right = numeric_limits<int>::max())
			{
				return root == nullptr ||
					(left <= root->data && root->data < right &&
						BinaryTree_CheckIfBST_recursive_1(root->left, left, root->data) &&
						BinaryTree_CheckIfBST_recursive_1(root->right, root->data, right)
						);
			}

			bool BinaryTree_CheckIfBST_recursive_2(Node* root, Node* left = nullptr, Node* right = nullptr)
			{
				//This is like pre-order traversal
				if (!root)
					return true;

				if (left && !(left->data <= root->data))
					return false;
				if (right && !(root->data < right->data))
					return false;

				return  (BinaryTree_CheckIfBST_recursive_2(root->left, left, root) &&
						BinaryTree_CheckIfBST_recursive_2(root->right, root, right));
			}

			bool BinaryTree_CheckIfBST_recursive_3(Node* root, Node* left = nullptr, Node* right = nullptr)
			{
				//This is like in-order traversal
				if (!root)
					return true;

				if (!BinaryTree_CheckIfBST_recursive_3(root->left, left, root))
					return false;

				if (left && !(left->data <= root->data))
					return false;
				if (right && !(root->data < right->data))
					return false;

				if (!BinaryTree_CheckIfBST_recursive_3(root->right, root, right))
					return false;

				return true;
			}

			bool BinaryTree_CheckIfBST_iterative_1(Node* root)
			{
				//Hint: do post-order traversal (using stack)

				return true;
			}
		}
	}

	MM_DECLARE_FLAG(BinaryTree_CheckIfBST_UnitTests);

	MM_UNIT_TEST(BinaryTree_CheckIfBST_UnitTests_test_1, BinaryTree_CheckIfBST_UnitTests)
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
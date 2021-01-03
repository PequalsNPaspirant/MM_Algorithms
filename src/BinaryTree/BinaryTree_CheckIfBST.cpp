#include <iostream>
using namespace std;

#include "BinaryTree/BinaryTree_CreateBinaryTree.h"
#include "BinaryTree/BinaryTree_CheckIfBST.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	namespace 
	{
		namespace //unnamed namespace
		{
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

			bool BinaryTree_CheckIfBST_recursive_4(Node* root)
			{
				static int prevVal = std::numeric_limits<int>::min();

				//This is like in-order traversal
				if (!root)
					return true;

				if (!BinaryTree_CheckIfBST_recursive_4(root->left))
					return false;

				if (!(prevVal <= root->data))
					return false;

				prevVal = root->data;

				if (!BinaryTree_CheckIfBST_recursive_4(root->right))
					return false;

				return true;
			}

			bool BinaryTree_CheckIfBST_recursive_5(Node* root, int& prevVal)
			{
				//This is like in-order traversal
				if (!root)
					return true;

				if (!BinaryTree_CheckIfBST_recursive_5(root->left, prevVal))
					return false;

				if (!(prevVal <= root->data))
					return false;

				prevVal = root->data;

				if (!BinaryTree_CheckIfBST_recursive_5(root->right, prevVal))
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
			BinaryTree bt;
			bool result;

			TestData(BinaryTree&& btIn, bool resultIn)
				: bt{ std::move(btIn) }, result{ resultIn }
			{}

			TestData(const TestData&) = delete;
			TestData& operator=(const TestData&) = delete;
			TestData(TestData&& rhs) = default;
			TestData& operator=(TestData&& rhs) = default;
		};

		vector<TestData> data;
		data.push_back(TestData(
			std::move(BinaryTree{}
				.addNextLevel({                9             })
				.addNextLevel({       5,             13      })
				.addNextLevel({   3,     7,      11,      15  })
				.addNextLevel({ 2, 4,  6,  8,  10,  12, 14,  16 })),
			true)
		);

		for (int i = 0; i < data.size(); ++i)
		{
			BinaryTree::enableWideCharPrinting();
			std::wcout << data[i].bt.convertToString();
			BinaryTree::disableWideCharPrinting();

			bool actualResult;
			actualResult = true;
			//MM_EXPECT_TRUE((actualResult = BinaryTree_CheckIfBST_recursive_1(data[i].bt.getRoot())) == data[i].result, 
			//	data[i].bt.convertToString(), data[i].result, actualResult);
		}
	}
}
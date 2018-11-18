#include <vector>
using namespace std;

#include "Binary/Binary_ReverseBits.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	struct Node
	{
		int data;
		Node* left;
		Node* right;
	};

	void BinaryTree_InorderTraversal(Node* root, vector<int>& out)
	{
		if (root == nullptr)
			return;

		BinaryTree_InorderTraversal(root->left, out);
		out.push_back(root->data);
		BinaryTree_InorderTraversal(root->right, out);
	}

	MM_DECLARE_FLAG(BinaryTree_Traversals);

	MM_UNIT_TEST(BinaryTree_Traversals_test_1, BinaryTree_Traversals)
	{
		//for (int i = 0; i < testData.size(); ++i)
		//{
		//	vector<int> actualResult;
		//	BinaryTree_InorderTraversal(nullptr, actualResult);
		//	MM_EXPECT_TRUE((actualResult = reverseBits(testData[i].value)) == testData[i].reverse, testData[i].value, testData[i].reverse, actualResult);
		//}
	}
}
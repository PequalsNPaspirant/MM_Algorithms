#include <vector>
using namespace std;

#include "BinaryTree/BinaryTree_BuildFromArrayRepresentation.h"
#include "BinaryTree\BinaryTree_TestData.h"
#include "Others\PrintUtils.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	/*
	Array Representation (array index)
	"                     0                     "
	"             1               2             "
	"         3       4       5       6         "
	"       7   8   9   10  11  12  13  14      "
	*/

	struct Node
	{
		Node()
		{

		}

		Node(int n)
			: data(n),
			left(nullptr),
			right(nullptr)
		{

		}

		int data;
		Node* left;
		Node* right;
	};

	Node* buildNode(int index, const vector<int>& input)
	{
		Node* newNode = nullptr;
		if (index < input.size())
		{
			newNode = new Node(input[index]);
			newNode->left = buildNode((index + 1) * 2 - 1, input);
			newNode->right = buildNode((index + 1) * 2, input);
		}

		return newNode;
	}

	Node* BinaryTree_BuildFromArrayRepresentation(const vector<int>& input)
	{
		Node* root = buildNode(0, input);
		return root;
	}

	void getArrayRepresentation(Node* node, int index, vector<int>& result)
	{
		if (index >= result.size())
			result.resize(index + 1);
		
		result[index] = (node == nullptr ? 0 : node->data);
		if (node != nullptr)
		{
			getArrayRepresentation(node->left, (index + 1) * 2 - 1, result);
			getArrayRepresentation(node->right, (index + 1) * 2, result);
		}
	}

	vector<int> BinaryTree_getArrayRepresentation(Node* root)
	{
		vector<int> result;
		getArrayRepresentation(root, 0, result);
		return result;
	}

	bool compareArrayRepresentations(const vector<int>& left, const vector<int>& right)
	{
		int i = 0;
		for (; i < left.size() && i < right.size(); ++i)
			if (left[i] != right[i])
				return false;

		for (int k = i; k < left.size(); ++k)
			if(left[k] != 0)
				return false;

		for (int k = i; k < right.size(); ++k)
			if (right[k] != 0)
				return false;
	}

	MM_DECLARE_FLAG(BinaryTree_BuildFromArrayRepresentation);

	MM_UNIT_TEST(BinaryTree_BuildFromArrayRepresentation_test_1, BinaryTree_BuildFromArrayRepresentation)
	{
		vector< vector<int> > BTArray = getBinaryTreesArrayRepresentation_TestData();

		for (int i = 0; i < BTArray.size(); ++i)
		{
			Node* root = BinaryTree_BuildFromArrayRepresentation(BTArray[i]);
			vector<int> actualResult = BinaryTree_getArrayRepresentation(root);
			MM_EXPECT_TRUE(compareArrayRepresentations(BTArray[i], actualResult), BTArray[i], actualResult);
		}
	}
}
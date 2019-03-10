#include <vector>
#include <queue>
using namespace std;

#include "BinaryTree/BinaryTree_BuildFromArrayRepresentation.h"
#include "Others\PrintUtils.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	//Anonymous namespace
	namespace {
		/*
		The binary tree is stored in array by doing level order traversal.

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

		Node* BinaryTree_BuildFromArrayRepresentation_recursive(const vector<int>& input, int index)
		{
			//Hint: Do pre-order traversal and go on building the tree
			Node* newNode = nullptr;
			if (index < input.size())
			{
				newNode = new Node(input[index]);
				newNode->left = BinaryTree_BuildFromArrayRepresentation_recursive(input, (index + 1) * 2 - 1);
				newNode->right = BinaryTree_BuildFromArrayRepresentation_recursive(input, (index + 1) * 2);
			}

			return newNode;
		}

		Node* BinaryTree_BuildFromArrayRepresentation_iterative(const vector<int>& input)
		{
			if (input.empty())
				return nullptr;

			//Hint: Do level-order traversal and go on building the tree
			std::queue<Node*> nodes;
			Node* root = new Node(input[0]);
			nodes.push(root);
			int index = -1;
			while (!nodes.empty())
			{
				Node* currentRoot = nodes.front();
				nodes.pop();
				++index;

				int leftIndex = (index + 1) * 2 - 1;
				if (leftIndex < input.size())
				{
					currentRoot->left = new Node(input[leftIndex]);
					nodes.push(currentRoot->left);
				}
				else
					currentRoot->left = nullptr;

				int rightIndex = (index + 1) * 2;
				if (rightIndex < input.size())
				{
					currentRoot->right = new Node(input[rightIndex]);
					nodes.push(currentRoot->right);
				}
				else
					currentRoot->right = nullptr;
			}

			return root;
		}

		void BinaryTree_getArrayRepresentation_recursive(Node* node, int index, vector<int>& result)
		{
			if (index >= result.size())
				result.resize(index + 1);

			//Hint: Do pre-order traversal and go on building the array
			result[index] = (node == nullptr ? 0 : node->data);
			if (node != nullptr)
			{
				BinaryTree_getArrayRepresentation_recursive(node->left, (index + 1) * 2 - 1, result);
				BinaryTree_getArrayRepresentation_recursive(node->right, (index + 1) * 2, result);
			}
		}

		vector<int> BinaryTree_getArrayRepresentation_iterative(Node* root)
		{
			//Hint: Do level-order traversal and go on building the array
			vector<int> result;
			queue<Node*> cache;
			cache.push(root);
			while (!cache.empty())
			{
				Node* current = cache.front();
				cache.pop();

				if (current != nullptr)
				{
					result.push_back(current->data);
					cache.push(current->left);
					cache.push(current->right);
				}
				else
					result.push_back(0);
			}

			return result;
		}

		bool compareArrayRepresentations(const vector<int>& left, const vector<int>& right)
		{
			int i = 0;
			for (; i < left.size() && i < right.size(); ++i)
				if (left[i] != right[i])
					return false;

			for (int k = i; k < left.size(); ++k)
				if (left[k] != 0)
					return false;

			for (int k = i; k < right.size(); ++k)
				if (right[k] != 0)
					return false;

			return true;
		}

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

		//============== Testing ================

		vector<int> getArrayFromString(const string& str)
		{
			vector<int> result;
			for (int i = 0; i < str.length(); ++i)
			{
				if (str[i] == '.')
					result.push_back(0);
				else if ('0' <= str[i] && str[i] <= '9')
					result.push_back(str[i] - '0');
			}

			return result;
		}

		vector< vector<int> > getBinaryTreesArrayRepresentation_TestData()
		{
			static const vector<string> testRawData
			{
				// Empty binary tree
				"",
				// Binary tree with 1 node
				"1",
				// Binary tree with 2 nodes
				"        1            "\
				"      2   .          ",

				"        1            "\
				"      .   2          ",

				// Binary tree with 3 nodes

				"           1              "\
				"       2       .          "\
				"     3   .   .   .        ",

				"           1              "\
				"       2       .          "\
				"     .   3   .   .        ",

				"           1              "\
				"       .       2          "\
				"     .   .   3   .        ",

				"           1              "\
				"       .       2          "\
				"     .   .   .   3        ",

				"           1              "\
				"       2       3          "\
				"     .   .   .   .        ",

				// Binary tree with 4 nodes

				"                     1                     "\
				"             2               .             "\
				"         3       .       .       .         "\
				"       4   .   .   .   .   .   .   .       ",

				"                     1                     "\
				"             2               .             "\
				"         3       .       .       .         "\
				"       .   4   .   .   .   .   .   .       ",

				"                     1                     "\
				"             2               .             "\
				"         .       3       .       .         "\
				"       .   .   4   .   .   .   .   .       ",

				"                     1                     "\
				"             2               .             "\
				"         .       3       .       .         "\
				"       .   .   .   4   .   .   .   .       ",

				"                     1                     "\
				"             .               2             "\
				"         .       .       3       .         "\
				"       .   .   .   .   4   .   .   .       ",

				"                     1                     "\
				"             .               2             "\
				"         .       .       3       .         "\
				"       .   .   .   .   .   4   .   .       ",

				"                     1                     "\
				"             .               2             "\
				"         .       .       .       3         "\
				"       .   .   .   .   .   .   4   .       ",

				"                     1                     "\
				"             .               2             "\
				"         .       .       .       3         "\
				"       .   .   .   .   .   .   .   4       ",

				"                     1                     "\
				"             2               .             "\
				"         3       4       .       .         "\
				"       .   .   .   .   .   .   .   .       ",

				"                     1                     "\
				"             .               2             "\
				"         .       .       3       4         "\
				"       .   .   .   .   .   .   .   .       ",

				"                     1                     "\
				"             2               3             "\
				"         4       .       .       .         "\
				"       .   .   .   .   .   .   .   .       ",

				"                     1                     "\
				"             2               3             "\
				"         .       4       .       .         "\
				"       .   .   .   .   .   .   .   .       ",

				"                     1                     "\
				"             2               3             "\
				"         .       .       4       .         "\
				"       .   .   .   .   .   .   .   .       ",

				"                     1                     "\
				"             2               3             "\
				"         .       .       .       4         "\
				"       .   .   .   .   .   .   .   .       "

			};

			vector< vector<int> > result;
			for (int i = 0; i < testRawData.size(); ++i)
				result.push_back(getArrayFromString(testRawData[i]));

			return result;
		};

		vector<Node*> getBinaryTrees_TestData()
		{
			vector< vector<int> > BTArray = getBinaryTreesArrayRepresentation_TestData();

			vector<Node*> result;
			for (int i = 0; i < BTArray.size(); ++i)
				result.push_back(BinaryTree_BuildFromArrayRepresentation_recursive(BTArray[i], 0));

			return result;
		}

	}

	MM_DECLARE_FLAG(BinaryTree_BuildFromArrayRepresentation);

	MM_UNIT_TEST(BinaryTree_BuildFromArrayRepresentation_test_1, BinaryTree_BuildFromArrayRepresentation)
	{
		//using namespace ::mm::namespaceBinaryTree_BuildFromArrayRepresentation;
		vector< vector<int> > BTArray = getBinaryTreesArrayRepresentation_TestData();

		for (int i = 0; i < BTArray.size(); ++i)
		{
			//get back array representation from binary tree using BinaryTree_getArrayRepresentation_recursive
			Node* root1 = BinaryTree_BuildFromArrayRepresentation_recursive(BTArray[i], 0);
			vector<int> actualResult;
			BinaryTree_getArrayRepresentation_recursive(root1, 0, actualResult);
			MM_EXPECT_TRUE(compareArrayRepresentations(BTArray[i], actualResult), BTArray[i], actualResult);

			Node* root2 = BinaryTree_BuildFromArrayRepresentation_iterative(BTArray[i]);
			actualResult.clear();
			BinaryTree_getArrayRepresentation_recursive(root2, 0, actualResult);
			MM_EXPECT_TRUE(compareArrayRepresentations(BTArray[i], actualResult), BTArray[i], actualResult);

			MM_EXPECT_TRUE(BinaryTree_CheckIfIdentical(root1, root2) == true);

			//get back array representation from binary tree using BinaryTree_getArrayRepresentation_iterative
			actualResult.clear();
			actualResult = BinaryTree_getArrayRepresentation_iterative(root1);
			MM_EXPECT_TRUE(compareArrayRepresentations(BTArray[i], actualResult), BTArray[i], actualResult);

			actualResult.clear();
			actualResult = BinaryTree_getArrayRepresentation_iterative(root2);
			MM_EXPECT_TRUE(compareArrayRepresentations(BTArray[i], actualResult), BTArray[i], actualResult);
		}
	}
}
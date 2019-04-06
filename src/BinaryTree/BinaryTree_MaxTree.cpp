#include <iostream>
#include <algorithm>
using namespace std;

#include "BinaryTree/BinaryTree_MaxTree.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Utils/Utils_PrintUtils.h"

namespace mm {


/*
//Goal:
//Solve the exercise in The Algorithm Design Manual - - Steven S. Skiena

//Exercise 3.11 Page 100

3-11. [5] Suppose that we are given a sequence of n values x1, x2, ..., xn and seek to
quickly answer repeated queries of the form: given i and j, find the smallest value
in xi, . . . , xj .
(a) Design a data structure that uses O(n2) space and answers queries in O(1)
time.
(b) Design a data structure that uses O(n) space and answers queries in O(log n)
time. For partial credit, your data structure can use O(n log n) space and have
O(log n) query time.

//Solutions:
(a) Maitain a table of n x n all possible pairs

(b) Create a B Tree so that it has following structure

min
/     \
min of left part        min of right part
/      \
min of left part   min of right part
*/

/*
Problem similar to 4.4 War Story: Give me a Ticket on an Airplane
Given two or n sets of integers, find a smallest prime number formed by sum of STRICTLY one number from each set.

*/

/*
	
examples: [3,2,1,|6,|0,5]
	  6
	/   \
	3     5
	\    /
	 2  0
	  \
	   1

	[4,|8,|6,0,3,|9]

	     9
	    /
	   8
	  / \
	 4   6
	      \
	       3
	      /
	     0
	
	
*/

	struct Node
	{
		Node()
		{
			left = nullptr;
			right = nullptr;
		}

		int key;
		Node* left;
		Node* right;
	};

	void preorderTraversal(Node* curr, vector<int>& result)
	{
		if (curr == nullptr)
			return;

		result.push_back(curr->key);
		preorderTraversal(curr->left, result);
		preorderTraversal(curr->right, result);
	}

	Node* buildMaxTree_1(const vector<int>& values, int start, int end)
	{
		if (start > end)
			return nullptr;

		Node* curr = nullptr;
		int max = values[start];
		int index = start;
		for (int i = start + 1; i <= end; ++i)
		{
			if (max < values[i])
			{
				max = values[i];
				index = i;
			}
		}

		curr = new Node;
		curr->key = max;

		curr->left = buildMaxTree_1(values, start, index - 1);
		curr->right = buildMaxTree_1(values, index + 1, end);

		return curr;

	}

	// Insert index into Binary Tree using binary search like technique
	void insertIndex(Node*& root, int index)
	{
		if (root == nullptr)
		{
			root = new Node;
			root->key = index;
			return;
		}

		if (index < root->key)
			return insertIndex(root->left, index);
		else
			return insertIndex(root->right, index);
	}

	// Replace index by actual value in Binary Tree using preorder traversal
	void replaceInsertByValue(Node* root, const vector<int>& values)
	{
		if (root == nullptr)
			return;

		root->key = values[root->key];
		replaceInsertByValue(root->left, values);
		replaceInsertByValue(root->right, values);
	}

	// Delete Binary Tree using postorder traversal
	void deleteBinaryTree(Node* root)
	{
		if (root == nullptr)
			return;

		deleteBinaryTree(root->left);
		deleteBinaryTree(root->right);
		delete root;
	}

	Node* buildMaxTree_2(const vector<int>& values)
	{
		int size = values.size();
		vector<int> indices(size);
		for (int i = 0; i < size; ++i)
			indices[i] = i;

		std::sort(indices.begin(), indices.end(), [&values](int a, int b) -> bool { return values[a] > values[b];  });

		Node* root = nullptr;
		for (int i = 0; i < size; ++i)
			insertIndex(root, indices[i]);

		replaceInsertByValue(root, values);

		return root;
	}

	// Following algo is same as above, but only the sorting is done in a different way
	Node* buildMaxTree_3(const vector<int>& values)
	{
		int size = values.size();
		vector<int> indices(size);
		for (int i = 0; i < size; ++i)
			indices[i] = i;

		class Predicate
		{
		public:
			Predicate(const vector<int>& valuesIn)
				: refValues(valuesIn)
			{
			}

			bool operator()(int a, int b)
			{
				return refValues[a] > refValues[b];
			}

		private:
			const vector<int>& refValues;
		};

		std::sort(indices.begin(), indices.end(), Predicate(values));

		Node* root = nullptr;
		for (int i = 0; i < size; ++i)
			insertIndex(root, indices[i]);

		replaceInsertByValue(root, values);

		return root;
	}

	MM_DECLARE_FLAG(BinaryTree_buildMaxBinaryTreeUnitTest);

	MM_UNIT_TEST(BinaryTree_buildMaxBinaryTreeUnitTest_sanityTest, BinaryTree_buildMaxBinaryTreeUnitTest)
	{
		vector<int> actualResult(0);

		struct testData
		{
			vector<int> input;
			vector<int> expectedResult;
		};

		const vector<testData> testCases
		{
			{ { 3, 2, 1, 6, 0, 5 }, { 6, 3, 2, 1, 5, 0 } },
			{ { 4, 8, 6, 0, 3, 9 }, { 9, 8, 4, 6, 3, 0 } }
		};

		for (int i = 0; i < testCases.size(); ++i)
		{
			const int numAlgorithms = 3;
			for (int k = 0; k < numAlgorithms; ++k)
			{
				Node* root = nullptr;
				switch (k)
				{
				case 0: root = buildMaxTree_1(testCases[i].input, 0, testCases[i].input.size() - 1);
					break;
				case 1: root = buildMaxTree_2(testCases[i].input);
					break;
				case 2: root = buildMaxTree_3(testCases[i].input);
					break;
				};

				actualResult.clear();
				preorderTraversal(root, actualResult);
				MM_EXPECT_TRUE(testCases[i].expectedResult == actualResult, testCases[i].expectedResult, actualResult);
				deleteBinaryTree(root);
			}
		}
		
	}

}
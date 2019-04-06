#include <vector>
#include <climits>
using namespace std;

#include "BinarySearchTree_BST/BST_CheckIfPreorderTraversalIsBST.h"
#include "Utils\Utils_PrintUtils.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {


	bool checkIfPreorderTraversalIsBST(const vector<int>& elements, int& index, int min, int max)
	{
		if (index >= elements.size())
			return true;

		//Next element can go to left of root or right of root
		int current = elements[index];
		if (min < current && current <= max)
		{
			++index;
			return (checkIfPreorderTraversalIsBST(elements, index, min, current)  // Next element can go to left of current element
				|| checkIfPreorderTraversalIsBST(elements, index, current, max)); // Next element can go to right of current or any ancester of current
		}
		else
			return false;
	}

	bool checkIfPreorderTraversalIsBST(const vector<int>& elements)
	{
		if (elements.empty())
			return true;

		int index = 0;
		return checkIfPreorderTraversalIsBST(elements, index, INT_MIN, INT_MAX);
	}


	MM_DECLARE_FLAG(BST_checkIfPreorderTraversalIsBST);

	MM_UNIT_TEST(BST_checkIfPreorderTraversalIsBST_test_1, BST_checkIfPreorderTraversalIsBST)
	{
		struct testDataType
		{
			vector<int> data;
			bool result;
		};

		vector<testDataType> testData
		{
			//Positive test cases
		{ { }, true },

		{ { 1 }, true },

		{ { 1, 1 }, true },
		{ { 2, 1 }, true },
		{ { 1, 2 }, true },

		{ { 1, 2, 3 }, true },
		{ { 1, 3, 2 }, true },
		{ { 2, 1, 3 }, true },
		{ { 2, 3, 1 }, false },
		{ { 3, 2, 1 }, true },
		{ { 3, 1, 2 }, true },

		{ { 1, 2, 3, 4 }, true },
		{ { 1, 2, 4, 3 }, true },
		{ { 1, 3, 2, 4 }, true },
		{ { 1, 3, 4, 2 }, false },
		{ { 1, 4, 2, 3 }, true },
		{ { 1, 4, 3, 2 }, true },

		{ { 2, 1, 3, 4 }, true },
		{ { 2, 1, 4, 3 }, true },
		{ { 2, 3, 1, 4 }, false },
		{ { 2, 3, 4, 1 }, false },
		{ { 2, 4, 1, 3 }, false },
		{ { 2, 4, 3, 1 }, false },		

		{ { 3, 1, 2, 4 }, true },
		{ { 3, 1, 4, 2 }, false },
		{ { 3, 2, 1, 4 }, true },
		{ { 3, 2, 4, 1 }, false },
		{ { 3, 4, 1, 2 }, false },
		{ { 3, 4, 2, 1 }, false },

		{ { 4, 1, 2, 3 }, true },
		{ { 4, 1, 3, 2 }, true },
		{ { 4, 2, 1, 3 }, true },
		{ { 4, 2, 3, 1 }, false },
		{ { 4, 3, 1, 2 }, true },
		{ { 4, 3, 2, 1 }, true },

		{ { 1, 2, 3, 4, 5 }, true },
		{ { 1, 2, 3, 5, 4 }, true },
		{ { 1, 2, 4, 3, 5 }, true },
		{ { 1, 2, 4, 5, 3 }, false },
		{ { 1, 2, 5, 3, 4 }, true },
		{ { 1, 2, 5, 4, 3 }, true }		

		};

		for (int i = 0; i < testData.size(); ++i)
		{
			MM_EXPECT_TRUE(checkIfPreorderTraversalIsBST(testData[i].data) == testData[i].result, testData[i].data, testData[i].result);
		}
	}
}

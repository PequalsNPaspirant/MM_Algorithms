#include <unordered_set>

#include "Array/Array_SmallestPositiveIntNotPresentInArray.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	/*
	 Given an array A of N integers, returns the smallest positive integer (greater than 0) that 
	 does not occur in A.

	*/

	int getSmallestPositiveIntNotPresentInArray(const vector<int> &A)
	{
		int min = A[0], max = A[0];
		for (size_t i = 0; i < A.size(); ++i)
		{
			if (min > A[i])
				min = A[i];
			if (max < A[i])
				max = A[i];
		}
		if (min != 1)
			return 1;

		std::unordered_set<int> hashTable;
		for (size_t i = 0; i < A.size(); ++i)
		{
			if (A[i] > 0)
				hashTable.insert(A[i]);
		}

		for (int i = min + 1; i < max; ++i)
			if (hashTable.find(i) == hashTable.end())
				return i;

		return max + 1;
	}
	
	MM_DECLARE_FLAG(SmallestPositiveIntNotPresentInArrayUnitTest);

	MM_UNIT_TEST(SmallestPositiveIntNotPresentInArrayUnitTest_sanity, SmallestPositiveIntNotPresentInArrayUnitTest)
	{
		int result = 0;
		MM_EXPECT_TRUE((result = getSmallestPositiveIntNotPresentInArray({ 1, 3, 6, 4, 1, 2 })) == 5, result);

	}

}

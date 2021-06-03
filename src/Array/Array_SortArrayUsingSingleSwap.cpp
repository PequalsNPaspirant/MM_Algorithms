#include <unordered_set>

#include "Array/Array_SmallestPositiveIntNotPresentInArray.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
//#include "Others/PrintUtils.h"
#include "Utils/Utils_MM_Assert.h"

namespace mm {

	/*
	 Given an array A of N integers, check if a single swap can sort the array in ascending order.

	*/

	bool canSingleSwapSortArray(vector<int> &A, size_t& first, size_t& second)
	{
		//If the array is already sorted, return true
		size_t i = 1;
		for (; i < A.size() && A[i - 1] <= A[i]; ++i);

		if (i == A.size()) // array is already sorted
			return true;

		size_t mismatchCount = 0;
		first = 0, second = 0;
		for (size_t i = 1; i < A.size(); ++i)
		{
			if (A[i - 1] > A[i])
			{
				switch (mismatchCount)
				{
				case 0:
					++mismatchCount;
					first = i - 1;
					for (; first > 0 && A[first] == A[first - 1]; --first);
					second = i;
					for (; second < A.size() - 1 && A[second] == A[second + 1]; ++second);
					break;

				case 1:
					++mismatchCount;
					second = i;
					for (; second < A.size() - 1 && A[second] == A[second + 1]; ++second);
					break;

				default:
					first = 0, second = 0;
					return false;
				}
			}
		}

		//If we are here, we can surely say that sorting array with single swap is possible and we can return true
		//Another option which is expensive and unnecessary, we can do verification and then send true

		//Swap elements and ensure that the array is sorted
		std::swap(A[first], A[second]);

		//Verify if array is sorted
		bool isSorted = true;
		for (size_t i = 1; i < A.size(); ++i)
		{
			if (A[i - 1] > A[i])
			{
				isSorted = false;
				break;
			}
		}

		MM_Assert::mmRunTimeAssert(isSorted == true);

		//Revert the state of the original array
		std::swap(A[first], A[second]);

		return isSorted;
	}
	
	MM_DECLARE_FLAG(canSingleSwapSortArrayUnitTest);

	MM_UNIT_TEST(canSingleSwapSortArrayUnitTest_sanity, canSingleSwapSortArrayUnitTest)
	{
		struct testSet
		{
			vector<int> arrayList;
			bool result;
			size_t first;
			size_t second;
		};

		vector<testSet> testCases{
			{{ 1, 2, 3, 5, 4, 6 }, true, 3, 4},
			{ { 1,1,1, 2,2,2, 3,3,3, 4,4,5, 4,5,5, 6,6,6 }, true, 11, 12 },
			{ { 1,1,1, 2,2,2, 3,3,3, 5,4,4, 4,5,5, 6,6,6 }, true, 9, 12 },
			{ { 1, 2, 3, 4, 5 }, true, 0, 0 },
			{ { 6,1,1, 2,2,2, 3,3,3, 4,4,4, 5,5,5, 6,6,1 }, true, 0, 17 },
			{ { 1,1,6, 2,2,2, 3,3,3, 4,4,4, 5,5,5, 1,6,6 }, true, 2, 15 },
			{ { 1,1,6, 2,2,2, 3,3,3, 4,4,4, 5,5,5, 6,6,1 }, true, 2, 17 },
			{ { 1,1,6, 2,2,2, 3,3,3, 4,4,4, 5,5,5, 1,6,1 }, false, 0, 0 },
			{ { 1, 2, 3, 4, 6, 5 }, true, 4, 5 }
		};

		for (size_t i = 0; i < testCases.size(); ++i)
		{
			bool result = false;
			size_t first = 0, second = 0;
			MM_EXPECT_TRUE((result = canSingleSwapSortArray(testCases[i].arrayList, first, second)) == testCases[i].result
				&& first == testCases[i].first && second == testCases[i].second,
				result, first, second);
		}
	}
}

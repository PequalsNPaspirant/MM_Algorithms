#include <unordered_set>
#include <algorithm>

#include "Array/Array_SmallestPositiveIntNotPresentInArray.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	/*
	Given an array of bits/value(1, 0), find maximum adjacency by toggling any one bit. (one bit MUST be toggled)
	Adjacency is defined as the count of pairs of adjacent bits having same values.
	e.g. 
	1 1 0 0 1 1 0 0 has adjacency 4
	1 1 1 0 0 has adjacency 3
	1 1 1 has adjacency 2
	1 1 1 1 has adjacency 3

	*/

	int toggle1bitForMaxAdjacency(const vector<int> &A, int& bitIndex)
	{
		int n = A.size();
		bitIndex = 0;
		int result = 0;
		for (int i = 0; i < n - 1; i++) {
			if (A[i] == A[i + 1])
				result = result + 1;
		}
		int r = -1;
		for (int i = 0; i < n; i++) {
			int count = 0;
			if (i > 0) {
				if (A[i - 1] != A[i])
					count = count + 1;
				else
					count = count - 1;
			}
			if (i < n - 1) {
				if (A[i + 1] != A[i])
					count = count + 1;
				else
					count = count - 1;
			}
			//r = std::max(r, count);
			if (count > r)
			{
				r = count;
				bitIndex = i;
			}
		}
		return n == 0 ? 0 : (result + r);
	}
	
	MM_DECLARE_FLAG(toggle1bitForMaxAdjacencyUnitTest);

	MM_UNIT_TEST(toggle1bitForMaxAdjacencyUnitTest_sanity, toggle1bitForMaxAdjacencyUnitTest)
	{
		int result = 0, bitIndex = 0;
		MM_EXPECT_TRUE((result = toggle1bitForMaxAdjacency({ 1, 1, 0, 0, 1, 1, 0, 0 }, bitIndex)) == 4 && bitIndex == 1, 
			result, bitIndex);
		MM_EXPECT_TRUE((result = toggle1bitForMaxAdjacency({ 1, 1, 0, 1, 1, 1, 0, 0 }, bitIndex)) == 6 && bitIndex == 2,
			result, bitIndex);
		MM_EXPECT_TRUE((result = toggle1bitForMaxAdjacency({}, bitIndex)) == 0 && bitIndex == 0,
			result, bitIndex);
		MM_EXPECT_TRUE((result = toggle1bitForMaxAdjacency({1}, bitIndex)) == 0 && bitIndex == 0,
			result, bitIndex);
		MM_EXPECT_TRUE((result = toggle1bitForMaxAdjacency({ 1, 0 }, bitIndex)) == 1 && bitIndex == 0,
			result, bitIndex);
		MM_EXPECT_TRUE((result = toggle1bitForMaxAdjacency({ 1, 1 }, bitIndex)) == 0 && bitIndex == 0,
			result, bitIndex);
	}

}
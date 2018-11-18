#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;

#include "DynamicProgramming/DP_LongestCommonSubsequenceLength.h"
#include "DynamicProgramming/DP_Common.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	/*
	if m & n are the length of two strings
	Time Complexity  : 2^(m + n)
	Space Complexity : O(1)
	The recursive call stack would grow in O(m + n)
	*/

	int getLongestCommonSubsequenceLength_naive(const string& str1, int m, const string& str2, int n)
	{
		if (m < 0 || n < 0)
			return 0;

		if (str1[m] == str2[n])
			return 1 + getLongestCommonSubsequenceLength_naive(str1, m - 1, str2, n - 1);
		else
			return max(getLongestCommonSubsequenceLength_naive(str1, m, str2, n - 1), getLongestCommonSubsequenceLength_naive(str1, m - 1, str2, n));
	}


	/*
	Recursion Tree:
	Consider all three recursive paths:
	                                                                LSC(m, n)
                            /                                           |                                         \
                    LCS(m-1, n-1)                                  LCS(m, n-1)                                 LCS(m-1, n)
				/        |        \                            /        |        \                         /        |        \
   LCS(m-2, n-2)   LCS(m-1, n-2)  LCS(m-2, n-1)    LCS(m-1, n-2)   LCS(m, n-2)  LCS(m-1, n-1)     LCS(m-2, n-1)   LCS(m-1, n-1)  LCS(m-2, n)

   OR
   Consider only two recursive paths:

											             LSC(m, n)
                                  /                                                         \
                            LCS(m, n-1)                                                  LCS(m-1, n)
                     /                      \                                     /                       \                          
              LCS(m, n-2)              LCS(m-1, n-1)                     LCS(m-1, n-1)                   LCS(m-2, n)
		     /          \              /           \                     /           \                 /             \
	 LCS(m, n-3)    LCS(m-1, n-2)   LCS(m-1, n-2)   LCS(m-2, n-1)    LCS(m-1, n-2)   LCS(m-2, n-1)  LCS(m-2, n-1)  LCS(m-3, n)
	*/

	/*
	if m & n are the length of two strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(m * n)
	Space Complexity : O(m * n)
	The recursive call stack would grow with O(m + n)
	*/

	// Using vector
	int getLongestCommonSubsequenceLength_top_down(const string& str1, int m, const string& str2, int n, vector< vector<int> >& lookupVector)
	{
		if (m < 0 || n < 0)
			return 0;

		if(lookupVector[m][n] == -1)
		{
			if (str1[m] == str2[n])
				lookupVector[m][n] = 1 + getLongestCommonSubsequenceLength_top_down(str1, m - 1, str2, n - 1, lookupVector);
			else
				lookupVector[m][n] = max(getLongestCommonSubsequenceLength_top_down(str1, m, str2, n - 1, lookupVector), getLongestCommonSubsequenceLength_top_down(str1, m - 1, str2, n, lookupVector));
		}

		return lookupVector[m][n];
	}

	// Using unordered_map
	int getLongestCommonSubsequenceLength_top_down(const string& str1, int m, const string& str2, int n, unordered_map<string, int>& lookup)
	{
		if (m < 0 || n < 0)
			return 0;

		string key = "0 to " + to_string(m) + "&" + "0 to " + to_string(n);
		if (lookup.find(key) == lookup.end())
		{
			if (str1[m] == str2[n])
				lookup[key] = 1 + getLongestCommonSubsequenceLength_top_down(str1, m - 1, str2, n - 1, lookup);
			else
				lookup[key] = max(getLongestCommonSubsequenceLength_top_down(str1, m, str2, n - 1, lookup), getLongestCommonSubsequenceLength_top_down(str1, m - 1, str2, n, lookup));
		}

		return lookup[key];
	}

	/*
	Bottom up approach:

	0 1 2 3 . . . n
	0 0 0 0 0 0 0 0 0
	1 0
	2 0
	. 0
	. 0
	m 0

	recurrence relation:
	LCSLength(0, 0) = 0
	LCSLength(m, n) = 1 + LCSLength(m - 1, n - 1)   ...When str1[m] == str2[n]
	= max(LCSLength(m, n - 1), LCSLength(m - 1, n))   ...Otherwise
	*/
	/*
	if m & n are the length of two strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(m * n)
	Space Complexity : O(m * n)
	The recursive call stack : NOT APPLICABLE
	*/
	int getLongestCommonSubsequenceLength_bottom_up(const string& str1, const string& str2)
	{
		int m = str1.size();
		int n = str2.size();
		vector< vector<int> > lengths(m + 1, vector<int>(n + 1, 0));
		for (int i = 1; i <= m; ++i)
		{
			for (int j = 1; j <= n; ++j)
			{
				if (str1[i - 1] == str2[j - 1])
					lengths[i][j] = 1 + lengths[i - 1][j - 1];
				else
					lengths[i][j] = max(lengths[i][j - 1], lengths[i - 1][j]);
			}
		}

		return lengths[m][n];
	}

	/*
	if m & n are the length of two strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(m * n)
	Space Complexity : O(m)
	The recursive call stack : NOT APPLICABLE
	*/
	//int getLongestCommonSubsequenceLength_bottom_up_space_efficient(const string& str1, const string& str2)
	//{
	//	int m = str1.size();
	//	int n = str2.size();
	//	int minStrLength = min(m, n);
	//	int maxStrLength = max(m, n);
	//	const string* pStr1 = &str1;
	//	const string* pStr2 = &str2;
	//	if (m != maxStrLength)
	//	{
	//		pStr1 = &str2;
	//		pStr2 = &str1;
	//	}
	//	vector<int> prevLengths(minStrLength + 1, 0);
	//	vector<int> currLengths(minStrLength + 1, 0);
	//	for (int i = 1; i <= maxStrLength; ++i)
	//	{
	//		for (int j = 1; j <= minStrLength; ++j)
	//		{
	//			if ((*pStr1)[i - 1] == (*pStr2)[j - 1])
	//				currLengths[j] = 1 + prevLengths[j - 1];
	//			else
	//				currLengths[j] = max(currLengths[j - 1], prevLengths[j]);
	//		}

	//		for (int j = 1; j <= minStrLength; ++j)
	//			prevLengths[j] = currLengths[j];
	//	}

	//	return currLengths[minStrLength];
	//}
	int getLongestCommonSubsequenceLength_bottom_up_space_efficient(const string& str1, const string& str2)
	{
		if (str1.length() < str2.length())
			return getLongestCommonSubsequenceLength_bottom_up_space_efficient(str2, str1);

		// When we reach here, we are sure, str2 is smaller string
		vector<int> prevLengths(str2.size() + 1, 0);
		vector<int> currLengths(str2.size() + 1, 0);
		for (int i = 1; i <= str1.size(); ++i)
		{
			for (int j = 1; j <= str2.size(); ++j)
			{
				if (str1[i - 1] == str2[j - 1])
					currLengths[j] = 1 + prevLengths[j - 1];
				else
					currLengths[j] = max(currLengths[j - 1], prevLengths[j]);
			}

			for (int j = 1; j <= str2.size(); ++j)
				prevLengths[j] = currLengths[j];
		}

		return currLengths[str2.size()];
	}




	// Testing
	int getLongestCommonSubsequenceLength(const string& str1, const string& str2, DP_Approach approach)
	{
		switch (approach)
		{
			case DP_Approach::naive:
				return getLongestCommonSubsequenceLength_naive(str1, str1.size() - 1, str2, str2.size() - 1);
			case DP_Approach::top_down_using_vector:
			{
				vector< vector<int> > lookupVector(str1.size(), vector<int>(str2.size(), -1));
				return getLongestCommonSubsequenceLength_top_down(str1, str1.size() - 1, str2, str2.size() - 1, lookupVector);
			}
			case DP_Approach::top_down_using_map:
			{
				unordered_map<string, int> lookup;
				return getLongestCommonSubsequenceLength_top_down(str1, str1.size() - 1, str2, str2.size() - 1, lookup);
			}
			case DP_Approach::bottom_up:
				return getLongestCommonSubsequenceLength_bottom_up(str1, str2);
			case DP_Approach::bottom_up_space_efficient:
				return getLongestCommonSubsequenceLength_bottom_up_space_efficient(str1, str2);
		}
	}

	MM_DECLARE_FLAG(DP_LongestCommonSubsequenceLength);

	MM_UNIT_TEST(DP_LongestCommonSubsequenceLength_test_1, DP_LongestCommonSubsequenceLength)
	{
		struct testData
		{
			string str1;
			string str2;
			int length;
		};

		const vector<testData> testDataVector = {
			{ "", "", 0 },
			{ "a", "b", 0 },
			{ "abc", "def", 0 },
			{ "a", "a", 1 },
			{ "abc", "daef", 1 },
			{ "abc", "daebfcg", 3 },
			{ "abc", "daebf", 2 },
			{ "abc", "def", 0 },
			{ "abc", "def", 0 }
		};

		int subSequenceLength = 0;
		for (int i = 0; i < testDataVector.size(); ++i)
		{
			subSequenceLength = getLongestCommonSubsequenceLength(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::naive);
			MM_EXPECT_TRUE(subSequenceLength == testDataVector[i].length, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].length, subSequenceLength);
			subSequenceLength = getLongestCommonSubsequenceLength(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::naive);
			MM_EXPECT_TRUE(subSequenceLength == testDataVector[i].length, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].length, subSequenceLength);

			subSequenceLength = getLongestCommonSubsequenceLength(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::top_down_using_vector);
			MM_EXPECT_TRUE(subSequenceLength == testDataVector[i].length, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].length, subSequenceLength);
			subSequenceLength = getLongestCommonSubsequenceLength(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::top_down_using_vector);
			MM_EXPECT_TRUE(subSequenceLength == testDataVector[i].length, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].length, subSequenceLength);
			subSequenceLength = getLongestCommonSubsequenceLength(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::top_down_using_map);
			MM_EXPECT_TRUE(subSequenceLength == testDataVector[i].length, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].length, subSequenceLength);
			subSequenceLength = getLongestCommonSubsequenceLength(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::top_down_using_map);
			MM_EXPECT_TRUE(subSequenceLength == testDataVector[i].length, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].length, subSequenceLength);

			subSequenceLength = getLongestCommonSubsequenceLength(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::bottom_up);
			MM_EXPECT_TRUE(subSequenceLength == testDataVector[i].length, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].length, subSequenceLength);
			subSequenceLength = getLongestCommonSubsequenceLength(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::bottom_up);
			MM_EXPECT_TRUE(subSequenceLength == testDataVector[i].length, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].length, subSequenceLength);
			subSequenceLength = getLongestCommonSubsequenceLength(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::bottom_up_space_efficient);
			MM_EXPECT_TRUE(subSequenceLength == testDataVector[i].length, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].length, subSequenceLength);
			subSequenceLength = getLongestCommonSubsequenceLength(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::bottom_up_space_efficient);
			MM_EXPECT_TRUE(subSequenceLength == testDataVector[i].length, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].length, subSequenceLength);
		}
	}
}

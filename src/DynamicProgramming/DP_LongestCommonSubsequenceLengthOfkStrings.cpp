#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;

#include "DynamicProgramming/DP_LongestCommonSubsequenceLengthOfkStrings.h"
#include "DynamicProgramming/DP_Common.h"
#include "Utils\Utils_PrintUtils.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	/*
	if l1, l2, l3, ..., lk are the length of k strings
	Time Complexity  : k^(l1 + l2 + l3 + ... + lk)
	Space Complexity : O(1)
	The recursive call stack would grow in O(l1 + l2 + l3 + ... + lk)
	*/

	int getLongestCommonSubsequenceLengthOfkStrings_naive(const vector<string>& str, vector<int> index)
	{
		for(int n : index)
			if (n < 0)
				return 0;

		bool equal = true;
		for(int i = 1; i < str.size(); ++i)
			if (str[0][ index[0] ] != str[i][ index[i] ])
			{
				equal = false;
				break;
			}

		if (equal)
		{
			for (int& n : index)
				--n;
			return 1 + getLongestCommonSubsequenceLengthOfkStrings_naive(str, index);
		}
		else
		{
			int max_len = INT_MIN;
			for (int k = 0; k < index.size(); ++k)
			{
				--index[k];
				max_len = max(max_len, getLongestCommonSubsequenceLengthOfkStrings_naive(str, index));
				++index[k];
			}
			return max_len;
		}
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
	if l1, l2, l3, ..., lk are the length of k strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(l1 * l2 * l3 * ... * lk)
	Space Complexity : O(l1 * l2 * l3 * ... * lk)
	The recursive call stack would grow with O(l1 + l2 + l3 + ... + lk)
	*/

	// Using unordered_map
	int getLongestCommonSubsequenceLengthOfkStrings_top_down(const vector<string>& str, vector<int> index, unordered_map<string, int>& lookup)
	{
		for (int n : index)
			if (n < 0)
				return 0;

		string key;
		for (int n : index)
			key += ("|" + to_string(n));

		if (lookup.find(key) == lookup.end())
		{
			bool equal = true;
			for (int i = 1; i < str.size(); ++i)
				if (str[0][index[0]] != str[i][index[i]])
				{
					equal = false;
					break;
				}

			if (equal)
			{
				for (int& n : index)
					--n;
				lookup[key] = 1 + getLongestCommonSubsequenceLengthOfkStrings_top_down(str, index, lookup);
			}
			else
			{
				int max_len = INT_MIN;
				for (int k = 0; k < index.size(); ++k)
				{
					--index[k];
					max_len = max(max_len, getLongestCommonSubsequenceLengthOfkStrings_top_down(str, index, lookup));
					++index[k];
				}
				lookup[key] = max_len;
			}
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
	LCSLengthOfkStrings(0, 0) = 0
	LCSLengthOfkStrings(m, n) = 1 + LCSLengthOfkStrings(m - 1, n - 1)   ...When str1[m] == str2[n]
	= max(LCSLengthOfkStrings(m, n - 1), LCSLengthOfkStrings(m - 1, n))   ...Otherwise
	*/

	/*
	if l1, l2, l3, ..., lk are the length of k strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(l1 * l2 * l3 * ... * lk)
	Space Complexity : O(max(l1, l2, l3, ..., lk)
	The recursive call stack : NOT APPLICABLE
	*/
	//TODO: modify to consider all LCSs, otherwise it does not give correct answer for k >= 3
	size_t getLongestCommonSubsequenceLengthOfkStrings_bottom_up_space_efficient(const vector<string>& str)
	{
		//vector<int> index(str.size(), 0);
		
		string ref(str[0]);
		for (size_t k = 1; k < str.size(); ++k)
		{
			vector<string> previous(str[k].size() + 1, "");
			vector<string> current(str[k].size() + 1, "");
			for (int i = 1; i <= ref.size(); ++i)
			{
				for (int j = 1; j <= str[k].size(); ++j)
				{
					if (ref[i - 1] == str[k][j - 1])
						current[j] = previous[j - 1] + ref[i - 1];
					else
						current[j] = (current[j - 1].length() > previous[j].length() ? current[j - 1] : previous[j]);
				}

				for (int j = 0; j < str[k].size(); ++j)
					previous[j] = current[j];
			}

			// take LCS of current two strings as base for next
			ref = current[str[k].size()];
		}

		return ref.length();
	}



	// Testing
	size_t getLongestCommonSubsequenceLengthOfkStrings(const vector<string>& str, DP_Approach approach)
	{
		vector<int> index;
		for (const string& s : str)
			index.push_back(static_cast<int>(s.length()) - 1);

		switch (approach)
		{
			case DP_Approach::naive:
				return getLongestCommonSubsequenceLengthOfkStrings_naive(str, index);
			case DP_Approach::top_down_using_map:
			{
				unordered_map<string, int> lookup;
				return getLongestCommonSubsequenceLengthOfkStrings_top_down(str, index, lookup);
			}
			case DP_Approach::bottom_up_space_efficient:
				return getLongestCommonSubsequenceLengthOfkStrings_bottom_up_space_efficient(str);
		}

		return 0;
	}

	MM_DECLARE_FLAG(DP_LongestCommonSubsequenceLengthOfkStrings);

	MM_UNIT_TEST(DP_LongestCommonSubsequenceLengthOfkStrings_test_1, DP_LongestCommonSubsequenceLengthOfkStrings)
	{
		struct testData
		{
			vector<string> str;
			int length;
		};

		const vector<testData> testDataVector = {
			{ {"", ""}, 0 },
			{ { "a", "b" }, 0 },
			{ { "abc", "def" }, 0 },
			{ { "a", "a" }, 1 },
			{ { "abc", "daef" }, 1 },
			{ { "abc", "daebfcg" }, 3 },
			{ { "abc", "daebf" }, 2 },
			{ { "abc", "def" }, 0 },
			{ { "abcbdab", "bdcaba", "badacb" }, 4 }
		};
		size_t subSequenceLengthOfkStrings = 0;

		for (int i = 0; i < testDataVector.size(); ++i)
		{
			//subSequenceLengthOfkStrings = getLongestCommonSubsequenceLengthOfkStrings(testDataVector[i].str, DP_Approach::naive);
			//MM_EXPECT_TRUE(subSequenceLengthOfkStrings == testDataVector[i].length, testDataVector[i].str, testDataVector[i].length, subSequenceLengthOfkStrings);
			subSequenceLengthOfkStrings = getLongestCommonSubsequenceLengthOfkStrings(testDataVector[i].str, DP_Approach::top_down_using_map);
			MM_EXPECT_TRUE(subSequenceLengthOfkStrings == testDataVector[i].length, testDataVector[i].str, testDataVector[i].length, subSequenceLengthOfkStrings);
			subSequenceLengthOfkStrings = getLongestCommonSubsequenceLengthOfkStrings(testDataVector[i].str, DP_Approach::bottom_up_space_efficient);
			MM_EXPECT_TRUE(subSequenceLengthOfkStrings == testDataVector[i].length, testDataVector[i].str, testDataVector[i].length, subSequenceLengthOfkStrings);
		}
	}
}

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;

#include "DynamicProgramming/DP_LongestCommonSubsequenceString.h"
#include "DynamicProgramming/DP_Common.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	/*
	if m & n are the length of two strings
	Time Complexity  : 2^(m + n)
	Space Complexity : O(1)
	The recursive call stack would grow with O(m + n)
	*/

	string getLongestCommonSubsequenceString_naive(const string& str1, int m, const string& str2, int n)
	{
		if (m < 0 || n < 0)
			return "";

		if (str1[m] == str2[n])
			return getLongestCommonSubsequenceString_naive(str1, m - 1, str2, n - 1) + str1[m];
		else
		{
			string s1 = getLongestCommonSubsequenceString_naive(str1, m, str2, n - 1);
			string s2 = getLongestCommonSubsequenceString_naive(str1, m - 1, str2, n);
			return (s1.size() > s2.size() ? s1 : s2);
		}
	}

	/*
	if m & n are the length of two strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(m * n)
	Space Complexity : O(m * n)
	The recursive call stack would grow with O(m + n)
	*/

	// Using vector
	string getLongestCommonSubsequenceString_top_down(const string& str1, int m, const string& str2, int n, vector< vector<string> >& lookupVector)
	{
		if (m < 0 || n < 0)
			return "";

		if (lookupVector[m][n].empty())
		{
			if (str1[m] == str2[n])
				lookupVector[m][n] = getLongestCommonSubsequenceString_top_down(str1, m - 1, str2, n - 1, lookupVector) + str1[m];
			else
			{
				string s1 = getLongestCommonSubsequenceString_top_down(str1, m, str2, n - 1, lookupVector);
				string s2 = getLongestCommonSubsequenceString_top_down(str1, m - 1, str2, n, lookupVector);
				lookupVector[m][n] = (s1.size() > s2.size() ? s1 : s2);
			}
		}

		return lookupVector[m][n];
	}

	// Using unordered_map
	string getLongestCommonSubsequenceString_top_down(const string& str1, int m, const string& str2, int n, unordered_map<string, string>& lookup)
	{
		if (m < 0 || n < 0)
			return "";

		string key = "0 to " + to_string(m) + "&" + "0 to " + to_string(n);
		if (lookup.find(key) == lookup.end())
		{
			if (str1[m] == str2[n])
				lookup[key] = getLongestCommonSubsequenceString_top_down(str1, m - 1, str2, n - 1, lookup) + str1[m];
			else
			{
				string s1 = getLongestCommonSubsequenceString_top_down(str1, m, str2, n - 1, lookup);
				string s2 = getLongestCommonSubsequenceString_top_down(str1, m - 1, str2, n, lookup);
				lookup[key] = ( s1.size() > s2.size() ? s1 : s2 );
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

	string getLongestCommonSubsequenceString_bottom_up(const string& str1, const string& str2)
	{
		int m = str1.size();
		int n = str2.size();
		vector< vector<string> > subseqStr(m + 1, vector<string>(n + 1, ""));
		for (int i = 1; i <= m; ++i)
		{
			for (int j = 1; j <= n; ++j)
			{
				if (str1[i - 1] == str2[j - 1])
					subseqStr[i][j] = subseqStr[i - 1][j - 1] + str1[i - 1];
				else
					subseqStr[i][j] = (subseqStr[i][j - 1].size() > subseqStr[i - 1][j].size() ? subseqStr[i][j - 1] : subseqStr[i - 1][j]);
			}
		}

		return subseqStr[m][n];
	}

	string getLongestCommonSubsequenceString_bottom_up_space_efficient(const string& str1, const string& str2)
	{
		int m = str1.size();
		int n = str2.size();
		int minStrLength = min(m, n);
		int maxStrLength = max(m, n);
		const string* pStr1 = &str1;
		const string* pStr2 = &str2;
		if (m != maxStrLength)
		{
			pStr1 = &str2;
			pStr2 = &str1;
		}
		vector<string> prevSubseqStrings(minStrLength + 1, "");
		vector<string> currSubseqStrings(minStrLength + 1, "");
		for (int i = 1; i <= maxStrLength; ++i)
		{
			for (int j = 1; j <= minStrLength; ++j)
			{
				if ((*pStr1)[i - 1] == (*pStr2)[j - 1])
					currSubseqStrings[j] = prevSubseqStrings[j - 1] + (*pStr1)[i - 1];
				else
					currSubseqStrings[j] = (currSubseqStrings[j - 1].size() > prevSubseqStrings[j].size() ? currSubseqStrings[j - 1] : prevSubseqStrings[j]);
			}

			for (int j = 1; j <= minStrLength; ++j)
				prevSubseqStrings[j] = currSubseqStrings[j];
		}

		return currSubseqStrings[minStrLength];
	}

	string getLongestCommonSubsequenceString(const string& str1, const string& str2, DP_Approach approach)
	{
		switch (approach)
		{
			case DP_Approach::naive:
				return getLongestCommonSubsequenceString_naive(str1, str1.size() - 1, str2, str2.size() - 1);
			case DP_Approach::top_down_using_vector:
			{
				vector< vector<string> > lookupVector(str1.size(), vector<string>(str2.size(), ""));
				return getLongestCommonSubsequenceString_top_down(str1, str1.size() - 1, str2, str2.size() - 1, lookupVector);
			}
			case DP_Approach::top_down_using_map:
			{
				unordered_map<string, string> lookup;
				return getLongestCommonSubsequenceString_top_down(str1, str1.size() - 1, str2, str2.size() - 1, lookup);
			}
			case DP_Approach::bottom_up:
				return getLongestCommonSubsequenceString_bottom_up(str1, str2);
			case DP_Approach::bottom_up_space_efficient:
				return getLongestCommonSubsequenceString_bottom_up_space_efficient(str1, str2);
		}
	}

	MM_DECLARE_FLAG(DP_LongestCommonSubsequenceString);

	MM_UNIT_TEST(DP_LongestCommonSubsequenceString_test_1, DP_LongestCommonSubsequenceString)
	{
		struct testData
		{
			string str1;
			string str2;
			string subSeq;
		};

		const vector<testData> testDataVector = {
			{ "", "", "" },
			{ "a", "b", "" },
			{ "abc", "def", "" },
			{ "a", "a", "a" },
			{ "abc", "daef", "a" },
			{ "abc", "daebfcg", "abc" },
			{ "abc", "daebf", "ab" },
			{ "abc", "def", "" },
			{ "abc", "def", "" }
		};

		string actualSubSequenceString;

		for (int i = 0; i < testDataVector.size(); ++i)
		{
			actualSubSequenceString = getLongestCommonSubsequenceString(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::naive);
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			actualSubSequenceString = getLongestCommonSubsequenceString(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::naive);
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			actualSubSequenceString = getLongestCommonSubsequenceString(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::top_down_using_vector);
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			actualSubSequenceString = getLongestCommonSubsequenceString(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::top_down_using_vector);
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			actualSubSequenceString = getLongestCommonSubsequenceString(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::top_down_using_map);
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			actualSubSequenceString = getLongestCommonSubsequenceString(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::top_down_using_map);
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			actualSubSequenceString = getLongestCommonSubsequenceString(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::bottom_up);
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			actualSubSequenceString = getLongestCommonSubsequenceString(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::bottom_up);
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			actualSubSequenceString = getLongestCommonSubsequenceString(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::bottom_up_space_efficient);
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			actualSubSequenceString = getLongestCommonSubsequenceString(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::bottom_up_space_efficient);
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
		}
	}
}

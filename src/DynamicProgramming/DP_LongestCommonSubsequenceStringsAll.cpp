#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;

#include "DynamicProgramming/DP_LongestCommonSubsequenceStringsAll.h"
#include "DynamicProgramming/DP_Common.h"
#include "Utils\Utils_PrintUtils.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	/*
	if m & n are the length of two strings
	Time Complexity  : 2^(m + n)
	Space Complexity : O(1)
	The recursive call stack would grow with O(m + n)
	*/

	void getLongestCommonSubsequenceStringsAll_naive(const string& str1, int m, const string& str2, int n, vector<string>& out)
	{
		if (m < 0 || n < 0)
			return;

		if (str1[m] == str2[n])
		{
			vector<string> s;
			getLongestCommonSubsequenceStringsAll_naive(str1, m - 1, str2, n - 1, s);
			if (s.empty())
				s.push_back("");
			
			for (string& str : s)
				str += str1[m];
			out = s;
		}
		else
		{
			vector<string> s1, s2;
			getLongestCommonSubsequenceStringsAll_naive(str1, m, str2, n - 1, s1);
			getLongestCommonSubsequenceStringsAll_naive(str1, m - 1, str2, n, s2);
			if (s1.empty() && !s2.empty())
				out = s2;
			else if(!s1.empty() && s2.empty())
				out = s1;
			else if(!s1.empty() && !s2.empty())
			{
				if(s1[0].length() > s2[0].length())
					out = s1;
				else if(s2[0].length() > s1[0].length())
					out = s2;
				else
				{
					out = s1;
					for (string& str : s2)
					{
						if (std::find(out.begin(), out.end(), str) == out.end())
							out.push_back(str);
					}
				}
			}
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
	void getLongestCommonSubsequenceStringsAll_top_down(const string& str1, int m, const string& str2, int n, vector< vector< vector<string> > >& lookupVector, vector<string>& out)
	{
		if (m < 0 || n < 0)
			return;

		if (lookupVector[m][n].empty())
		{
			if (str1[m] == str2[n])
			{
				getLongestCommonSubsequenceStringsAll_top_down(str1, m - 1, str2, n - 1, lookupVector, lookupVector[m][n]);
				if (lookupVector[m][n].empty())
					lookupVector[m][n].push_back("");
					
				for (string& str : lookupVector[m][n])
					str += str1[m];
			}
			else
			{
				vector<string> s1, s2;
				getLongestCommonSubsequenceStringsAll_top_down(str1, m, str2, n - 1, lookupVector, s1);
				getLongestCommonSubsequenceStringsAll_top_down(str1, m - 1, str2, n, lookupVector, s2);
				if (s1.empty() && !s2.empty())
					lookupVector[m][n] = s2;
				else if (!s1.empty() && s2.empty())
					lookupVector[m][n] = s1;
				else if (!s1.empty() && !s2.empty())
				{
					if (s1[0].length() > s2[0].length())
						lookupVector[m][n] = s1;
					else if (s2[0].length() > s1[0].length())
						lookupVector[m][n] = s2;
					else
					{
						lookupVector[m][n] = s1;
						for (string& str : s2)
						{
							if (std::find(lookupVector[m][n].begin(), lookupVector[m][n].end(), str) == lookupVector[m][n].end())
								lookupVector[m][n].push_back(str);
						}
					}
				}
			}
		}

		out = lookupVector[m][n];
	}

	// Using unordered_map
	void getLongestCommonSubsequenceStringsAll_top_down(const string& str1, int m, const string& str2, int n, unordered_map<string, vector<string> >& lookup, vector<string>& out)
	{
		if (m < 0 || n < 0)
			return;

		string key = "0 to " + to_string(m) + "&" + "0 to " + to_string(n);
		if (lookup.find(key) == lookup.end())
		{
			if (str1[m] == str2[n])
			{
				vector<string> s;
				getLongestCommonSubsequenceStringsAll_top_down(str1, m - 1, str2, n - 1, lookup, s);
				if (s.empty())
					s.push_back("");
				
				for (string& str : s)
					str += str1[m];
				lookup[key] = s;
			}
			else
			{
				vector<string> s1, s2;
				getLongestCommonSubsequenceStringsAll_top_down(str1, m, str2, n - 1, lookup, s1);
				getLongestCommonSubsequenceStringsAll_top_down(str1, m - 1, str2, n, lookup, s2);
				if (s1.empty() && !s2.empty())
					lookup[key] = s2;
				else if (!s1.empty() && s2.empty())
					lookup[key] = s1;
				else if (!s1.empty() && !s2.empty())
				{
					if (s1[0].length() > s2[0].length())
						lookup[key] = s1;
					else if (s2[0].length() > s1[0].length())
						lookup[key] = s2;
					else
					{
						lookup[key] = s1;
						for (string& str : s2)
						{
							if (std::find(lookup[key].begin(), lookup[key].end(), str) == lookup[key].end())
								lookup[key].push_back(str);
						}
					}
				}
			}
		}

		out = lookup[key];
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

	void getLongestCommonSubsequenceStringsAll_bottom_up(const string& str1, const string& str2, vector<string>& out)
	{
		int m = str1.size();
		int n = str2.size();
		vector< vector< vector<string> > > subseqStr(m + 1, vector< vector<string> >(n + 1, vector<string>()));
		for (int i = 1; i <= m; ++i)
		{
			for (int j = 1; j <= n; ++j)
			{
				if (str1[i - 1] == str2[j - 1])
				{
					subseqStr[i][j] = subseqStr[i - 1][j - 1];
					if (subseqStr[i][j].empty())
						subseqStr[i][j].push_back("");
					for(string& str : subseqStr[i][j])
						str += str1[i - 1];
				}
				else
				{
					if(!subseqStr[i][j - 1].empty() && subseqStr[i - 1][j].empty())
						subseqStr[i][j] = subseqStr[i][j - 1];
					else if(subseqStr[i][j - 1].empty() && !subseqStr[i - 1][j].empty())
						subseqStr[i][j] = subseqStr[i - 1][j];
					else if(!subseqStr[i][j - 1].empty() && !subseqStr[i - 1][j].empty())
					{
						if (subseqStr[i][j - 1][0].size() > subseqStr[i - 1][j][0].size())
							subseqStr[i][j] = subseqStr[i][j - 1];
						else if (subseqStr[i - 1][j][0].size() > subseqStr[i][j - 1][0].size())
							subseqStr[i][j] = subseqStr[i - 1][j];
						else
						{
							subseqStr[i][j] = subseqStr[i][j - 1];
							for (string& str : subseqStr[i - 1][j])
							{
								if (std::find(subseqStr[i][j].begin(), subseqStr[i][j].end(), str) == subseqStr[i][j].end())
									subseqStr[i][j].push_back(str);
							}
						}
					}
				}
			}
		}

		out = subseqStr[m][n];
	}

	void getLongestCommonSubsequenceStringsAll_bottom_up_space_efficient(const string& str1, const string& str2, vector<string>& out)
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
		vector< vector<string> > prevSubseqStrings(minStrLength + 1, vector<string>());
		vector< vector<string> > currSubseqStrings(minStrLength + 1, vector<string>());
		for (int i = 1; i <= maxStrLength; ++i)
		{
			for (int j = 1; j <= minStrLength; ++j)
			{
				if ((*pStr1)[i - 1] == (*pStr2)[j - 1])
				{
					currSubseqStrings[j] = prevSubseqStrings[j - 1];
					if (currSubseqStrings[j].empty())
						currSubseqStrings[j].push_back("");
					for(string& str : currSubseqStrings[j])
						str += (*pStr1)[i - 1];
				}
				else
				{
					if(!currSubseqStrings[j - 1].empty() && prevSubseqStrings[j].empty())
						currSubseqStrings[j] = currSubseqStrings[j - 1];
					else if(currSubseqStrings[j - 1].empty() && !prevSubseqStrings[j].empty())
						currSubseqStrings[j] = prevSubseqStrings[j];
					else if (!currSubseqStrings[j - 1].empty() && !prevSubseqStrings[j].empty())
					{
						if (currSubseqStrings[j - 1][0].size() > prevSubseqStrings[j][0].size())
							currSubseqStrings[j] = currSubseqStrings[j - 1];
						else if (currSubseqStrings[j - 1][0].size() < prevSubseqStrings[j][0].size())
							currSubseqStrings[j] = prevSubseqStrings[j];
						else
						{
							currSubseqStrings[j] = prevSubseqStrings[j];
							for (string& str : currSubseqStrings[j - 1])
							{
								if (std::find(currSubseqStrings[j].begin(), currSubseqStrings[j].end(), str) == currSubseqStrings[j].end())
									currSubseqStrings[j].push_back(str);
							}
						}
					}
				}
			}

			for (int j = 1; j <= minStrLength; ++j)
				prevSubseqStrings[j] = currSubseqStrings[j];
		}

		out = currSubseqStrings[minStrLength];
	}

	vector<string> getLongestCommonSubsequenceStringsAll_bottom_up_most_efficient(const string& str1, int m, const string& str2, int n, const vector< vector< int > >& LCSLength)
	{
		if (m < 0 || n < 0)
			return vector<string>();

		if (str1[m] == str2[n])
		{
			vector<string> s = getLongestCommonSubsequenceStringsAll_bottom_up_most_efficient(str1, m - 1, str2, n - 1, LCSLength);
			if (s.empty())
				s.push_back("");
			for (string& str : s)
				str += str1[m];
			return s;
		}
		else
		{
			if(LCSLength[m][n + 1] > LCSLength[m + 1][n])
				return getLongestCommonSubsequenceStringsAll_bottom_up_most_efficient(str1, m - 1, str2, n, LCSLength);
			else if(LCSLength[m][n + 1] < LCSLength[m + 1][n])
				return getLongestCommonSubsequenceStringsAll_bottom_up_most_efficient(str1, m, str2, n - 1, LCSLength);
			else
			{
				vector<string> s1, s2;
				s1 = getLongestCommonSubsequenceStringsAll_bottom_up_most_efficient(str1, m - 1, str2, n, LCSLength);
				s2 = getLongestCommonSubsequenceStringsAll_bottom_up_most_efficient(str1, m, str2, n - 1, LCSLength);
				for (string& str : s2)
				{
					if (std::find(s1.begin(), s1.end(), str) == s1.end())
						s1.push_back(str);
				}
				return s1;
			}
		}
	}

	void getLongestCommonSubsequenceStringsAll_bottom_up_most_efficient(const string& str1, const string& str2, vector<string>& out)
	{
		// First create a matrix with LCS lengths
		int m = str1.size();
		int n = str2.size();
		vector< vector< int > > LCSLength(m + 1, vector< int >(n + 1, 0));
		for (int i = 1; i <= m; ++i)
		{
			for (int j = 1; j <= n; ++j)
			{
				if (str1[i - 1] == str2[j - 1])
					LCSLength[i][j] = LCSLength[i - 1][j - 1] + 1;
				else
					LCSLength[i][j] = max(LCSLength[i][j - 1], LCSLength[i - 1][j]);
			}
		}

		out.push_back("");
		// Find all LCSs using above matrix
		out = getLongestCommonSubsequenceStringsAll_bottom_up_most_efficient(str1, str1.length() - 1, str2, str2.length() - 1, LCSLength);
	}

	void getLongestCommonSubsequenceStringsAll(const string& str1, const string& str2, DP_Approach approach, vector<string>& out)
	{
		out.clear();
		switch (approach)
		{
			case DP_Approach::naive:
				getLongestCommonSubsequenceStringsAll_naive(str1, str1.size() - 1, str2, str2.size() - 1, out);
				break;
			case DP_Approach::top_down_using_vector:
			{
				vector< vector< vector<string> > > lookupVector(str1.size(), vector< vector<string> >(str2.size(), vector<string>()));
				getLongestCommonSubsequenceStringsAll_top_down(str1, str1.size() - 1, str2, str2.size() - 1, lookupVector, out);
				break;
			}
			case DP_Approach::top_down_using_map:
			{
				unordered_map<string, vector<string> > lookup;
				getLongestCommonSubsequenceStringsAll_top_down(str1, str1.size() - 1, str2, str2.size() - 1, lookup, out);
				break;
			}
			case DP_Approach::bottom_up:
				getLongestCommonSubsequenceStringsAll_bottom_up(str1, str2, out);
				break;
			case DP_Approach::bottom_up_space_efficient:
				getLongestCommonSubsequenceStringsAll_bottom_up_space_efficient(str1, str2, out);
				break;
			case DP_Approach::bottom_up_most_efficient:
				getLongestCommonSubsequenceStringsAll_bottom_up_most_efficient(str1, str2, out);
				break;
		}
	}

	MM_DECLARE_FLAG(DP_LongestCommonSubsequenceStringsAll);

	MM_UNIT_TEST(DP_LongestCommonSubsequenceStringsAll_test_1, DP_LongestCommonSubsequenceStringsAll)
	{
		struct testData
		{
			string str1;
			string str2;
			vector<string> subSeq;
		};

		const vector<testData> testDataVector = {
			{ "",				"",					{ }				},
			{ "a",				"b",				{ }				},
			{ "abc",			"def",				{ }				},
			{ "a",				"a",				{ "a" }				},
			{ "abc",			"daef",				{ "a" }				},
			{ "abc",			"daebfcg",			{ "abc" }			},
			{ "abc",			"daebf",			{ "ab" }			},
			{ "abc",			"def",				{ }				},
			{ "abcbdab",		"bdcaba",			{ "bcab", "bcba", "bdab" }				}
		};

		vector<string> actualSubSequenceString;
		for (int i = 0; i < testDataVector.size(); ++i)
		{
			getLongestCommonSubsequenceStringsAll(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::naive, actualSubSequenceString);
			std::sort(actualSubSequenceString.begin(), actualSubSequenceString.end());
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			getLongestCommonSubsequenceStringsAll(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::naive, actualSubSequenceString);
			std::sort(actualSubSequenceString.begin(), actualSubSequenceString.end());
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			
			getLongestCommonSubsequenceStringsAll(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::top_down_using_vector, actualSubSequenceString);
			std::sort(actualSubSequenceString.begin(), actualSubSequenceString.end());
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			getLongestCommonSubsequenceStringsAll(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::top_down_using_vector, actualSubSequenceString);
			std::sort(actualSubSequenceString.begin(), actualSubSequenceString.end());
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			getLongestCommonSubsequenceStringsAll(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::top_down_using_map, actualSubSequenceString);
			std::sort(actualSubSequenceString.begin(), actualSubSequenceString.end());
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			getLongestCommonSubsequenceStringsAll(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::top_down_using_map, actualSubSequenceString);
			std::sort(actualSubSequenceString.begin(), actualSubSequenceString.end());
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			
			getLongestCommonSubsequenceStringsAll(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::bottom_up, actualSubSequenceString);
			std::sort(actualSubSequenceString.begin(), actualSubSequenceString.end());
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			getLongestCommonSubsequenceStringsAll(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::bottom_up, actualSubSequenceString);
			std::sort(actualSubSequenceString.begin(), actualSubSequenceString.end());
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			getLongestCommonSubsequenceStringsAll(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::bottom_up_space_efficient, actualSubSequenceString);
			std::sort(actualSubSequenceString.begin(), actualSubSequenceString.end());
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			getLongestCommonSubsequenceStringsAll(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::bottom_up_space_efficient, actualSubSequenceString);
			std::sort(actualSubSequenceString.begin(), actualSubSequenceString.end());
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);

			getLongestCommonSubsequenceStringsAll(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::bottom_up_most_efficient, actualSubSequenceString);
			std::sort(actualSubSequenceString.begin(), actualSubSequenceString.end());
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);
			getLongestCommonSubsequenceStringsAll(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::bottom_up_most_efficient, actualSubSequenceString);
			std::sort(actualSubSequenceString.begin(), actualSubSequenceString.end());
			MM_EXPECT_TRUE(actualSubSequenceString == testDataVector[i].subSeq, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].subSeq, actualSubSequenceString);

		}
	}
}

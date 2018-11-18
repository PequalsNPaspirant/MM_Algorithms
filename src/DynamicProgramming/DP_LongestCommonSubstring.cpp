#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

//#include "Assert/MyAssert.h"
#include "DynamicProgramming/DP_Common.h"
#include "DynamicProgramming/DP_LongestCommonSubstring.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	// ============================= Naive Approach =============================
	/*
	length of first  string = m
	length of second string = n
	Complexity:
	The naive approach is to check if "one of the all possible substrings of first string" is substring of second string.
	number of all possible substrings of first string (from length m to 1) = 1 + 2 + 3 + ... + m
	So the worst case complexity = (1 + 2 + 3 + ... + m) * n = O(m * m * n)

	The better approach is to check for first matching character of such substring trying all possible combinations of first and second strings,
	and if we get it, search for next matching characters,
	Otherwise try to get first matching char of another possible substring.
	The complexity of this approach is O(m*n)
	The running time is m*n + sum of lengths of all substrings
	*/

	/*
	if m & n are the length of two strings
	Time Complexity  : 2^(m + n)
	Space Complexity : O(1)
	The recursive call stack would grow with O(m + n)
	*/

	//MyString getLongestCommonSubstring(const MyString& str1, const MyString& str2)
	string getLongestCommonSubstring_naive(const string& str1, const string& str2)
	{
		int maxLength = INT_MIN;
		string maxLenSubString;
		for (int i = 0; i < str1.length(); i++)
			for (int j = 0; j < str2.length(); j++)
			{
				if (str1[i] == str2[j]) // First letter of substring matches
				{
					// Check the length of possible substring
					int a = i, b = j;
					for (; a < str1.length() && b < str2.length(); a++, b++)
					{
						if (str1[a] != str2[b])
							break;
					}
					if (maxLength < (a - i))
					{
						maxLength = a - i;
						maxLenSubString = str1.substr(i, maxLength);
					}
				}
			}

		return maxLenSubString;
	}

	// ============================= Dynamic Programing Approach =============================

	/*
	if m & n are the length of two strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(m * n)
	Space Complexity : O(m * n)
	The recursive call stack would grow with O(m + n)
	*/

	// Using vector
	string getLongestCommonSubstring_top_down(const string& str1, int m, const string& str2, int n, vector< vector<string> >& lookupVector)
	{
		if (m < 0 || n < 0)
			return "";

		if (lookupVector[m][n].empty())
		{
			if (str1[m] == str2[n])
				lookupVector[m][n] = getLongestCommonSubstring_top_down(str1, m - 1, str2, n - 1, lookupVector) + str1[m];
			else
			{
				string s1 = getLongestCommonSubstring_top_down(str1, m, str2, n - 1, lookupVector);
				string s2 = getLongestCommonSubstring_top_down(str1, m - 1, str2, n, lookupVector);
				lookupVector[m][n] = (s1.length() > s2.length() ? s1 : s2);
			}
		}

		return lookupVector[m][n];
	}

	// Using unordered_map
	string getLongestCommonSubstring_top_down(const string& str1, int m, const string& str2, int n, unordered_map<string, string>& lookup)
	{
		if (m < 0 || n < 0)
			return "";

		string key = "0 to " + to_string(m) + "&" + "0 to " + to_string(n);
		if (lookup.find(key) == lookup.end())
		{
			if (str1[m] == str2[n])
				lookup[key] = getLongestCommonSubstring_top_down(str1, m - 1, str2, n - 1, lookup) + str1[m];
			else
			{
				string s1 = getLongestCommonSubstring_top_down(str1, m, str2, n - 1, lookup);
				string s2 = getLongestCommonSubstring_top_down(str1, m - 1, str2, n, lookup);
				lookup[key] = (s1.length() > s2.length() ? s1 : s2);
			}
		}

		return lookup[key];
	}


	/*
	if m & n are the length of two strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(m * n)
	Space Complexity : O(m * n)
	The recursive call stack : NOT APPLICABLE
	*/
	string getLongestCommonSubstring_bottom_up(const string& str1, const string& str2)
	{
		int maxLength = 0, endIndex = 0;
		vector< vector<int> > lookUpTable(str1.length() + 1, vector<int>(str2.length() + 1, 0));

		for (int i = 1; i <= str1.length(); i++)
			for (int j = 1; j <= str2.length(); j++)
			{
				if (str1[i - 1] == str2[j - 1])
					lookUpTable[i][j] = lookUpTable[i - 1][j - 1] + 1;
				else
					lookUpTable[i][j] = 0;
				
				if (maxLength < lookUpTable[i][j])
				{
					maxLength = lookUpTable[i][j];
					endIndex = i;
				}
			}

		return str1.substr(endIndex - maxLength, maxLength);
	}

	/*
	if m & n are the length of two strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(m * n)
	Space Complexity : O(m)
	The recursive call stack : NOT APPLICABLE
	*/
	string getLongestCommonSubstring_bottom_up_space_efficient(const string& str1, const string& str2)
	{
		if (str1.length() < str2.length())
			return getLongestCommonSubstring_bottom_up_space_efficient(str2, str1); // if second string is short, we use lesser space 

		int maxLength = 0, endIndex = 0;
		vector<int> previous(str2.length() + 1, 0);
		vector<int> current(str2.length() + 1, 0);

		for (int i = 1; i <= str1.length(); i++)
		{
			for (int j = 1; j <= str2.length(); j++)
			{
				if (str1[i - 1] == str2[j - 1])
					current[j] = previous[j - 1] + 1;
				else
					current[j] = 0;

				if (maxLength < current[j])
				{
					maxLength = current[j];
					endIndex = i;
				}
			}

			for (int j = 1; j <= str2.length(); j++)
				previous[j] = current[j];
		}

		return str1.substr(endIndex - maxLength, maxLength);
	}

	// ============================= Solution using suffix tree =============================
	/*
	if m & n are the length of two strings
	Time Complexity  : number of distinct subproblems * Complexity of each subproblem
	Time Complexity  : O(m + n)
	Space Complexity : O( )
	The recursive call stack : NOT APPLICABLE
	*/

	string getLongestCommonSubstring_usingSuffixTree(const string& str1, const string& str2)
	{
		//TODO:

		return "";
	}


	// ============================= Testing =============================

	string getLongestCommonSubstring(const string& str1, const string& str2, DP_Approach approach)
	{
		switch (approach)
		{
		case DP_Approach::naive:
			return getLongestCommonSubstring_naive(str1, str2);
		case DP_Approach::top_down_using_vector:
		{
			vector< vector<string> > lookupVector(str1.size(), vector<string>(str2.size()));
			return getLongestCommonSubstring_top_down(str1, str1.size() - 1, str2, str2.size() - 1, lookupVector);
		}
		case DP_Approach::top_down_using_map:
		{
			unordered_map<string, string> lookup;
			return getLongestCommonSubstring_top_down(str1, str1.size() - 1, str2, str2.size() - 1, lookup);
		}
		case DP_Approach::bottom_up:
			return getLongestCommonSubstring_bottom_up(str1, str2);
		case DP_Approach::bottom_up_space_efficient:
			return getLongestCommonSubstring_bottom_up_space_efficient(str1, str2);
		case DP_Approach::others_1:
			return getLongestCommonSubstring_usingSuffixTree(str1, str2);
		}
	}

	MM_DECLARE_FLAG(DP_LongestCommonSubstring);

	MM_UNIT_TEST(DP_LongestCommonSubstring_test_1, DP_LongestCommonSubstring)
	{
		struct DataSet
		{
			string str1;
			string str2;
			string substr;
		};

		vector<DataSet> testDataVector({
			{"", "", ""},
			{ "a", "b", "" },
			{ "a", "a", "a" },
			{ "ab", "bc", "b" },
			{ "aa", "a", "a" },
			{ "aa", "aaa", "aa" },
			{ "abcd", "", "" },
			{ "abcd", "a", "a" },
			{ "abcd", "ab", "ab" },
			{ "abcd", "bc", "bc" },
			{ "abcd", "cd", "cd" },
			{ "abcd", "bcde", "bcd" },
			{ "aabbcc", "aaabb", "aabb" },
			{ "aaabbbccc", "xxxabbbcyyy", "abbbc" }
		});
		
		string subString;
		for (int i = 0; i < testDataVector.size(); ++i)
		{
			subString = getLongestCommonSubstring(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::naive);
			MM_EXPECT_TRUE(subString == testDataVector[i].substr, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].substr, subString);
			subString = getLongestCommonSubstring(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::naive);
			MM_EXPECT_TRUE(subString == testDataVector[i].substr, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].substr, subString);

			subString = getLongestCommonSubstring(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::top_down_using_vector);
			MM_EXPECT_TRUE(subString == testDataVector[i].substr, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].substr, subString);
			subString = getLongestCommonSubstring(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::top_down_using_vector);
			MM_EXPECT_TRUE(subString == testDataVector[i].substr, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].substr, subString);
			subString = getLongestCommonSubstring(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::top_down_using_map);
			MM_EXPECT_TRUE(subString == testDataVector[i].substr, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].substr, subString);
			subString = getLongestCommonSubstring(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::top_down_using_map);
			MM_EXPECT_TRUE(subString == testDataVector[i].substr, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].substr, subString);

			subString = getLongestCommonSubstring(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::bottom_up);
			MM_EXPECT_TRUE(subString == testDataVector[i].substr, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].substr, subString);
			subString = getLongestCommonSubstring(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::bottom_up);
			MM_EXPECT_TRUE(subString == testDataVector[i].substr, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].substr, subString);
			subString = getLongestCommonSubstring(testDataVector[i].str1, testDataVector[i].str2, DP_Approach::bottom_up_space_efficient);
			MM_EXPECT_TRUE(subString == testDataVector[i].substr, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].substr, subString);
			subString = getLongestCommonSubstring(testDataVector[i].str2, testDataVector[i].str1, DP_Approach::bottom_up_space_efficient);
			MM_EXPECT_TRUE(subString == testDataVector[i].substr, testDataVector[i].str1, testDataVector[i].str2, testDataVector[i].substr, subString);
		}
	}
}

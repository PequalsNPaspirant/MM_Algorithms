#include <queue>
#include <vector>
#include <memory>

#include "String/String_FirstKNonRepeatingChars.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

// Problem 1: Find first non-repeating char

	char getFirstNonRepeatingChar(const string& str)
	{
		charInfo hashTable[256];
		for (int i = 0; i < str.size(); ++i)
		{
			hashTable[str[i]].count += 1;
			if (hashTable[str[i]].index == -1)
				hashTable[str[i]].index = i;
		}

		size_t minIndex = str.size();
		for (int i = 0; i < 256; ++i)
		{
			if (hashTable[i].count == 1 && hashTable[i].index < minIndex)
				minIndex = hashTable[i].index;
		}

		if (minIndex == str.size())
			return '\0';
		else
			return str[minIndex];
	}

	MM_DECLARE_FLAG(String_FirstNonRepeatingChar)

	MM_UNIT_TEST(String_FirstNonRepeatingChar_test_1, String_FirstNonRepeatingChar)
	{
		MM_EXPECT_TRUE(getFirstNonRepeatingChar("") == '\0')
		MM_EXPECT_TRUE(getFirstNonRepeatingChar("abcde") == 'a')
		MM_EXPECT_TRUE(getFirstNonRepeatingChar("aabcde") == 'b')
		MM_EXPECT_TRUE(getFirstNonRepeatingChar("aabbcde") == 'c')
		MM_EXPECT_TRUE(getFirstNonRepeatingChar("abcdeabc") == 'd')
		MM_EXPECT_TRUE(getFirstNonRepeatingChar("ab ac dbeced") == '\0')
		MM_EXPECT_TRUE(getFirstNonRepeatingChar("abcdeedcb") == 'a')
	}

//Problem 2: Generalization to find first k non-repeating chars
//Solution 1: 

	string getFirstKNonRepeatingChars_usingPriorityQueue_1(const string& str, int n)
	{
		std::vector<std::unique_ptr<charInfo2>> hashTable(256); // All pointers are nullptr
		for (int i = 0; i < str.size(); ++i)
		{
			if(!hashTable[str[i]])
				hashTable[str[i]] = std::make_unique<charInfo2>();
			hashTable[str[i]]->count += 1;
			if (hashTable[str[i]]->index == -1)
				hashTable[str[i]]->index = i;
		}

		// build min heap
		std::priority_queue<std::unique_ptr<charInfo2>, std::vector<std::unique_ptr<charInfo2>>, CompareCharInfoPtrs> topKChars;
		// OR
		auto cmp = [](const std::unique_ptr<charInfo2>& left, const std::unique_ptr<charInfo2>& right) 
		{ return left->index > right->index; };
		//std::priority_queue<std::unique_ptr<charInfo2>, std::vector<std::unique_ptr<charInfo2>>, decltype(cmp)> topKChars(cmp);

		for (int i = 0; i < 256; ++i)
		{
			if (hashTable[i] && hashTable[i]->count == 1)
			{
				topKChars.push(std::move(hashTable[i]));
			}
		}

		string retStr;
		for (int i = 0; i < n; ++i)
		{
			if (!topKChars.empty())
			{
				retStr += str[(topKChars.top())->index];
				topKChars.pop();
			}
		}
		
		return retStr;

	}

	MM_DECLARE_FLAG(String_FirstKNonRepeatingChars)

	MM_UNIT_TEST(String_FirstKNonRepeatingChars_test_1, String_FirstKNonRepeatingChars)
	{
		string str;
		MM_EXPECT_TRUE((str = getFirstKNonRepeatingChars_usingPriorityQueue_1("", 10)) == "", str)
		MM_EXPECT_TRUE((str = getFirstKNonRepeatingChars_usingPriorityQueue_1("abcde", 1)) == "a", str)
		MM_EXPECT_TRUE((str = getFirstKNonRepeatingChars_usingPriorityQueue_1("aabcde", 2)) == "bc", str)
		MM_EXPECT_TRUE((str = getFirstKNonRepeatingChars_usingPriorityQueue_1("aabbcde", 3)) == "cde", str)
		MM_EXPECT_TRUE((str = getFirstKNonRepeatingChars_usingPriorityQueue_1("abcdeabc", 10)) == "de", str)
		MM_EXPECT_TRUE((str = getFirstKNonRepeatingChars_usingPriorityQueue_1("ab ac dbeced", 4)) == "", str)
		MM_EXPECT_TRUE((str = getFirstKNonRepeatingChars_usingPriorityQueue_1("abcdeedcb", 50)) == "a", str)
	}

//Solution 1: Optimized version of above solution:
	// Instead if keeping pointers in heap, just keep indexes in heap
	// Instead of pushing all chars in heap, push only k and compare k + 1 element onwards with top of heap, and then replace the top if required

	string getFirstKNonRepeatingChars_usingPriorityQueue_2(const string& str, int n)
	{
		std::vector<std::unique_ptr<charInfo2>> hashTable(256); // All pointers are nullptr
		for (int i = 0; i < str.size(); ++i)
		{
			if (hashTable[str[i]] == nullptr)
				hashTable[str[i]] = std::make_unique<charInfo2>(0, -1);
			hashTable[str[i]]->count += 1;
			if(hashTable[str[i]]->index == -1)
				hashTable[str[i]]->index = i;
		}

		// build a max heap
		std::priority_queue<int, std::vector<int>, std::less<int>> firstKChars;
		// to build a mean heap, use std::greater<int> as shown below
		// std::priority_queue<int, std::vector<int>, std::greater<int> firstKChars;

		for (int i = 0; i < 256; ++i)
		{
			if (hashTable[i] == nullptr || hashTable[i]->count != 1)
				continue;

			if (firstKChars.size() < n)
				firstKChars.push(hashTable[i]->index);
			else
			{
				if (hashTable[i]->index < firstKChars.top())
				{
					firstKChars.pop();
					firstKChars.push(hashTable[i]->index);
				}
			}
		}

		string retval;
		while (!firstKChars.empty())
		{
			retval = str[firstKChars.top()] + retval;
			firstKChars.pop();
		}

		return retval;
	}

	MM_DECLARE_FLAG(String_FirstKNonRepeatingChars_optimized)

	MM_UNIT_TEST(String_FirstKNonRepeatingChars_test_2, String_FirstKNonRepeatingChars_optimized)
	{
		string str;
		MM_EXPECT_TRUE((str = getFirstKNonRepeatingChars_usingPriorityQueue_2("", 10)) == "", str)
		MM_EXPECT_TRUE((str = getFirstKNonRepeatingChars_usingPriorityQueue_2("abcde", 1)) == "a", str)
		MM_EXPECT_TRUE((str = getFirstKNonRepeatingChars_usingPriorityQueue_2("aabcde", 2)) == "bc", str)
		MM_EXPECT_TRUE((str = getFirstKNonRepeatingChars_usingPriorityQueue_2("aabbcde", 3)) == "cde", str)
		MM_EXPECT_TRUE((str = getFirstKNonRepeatingChars_usingPriorityQueue_2("abcdeabc", 10)) == "de", str)
		MM_EXPECT_TRUE((str = getFirstKNonRepeatingChars_usingPriorityQueue_2("ab ac dbeced", 4)) == "", str)
		MM_EXPECT_TRUE((str = getFirstKNonRepeatingChars_usingPriorityQueue_2("abcdeedcb", 50)) == "a", str)
	}

}

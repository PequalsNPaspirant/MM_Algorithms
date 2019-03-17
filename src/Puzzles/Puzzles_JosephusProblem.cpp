#include <iostream>
using namespace std;

#include "Puzzles/Puzzles_JosephusProblem.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	/*
	Problem description: There are n people standing in a circle indexed from 0 to n-1, 
	the person with index s has a sword, he kills the next kth person i.e. with index (s + k) and passes the sword on to next person.
	The above process goes on until there is only one person alive. The below algorithm finds the person who is alive at the end.
	*/

	/*
	Time complexity: O(n(k+n))
	*/
	unsigned int JosephusProblem_naive_v1(unsigned int n, unsigned int k, unsigned int s)
	{
		//invalid case
		if (n == 0 || s >= n)
			return 0; //ideally we should return -1 i.e. the invalid index

		unsigned int start = s; //Initially the sword is present at s'th person
		vector<bool> all(n, true);
		for(int i = 1; i < n; ++i) //skip 0 because we want 1 person remaining at the end
		{
			for (int j = 0; j < k;)
			{
				start = (++start % n);
				if (all[start] == true)
					++j;
			}

			all[start] = false; //kill this person
			while(all[++start %= n] == false); //pass the sword to next alive person
		}

		return start;
	}

	/*
	Recurrence relation:
	Note: s is always less than n. k can be anything from 0 to infinity

	f(n, k, s) = (s + f(n, k, 0))              ---Rotate the circle of indices (by s indices) to bring the index 0 at the person with sword,
	                                           ---then complete the process and rotate the circle of indices in opposite direction (by s indices)
	//Make sure s is always less than n
	f(n, k, 0) = f(n-1, k, (k + 1) % n)        ---When first person is killed, the problem remaing is f(n-1, k, 0), so the survivor in this case
	                                           ---is the actual survivor, but it starts at (k + 1)st position, so we need to adjust the index
	                                           ---by adding (1 + k) to solution of f(n-1, k, 0)
	f(1, k, s) = 0                             ---If there is only one person, he is the survivor

	Time complexity: O(n)
	*/
	unsigned int JosephusProblem_DP_topdown_recursive_v1_helper(unsigned int n, unsigned int k, unsigned int s, const unsigned int orig_n)
	{
		if (n == 1)
			return s;

		return JosephusProblem_DP_topdown_recursive_v1_helper(n - 1, k, (s + k + 1) % orig_n, orig_n);
	}

	unsigned int JosephusProblem_DP_topdown_recursive_v1(unsigned int n, unsigned int k, unsigned int s)
	{
		//invalid case
		if (n == 0 || s >= n)
		//if(n == 0)
			return 0; //ideally we should return -1 i.e. the invalid index

		return JosephusProblem_DP_topdown_recursive_v1_helper(n, k, s, n);
	}

	/*
	Slightly different and un-necessarily complex Recurrence relation: 
	f(n, k, s) = (s + f(n, k, 0)) % n          ---Rotate the circle of indices (by s indices) to bring the index 0 at the person with sword, 
	                                           ---then complete the process and rotate the circle of indices in opposite direction (by s indices)
	f(n, k, 0) = ((k + 1) + f(n-1, k, 0)) % n  ---When first person is killed, the problem remaing is f(n-1, k, 0), so the survivor in this case 
	                                           ---is the actual survivor, but it starts at (k + 1)st position, so we need to adjust the index 
											   ---by adding (1 + k) to solution of f(n-1, k, 0)
	f(1, k, s) = 0                             ---If there is only one person, he is the survivor

	Time complexity: O(n)
	*/
	unsigned int JosephusProblem_DP_topdown_recursive_v2(unsigned int n, unsigned int k, unsigned int s)
	{
		//invalid case
		//if (n == 0 || s >= n) //recursively n goes on decreasing, but index s can be anything from 0 to original n
		if (n == 0)
			return 0; //ideally we should return -1 i.e. the invalid index

		if (n == 1 && s == 0)
			return 0;

		if (s > 0)
			return (s + JosephusProblem_DP_topdown_recursive_v2(n, k, 0)) % n;

		return ((k + 1) + JosephusProblem_DP_topdown_recursive_v2(n - 1, k, 0)) % n ;
	}

	/*
	Time complexity: O(n)
	*/
	unsigned int JosephusProblem_DP_bottomup_v1(unsigned int n, unsigned int k, unsigned int s)
	{
		//invalid case
		if (n == 0 || s >= n)
			return 0; //ideally we should return -1 i.e. the invalid index

		unsigned int retVal = 0;
		for (int current_n = 2; current_n <= n; ++current_n)
		{
			retVal = ((k + 1) + retVal) % current_n;
		}

		return (s + retVal) % n;
	}

	

	MM_DECLARE_FLAG(Puzzles_JosephusProblem);

	void generateTestCases()
	{
		int maxPersons = 5;
		for (int k = 0; k < 2 * maxPersons; ++k)
		{
			cout << "\n//{ n, k, s, expectedResult }";
			for (int n = 1; n <= maxPersons; ++n)
			{
				for (int s = 0; s < n; ++s)
				{
					unsigned int actualResult = JosephusProblem_naive_v1(n, k, s);
					cout << "\n{ " << n << ", " << k << ", " << s << ", " << actualResult << " },";
				}
				cout << "\n";
			}
			cout << "\n";
		}
	}

	MM_UNIT_TEST(Puzzles_JosephusProblem_test_1, Puzzles_JosephusProblem)
	{
		//Generate test cases
		//return generateTestCases();

		struct testDataStruct
		{
			unsigned int n;
			unsigned int k;
			unsigned int s;
			unsigned int expectedResult;
		};

		std::vector<testDataStruct> testData{
	  //{ n, k, s, expectedResult }
		{ 1, 0, 0, 0 },

		{ 2, 0, 0, 1 },
		{ 2, 0, 1, 0 },

		{ 3, 0, 0, 2 },
		{ 3, 0, 1, 0 },
		{ 3, 0, 2, 1 },

		{ 4, 0, 0, 3 },
		{ 4, 0, 1, 0 },
		{ 4, 0, 2, 1 },
		{ 4, 0, 3, 2 },

		{ 5, 0, 0, 4 },
		{ 5, 0, 1, 0 },
		{ 5, 0, 2, 1 },
		{ 5, 0, 3, 2 },
		{ 5, 0, 4, 3 },


	  //{ n, k, s, expectedResult }
		{ 1, 1, 0, 0 },

		{ 2, 1, 0, 0 },
		{ 2, 1, 1, 1 },

		{ 3, 1, 0, 2 },
		{ 3, 1, 1, 0 },
		{ 3, 1, 2, 1 },

		{ 4, 1, 0, 0 },
		{ 4, 1, 1, 1 },
		{ 4, 1, 2, 2 },
		{ 4, 1, 3, 3 },

		{ 5, 1, 0, 2 },
		{ 5, 1, 1, 3 },
		{ 5, 1, 2, 4 },
		{ 5, 1, 3, 0 },
		{ 5, 1, 4, 1 },


	  //{ n, k, s, expectedResult }
		{ 1, 2, 0, 0 },

		{ 2, 2, 0, 1 },
		{ 2, 2, 1, 0 },

		{ 3, 2, 0, 1 },
		{ 3, 2, 1, 2 },
		{ 3, 2, 2, 0 },

		{ 4, 2, 0, 0 },
		{ 4, 2, 1, 1 },
		{ 4, 2, 2, 2 },
		{ 4, 2, 3, 3 },

		{ 5, 2, 0, 3 },
		{ 5, 2, 1, 4 },
		{ 5, 2, 2, 0 },
		{ 5, 2, 3, 1 },
		{ 5, 2, 4, 2 },


	  //{ n, k, s, expectedResult }
		{ 1, 3, 0, 0 },

		{ 2, 3, 0, 0 },
		{ 2, 3, 1, 1 },

		{ 3, 3, 0, 1 },
		{ 3, 3, 1, 2 },
		{ 3, 3, 2, 0 },

		{ 4, 3, 0, 1 },
		{ 4, 3, 1, 2 },
		{ 4, 3, 2, 3 },
		{ 4, 3, 3, 0 },

		{ 5, 3, 0, 0 },
		{ 5, 3, 1, 1 },
		{ 5, 3, 2, 2 },
		{ 5, 3, 3, 3 },
		{ 5, 3, 4, 4 },


	  //{ n, k, s, expectedResult }
		{ 1, 4, 0, 0 },

		{ 2, 4, 0, 1 },
		{ 2, 4, 1, 0 },

		{ 3, 4, 0, 0 },
		{ 3, 4, 1, 1 },
		{ 3, 4, 2, 2 },

		{ 4, 4, 0, 1 },
		{ 4, 4, 1, 2 },
		{ 4, 4, 2, 3 },
		{ 4, 4, 3, 0 },

		{ 5, 4, 0, 1 },
		{ 5, 4, 1, 2 },
		{ 5, 4, 2, 3 },
		{ 5, 4, 3, 4 },
		{ 5, 4, 4, 0 },


	  //{ n, k, s, expectedResult }
		{ 1, 5, 0, 0 },

		{ 2, 5, 0, 0 },
		{ 2, 5, 1, 1 },

		{ 3, 5, 0, 0 },
		{ 3, 5, 1, 1 },
		{ 3, 5, 2, 2 },

		{ 4, 5, 0, 2 },
		{ 4, 5, 1, 3 },
		{ 4, 5, 2, 0 },
		{ 4, 5, 3, 1 },

		{ 5, 5, 0, 3 },
		{ 5, 5, 1, 4 },
		{ 5, 5, 2, 0 },
		{ 5, 5, 3, 1 },
		{ 5, 5, 4, 2 },


	  //{ n, k, s, expectedResult }
		{ 1, 6, 0, 0 },

		{ 2, 6, 0, 1 },
		{ 2, 6, 1, 0 },

		{ 3, 6, 0, 2 },
		{ 3, 6, 1, 0 },
		{ 3, 6, 2, 1 },

		{ 4, 6, 0, 1 },
		{ 4, 6, 1, 2 },
		{ 4, 6, 2, 3 },
		{ 4, 6, 3, 0 },

		{ 5, 6, 0, 3 },
		{ 5, 6, 1, 4 },
		{ 5, 6, 2, 0 },
		{ 5, 6, 3, 1 },
		{ 5, 6, 4, 2 },


	  //{ n, k, s, expectedResult }
		{ 1, 7, 0, 0 },

		{ 2, 7, 0, 0 },
		{ 2, 7, 1, 1 },

		{ 3, 7, 0, 2 },
		{ 3, 7, 1, 0 },
		{ 3, 7, 2, 1 },

		{ 4, 7, 0, 2 },
		{ 4, 7, 1, 3 },
		{ 4, 7, 2, 0 },
		{ 4, 7, 3, 1 },

		{ 5, 7, 0, 0 },
		{ 5, 7, 1, 1 },
		{ 5, 7, 2, 2 },
		{ 5, 7, 3, 3 },
		{ 5, 7, 4, 4 },


	  //{ n, k, s, expectedResult }
		{ 1, 8, 0, 0 },

		{ 2, 8, 0, 1 },
		{ 2, 8, 1, 0 },

		{ 3, 8, 0, 1 },
		{ 3, 8, 1, 2 },
		{ 3, 8, 2, 0 },

		{ 4, 8, 0, 2 },
		{ 4, 8, 1, 3 },
		{ 4, 8, 2, 0 },
		{ 4, 8, 3, 1 },

		{ 5, 8, 0, 1 },
		{ 5, 8, 1, 2 },
		{ 5, 8, 2, 3 },
		{ 5, 8, 3, 4 },
		{ 5, 8, 4, 0 },


	  //{ n, k, s, expectedResult }
		{ 1, 9, 0, 0 },

		{ 2, 9, 0, 0 },
		{ 2, 9, 1, 1 },

		{ 3, 9, 0, 1 },
		{ 3, 9, 1, 2 },
		{ 3, 9, 2, 0 },

		{ 4, 9, 0, 3 },
		{ 4, 9, 1, 0 },
		{ 4, 9, 2, 1 },
		{ 4, 9, 3, 2 },

		{ 5, 9, 0, 3 },
		{ 5, 9, 1, 4 },
		{ 5, 9, 2, 0 },
		{ 5, 9, 3, 1 },
		{ 5, 9, 4, 2 }
		};

		for (int i = 0; i < testData.size(); ++i)
		{
			unsigned int actualResult;
			MM_EXPECT_TRUE((actualResult = JosephusProblem_naive_v1               (testData[i].n, testData[i].k, testData[i].s)) == testData[i].expectedResult, testData[i].n, testData[i].k, testData[i].s, testData[i].expectedResult, actualResult);
			//MM_EXPECT_TRUE((actualResult = JosephusProblem_DP_topdown_recursive_v1(testData[i].n, testData[i].k, testData[i].s)) == testData[i].expectedResult, testData[i].n, testData[i].k, testData[i].s, testData[i].expectedResult, actualResult);
			MM_EXPECT_TRUE((actualResult = JosephusProblem_DP_topdown_recursive_v2(testData[i].n, testData[i].k, testData[i].s)) == testData[i].expectedResult, testData[i].n, testData[i].k, testData[i].s, testData[i].expectedResult, actualResult);
			MM_EXPECT_TRUE((actualResult = JosephusProblem_DP_bottomup_v1         (testData[i].n, testData[i].k, testData[i].s)) == testData[i].expectedResult, testData[i].n, testData[i].k, testData[i].s, testData[i].expectedResult, actualResult);
		}		
	}
}
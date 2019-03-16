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
		if (n == 0)
			return 0; //ideally we should return -1 i.e. the invalid index

		unsigned int start = s % n; //Initially the sword is present at first person
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
	f(n, k, s) = (s + f(n, k, 0)) % n          ---Rotate the circle of indices (by s indices) to bring the index 0 at the person with sword, 
	                                           ---then complete the process and rotate the circle of indices in opposite direction (by s indices)
	f(n, k, 0) = (1 + k + f(n-1, k, 0)) % n    ---
	f(1, k, 0) = 0

	Time complexity: O(n)
	*/
	unsigned int JosephusProblem_DP_topdown_recursive_v1(unsigned int n, unsigned int k, unsigned int s)
	{
		//invalid case
		if (n == 0)
			return 0; //ideally we should return -1 i.e. the invalid index

		if (n == 1 && s == 0)
			return 0;

		if (s > 0)
			return (s + JosephusProblem_DP_topdown_recursive_v1(n, k, 0)) % n;

		return (1 + k + JosephusProblem_DP_topdown_recursive_v1(n - 1, k, 0)) % n ;
	}

	unsigned int JosephusProblem_DP_topdown_v2(unsigned int n, unsigned int k, unsigned int count, unsigned int start)
	{
		//return JosephusProblem_DP_v1(n, k + 1, (count > (n - start) ? count - (n - start) / k : 0), k - n % k);

		if (count == 1)
			return start;

		//start = start + k + 1;
		//k += start / n;
		//start %= n;
		return JosephusProblem_DP_topdown_v2(n, k, count - 1, (start + k + 1));
	}

	MM_DECLARE_FLAG(Puzzles_JosephusProblem);

	void generateTestCases()
	{
		int maxPersons = 5;
		for (int k = 0; k < 2 * maxPersons; ++k)
		{
			cout << "\n//{ n, k, s, expectedResult }";
			for (int n = 0; n <= maxPersons; ++n)
			{
				for (int s = 0; s < maxPersons; ++s)
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
		//generateTestCases();

		struct testDataStruct
		{
			unsigned int n;
			unsigned int k;
			unsigned int s;
			unsigned int result;
		};

		std::vector<testDataStruct> testData{
		//{ n, k, s, expectedResult }
		{ 0, 0, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 2, 0 },
		{ 0, 0, 3, 0 },
		{ 0, 0, 4, 0 },

		{ 1, 0, 0, 0 },
		{ 1, 0, 1, 0 },
		{ 1, 0, 2, 0 },
		{ 1, 0, 3, 0 },
		{ 1, 0, 4, 0 },

		{ 2, 0, 0, 1 },
		{ 2, 0, 1, 0 },
		{ 2, 0, 2, 1 },
		{ 2, 0, 3, 0 },
		{ 2, 0, 4, 1 },

		{ 3, 0, 0, 2 },
		{ 3, 0, 1, 0 },
		{ 3, 0, 2, 1 },
		{ 3, 0, 3, 2 },
		{ 3, 0, 4, 0 },

		{ 4, 0, 0, 3 },
		{ 4, 0, 1, 0 },
		{ 4, 0, 2, 1 },
		{ 4, 0, 3, 2 },
		{ 4, 0, 4, 3 },

		{ 5, 0, 0, 4 },
		{ 5, 0, 1, 0 },
		{ 5, 0, 2, 1 },
		{ 5, 0, 3, 2 },
		{ 5, 0, 4, 3 },


		//{ n, k, s, expectedResult }
		{ 0, 1, 0, 0 },
		{ 0, 1, 1, 0 },
		{ 0, 1, 2, 0 },
		{ 0, 1, 3, 0 },
		{ 0, 1, 4, 0 },

		{ 1, 1, 0, 0 },
		{ 1, 1, 1, 0 },
		{ 1, 1, 2, 0 },
		{ 1, 1, 3, 0 },
		{ 1, 1, 4, 0 },

		{ 2, 1, 0, 0 },
		{ 2, 1, 1, 1 },
		{ 2, 1, 2, 0 },
		{ 2, 1, 3, 1 },
		{ 2, 1, 4, 0 },

		{ 3, 1, 0, 2 },
		{ 3, 1, 1, 0 },
		{ 3, 1, 2, 1 },
		{ 3, 1, 3, 2 },
		{ 3, 1, 4, 0 },

		{ 4, 1, 0, 0 },
		{ 4, 1, 1, 1 },
		{ 4, 1, 2, 2 },
		{ 4, 1, 3, 3 },
		{ 4, 1, 4, 0 },

		{ 5, 1, 0, 2 },
		{ 5, 1, 1, 3 },
		{ 5, 1, 2, 4 },
		{ 5, 1, 3, 0 },
		{ 5, 1, 4, 1 },


		//{ n, k, s, expectedResult }
		{ 0, 2, 0, 0 },
		{ 0, 2, 1, 0 },
		{ 0, 2, 2, 0 },
		{ 0, 2, 3, 0 },
		{ 0, 2, 4, 0 },

		{ 1, 2, 0, 0 },
		{ 1, 2, 1, 0 },
		{ 1, 2, 2, 0 },
		{ 1, 2, 3, 0 },
		{ 1, 2, 4, 0 },

		{ 2, 2, 0, 1 },
		{ 2, 2, 1, 0 },
		{ 2, 2, 2, 1 },
		{ 2, 2, 3, 0 },
		{ 2, 2, 4, 1 },

		{ 3, 2, 0, 1 },
		{ 3, 2, 1, 2 },
		{ 3, 2, 2, 0 },
		{ 3, 2, 3, 1 },
		{ 3, 2, 4, 2 },

		{ 4, 2, 0, 0 },
		{ 4, 2, 1, 1 },
		{ 4, 2, 2, 2 },
		{ 4, 2, 3, 3 },
		{ 4, 2, 4, 0 },

		{ 5, 2, 0, 3 },
		{ 5, 2, 1, 4 },
		{ 5, 2, 2, 0 },
		{ 5, 2, 3, 1 },
		{ 5, 2, 4, 2 },


		//{ n, k, s, expectedResult }
		{ 0, 3, 0, 0 },
		{ 0, 3, 1, 0 },
		{ 0, 3, 2, 0 },
		{ 0, 3, 3, 0 },
		{ 0, 3, 4, 0 },

		{ 1, 3, 0, 0 },
		{ 1, 3, 1, 0 },
		{ 1, 3, 2, 0 },
		{ 1, 3, 3, 0 },
		{ 1, 3, 4, 0 },

		{ 2, 3, 0, 0 },
		{ 2, 3, 1, 1 },
		{ 2, 3, 2, 0 },
		{ 2, 3, 3, 1 },
		{ 2, 3, 4, 0 },

		{ 3, 3, 0, 1 },
		{ 3, 3, 1, 2 },
		{ 3, 3, 2, 0 },
		{ 3, 3, 3, 1 },
		{ 3, 3, 4, 2 },

		{ 4, 3, 0, 1 },
		{ 4, 3, 1, 2 },
		{ 4, 3, 2, 3 },
		{ 4, 3, 3, 0 },
		{ 4, 3, 4, 1 },

		{ 5, 3, 0, 0 },
		{ 5, 3, 1, 1 },
		{ 5, 3, 2, 2 },
		{ 5, 3, 3, 3 },
		{ 5, 3, 4, 4 },


		//{ n, k, s, expectedResult }
		{ 0, 4, 0, 0 },
		{ 0, 4, 1, 0 },
		{ 0, 4, 2, 0 },
		{ 0, 4, 3, 0 },
		{ 0, 4, 4, 0 },

		{ 1, 4, 0, 0 },
		{ 1, 4, 1, 0 },
		{ 1, 4, 2, 0 },
		{ 1, 4, 3, 0 },
		{ 1, 4, 4, 0 },

		{ 2, 4, 0, 1 },
		{ 2, 4, 1, 0 },
		{ 2, 4, 2, 1 },
		{ 2, 4, 3, 0 },
		{ 2, 4, 4, 1 },

		{ 3, 4, 0, 0 },
		{ 3, 4, 1, 1 },
		{ 3, 4, 2, 2 },
		{ 3, 4, 3, 0 },
		{ 3, 4, 4, 1 },

		{ 4, 4, 0, 1 },
		{ 4, 4, 1, 2 },
		{ 4, 4, 2, 3 },
		{ 4, 4, 3, 0 },
		{ 4, 4, 4, 1 },

		{ 5, 4, 0, 1 },
		{ 5, 4, 1, 2 },
		{ 5, 4, 2, 3 },
		{ 5, 4, 3, 4 },
		{ 5, 4, 4, 0 },


		//{ n, k, s, expectedResult }
		{ 0, 5, 0, 0 },
		{ 0, 5, 1, 0 },
		{ 0, 5, 2, 0 },
		{ 0, 5, 3, 0 },
		{ 0, 5, 4, 0 },

		{ 1, 5, 0, 0 },
		{ 1, 5, 1, 0 },
		{ 1, 5, 2, 0 },
		{ 1, 5, 3, 0 },
		{ 1, 5, 4, 0 },

		{ 2, 5, 0, 0 },
		{ 2, 5, 1, 1 },
		{ 2, 5, 2, 0 },
		{ 2, 5, 3, 1 },
		{ 2, 5, 4, 0 },

		{ 3, 5, 0, 0 },
		{ 3, 5, 1, 1 },
		{ 3, 5, 2, 2 },
		{ 3, 5, 3, 0 },
		{ 3, 5, 4, 1 },

		{ 4, 5, 0, 2 },
		{ 4, 5, 1, 3 },
		{ 4, 5, 2, 0 },
		{ 4, 5, 3, 1 },
		{ 4, 5, 4, 2 },

		{ 5, 5, 0, 3 },
		{ 5, 5, 1, 4 },
		{ 5, 5, 2, 0 },
		{ 5, 5, 3, 1 },
		{ 5, 5, 4, 2 },


		//{ n, k, s, expectedResult }
		{ 0, 6, 0, 0 },
		{ 0, 6, 1, 0 },
		{ 0, 6, 2, 0 },
		{ 0, 6, 3, 0 },
		{ 0, 6, 4, 0 },

		{ 1, 6, 0, 0 },
		{ 1, 6, 1, 0 },
		{ 1, 6, 2, 0 },
		{ 1, 6, 3, 0 },
		{ 1, 6, 4, 0 },

		{ 2, 6, 0, 1 },
		{ 2, 6, 1, 0 },
		{ 2, 6, 2, 1 },
		{ 2, 6, 3, 0 },
		{ 2, 6, 4, 1 },

		{ 3, 6, 0, 2 },
		{ 3, 6, 1, 0 },
		{ 3, 6, 2, 1 },
		{ 3, 6, 3, 2 },
		{ 3, 6, 4, 0 },

		{ 4, 6, 0, 1 },
		{ 4, 6, 1, 2 },
		{ 4, 6, 2, 3 },
		{ 4, 6, 3, 0 },
		{ 4, 6, 4, 1 },

		{ 5, 6, 0, 3 },
		{ 5, 6, 1, 4 },
		{ 5, 6, 2, 0 },
		{ 5, 6, 3, 1 },
		{ 5, 6, 4, 2 },


		//{ n, k, s, expectedResult }
		{ 0, 7, 0, 0 },
		{ 0, 7, 1, 0 },
		{ 0, 7, 2, 0 },
		{ 0, 7, 3, 0 },
		{ 0, 7, 4, 0 },

		{ 1, 7, 0, 0 },
		{ 1, 7, 1, 0 },
		{ 1, 7, 2, 0 },
		{ 1, 7, 3, 0 },
		{ 1, 7, 4, 0 },

		{ 2, 7, 0, 0 },
		{ 2, 7, 1, 1 },
		{ 2, 7, 2, 0 },
		{ 2, 7, 3, 1 },
		{ 2, 7, 4, 0 },

		{ 3, 7, 0, 2 },
		{ 3, 7, 1, 0 },
		{ 3, 7, 2, 1 },
		{ 3, 7, 3, 2 },
		{ 3, 7, 4, 0 },

		{ 4, 7, 0, 2 },
		{ 4, 7, 1, 3 },
		{ 4, 7, 2, 0 },
		{ 4, 7, 3, 1 },
		{ 4, 7, 4, 2 },

		{ 5, 7, 0, 0 },
		{ 5, 7, 1, 1 },
		{ 5, 7, 2, 2 },
		{ 5, 7, 3, 3 },
		{ 5, 7, 4, 4 },


		//{ n, k, s, expectedResult }
		{ 0, 8, 0, 0 },
		{ 0, 8, 1, 0 },
		{ 0, 8, 2, 0 },
		{ 0, 8, 3, 0 },
		{ 0, 8, 4, 0 },

		{ 1, 8, 0, 0 },
		{ 1, 8, 1, 0 },
		{ 1, 8, 2, 0 },
		{ 1, 8, 3, 0 },
		{ 1, 8, 4, 0 },

		{ 2, 8, 0, 1 },
		{ 2, 8, 1, 0 },
		{ 2, 8, 2, 1 },
		{ 2, 8, 3, 0 },
		{ 2, 8, 4, 1 },

		{ 3, 8, 0, 1 },
		{ 3, 8, 1, 2 },
		{ 3, 8, 2, 0 },
		{ 3, 8, 3, 1 },
		{ 3, 8, 4, 2 },

		{ 4, 8, 0, 2 },
		{ 4, 8, 1, 3 },
		{ 4, 8, 2, 0 },
		{ 4, 8, 3, 1 },
		{ 4, 8, 4, 2 },

		{ 5, 8, 0, 1 },
		{ 5, 8, 1, 2 },
		{ 5, 8, 2, 3 },
		{ 5, 8, 3, 4 },
		{ 5, 8, 4, 0 },


		//{ n, k, s, expectedResult }
		{ 0, 9, 0, 0 },
		{ 0, 9, 1, 0 },
		{ 0, 9, 2, 0 },
		{ 0, 9, 3, 0 },
		{ 0, 9, 4, 0 },

		{ 1, 9, 0, 0 },
		{ 1, 9, 1, 0 },
		{ 1, 9, 2, 0 },
		{ 1, 9, 3, 0 },
		{ 1, 9, 4, 0 },

		{ 2, 9, 0, 0 },
		{ 2, 9, 1, 1 },
		{ 2, 9, 2, 0 },
		{ 2, 9, 3, 1 },
		{ 2, 9, 4, 0 },

		{ 3, 9, 0, 1 },
		{ 3, 9, 1, 2 },
		{ 3, 9, 2, 0 },
		{ 3, 9, 3, 1 },
		{ 3, 9, 4, 2 },

		{ 4, 9, 0, 3 },
		{ 4, 9, 1, 0 },
		{ 4, 9, 2, 1 },
		{ 4, 9, 3, 2 },
		{ 4, 9, 4, 3 },

		{ 5, 9, 0, 3 },
		{ 5, 9, 1, 4 },
		{ 5, 9, 2, 0 },
		{ 5, 9, 3, 1 },
		{ 5, 9, 4, 2 }
		};

		for (int i = 0; i < testData.size(); ++i)
		{
			unsigned int actualResult;
			MM_EXPECT_TRUE((actualResult = JosephusProblem_naive_v1(testData[i].n, testData[i].k, testData[i].s)) == testData[i].result, testData[i].n, testData[i].k, testData[i].result, actualResult);
			MM_EXPECT_TRUE((actualResult = JosephusProblem_DP_topdown_recursive_v1(testData[i].n, testData[i].k, testData[i].s)) == testData[i].result, testData[i].n, testData[i].k, testData[i].result, actualResult);
		}		
	}
}
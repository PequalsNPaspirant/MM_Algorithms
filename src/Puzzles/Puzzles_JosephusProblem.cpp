#include <iostream>
using namespace std;

#include "Puzzles/Puzzles_JosephusProblem.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	unsigned int JosephusProblem_naive_v1(const unsigned int n, const unsigned int k)
	{
		unsigned int next = 0; //Initially the sword is present at first person
		vector<bool> all(n, true);
		for(int i = 1; i < n; ++i) //start with 1 because we want 1 person remaining at the end
		{
			for (int j = 0; j < k;)
			{
				next = (++next % n);
				if (all[next] == true)
					++j;
			}

			all[next] = false; //kill this person
			while(all[++next %= n] == false); //pass the sword to next alive person
		}

		return next;
	}

	MM_DECLARE_FLAG(Puzzles_JosephusProblem);

	MM_UNIT_TEST(Puzzles_JosephusProblem_test_1, Puzzles_JosephusProblem)
	{
		struct testDataStruct
		{
			unsigned int n;
			unsigned int k;
			unsigned int result;
		};

		std::vector<testDataStruct> testData{
			{ 10, 0, 9 },
			{ 10, 1, 4 },
			{ 10, 2, 3 },
			{ 10, 3, 4 },
			{ 10, 4, 2 },
			{ 10, 5, 2 },
			{ 10, 6, 8 },
			{ 10, 7, 0 },
			{ 10, 8, 6 },
			{ 10, 9, 7 },
			{ 10, 10, 6 },

			{ 10, 11, 9 },
			{ 10, 12, 4 },
			{ 10, 13, 6 },
			{ 10, 14, 5 },
			{ 10, 15, 6 },
			{ 10, 16, 2 },
			{ 10, 17, 4 },
			{ 10, 18, 1 },
			{ 10, 19, 0 },
			{ 10, 20, 9 },

			{ 10, 21, 9 },
			{ 10, 22, 9 },
			{ 10, 23, 0 },
			{ 10, 24, 5 },
			{ 10, 25, 8 },
			{ 10, 26, 4 },
			{ 10, 27, 5 },
			{ 10, 28, 4 },
			{ 10, 29, 5 },
			{ 10, 30, 1 },
		};

		for (int i = 0; i < testData.size(); ++i)
		{
			unsigned int actualResult;
			MM_EXPECT_TRUE((actualResult = JosephusProblem_naive_v1(testData[i].n, testData[i].k)) == testData[i].result, testData[i].n, testData[i].k, testData[i].result, actualResult);
		}		
	}
}
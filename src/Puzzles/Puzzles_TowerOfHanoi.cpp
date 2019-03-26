#include <stack>
#include <vector>
using namespace std;

#include "Puzzles/Puzzles_TowerOfHanoi.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"


/*
There are n disks. disk 1 is smallest and disk n is the largest disk.
There are three poles with index 1, 2 and 3.
*/

namespace mm
{
	/*
	This recursive function will be called 2^n times where n = numDisks
	Time Complexity: O(2^n)
	*/
	unsigned long long Puzzles_TowerOfHanoi_recursive_v1(int numDisks, int from, int to, int aux)
	{
		if (numDisks == 0)
			return 0;

		unsigned long long numMoves = 0;
		numMoves += Puzzles_TowerOfHanoi_recursive_v1(numDisks - 1, from, aux, to);
		//Move top disk: from -> to
		//We can display it on screen
		//cout << "Move disk " << numDisks << " from " << from << " to " << to;
		//OR we can store it in vector
		//vector<Moves> retVec; retVec.push_back(Moves{numDisks, from, to});
		numMoves += 1;
		numMoves += Puzzles_TowerOfHanoi_recursive_v1(numDisks - 1, aux, to, from);
		return numMoves;
	}

	/*
	This recursive function will be called n times where n = numDisks. It keeps previously calculated values in cache.
	Time Complexity: O(n)
	Space Complexity: O(n)
	*/
	unsigned long long Puzzles_TowerOfHanoi_DP_v1(int numDisks, int from, int to, int aux, vector<unsigned long long>& cache)
	{
		if (numDisks == 0)
			return 0;

		if(cache[numDisks] == 0)
		{
			unsigned long long numMoves = 0;
			numMoves += Puzzles_TowerOfHanoi_DP_v1(numDisks - 1, from, aux, to, cache);
			//Move top disk: from -> to
			numMoves += 1;
			numMoves += Puzzles_TowerOfHanoi_DP_v1(numDisks - 1, aux, to, from, cache);
			cache[numDisks] = numMoves;
		}

		return cache[numDisks];
	}

	// Test
	MM_DECLARE_FLAG(Puzzles_TowerOfHanoi_test);

	MM_UNIT_TEST(Puzzles_TowerOfHanoi_test_1, Puzzles_TowerOfHanoi_test)
	{
		//MM_PRINT_TEST_RESULTS_ON_SINGLE_LINE(true);
		MM_PRINT_TEST_RESULTS_ON_SINGLE_LINE(false);

		struct testData
		{
			int numDisks;
			unsigned long long expectedMoves;

		};

		vector<testData> data{
		{ 0,  0 },
		{ 1,  2 - 1 },
		{ 2,  4 - 1 },
		{ 3,  8 - 1 },
		{ 4,  16 - 1 },
		{ 5,  32 - 1 },
		{ 6,  64 - 1 },
		{ 7,  128 - 1 },
		{ 8,  256 - 1 },
		{ 9,  512 - 1 },
		{ 10, 1024 - 1 },
		{ 11, 2048 - 1 },
		{ 12, 4096 - 1 },
		{ 13, 8192 - 1 },
		{ 14, 16384 - 1 },
		{ 15, 32768 - 1 },
		{ 16, 65536 - 1 },
		{ 17, 131072 - 1 },
		{ 18, 262144 - 1 },
		{ 19, 524288 - 1 },
		{ 20, 1048576 - 1 },
		{ 21, 2097152 - 1 },
		{ 22, 4194304 - 1 },
		{ 23, 8388608 - 1 },
		{ 24, 16777216 - 1 },
		{ 25, 33554432 - 1 },
		{ 26, 67108864 - 1 },
		{ 27, 134217728 - 1 },
		{ 28, 268435456 - 1 },
		{ 29, 536870912 - 1 },
		{ 30, 1073741824 - 1 },
		{ 31, 2147483648 - 1 },

		{ 50, 1125899906842624 - 1 },
		{ 60, 1152921504606846976 - 1 },
		{ 64, numeric_limits<unsigned long long>::max() },
		};

		for (int i = 0; i < data.size(); ++i)
		{
			unsigned long long actualMoves = 0;
			if(i <= 25)
			{
				MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_recursive_v1(data[i].numDisks, 1, 2, 3)) == data[i].expectedMoves,
					data[i].numDisks, actualMoves, data[i].expectedMoves);
			}

			vector<unsigned long long> cache(data[i].numDisks + 1, 0);
			MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_DP_v1(data[i].numDisks, 1, 2, 3, cache)) == data[i].expectedMoves,
				data[i].numDisks, actualMoves, data[i].expectedMoves);
		}
	}
}
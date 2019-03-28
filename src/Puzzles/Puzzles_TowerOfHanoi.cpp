#include <stack>
#include <vector>
#include <cassert>
using namespace std;

#include "Puzzles/Puzzles_TowerOfHanoi.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"


/*
There are n disks. disk 1 is smallest and disk n is the largest disk.
There are three poles with index 1, 2 and 3.
The number of moves required is: 2^n – 1
*/

namespace mm
{
	/*
	This recursive function will be called 2^n times where n = numDisks
	Time Complexity: O(2^n)
	*/
	unsigned long long Puzzles_TowerOfHanoi_getNumMoves_recursive_v1(int numDisks, int from, int to, int aux)
	{
		if (numDisks == 0)
			return 0;

		unsigned long long numMoves = 0;
		numMoves += Puzzles_TowerOfHanoi_getNumMoves_recursive_v1(numDisks - 1, from, aux, to);
		//Move top disk: from -> to
		//We can display it on screen
		//cout << "Move disk " << numDisks << " from " << from << " to " << to;
		numMoves += 1;
		numMoves += Puzzles_TowerOfHanoi_getNumMoves_recursive_v1(numDisks - 1, aux, to, from);
		return numMoves;
	}

	/*
	This recursive function will be called 2^n times where n = numDisks
	Time Complexity: O(2^n)
	*/
	unsigned long long Puzzles_TowerOfHanoi_recursive_v1(int numDisks, stack<int>& from, stack<int>& to, stack<int>& aux)
	{
		if (numDisks == 0)
			return 0;

		unsigned long long numMoves = 0;
		numMoves += Puzzles_TowerOfHanoi_recursive_v1(numDisks - 1, from, aux, to);
		//Move top disk: from -> to
		//We can display it on screen
		//cout << "Move disk " << numDisks << " from " << from << " to " << to;
		//OR we can store it in stack
		int disk = from.top();
		from.pop();
		assert(to.empty() || to.top() > disk);
		to.push(disk);
		numMoves += 1;
		numMoves += Puzzles_TowerOfHanoi_recursive_v1(numDisks - 1, aux, to, from);
		return numMoves;
	}

	/*
	This recursive function will be called n times where n = numDisks. It keeps previously calculated values in cache.
	Time Complexity: O(n)
	Space Complexity: O(n)
	*/
	unsigned long long Puzzles_TowerOfHanoi_getNumMoves_DP_topdown_v1(int numDisks, int from, int to, int aux, vector<unsigned long long>& cache)
	{
		if (numDisks == 0)
			return 0;

		if(cache[numDisks] == 0)
		{
			unsigned long long numMoves = 0;
			numMoves += Puzzles_TowerOfHanoi_getNumMoves_DP_topdown_v1(numDisks - 1, from, aux, to, cache);
			//Move top disk: from -> to
			numMoves += 1;
			numMoves += Puzzles_TowerOfHanoi_getNumMoves_DP_topdown_v1(numDisks - 1, aux, to, from, cache);
			cache[numDisks] = numMoves;
		}

		return cache[numDisks];
	}

	/*
	Time Complexity: O(n)
	Space Complexity: O(n)
	*/
	unsigned long long Puzzles_TowerOfHanoi_getNumMoves_DP_bottomup_v1(int numDisks, int from, int to, int aux, vector<unsigned long long>& cache)
	{
		return 0;
	}

	/*
	Time Complexity: O(1)
	Space Complexity: O(n)
	*/
	unsigned long long Puzzles_TowerOfHanoi_getNumMoves(int numDisks)
	{
		//double can not keep big integer part of values precisely
		//return pow(2, numDisks) - 1;

		//This function can calculate num moves for disks upto 64
		assert(numDisks <= sizeof(unsigned long long) * 8);

		if (numDisks < sizeof(unsigned long long) * 8)
			return (1ull << numDisks) - 1;
		else
			return numeric_limits<unsigned long long>::max();
	}

	/*
	Time Complexity: O(2^n)
	Space Complexity: O(n)
	*/
	unsigned long long Puzzles_TowerOfHanoi_binary_v1(int numDisks, int from, int to, int aux, vector<unsigned long long>& cache)
	{
		if (numDisks == 0)
			return 0;

		if (cache[numDisks] == 0)
		{

		}

		return cache[numDisks];
	}

	/*
	Time Complexity: O(2^n)
	Space Complexity: O(n)
	*/
	unsigned long long Puzzles_TowerOfHanoi_binary_v2(int numDisks, stack<int>& from, stack<int>& to, stack<int>& aux)
	{
		if (numDisks == 0)
			return 0;

		stack<int>* source = &from;
		stack<int>* destination = &to;
		stack<int>* auxiliary = &aux;
		//If number of disks is even, then interchange  
		//destination pole and auxiliary pole 
		if ((numDisks & 1) == 0)
		{
			stack<int>* temp = destination;
			destination = auxiliary;
			auxiliary = temp;
		}

		unsigned long long totalMoves = 0;
		if (numDisks < sizeof(unsigned long long) * 8)
			totalMoves = (1ull << numDisks) - 1;
		else
			totalMoves = numeric_limits<unsigned long long>::max();

		auto moveDisk = [](stack<int>* fromStack, stack<int>* toStack) {
			if (fromStack->empty() && toStack->empty())
				assert(false);
			else if (fromStack->empty())
			{
				fromStack->push(toStack->top());
				toStack->pop();
			}
			else if (toStack->empty())
			{
				toStack->push(fromStack->top());
				fromStack->pop();
			}
			else if (fromStack->top() < toStack->top())
			{
				toStack->push(fromStack->top());
				fromStack->pop();
			}
			else if (fromStack->top() > toStack->top())
			{
				fromStack->push(toStack->top());
				toStack->pop();
			}
		};

		for (int i = 1; i <= totalMoves; ++i)
		{
			if (i % 3 == 1)
				moveDisk(source, destination);
			else if (i % 3 == 2)
				moveDisk(source, auxiliary);
			else if (i % 3 == 0)
				moveDisk(auxiliary, destination);
		}

		return totalMoves;
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

		auto validate = [](stack<int>& st) -> bool {
			for (int j = 1; j <= st.size(); ++j)
				if (st.top() == j)
					st.pop();
				else
					return false;

			return true;
		};

		for (int i = 0; i < data.size(); ++i)
		{
			unsigned long long actualMoves = 0;
			if (data[i].numDisks <= 25)
			{
				MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_getNumMoves_recursive_v1(data[i].numDisks, 1, 2, 3)) == data[i].expectedMoves,
					data[i].numDisks, actualMoves, data[i].expectedMoves);
			}

			if(data[i].numDisks <= 18)
			{
				stack<int> from;
				stack<int> to;
				stack<int> aux;
				for (int j = data[i].numDisks; j > 0; --j)
					from.push(j);
				MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_recursive_v1(data[i].numDisks, from, to, aux)) == data[i].expectedMoves
					&& from.empty() && aux.empty() && to.size() == data[i].numDisks && validate(to),
					data[i].numDisks, actualMoves, data[i].expectedMoves);
			}

			vector<unsigned long long> cache(data[i].numDisks + 1, 0);
			MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_getNumMoves_DP_topdown_v1(data[i].numDisks, 1, 2, 3, cache)) == data[i].expectedMoves,
				data[i].numDisks, actualMoves, data[i].expectedMoves);

			//MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_getNumMoves_DP_bottomup_v1(data[i].numDisks, 1, 2, 3, cache)) == data[i].expectedMoves,
			//	data[i].numDisks, actualMoves, data[i].expectedMoves);

			MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_getNumMoves(data[i].numDisks)) == data[i].expectedMoves,
				data[i].numDisks, actualMoves, data[i].expectedMoves);

			//MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_binary_v1(data[i].numDisks, 1, 2, 3, cache)) == data[i].expectedMoves,
			//	data[i].numDisks, actualMoves, data[i].expectedMoves);

			if (data[i].numDisks <= 18)
			{
				stack<int> from;
				stack<int> to;
				stack<int> aux;
				for (int j = data[i].numDisks; j > 0; --j)
					from.push(j);
				MM_TIMED_EXPECT_TRUE((actualMoves = Puzzles_TowerOfHanoi_binary_v2(data[i].numDisks, from, to, aux)) == data[i].expectedMoves
					&& from.empty() && aux.empty() && to.size() == data[i].numDisks && validate(to),
					data[i].numDisks, actualMoves, data[i].expectedMoves);
			}

		}
	}
}
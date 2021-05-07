#include <iostream>

#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

//First Solution O(n!)

	void getWaysToClimbNSteps_recursively(size_t& count, size_t remainingSteps, size_t maxStepsClimbedAtOnce)
	{
		if (remainingSteps == 0)
		{
			++count;
			return;
		}

		for (size_t i = 1; i <= maxStepsClimbedAtOnce; ++i)
		{
			if (remainingSteps >= i)
				getWaysToClimbNSteps_recursively(count, remainingSteps - i, maxStepsClimbedAtOnce);
		}
	}

	size_t getWaysToClimbNSteps_recursively(size_t totalSteps, size_t maxStepsClimbedAtOnce)
	{
		// If a person can not climb steps, there are 0 ways
		if (maxStepsClimbedAtOnce == 0)
			return 0;

		size_t count = 0;
		getWaysToClimbNSteps_recursively(count, totalSteps, maxStepsClimbedAtOnce);
		return count;
	}


// Second Solution: Using Fibonnaci like property
//f(0, 0) = 0 // If a person can not climb steps (k = 0), there are 0 ways
//f(n, 0) = 0 // If a person can not climb steps (k = 0), there are 0 ways
//f(0, k) = 1 // The 0 steps can be climbed by doing nothing i.e. in one way
//f(1, k) = 1
//f(n, 1) = 1
//f(n, k) = fibonacci_summing_last_k_terms(n) when n > k
//f(n, n) = f(n, n - 1) + 1 // when n = k
//f(n, n + p) = f(n, n) // when n < k

	size_t getWaysToClimbNSteps_fibonacci_1(size_t n, size_t k)
	{
		if (k == 0) // If a person can not climb steps (k = 0), there are 0 ways
			return 0;
		else if (n == 0) // (k != 0) The 0 steps can be climbed by doing nothing i.e. in one way 
			return 1;
		else if (n == 1 || k == 1)
			return 1;
		else if (n == k) // n = k = 1 is also handled by this case
			return getWaysToClimbNSteps_fibonacci_1(n, k - 1) + 1;  // k is reduced by 1
		else if (n < k)
			return getWaysToClimbNSteps_fibonacci_1(n, n); // k is reduced by k - n
		
		// n > k
		size_t returnValue = 0;
		for (int i = 1; i <= k; ++i)
			returnValue += getWaysToClimbNSteps_fibonacci_1(n - i, k);
		return returnValue;
	}

// Third Solution: Using Fibonnaci like property in more efficient way
	int getWaysToClimbNSteps_fibonacci_2(int n, int k)
	{
		if (k == 0)
			return 0;
		if (n == 0 || n == 1)
			return 1;
		int res = 0;
		for (int i = 1; i <= k && i <= n; i++)
			res += getWaysToClimbNSteps_fibonacci_2(n - i, k);
		return res;
	}

// Fourth Solution: A fibonacci like algorithm as above, but implemented in bottom up approach using memoization
// Advantages: avoids recursion, avoids recalculation of same values
// Complexity: O(n*k)

	size_t getWaysToClimbNSteps_DP_1(size_t n_max, size_t k)
	{
		if (k == 0)
			return 0;

		vector<size_t> lastResults(n_max + 1, 0);
		lastResults[0] = 1;
		for (int n = 1; n <= n_max; ++n)
		{
			for (int i = 1; i <= k; ++i)
				if(i <= n) // to avoid array index going negative. Ignore terms with negative index. 
					       // It works because the first k terms of such fibonnacci like series are not dependent upon the terms before 0th term 
					lastResults[n] += lastResults[n - i];
		}

		return lastResults[n_max];
	}

// Fourth Solution: Same as above except, avoids large stack memory usage involved in memoization (very useful when n & k are large)
	size_t getWaysToClimbNSteps_DP_2(size_t n_max, size_t k)
	{
		// If a person can not climb steps (k = 0), there are 0 ways
		if (k == 0)
			return 0;

		// If n < k, we dont need to event find first k terms, first n terms are enough. And then return after that
		if (n_max < k)
			k = n_max + 1;

		vector<size_t> lastResults(k, 0);

		//Construct initial k terms
		lastResults[0] = 1;
		for (int i = 1; i < k; ++i)
			for (int j = i - 1; j >= 0; --j)
				lastResults[i] += lastResults[j];

		// No need to return explicitly, the loop is not executed in this case, so last return statement does it
		//if (n_max < k)
		//	return lastResults[n_max];

		for (size_t n = k; n <= n_max; ++n)
		{
			size_t result = 0;
			for (size_t i = k; i > 0; --i)
				result += lastResults[i - 1];

			// Update first k terms
			for (size_t i = 1; i < k; ++i)
				lastResults[i - 1] = lastResults[i];
			lastResults[k - 1] = result;
		}

		return lastResults[k - 1];
	}


	MM_DECLARE_FLAG(DP_WaysToClimbNSteps)

	MM_UNIT_TEST(DP_WaysToClimbNSteps_recursion, DP_WaysToClimbNSteps)
	{
		const int n_size = 11;
		const int k_size = 11;
		int expectedValues[n_size][k_size] =
		{
			 //k = 0    1    2    3    4    5    6    7    8    9   10
	/* n = 0 */{   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1 },
	/* n = 1 */{   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1 },
	/* n = 2 */{   0,   1,   2,   2,   2,   2,   2,   2,   2,   2,   2 },
	/* n = 3 */{   0,   1,   3,   4,   4,   4,   4,   4,   4,   4,   4 },
	/* n = 4 */{   0,   1,   5,   7,   8,   8,   8,   8,   8,   8,   8 },
	/* n = 5 */{   0,   1,   8,  13,  15,  16,  16,  16,  16,  16,  16 },
	/* n = 6 */{   0,   1,  13,  24,  29,  31,  32,  32,  32,  32,  32 },
	/* n = 7 */{   0,   1,  21,  44,  56,  61,  63,  64,  64,  64,  64 },
	/* n = 8 */{   0,   1,  34,  81, 108, 120, 125, 127, 128, 128, 128 },
	/* n = 9 */{   0,   1,  55, 149, 208, 236, 248, 253, 255, 256, 256 },
   /* n = 10 */{   0,   1,  89, 274, 401, 464, 492, 504, 509, 511, 512 }
		};


		for (int n = 0; n < n_size; ++n)
		{
			for (int k = 0; k < k_size; ++k)
			{
				size_t actualValue1 = getWaysToClimbNSteps_recursively(n, k);
				size_t actualValue2 = getWaysToClimbNSteps_fibonacci_1(n, k);
				size_t actualValue3 = getWaysToClimbNSteps_fibonacci_2(n, k);
				size_t actualValue4 = getWaysToClimbNSteps_DP_1(n, k);
				size_t actualValue5 = getWaysToClimbNSteps_DP_2(n, k);
				MM_EXPECT_TRUE_PRINTALLRESULTS(actualValue1 == actualValue2
					&& actualValue2 == actualValue3 
					&& actualValue3 == actualValue4 
					&& actualValue4 == actualValue5
					&& actualValue5 == expectedValues[n][k],
					n, k, expectedValues[n][k], actualValue1, actualValue2, actualValue3, actualValue4, actualValue5)

			}
		}

		// Test large n and k
		// size_t is too small to hold the results. The intention of this test is to check whether the algorithm completes in reasonable time
		//for (size_t n = 1000000, i = 0; i < 10; n += 1000, ++i)
		for (size_t n = 10000, i = 0; i < 10; n += 1000, ++i)
		{
			//for (size_t k = 100000, j = 0; j < 10; k += 1000, ++j)
			for (size_t k = 1000, j = 0; j < 10; k += 1000, ++j)
			{
				//size_t actualValue1 = getWaysToClimbNSteps_recursively(n, k);  // recursive does not work for large values
				//size_t actualValue2 = getWaysToClimbNSteps_fibonacci_1(n, k);
				//size_t actualValue3 = getWaysToClimbNSteps_fibonacci_2(n, k);
				size_t actualValue4 = getWaysToClimbNSteps_DP_1(n, k);
				size_t actualValue5 = getWaysToClimbNSteps_DP_2(n, k);
				MM_EXPECT_TRUE_PRINTALLRESULTS(
					true
					//&& actualValue1 == actualValue2
					//&& actualValue2 == actualValue3
					//&& actualValue3 == actualValue4
					&& actualValue4 == actualValue5,
					n, k, 
					//actualValue1, 
					//actualValue2, 
					//actualValue3, 
					actualValue4, 
					actualValue5)

			}
		}
	}
	

/*

The above problem can be solved in O(log n) by using following solution to linear recurrence relation

if f(k+1) = a1 * f(1) + a2 * f(2) + ... + ak * f(k)
then
| f(2)     | = | 0  1  0  0  ... 0  | | f(1) |
| f(3)     |   | 0  0  1  0  ... 0  | | f(2) |
|    .     |   | 0  0  0  1  ... 0  | | f(3) |
|	 .     |   |                    | |      |
|	 .     |   |                    | |      |
| f(k-1)   |   |                    | |      |
| f(k)     |   |                    | |      |
| f(k+1)   |   | a1 a2 ...   ... ak | | f(k) |

R(n)   = T^(n - k) * R(k)

*/




}
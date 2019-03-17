#include <iostream>
using namespace std;

#include "Puzzles/Puzzles_JosephusProblem.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	//TODO: 
	//Convert the method for special case where k = 1, into generic method for all k
	//Add segment tree solution where we can get the number of persns surviving in range (left, right) at any time
	//Ref: https://www.youtube.com/watch?v=nyVX2lqX8lA

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
	Slightly different Recurrence relation: 
	f(n, k, s) = (s + f(n, k)) % n    ---Rotate the circle of indices (by s indices) to bring the index 0 at the person with sword, 
	                                  ---then complete the process and rotate the circle of indices in opposite direction (by s indices)
	
	f(n, k) = ((k + 1) + f(n-1, k)) % n   ---When first person is killed, the problem remaing is f(n-1, k, 0), so the survivor in this case 
	                                      ---is the actual survivor, but it starts at (k + 1)st position, so we need to adjust the index 
									      ---by adding (1 + k) to solution of f(n-1, k, 0)
	f(1, k) = 0                           ---If there is only one person, he is the survivor


	example:
	Lets say following persons are in circle
	The person who is killed, will be replaced by X
	n = 8, k = 3, s = 0

	a b c d e f g h -- _ is killed, a gets sword, n = 8 from a to h
	a b c X e f g h -- d is killed, e gets sword, n = 7 from e to c -- Note, it's a circle, so go around back to the start of line
	a b c X e f g X -- h is killed, a gets sword, n = 6 from a to g
	a b X X e f g X -- c is killed, e gets sword, n = 5 from e to b
	X b X X e f g X -- a is killed, b gets sword, n = 4 from b to g
	X b X X e f X X -- g is killed, b gets sword, n = 3 from b to f
	X X X X e f X X -- b is killed, e gets sword, n = 2 from e to f
	X X X X e X X X -- f is killed, e gets sword, n = 1 from e to e

	Time complexity: O(n)
	*/

	//This function assumes, we always start at index 0 i.e. s = 0
	unsigned int JosephusProblem_DP_topdown_recursive_v1(unsigned int n, unsigned int k)
	{
		if (n == 1)
			return 0;

		return ((k + 1) + JosephusProblem_DP_topdown_recursive_v1(n - 1, k)) % n;
	}

	unsigned int JosephusProblem_DP_topdown_recursive_v1(unsigned int n, unsigned int k, unsigned int s)
	{
		//invalid case
		if (n == 0 || s >= n) //recursively n goes on decreasing, but index s can be anything from 0 to original n
			return 0; //ideally we should return -1 i.e. the invalid index

		return (s + JosephusProblem_DP_topdown_recursive_v1(n, k)) % n;
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

	/*
	Special case: k = 2

	Below are some interesting facts.

	In first round all even positioned persons are killed.
	For second round two cases arise
	If n is even : For example n = 8. In first round, first 2 is killed, then 4, then 6, then 8. In second round, we have 1, 3, 5 and 7 in positions 1st, 2nd, 3rd and 4th respectively.
	If n is odd : For example n = 7. In first round, first 2 is killed, then 4, then 6. In second round, we have 3, 5, 7 in positions 1st, 2nd and 3rd respectively.
	If n is even and a person is in position x in current round, then the person was in position 2x – 1 in previous round.

	If n is odd and a person is in position x in current round, then the person was in position 2x + 1 in previous round.

	From above facts, we can recursively define the formula for finding position of survivor.

	Let f(n) be position of survivor for input n,
	the value of f(n) can be recursively written
	as below.

	if n is even
		f(n) = 2f(n/2) - 1
	else
		f(n) = 2f((n-1)/2)

	Solution of above recurrence is

	f(n) = 2(n - 2 ^ floor(Log2(n))
	     = 2n - 2 ^ (1 + floor(Log2(n)))

	Time complexity: O(1)
	*/
	unsigned int JosephusProblem_when_k_is_1_v1(unsigned int n, unsigned int s)
	{
		//invalid case
		if (n == 0 || s >= n)
			return 0; //ideally we should return -1 i.e. the invalid index

		// Find value of 2 ^ (1 + floor(Log n)) 
		// which is a power of 2 whose value 
		// is just above n. 
		int p = 1;
		while (p <= n)
			p *= 2;

		// Return 2n - 2^(1+floor(Log2(n))) 
		unsigned int retVal = (2 * n) - p;

		return (s + retVal) % n;
	}

	/*
	Time complexity : O(1)
	*/
	// function to find the position of the Most 
	// Significant Bit 
	int msbPos(int n)
	{
		int pos = 0;
		while (n != 0) {
			pos++;

			// keeps shifting bits to the right 
			// until we are left with 0 
			n = n >> 1;
		}
		return pos;
	}

	unsigned int JosephusProblem_when_k_is_1_v2(unsigned int n, unsigned int s)
	{
		//invalid case
		if (n == 0 || s >= n)
			return 0; //ideally we should return -1 i.e. the invalid index

		unsigned int retVal = 0;

		/* Getting the position of the Most Significant
		Bit(MSB). The leftmost '1'. If the number is
		'41' then its binary is '101001'.
		So msbPos(41) = 6 */
		int position = msbPos(n);

		/* 'j' stores the number with which to XOR the
		number 'n'. Since we need '100000'
		We will do 1<<6-1 to get '100000' */
		int j = 1 << (position - 1);

		/* Toggling the Most Significant Bit. Changing
		the leftmost '1' to '0'.
		101001 ^ 100000 = 001001 (9) */
		retVal = n ^ j;

		/* Left-shifting once to add an extra '0' to
		the right end of the binary number
		001001 = 010010 (18) */
		retVal = retVal << 1;

		/* Toggling the '0' at the end to '1' which
		is essentially the same as putting the
		MSB at the rightmost place. 010010 | 1
		= 010011 (19) */
		retVal = retVal | 1;

		return (s + retVal - 1) % n;
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
			MM_EXPECT_TRUE((actualResult = JosephusProblem_DP_topdown_recursive_v1(testData[i].n, testData[i].k, testData[i].s)) == testData[i].expectedResult, testData[i].n, testData[i].k, testData[i].s, testData[i].expectedResult, actualResult);
			MM_EXPECT_TRUE((actualResult = JosephusProblem_DP_bottomup_v1         (testData[i].n, testData[i].k, testData[i].s)) == testData[i].expectedResult, testData[i].n, testData[i].k, testData[i].s, testData[i].expectedResult, actualResult);
			if(testData[i].k == 1)
			{
				MM_EXPECT_TRUE((actualResult = JosephusProblem_when_k_is_1_v1     (testData[i].n,                testData[i].s)) == testData[i].expectedResult, testData[i].n, testData[i].k, testData[i].s, testData[i].expectedResult, actualResult);
				MM_EXPECT_TRUE((actualResult = JosephusProblem_when_k_is_1_v2     (testData[i].n,                testData[i].s)) == testData[i].expectedResult, testData[i].n, testData[i].k, testData[i].s, testData[i].expectedResult, actualResult);
			}
		}		
	}
}
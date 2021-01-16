#include <iostream>
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <locale> 
using namespace std;

#ifdef _MSC_VER
#  include <intrin.h>
#  define __builtin_popcount __popcnt
#endif

#include "Others_h/Others_h_DistinctNumbersGenerated.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Utils/Utils_TimerUtils.h"

/*
Find the distinct numbers generated in the following series:
Input integers: N, S, P, Q
F(0) = S % 2^31
F(n) = P * F(n-1) + Q % 2^31

*/

namespace mm {

	unsigned int findDistrinctNumbersGenerated_v1(unsigned long n, unsigned long s, unsigned long p, unsigned long q)
	{
		constexpr unsigned int mod = (1u << 31);
		constexpr unsigned int divisor = mod - 1;

		constexpr unsigned int numElements = (divisor >> 5) + 1; // mod / 32 + 1;
		vector<unsigned int> cache(numElements, 0);

		unsigned long current = s & divisor;
		unsigned int index = current >> 5; // = current / 32
		unsigned int flag = (1u << (current & (31)));  // (1u << (current % 32))
		cache[index] |= flag;
		unsigned int numDistinctInts = 1u;
		for (unsigned int i = 1; i < n; ++i)
		{
			current = ((current * p) + q) & divisor;
			unsigned int index = current >> 5; // = current / 32 
			unsigned int flag = (1u << (current & (31))); // (1u << (current % 32))
			if ((cache[index] & flag) == 0)
			{
				cache[index] |= flag;
				++numDistinctInts;
			}
			else
				break;
		}

		return numDistinctInts;
	}
	
	unsigned int findDistrinctNumbersGenerated_v2(unsigned long n, unsigned long s, unsigned long p, unsigned long q)
	{
		constexpr unsigned int mod = (1u << 31);
		constexpr unsigned int divisor = mod - 1;

		constexpr unsigned int numElements = (divisor >> 5) + 1; // mod / 32 + 1;
		vector<bool> cache(mod, false);

		unsigned long current = s & divisor;
		//unsigned int index = current >> 5; // = current / 32
		//unsigned int flag = (1u << (current & (31)));  // (1u << (current % 32))
		//cache[index] |= flag;
		unsigned int numDistinctInts = 1u;
		for (unsigned int i = 1; i < n; ++i)
		{
			current = ((current * p) + q) & divisor;
			//unsigned int index = current >> 5; // = current / 32 
			//unsigned int flag = (1u << (current & (31))); // (1u << (current % 32))
			if (cache[current] == false)
			{
				cache[current] = true;
				++numDistinctInts;
			}
			else
				break;
		}

		return numDistinctInts;
	}

	//Floyd's cycle-finding algorithm
	unsigned int findDistrinctNumbersGenerated_v3(unsigned long n, unsigned long s, unsigned long p, unsigned long q)
	{
		/*
        X             P
        --------------+------+
                      |      | 
					  |      +  Y = meeting point
                      |      |
                      +------+

			lenght of loop/cycle = period = λ = lambda
			Index of node X = 0
			Index of node P = μ = mu = the index of the first element of the cycle
			length of tail = XP = μ = mu

			The hare and tortoise both start at end of tail. The hare takes 2 steps at a time and tortoise takes 1 steo at a time.
			Then they meet at some point Y on loop.

			Lets say they both took s steps. Then hare must have covered 2s distance and tortoise must have covered s distance.
			Lets say hare made m loops and tortoise made n loops before meeting at Y.
			So,
			hare travelled    2s = XP + m*λ + PY
			tortoise travelled s = XP + n*λ + PY
			So,
			XP + m*λ + PY = 2 * (XP + n*λ + PY)
			(m - n) * λ = XP + PY
			So,
			XP = (m - n) * λ - PY

			If hare starts at X and tortoise at meeting point Y and now they are moving one step at a time,
			when hare reaches P, tortoise would have moved (m - n) * λ - PY distance.
			If tortoise moves (m - n) * λ distance, then it would come to original position i.e. meeting point Y,
			but it is falling short by PY distance, so it would be at P.
		*/
		constexpr unsigned int mod = (1u << 31);
		constexpr unsigned int divisor = mod - 1;

		unsigned long tortoise = s & divisor;
		unsigned long hare = s & divisor;
		unsigned int numDistinctInts = 1u;
		do
		{
			tortoise = ((tortoise * p) + q) & divisor;
			hare = ((hare * p) + q) & divisor;
			hare = ((hare * p) + q) & divisor;
			++numDistinctInts;
		} while (tortoise != hare && numDistinctInts < n);

		//If we are unfortunate, and there is no loop in above trajectory
		if (numDistinctInts == n)
			return numDistinctInts;

		//At this stage, hare and tortoise both are at point Y

		unsigned long meetingPoint = hare; // = tortoise //both are at same position
		hare = s & divisor;
		tortoise = meetingPoint;
		
		unsigned int lengthOfTail = 0;
		while (tortoise != hare)
		{
			tortoise = ((tortoise * p) + q) & divisor;
			hare = ((hare * p) + q) & divisor;
			++lengthOfTail;
		}

		//At this stage, hare and tortoise both are at point P
		unsigned int lengthOfLoop = 0;
		do
		{
			hare = ((hare * p) + q) & divisor;
			++lengthOfLoop;
		} while (tortoise != hare);

		return lengthOfTail + lengthOfLoop;
	}


	//Brent's algorithm
	unsigned int findDistrinctNumbersGenerated_v4(unsigned long n, unsigned long s, unsigned long p, unsigned long q)
	{
		/*
		X             P
		---------------+------+
		               |      |
		               |      | Y
		               +------+

		lenght of loop/cycle = period = λ = lambda
		Index of node X = 0
		Index of node P = μ = mu = the index of the first element of the cycle
		length of tail = XP = μ = mu

		



		*/
		constexpr unsigned int mod = (1u << 31);
		constexpr unsigned int divisor = mod - 1;

		unsigned long tortoise = s & divisor;
		unsigned long hare = s & divisor;
		unsigned int numDistinctInts = 1u;
		do
		{
			tortoise = ((tortoise * p) + q) & divisor;
			hare = ((hare * p) + q) & divisor;
			hare = ((hare * p) + q) & divisor;
			++numDistinctInts;
		} while (tortoise != hare && numDistinctInts < n);

		//If we are unfortunate, and there is no loop in above trajectory
		if (numDistinctInts == n)
			return numDistinctInts;

		//At this stage, hare and tortoise both are at point Y

		unsigned long meetingPoint = hare; // = tortoise //both are at same position
		hare = s & divisor;
		tortoise = meetingPoint;

		unsigned int lengthOfTail = 0;
		while (tortoise != hare)
		{
			tortoise = ((tortoise * p) + q) & divisor;
			hare = ((hare * p) + q) & divisor;
			++lengthOfTail;
		}

		//At this stage, hare and tortoise both are at point P
		unsigned int lengthOfLoop = 0;
		do
		{
			hare = ((hare * p) + q) & divisor;
			++lengthOfLoop;
		} while (tortoise != hare);

		return lengthOfTail + lengthOfLoop;
	}

	/*
	The problem boils down to finding minimal m > 0 and n >= 0 such that a[n + m] = a[n].

	As pointed out by others Floyd's and Brent's algorithms are nice for this problem. 
	These algorithms are generic and work for any recurrence a[i] = f(a[i-1]), ie, they don't make any assumption about f. 
	For the problem at hand, f(x) = p*x + q and we can buy a lot of time if we use f's mathematical properties. 
	Indeed, one can easily show that

	a[n] = s * p^n + q * (p^(n-1) + ... + 1) for all n >= 1.

	The arguments are a bit lengthy for here but to give an idea of the math suppose s > 0 and q = 0 
	(the case where q > 0 brings some but not much complexity). Then

	a[n] = s * p^n for all n >= 1.

	It follows that
	a[n + m] - a[n] = s * (p^m - 1) * p^n.

	Consider the case where p is even but not zero. One can find unique integers x and ysuch that p = x * 2^y and x is odd. 
	Analogously, we can find integers u and v such that s = u * 2^v and u is odd. Using this we obtain

	a[m + n] - a[n] = x^n * (p^m - 1) * u * 2^(n * y + v). 

	Since x^n, p^m - 1 and u are odd, the only way for the rhs (and thus for the lhs) to be zero modulo 2^31 is when n * y + v >= 31. 
	We are looking for the minimal n which implies n = ceiling((31 - v) / y). We have shown that

	a[m + n] = a[n] mod 2^31 for all m > 0,

	that is, the sequence is constant from n onwards. It can also be shown that a[0], ..., a[n] are all distinct. 
	Therefore, the solution to the problem is min(n + 1, N).

	When p is odd, the argument is much more complex and relies on a property of the group ((Z/rZ)*, *) 
	where r=2^k is a power of 2, namely, if p^m = 1 modulo r and m is minimal, then m divides 2^(k - 1), 
	i.e., m belongs to {1, 2, 4, ..., 2^(k - 1)}.

	I've implemented my solution and it took 0s to solve all test cases. For comparison, I've also implemented 
	the Floyd's and Brent's algorithm. An interesting test case is N = 100000000, s = 1, p = 3 and q = 1 
	for which the answer is N. My solution took 0.0s, Floyd's took 2.5s and Brent took 16.0s.

	*/

	unsigned int solve_p_even(unsigned int n, unsigned int s, unsigned int p, unsigned int q)
	{
		constexpr unsigned int exponent = 31;
		constexpr unsigned int two_to_exponent = 1u << exponent;
		unsigned int neg_p = ~p + 1;

		if (p == 0) {
			if (s == q)
				return 1;
			return 2;
		}

		if (s == 0 && q == 0)
			return 1;

		unsigned int a1_minus_a0 = (p - 1) * s + q;
		//unsigned long long neg_a1_minus_a0 = -a1_minus_a0; //warning C4146: unary minus operator applied to unsigned type, result still unsigned
		unsigned long long neg_a1_minus_a0 = ~a1_minus_a0 + 1; //Do Two's Complement which is equivalent of negation

		unsigned int numerator = exponent - __builtin_popcount((a1_minus_a0 & neg_a1_minus_a0) - 1);
		unsigned int denominator = __builtin_popcount((p & neg_p) - 1);
		unsigned int m = numerator / denominator + (numerator % denominator == 0 ? 1 : 2);
		return std::min(m, n);
	}

	unsigned int solve_p_odd(unsigned int n, unsigned int s, unsigned int p, unsigned int q)
	{
		constexpr unsigned int exponent = 31;
		constexpr unsigned int two_to_exponent = 1u << exponent;
		unsigned int neg_q = ~q + 1;

		if (p == 1)
			return q == 0 ? 1 : std::min(n, two_to_exponent / (q & neg_q));

		if (s == 0 && q == 0)
			return 1;

		unsigned int       m = 1;
		unsigned long long p_minus_1 = p - 1;
		unsigned long long neg_p_minus_1 = ~p_minus_1 + 1;
		unsigned long long a1_minus_a0 = p_minus_1 * s + q;
		//unsigned long long neg_a1_minus_a0 = -a1_minus_a0; //warning C4146: unary minus operator applied to unsigned type, result still unsigned
		unsigned long long neg_a1_minus_a0 = ~a1_minus_a0 + 1; //Do Two's Complement which is equivalent of negation
		unsigned long long p_to_m = p;
		unsigned long long mask = (two_to_exponent *
			(p_minus_1 & neg_p_minus_1) / (a1_minus_a0 & neg_a1_minus_a0)) - 1;

		while (m < n && (p_to_m & mask) != 1) {
			p_to_m = p_to_m * p_to_m;
			m = m * 2;
		}

		return std::min(m, n);
	}

	unsigned int findDistrinctNumbersGenerated_v5(unsigned long n, unsigned long s, unsigned long p, unsigned long q)
	{
		if (p % 2 == 0)
			return solve_p_even(n, s, p, q);
		else
			return solve_p_odd(n, s, p, q);
	}

	//===============================================================================

	MM_DECLARE_FLAG(Others_h_findDistrinctNumbersGenerated);

	MM_UNIT_TEST(Others_h_findDistrinctNumbersGenerated_sanity, Others_h_findDistrinctNumbersGenerated)
	{
		// Test Date creation

		struct testInputStruct
		{
			testInputStruct(unsigned long n, unsigned long s, unsigned long p, unsigned long q, unsigned long result)
				: n_(n),
				s_(s),
				p_(p),
				q_(q),
				result_(result)
			{
			}

			unsigned long n_;
			unsigned long s_;
			unsigned long p_;
			unsigned long q_;
			unsigned int result_;
		};

		vector<testInputStruct> testData = {
			{ 3,			1,				1,				1,				3			},
			{ 10,			2,				3,				4,				10			},
			{ 12345678,		524867,			4865215,		48976163,		12345678	},
			{ 100'000'000,	1,				3,				1,				100'000'000 },
			{ 100'000'000,	2'065'188'356,	657'733'125,	1'636'844'041,	100'000'000 },
			{ 100'000'041,	18'467,			6'334,			26'500,			32			},
			{ 100'019'169,	15'724,			11'478,			29'358,			31			},
			{ 100'026'962,	24'464,			5'705,			28'145,			100'026'962 },
			{ 100'023'281,	16'827,			9'961,			491,			100'023'281 },
			{ 100'002'995,	11'942,			4'827,			5'436,			100'002'995 },
			{ 100'032'391,	14'604,			3'902,			153,			32			},
			{ 100'000'292,	12'382,			17'421,			18'716,			100'000'292 },
			{ 100'019'718,	19'895,			5'447,			21'726,			67'108'864	},
			{ 100'014'771,	11'538,			1'869,			19'912,			67'108'864	},
			{ 100'025'667,	26'299,			17'035,			9'894,			100'025'667 },
			{ 100'028'703,	23'811,			31'322,			30'333,			29			},
			{ 100'017'673,	4'664,			15'141,			7'711,			100'017'673 },
			{ 100'028'253,	6'868,			25'547,			27'644,			100'028'253 },
			{ 100'032'662,	32'757,			20'037,			12'859,			100'032'662 },
			{ 100'008'723,	9'741,			27'529,			778,			100'008'723 },
			{ 100'012'316,	3'035,			22'190,			1'842,			32			},
			{ 100'000'288,	30'106,			9'040,			8'942,			9			},
			{ 100'019'264,	22'648,			27'446,			23'805,			32			},
			{ 100'015'890,	6'729,			24'370,			15'350,			32			},
			{ 100'015'006,	31'101,			24'393,			3'548,			100'015'006 },
			{ 100'019'629,	12'623,			24'084,			19'954,			17			},
			{ 100'018'756,	11'840,			4'966,			7'376,			28			},
			{ 100'013'931,	26'308,			16'944,			32'439,			9			},
			{ 100'024'626,	11'323,			5'537,			21'538,			100'024'626 },
			{ 100'016'118,	2'082,			22'929,			16'541,			100'016'118 },
			{ 100'004'833,	31'115,			4'639,			29'658,			33'554'432	},
			{ 100'022'704,	9'930,			13'977,			2'306,			100'022'704 },
			{ 100'031'673,	22'386,			5'021,			28'745,			100'031'673 },
			{ 100'026'924,	19'072,			6'270,			5'829,			32			},
			{ 100'026'777,	15'573,			5'097,			16'512,			100'026'777 },
		};

		//Generate random test data
		testData.reserve(testData.size() + 100);
		constexpr unsigned two_to_exponent = 1u << 31;
		for (int i = 0; i < 100; ++i)
		{
			unsigned long n = rand() % (two_to_exponent - 100'000'000) + 100'000'000; //range 100'000'000 to 2^31
			unsigned long s = rand();
			unsigned long p = rand();
			unsigned long q = rand();
			unsigned int result = 0;

			testData.push_back({ n, s, p, q, result });
		}

		std::ios_base::fmtflags f(cout.flags());
		cout.imbue(std::locale(""));

		for (int i = 0; i < testData.size(); ++i)
		{
			unsigned int actualNum;
			actualNum = mm_time<decltype(actualNum)>{}("findDistrinctNumbersGenerated_v1: ", findDistrinctNumbersGenerated_v1, testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_);
			//Set the result if its not set
			if (testData[i].result_ == 0)
				testData[i].result_ = actualNum;
			MM_EXPECT_TRUE(actualNum == testData[i].result_, i, actualNum, testData[i].result_);
			cout << "\n";
			actualNum = mm_time<decltype(actualNum)>{}("findDistrinctNumbersGenerated_v2: ", findDistrinctNumbersGenerated_v2, testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_);
			MM_EXPECT_TRUE(actualNum == testData[i].result_, i, actualNum, testData[i].result_);
			cout << "\n";
			actualNum = mm_time<decltype(actualNum)>{}("findDistrinctNumbersGenerated_v3: ", findDistrinctNumbersGenerated_v3, testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_);
			MM_EXPECT_TRUE(actualNum == testData[i].result_, i, actualNum, testData[i].result_);
			cout << "\n";
			actualNum = mm_time<decltype(actualNum)>{}("findDistrinctNumbersGenerated_v4: ", findDistrinctNumbersGenerated_v4, testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_);
			MM_EXPECT_TRUE(actualNum == testData[i].result_, i, actualNum, testData[i].result_);
			cout << "\n";
			actualNum = mm_time<decltype(actualNum)>{}("findDistrinctNumbersGenerated_v5: ", findDistrinctNumbersGenerated_v5, testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_);
			MM_EXPECT_TRUE(actualNum == testData[i].result_, i, actualNum, testData[i].result_);
			cout << "\n\n";

		}

		//Print the input and result of this random generted test case
		cout.flags(f);
		for (int i = 0; i < testData.size(); ++i)
		{
			cout << "\ni = " << i << " { " << testData[i].n_ << ", " << testData[i].s_ << ", " << testData[i].p_ << ", " << testData[i].q_ << ", " << testData[i].result_ << " }\n";
		}
	}

}
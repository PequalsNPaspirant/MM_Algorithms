#include <iostream>
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
using namespace std;

#ifdef _MSC_VER
#  include <intrin.h>
#  define __builtin_popcount __popcnt
#endif

#include "Others_h/Others_h_DistinctNumbersGenerated.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	string getCommaSeparatedTimeDuration(unsigned long long duration)
	{
		string durationStr = "000,000.000,000,000";
		int pos = durationStr.length() - 1;
		for (; pos > 0 && duration > 0; --pos)
		{
			if (durationStr[pos] == '0')
			{
				durationStr[pos] = '0' + duration % 10;
				duration /= 10;
			}
		}
		if (pos > 6)
			pos = 6;
		durationStr = durationStr.substr(pos);
		durationStr += " sec";

		return durationStr;
	}

#define MM_TIME2(msg, statement, time) \
	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now(); \
	statement; \
	std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now(); \
	time = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count(); \
	int columnWidth = 25; \
	cout << "\n" << setw(columnWidth) << std::right << msg; \
	cout << setw(columnWidth) << std::right << getCommaSeparatedTimeDuration(time);

#define MM_TIME(msg, statement) \
	{ \
	unsigned long long time; \
	MM_TIME2(msg, statement, time) \
	}

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
		for (int i = 1; i < n; ++i)
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
		unsigned int numDistinctInts = 0u;
		for (int i = 0; i < n; ++i)
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
                      |      | Y
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

	unsigned solve_p_even(unsigned n, unsigned s, unsigned p, unsigned q) 
	{
		constexpr unsigned exponent = 31;
		constexpr unsigned two_to_exponent = 1u << exponent;

		if (p == 0) {
			if (s == q)
				return 1;
			return 2;
		}

		if (s == 0 && q == 0)
			return 1;

		unsigned a1_minus_a0 = (p - 1) * s + q;

		unsigned numerator = exponent - __builtin_popcount((a1_minus_a0 & -a1_minus_a0) - 1);
		unsigned denominator = __builtin_popcount((p & -p) - 1);
		unsigned m = numerator / denominator + (numerator % denominator == 0 ? 1 : 2);
		return std::min(m, n);
	}

	unsigned solve_p_odd(unsigned n, unsigned s, unsigned p, unsigned q) 
	{
		constexpr unsigned exponent = 31;
		constexpr unsigned two_to_exponent = 1u << exponent;

		if (p == 1)
			return q == 0 ? 1 : std::min(n, two_to_exponent / (q & -q));

		if (s == 0 && q == 0)
			return 1;

		unsigned           m = 1;
		unsigned long long p_minus_1 = p - 1;
		unsigned long long a1_minus_a0 = p_minus_1 * s + q;
		unsigned long long p_to_m = p;
		unsigned long long mask = (two_to_exponent *
			(p_minus_1 & -p_minus_1) / (a1_minus_a0 & -a1_minus_a0)) - 1;

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
			{ 3, 1, 1, 1, 3 },
			{ 10, 2, 3, 4, 10 },
			{ 12345678, 524867, 4865215, 48976163, 12345678 },
			{ 100'000'000, 1, 3, 1, 100'000'000 },
			{ 100'000'000, 2'065'188'356, 657'733'125, 1'636'844'041, 100'000'000 }
		};

		//Generate random test data
		testData.reserve(testData.size() + 10'000);
		constexpr unsigned two_to_exponent = 1u << 31;
		for (int i = 0; i < 10'000; ++i)
		{
			unsigned long n = rand() % (two_to_exponent - 100'000'000) + 100'000'000; //range 100'000'000 to 2^31
			unsigned long s = rand();
			unsigned long p = rand();
			unsigned long q = rand();
			unsigned int result = 0;

			testData.push_back({ n, s, p, q, result });
		}

		for (int i = 0; i < testData.size(); ++i)
		{
			unsigned int actualNum;
			cout << endl;
			MM_TIME("findDistrinctNumbersGenerated_v1: ", actualNum = findDistrinctNumbersGenerated_v1(testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_));
			MM_EXPECT_TRUE(actualNum == testData[i].result_, actualNum, testData[i].result_);
			cout << endl;
			MM_TIME("findDistrinctNumbersGenerated_v2: ", actualNum = findDistrinctNumbersGenerated_v2(testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_));
			MM_EXPECT_TRUE(actualNum == testData[i].result_, actualNum, testData[i].result_);
			cout << endl;
			MM_TIME("findDistrinctNumbersGenerated_v3: ", actualNum = findDistrinctNumbersGenerated_v3(testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_));
			MM_EXPECT_TRUE(actualNum == testData[i].result_, actualNum, testData[i].result_);
			cout << endl;
			MM_TIME("findDistrinctNumbersGenerated_v4: ", actualNum = findDistrinctNumbersGenerated_v4(testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_));
			MM_EXPECT_TRUE(actualNum == testData[i].result_, actualNum, testData[i].result_);
			cout << endl;
			MM_TIME("findDistrinctNumbersGenerated_v5: ", actualNum = findDistrinctNumbersGenerated_v5(testData[i].n_, testData[i].s_, testData[i].p_, testData[i].q_));
			MM_EXPECT_TRUE(actualNum == testData[i].result_, actualNum, testData[i].result_);
		}
	}

}
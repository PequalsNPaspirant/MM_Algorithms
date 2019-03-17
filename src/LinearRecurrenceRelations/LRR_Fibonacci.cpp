#include <vector>
#include <limits>
using namespace std;

#include "LinearRecurrenceRelations/LRR_Fibonacci.h"
//#include "my_math_lib/Matrix.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

/*
More at:
http://fusharblog.com/solving-linear-recurrence-for-programming-contest/
http://www.maths.surrey.ac.uk/hosted-sites/R.Knott/Fibonacci/fibFormula.html
http://www.askamathematician.com/2011/04/q-is-there-a-formula-to-find-the-nth-term-in-the-fibonacci-sequence/
*/

namespace mm {

	/*
	DP bottom up
	*/
	unsigned long long fibonacci_DP_bottomup_v1(const unsigned long long index)
	{
		unsigned long long f0 = 0;
		unsigned long long f1 = 1;
		if (index == 0) return f0; // first term in Fibonacci sequence
		if (index == 1) return f1; // second term in Fibonacci sequence

		unsigned long long r;
		for (unsigned long long i = 2; i <= index; ++i)
		{
			r = f1 + f0;
			f0 = f1;
			f1 = r;
		}

		return r;
	}

	/*
	Note: For complete analysis, take a look at LinearRecurrenceRelations_concept.cpp

	Fibonacci sequence:
	f(n) = 1 * f(n-1) + 1 * f(n-2) 
		 = 1 * f(n-2) + 1 * f(n-1)  ... first k terms arranged in ascending order (k = 2)
	OR we can say
	f(2) = 1 * f(0) + 1 * f(1)
	First two terms: f(0) = 0, f(1) = 1
	Thus,
	 | f(n-1) | = | 0  1 | ^ (n - 1) * | f(0) |  
	 | f(n)   |   | 1  1 |             | f(1) |


	 | f(n-1) | = | 0  1 | ^ (n - 1) * |  0   |
	 | f(n)   |   | 1  1 |             |  1   |

	*/
	
	unsigned long long fibonacci_usingLinearRecurrence_v1(const unsigned long long index)
	{
		// Take a look at explaination in \src\MM_LinearRecurrenceRelations_main.cpp
		//index starts from zero, but k starts from 1 
		//column matrix to store first two numbers in fibonnacci sequence
		unsigned long long c[2][1] = {
			0ull,
			1ull
		};

		if (index == 0) return c[0][0]; // first term in Fibonacci sequence
		if (index == 1) return c[1][0]; // second term in Fibonacci sequence

		//intermediate matrix to store powers of 2
		unsigned long long i[2][2] = {
			0ull, 1ull,
			1ull, 1ull
		};

		//resultant transformation matrix, initialised with unit matrix
		unsigned long long t[2][2] = {
			1ull, 0ull,
			0ull, 1ull
		};

		for (unsigned int power = index - 1; power > 0; power /= 2)
		{
			//General idea
			//express power as binary number. power = a 2^0 + b 2^1 + c 2^2 + ...
			//                                      = a + 2b + 4c + ...
			//a, b, c, ... can be either 0 or 1
			//x ^ power = x ^ a  *  (x^2) ^ b  *  (x^4) ^ c  *  ...

			if (power & 1u)
			{
				//t = t * i;
				//create a temp matrix
				unsigned long long p[2][2] = { 
					t[0][0], t[0][1],
					t[1][0], t[1][1] 
				};
				t[0][0] = p[0][0] * i[0][0] + p[0][1] * i[1][0];   t[0][1] = p[0][0] * i[0][1] + p[0][1] * i[1][1];
				t[1][0] = p[1][0] * i[0][0] + p[1][1] * i[1][0];   t[1][1] = p[1][0] * i[0][1] + p[1][1] * i[1][1];
			}

			//i = i ^ 2 = i * i//Do square
			//create a temp matrix
			unsigned long long p[2][2] = { 
				i[0][0], i[0][1],
				i[1][0], i[1][1] 
			};
			i[0][0] = p[0][0] * p[0][0] + p[0][1] * p[1][0];   i[0][1] = p[0][0] * p[0][1] + p[0][1] * p[1][1];
			i[1][0] = p[1][0] * p[0][0] + p[1][1] * p[1][0];   i[1][1] = p[1][0] * p[0][1] + p[1][1] * p[1][1];
		}

		//rc = t * c
		unsigned long long rc[2][1] = {
			t[0][0] * c[0][0] + t[0][1] * c[1][0],
			t[1][0] * c[0][0] + t[1][1] * c[1][0]
		};

		return rc[1][0];
		//return t[1][1]; //We can also return t[1][1]
	}







	//Testing
	MM_DECLARE_FLAG(LLR_Fibonacci)

	MM_UNIT_TEST(testFibonacciUsingLLR_1, LLR_Fibonacci)
	{
		vector<pair<unsigned long long, unsigned long long>> data{
		{ 0, 0 },
		{ 1, 1 },
		{ 2, 1 },
		{ 3, 2 },
		{ 4, 3 },
		{ 5, 5 },
		{ 6, 8 },
		{ 7, 13 },
		{ 8, 21 },
		{ 9, 34 },
		{ 10, 55 },
		{ 11, 89 },
		{ 12, 144 },
		{ 13, 233 },
		{ 14, 377 },
		{ 15, 610 },
		{ 16, 987 },
		{ 17, 1597 },
		{ 18, 2584 },
		{ 19, 4181 },
		{ 20, 6765 },
		{ 21, 10946 },
		{ 22, 17711 },
		{ 23, 28657 },
		{ 24, 46368 },
		{ 25, 75025 },
		{ 26, 121393 },
		{ 27, 196418 },
		{ 28, 317811 },
		{ 29, 514229 },
		{ 30, 832040 },
		{ 31, 1346269 },
		{ 32, 2178309 },
		{ 33, 3524578 },
		{ 34, 5702887 },
		{ 35, 9227465 },
		{ 36, 14930352 },
		{ 37, 24157817 },
		{ 38, 39088169 },
		{ 39, 63245986 },
		{ 40, 102334155 },
		{ 41, 165580141 },
		{ 42, 267914296 },
		{ 43, 433494437 },
		{ 44, 701408733 },
		{ 45, 1134903170 },
		{ 46, 1836311903 },
		{ 47, 2971215073 },
		{ 48, 4807526976 },
		{ 49, 7778742049 },
		{ 50, 12586269025 },
		{ 51, 20365011074 },
		{ 52, 32951280099 },
		{ 53, 53316291173 },
		{ 54, 86267571272 },
		{ 55, 139583862445 },
		{ 56, 225851433717 },
		{ 57, 365435296162 },
		{ 58, 591286729879 },
		{ 59, 956722026041 },
		{ 60, 1548008755920 },
		{ 61, 2504730781961 },
		{ 62, 4052739537881 },
		{ 63, 6557470319842 },
		{ 64, 10610209857723 },
		{ 65, 17167680177565 },
		{ 66, 27777890035288 },
		{ 67, 44945570212853 },
		{ 68, 72723460248141 },
		{ 69, 117669030460994 },
		{ 70, 190392490709135 },
		{ 71, 308061521170129 },
		{ 72, 498454011879264 },
		{ 73, 806515533049393 },
		{ 74, 1304969544928657 },
		{ 75, 2111485077978050 },
		{ 76, 3416454622906707 },
		{ 77, 5527939700884757 },
		{ 78, 8944394323791464 },
		{ 79, 14472334024676221 },
		{ 80, 23416728348467685 },
		{ 81, 37889062373143906 },
		{ 82, 61305790721611591 },
		{ 83, 99194853094755497 },
		{ 84, 160500643816367088 },
		{ 85, 259695496911122585 },
		{ 86, 420196140727489673 },
		{ 87, 679891637638612258 },
		{ 88, 1100087778366101931 },
		{ 89, 1779979416004714189 },
		{ 90, 2880067194370816120 },
		{ 91, 4660046610375530309 },
		{ 92, 7540113804746346429 }, 
		{ 93, 12'200'160'415'121'876'738 } //max 8 byte unsigned fibonnacci number < numeric_limit<unsigned long long>::max() 18'446'744'073'709'551'615
		//{ 94, 1'293'530'146'158'671'551 }
		};

		cout << numeric_limits<unsigned long long>::max();

		for (int index = 0; index < data.size(); ++index)
		{
			unsigned long long expectedValue = data[index].second;
			unsigned long long actualValue = 0;

			//MM_EXPECT_TRUE((actualValue = fibonacci_naive_v1(data[index].first)) == expectedValue, data[index].first, actualValue, expectedValue);
			//MM_EXPECT_TRUE((actualValue = fibonacci_fast_recursion_v1(data[index].first)) == expectedValue, data[index].first, actualValue, expectedValue);
			//MM_EXPECT_TRUE((actualValue = fibonacci_DP_topdown_v1(data[index].first)) == expectedValue, data[index].first, actualValue, expectedValue);
			MM_EXPECT_TRUE((actualValue = fibonacci_DP_bottomup_v1(data[index].first)) == expectedValue, data[index].first, actualValue, expectedValue);
			MM_EXPECT_TRUE((actualValue = fibonacci_usingLinearRecurrence_v1(data[index].first)) == expectedValue, data[index].first, actualValue, expectedValue);
			//MM_EXPECT_TRUE((actualValue = fibonacci_Binet_v1(data[index].first)) == expectedValue, data[index].first, actualValue, expectedValue);
		}
	}

}

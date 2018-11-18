//Goal
//Solve puzzles using dynamic programming

#include <iostream>
using namespace std;

#include "DynamicProgramming/DP_Fibonacci.h"

namespace mm {

	//========================= Method 1 =========================
	unsigned long long calculateFibonacciUsingDP_BottomUp(const unsigned long long index)
	{
		unsigned long long numMinusTwo = 0;
		unsigned long long numMinusOne = 1;
		if (index == 0) return numMinusTwo;
		if (index == 1) return numMinusOne;

		unsigned long long num = numMinusOne + numMinusTwo;
		unsigned long long currentIndex = 1;
		while (++currentIndex <= index)
		{
			num = numMinusOne + numMinusTwo;
			numMinusTwo = numMinusOne; numMinusOne = num;
		}

		return num;
	}

}

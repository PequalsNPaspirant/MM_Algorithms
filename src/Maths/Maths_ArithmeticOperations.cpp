#include <iostream>
#include <iomanip>
using namespace std;

#include "Maths/Maths_ArithmeticOperations.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Utils/Utils_PrintUtils.h"

namespace mm {

	bool specialCase(double base, int exponent, double& result)
	{
		//if (base == 0.0 && exponent == 0)
		//	return std::nan(""); //NaN
		//else if (base == 0.0 && exponent < 0)
		//	return 1.0/0.0; //infinity
		//else if (base == 0.0)
		//	return 0.0;
		//else if (exponent == 0)
		//	return 1;
		//else if (exponent == 1)
		//	return base;
		//else if (exponent < 0)
		//	return 1.0 / power_recursive(base, -exponent);

		if (base == 0.0)
		{
			if (exponent == 0)
				result = std::nan(""); //NaN
			else if (exponent < 0)
				result = std::numeric_limits<double>::infinity(); //infinity
			else // exponent > 0
				result = 0.0;
		}
		else if (exponent == 0)
			result = 1;
		else if (exponent == 1)
			result = base;
		//else if (exponent < 0)
		//	return 1.0 / power_recursive(base, -exponent);
		else
			return false;

		return true;
	}

	//Final methods (These are copy of power1 and power2 below)
	//TODO: Handle negative exponents
	/*
	Example: 
	x^15 = x * (x^7) * (x^7)
	            x^7 = x * (x^3) * (x^3)
				           x^3 = x * (x^1) * (x^1)

	power(b, e) = ((e % 2) == 0 ? 1 : b) * power(b * b, e/2)

	OR

	Divide and Conquer

	power(b, e) = ((e % 2) == 0 ? 1 : b) * power(b, e/2) * power(b, e/2)

	*/
	double power_recursive(double base, int exponent)
	{
		double res;
		if (specialCase(base, exponent, res))
			return res;

		if (exponent < 0)
			return 1.0 / power_recursive(base, -exponent);

		return (exponent % 2 == 0 ? 1 : base) * power_recursive(base * base, exponent / 2);
	}


	/*
	This iterative version has almost exact behavior as above recursive version
	x^15 = 
	exponent	factor	result = result * factor
	15			x		x
	7			x^2		(x) * x^2
	3			x^4		(x * x^2) * x^4
	1			x^8		(x * x^2 * x^4) * x^8


	exponent decay in recursion		exponent growth in iterative version
	log2( 1) = 0;		// 1 = bx0000 0001		// 1							// 1
	log2( 2) = 1;		// 2 = bx0000 0010		// 2, 1							// 1, 2
	log2( 3) = 1.x;		// 2 = bx0000 0011		// 3, 1							// 1, 3,
	log2( 4) = 2;		// 4 = bx0000 0100		// 4, 2, 1						// 1, 2, 4
	log2( 5) = 2.x;		// 4 = bx0000 0101      // 5, 2, 1						// 1, 2, 5
	log2( 6) = 2.x;		// 4 = bx0000 0110		// 6, 3, 1						// 1, 3, 6
	log2( 7) = 2.x;		// 4 = bx0000 0111		// 7, 3, 1						// 1, 3, 7
	log2( 8) = 3;		// 8 = bx0000 1000		// 8, 4, 2, 1					// 1, 2, 4,  8
	log2( 9) = 3.x;		// 9 = bx0000 1001		// 9, 4, 2, 1					// 1, 2, 4,  9
	log2(10) = 3.x;		//10 = bx0000 1010		//10, 5, 2, 1					// 1, 2, 5, 10
	log2(11) = 3.x;		//10 = bx0000 1011		//11, 5, 2, 1					// 1, 2, 5, 11
	log2(12) = 3.x;		//10 = bx0000 1100		//12, 6, 3, 1					// 1, 3, 6, 12
	log2(13) = 3.x;		//10 = bx0000 1101		//13, 6, 3, 1					// 1, 3, 6, 13
	log2(14) = 3.x;		//10 = bx0000 1110		//14, 7, 3, 1					// 1, 3, 7, 14
	log2(15) = 3.x;		//10 = bx0000 1111		//15, 7, 3, 1					// 1, 3, 7, 15
	log2(16) = 4;		//16 = bx0001 0000		//16, 8, 4, 2, 1				// 1, 2, 4,  8, 16

	If the exponent is 13, in recursive version it will decay like 13, 6, 3, 1 ..i.e. right shifting of exponent (1101) 1 bit at a time
	The base is doubled at each step, and multiplied by base if exponent at that stage is odd i.e. when the rightmost bit is 1

	So if we want to implement the similar approach from bottom to up, exponent should grow like 1, 3, 6, 13
	i.e. the exponent will be doubled, but will also be incremented by 1 at some steps when the ith bit from MSB is 1
	i.e. for base = x, we should have something like:
	i		result
	0		1.0 * x
	1		x^2 * x = x^3
	2		x^6
	3		x^13 * x = x^13

	*/

	double power_iterative_v1(double base, int exponent)
	{
		double result;
		if (specialCase(base, exponent, result))
			return result;

		if (exponent < 0)
			return 1.0 / power_iterative_v1(base, -exponent);

		int positionOfMSB = static_cast<int>(floor(log2(static_cast<double>(exponent)))); //rightmost bit is at position 0
		int mask = 1 << positionOfMSB;

		int exp = 0;
		result = 1.0;  //result = base^exp = base^0
		for (; exp != exponent; ) //OR run this loop until mask > 0 OR for positionOfMSB times
		{
			result = result * result;
			exp *= 2;

			if ((exponent & mask) > 0)
			{
				result *= base;
				exp += 1;
			}

			mask = mask >> 1;
		}

		MyAssert::myRunTimeAssert(exp == exponent, "exp not matching exponent");

		return result;
	}

	double power_iterative_v2(double base, int exponent)
	{
		double result;
		if (specialCase(base, exponent, result))
			return result;

		if (exponent < 0)
			return 1.0 / power_iterative_v2(base, -exponent);

		int mask = 1;
		int exp = exponent;
		for (; exp > 1; exp = exp >> 1)
			mask = mask << 1;

		exp = 0;
		result = 1.0;  //result = base^exp = base^0
		for (; mask > 0; ) //OR run this loop until exp != exponent
		{
			result = result * result;
			exp *= 2; //for tally

			if ((exponent & mask) > 0)
			{
				result *= base;
				exp += 1; //for tally
			}
			
			mask = mask >> 1;
		}

		MyAssert::myRunTimeAssert(exp == exponent, "exp not matching exponent");

		return result;
	}

	double power_iterative_v3(double base, int exponent)
	{
		double result;
		if (specialCase(base, exponent, result))
			return result;

		if (exponent < 0)
			return 1.0 / power_iterative_v3(base, -exponent);

		int mask = 1;
		int exp = exponent;
		for (; exp > 1; exp = exp >> 1)
			mask = mask << 1;

		exp = 0;
		result = 1.0;  //result = base^exp = base^0
		for (; mask > 0; ) //OR run this loop until exp != exponent
		{
			result = result * result;
			exp *= 2; //for tally

			if ((exponent & mask) > 0)
			{
				result *= base;
				exp += 1; //for tally
			}
			
			mask = mask >> 1;
		}

		MyAssert::myRunTimeAssert(exp == exponent, "exp not matching exponent");

		return result;
	}

	
	

	/*
	Example:
	x^15 = 
	exponent	factor	result = result * factor
	15			x		x
	7			x^2		(x) * x^2
	3			x^4		(x * x^2) * x^4
	1			x^8		(x * x^2 * x^4) * x^8

	In other words, it is like repesenting exponents in terms of powers of 2
	i.e. as per its value in binary
	15 = 0000 1111
	15 = 2^3 + 2^2 + 2^1 + 2^0 = 8 + 4 + 2 + 1
	So x^15 = x^8 * x^4 * x^2 * x^1
	*/
	double power_iterative_using_bits_in_exponent(double base, int exponent)
	{
		double res;
		if (specialCase(base, exponent, res))
			return res;

		if (exponent < 0)
			return 1.0 / power_iterative_using_bits_in_exponent(base, -exponent);

		double result = 1;
		
		double factor = base;
		for (; exponent > 0; exponent = exponent >> 1 /*or exponent /= 2*/)
		{
			if ((exponent & 1) == 1) //exponent % 2 == 1 
				result = result * factor;

			factor = factor * factor;
		}

		return result;
	}

	//Trial methods
	double power1(double base, int exponent, int& numMultiplicatiions)
	{
		double res;
		if (specialCase(base, exponent, res))
			return res;

		if (exponent < 0)
			return 1.0 / power3(base, -exponent, numMultiplicatiions);

		if (exponent % 2 == 0)
			numMultiplicatiions += 1;
		else
			numMultiplicatiions += 2;

		return (exponent % 2 == 0 ? 1 : base) * power3(base * base, exponent / 2, numMultiplicatiions);
	}

	double power2(double base, int exponent, int& numMultiplicatiions)
	{
		double res;
		if (specialCase(base, exponent, res))
			return res;

		if (exponent < 0)
			return 1.0 / power2(base, -exponent, numMultiplicatiions);

		if (exponent % 2 == 0)
			numMultiplicatiions += 1;
		else
			numMultiplicatiions += 2;

		return exponent % 2 == 0 
			? power2(base * base, exponent / 2, numMultiplicatiions) 
			: base * power2(base * base, exponent / 2, numMultiplicatiions);
	}

	double power3(double base, int exponent, int& numMultiplicatiions)
	{
		double res;
		if (specialCase(base, exponent, res))
			return res;

		if (exponent < 0)
			return 1.0 / power1(base, -exponent, numMultiplicatiions);

		double result = 1;
		if (exponent % 2 == 1)
		{
			result = base * power1(base, exponent - 1, numMultiplicatiions);
			++numMultiplicatiions;
		}
		else
		{
			result = power1(base, exponent / 2, numMultiplicatiions);
			result *= result;
			++numMultiplicatiions;
		}

		return result;
	}

	//The logic in followng function is made cleaner, clear and easy to understand in next version power2()
	double power4(double base, int exponent, int& numMultiplicatiions)
	{
		double result = 1.0;
		if (specialCase(base, exponent, result))
			return result;

		bool isNegExp = false;
		if (exponent < 0)
		{
			exponent = -exponent;
			isNegExp = true;
		}

		double factor = base;
		while (true)
		{
			if (exponent % 2 == 1)
			{
				result = result * factor;
				++numMultiplicatiions;
			}

			exponent /= 2;
			if (exponent == 0)
				break;

			factor = factor * factor;
			++numMultiplicatiions;
		}

		return isNegExp ? 1.0 / result : result;
	}

	double power5(double base, int exponent, int& numMultiplicatiions)
	{
		double res;
		if (specialCase(base, exponent, res))
			return res;

		bool isNegExp = false;
		if (exponent < 0)
		{
			exponent = -exponent;
			isNegExp = true;
		}

		double result = (exponent % 2 == 0) ? 1 : base;
		double factor = base;
		while (exponent > 1)
		{
			exponent /= 2;

			factor = factor * factor;
			++numMultiplicatiions;

			if (exponent % 2 == 1)
			{
				result = result * factor;
				++numMultiplicatiions;
			}
		}

		return isNegExp ? 1.0 / result : result;
	}

	double power6(double base, int exponent, int& numMultiplicatiions)
	{
		double res;
		if (specialCase(base, exponent, res))
			return res;

		bool isNegExp = false;
		if (exponent < 0)
		{
			exponent = -exponent;
			isNegExp = true;
		}

		double result = (exponent % 2 == 1) ? base : 1;
		double factor = base;
		for (exponent /= 2; exponent > 0; exponent /= 2)
		{
			factor = factor * factor;
			++numMultiplicatiions;

			if (exponent % 2 == 1)
			{
				result = result * factor;
				++numMultiplicatiions;
			}
		}

		return isNegExp ? 1.0 / result : result;
	}

	bool AreAllValuesEqual(const vector<double>& results)
	{
		for (int i = 1; i < results.size(); ++i)
		{
			if (fabs(results[i - 1] - results[i]) > 0.0001)
				return false;
		}

		return true;
	}

	void testPower(double base, int exponent)
	{
		vector<int> mul(6, 0);
		vector<double> results;
		results.push_back(power_recursive(base, exponent));
		results.push_back(power_iterative_v1(base, exponent));
		results.push_back(power_iterative_v2(base, exponent));
		results.push_back(power_iterative_v3(base, exponent));
		results.push_back(power_iterative_using_bits_in_exponent(base, exponent));
		results.push_back(power1(base, exponent, mul[0]));
		results.push_back(power2(base, exponent, mul[1]));
		results.push_back(power3(base, exponent, mul[2]));
		results.push_back(power4(base, exponent, mul[3]));
		results.push_back(power5(base, exponent, mul[4]));
		results.push_back(power6(base, exponent, mul[5]));
		cout << "\n";
		MM_EXPECT_TRUE(AreAllValuesEqual(results),
			base, exponent, results);
		cout << "\n" << std::right
			<< std::setw(12) << base
			<< std::setw(12) << exponent
			<< std::setw(12) << mul[0]
			<< std::setw(12) << mul[1]
			<< std::setw(12) << mul[2]
			<< std::setw(12) << mul[3]
			<< std::setw(12) << mul[4]
			<< std::setw(12) << mul[5];
	}

	MM_DECLARE_FLAG(arithmeticOperations_power);

	MM_UNIT_TEST(arithmeticOperations_power_test_1, arithmeticOperations_power)
	{
		cout << "\n" << std::right
			<< std::setw(12) << "base"
			<< std::setw(12) << "exponent"
			<< std::setw(12) << "mul"
			<< std::setw(12) << "mul"
			<< std::setw(12) << "mul"
			<< std::setw(12) << "mul"
			<< std::setw(12) << "mul"
			<< std::setw(12) << "mul";

		double base = 2;
		for (int exponent = 0; exponent < 100; exponent++)
			testPower(2, exponent);
	}

}

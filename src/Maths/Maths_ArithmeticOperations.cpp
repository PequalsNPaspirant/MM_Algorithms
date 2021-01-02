#include <iostream>
#include <iomanip>
using namespace std;

#include "Maths/Maths_ArithmeticOperations.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

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

	power(b, e) = (e % 2) == 0 ? power(b * b, e/2) : b * power(b * b, e/2)

	OR

	power(b, e) = ((e % 2) == 0 ? 1 : b) * power(b * b, e/2)

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
	double power_iterative(double base, int exponent)
	{
		double res;
		if (specialCase(base, exponent, res))
			return res;

		if (exponent < 0)
			return 1.0 / power_iterative(base, -exponent);

		double result = 1;
		double factor = base;
		while (true)
		{
			if (exponent % 2 == 1) // instead we can also use (exponent & 1) == 1 
				result = result * factor;

			exponent /= 2; // instead we can also use exponent = (exponent >> 1)
			if (exponent == 0) 
				break;

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
		double result1 = power_recursive(base, exponent);
		double result2 = power_iterative(base, exponent);
		double result3 = power1(base, exponent, mul[0]);
		double result4 = power2(base, exponent, mul[1]);
		double result5 = power3(base, exponent, mul[2]);
		double result6 = power4(base, exponent, mul[3]);
		double result7 = power5(base, exponent, mul[4]);
		double result8 = power6(base, exponent, mul[5]);
		//MM_EXPECT_TRUE(AreAllValuesEqual({ result1, result2, result3, result4, result5, result6, result7, result8 }),
		//	base, exponent, result1, result2, result3, result4, result5, result6, result7, result8);
		//MM_EXPECT_TRUE(mul1 == mul2 && mul2 == mul3 && mul3 == mul4, base, exponent, mul1, mul2, mul3, mul4);
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

/*
Output:

Executing unit tests...

==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 0, 1, 0, 0
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 1, 2, 2, 2
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 2, 3, 2, 2
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 2, 3, 3, 3
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 3, 4, 3, 3
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 3, 4, 4, 4
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 4, 5, 4, 4
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 3, 4, 4, 4
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 4, 5, 4, 4
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 4, 5, 5, 5
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 5, 6, 5, 5
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 4, 5, 5, 5
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 5, 6, 5, 5
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 5, 6, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 6, 7, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 4, 5, 5, 5
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 5, 6, 5, 5
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 5, 6, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 6, 7, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 5, 6, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 6, 7, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 6, 7, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 5, 6, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 6, 7, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 6, 7, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 6, 7, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 5, 6, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 6, 7, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 6, 7, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 6, 7, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 6, 7, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 6, 7, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 9, 10, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 10, 11, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 6, 7, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 9, 10, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 10, 11, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 9, 10, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 10, 11, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 9, 10, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 10, 11, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 9, 10, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 10, 11, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 10, 11, 11, 11
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 11, 12, 11, 11
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
==> MM_UNIT_TEST_RESULT : FAILED | Test Name: mm::testPower
Validation Statement: mul1 == mul2 && mul2 == mul3 && mul3 == mul4
Variables: mul1, mul2, mul3, mul4
Values   : 9, 10, 9, 9

Execution of unit tests is finished. All tests have passed unless any failure printed above!


1970-01-02_17-35-59.963,693,520 CONGRATULATIONS!!! End of program reached successfully.










Output of previous test when all validation done in same clause:


Executing unit tests...

==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1, 1, 1, 1, 1, 1, 0, 0, 0, 0
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 2, 2, 2, 2, 2, 2, 0, 1, 0, 0
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 4, 4, 4, 4, 4, 4, 1, 2, 2, 2
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 8, 8, 8, 8, 8, 8, 2, 3, 2, 2
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 16, 16, 16, 16, 16, 16, 2, 3, 3, 3
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 32, 32, 32, 32, 32, 32, 3, 4, 3, 3
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 64, 64, 64, 64, 64, 64, 3, 4, 4, 4
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 128, 128, 128, 128, 128, 128, 4, 5, 4, 4
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 256, 256, 256, 256, 256, 256, 3, 4, 4, 4
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 512, 512, 512, 512, 512, 512, 4, 5, 4, 4
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1024, 1024, 1024, 1024, 1024, 1024, 4, 5, 5, 5
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 2048, 2048, 2048, 2048, 2048, 2048, 5, 6, 5, 5
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 4096, 4096, 4096, 4096, 4096, 4096, 4, 5, 5, 5
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 8192, 8192, 8192, 8192, 8192, 8192, 5, 6, 5, 5
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 16384, 16384, 16384, 16384, 16384, 16384, 5, 6, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 32768, 32768, 32768, 32768, 32768, 32768, 6, 7, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 65536, 65536, 65536, 65536, 65536, 65536, 4, 5, 5, 5
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 131072, 131072, 131072, 131072, 131072, 131072, 5, 6, 5, 5
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 262144, 262144, 262144, 262144, 262144, 262144, 5, 6, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 524288, 524288, 524288, 524288, 524288, 524288, 6, 7, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.04858e+06, 1.04858e+06, 1.04858e+06, 1.04858e+06, 1.04858e+06, 1.04858e+06, 5, 6, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 2.09715e+06, 2.09715e+06, 2.09715e+06, 2.09715e+06, 2.09715e+06, 2.09715e+06, 6, 7, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 4.1943e+06, 4.1943e+06, 4.1943e+06, 4.1943e+06, 4.1943e+06, 4.1943e+06, 6, 7, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 8.38861e+06, 8.38861e+06, 8.38861e+06, 8.38861e+06, 8.38861e+06, 8.38861e+06, 7, 8, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.67772e+07, 1.67772e+07, 1.67772e+07, 1.67772e+07, 1.67772e+07, 1.67772e+07, 5, 6, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 3.35544e+07, 3.35544e+07, 3.35544e+07, 3.35544e+07, 3.35544e+07, 3.35544e+07, 6, 7, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 6.71089e+07, 6.71089e+07, 6.71089e+07, 6.71089e+07, 6.71089e+07, 6.71089e+07, 6, 7, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.34218e+08, 1.34218e+08, 1.34218e+08, 1.34218e+08, 1.34218e+08, 1.34218e+08, 7, 8, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 2.68435e+08, 2.68435e+08, 2.68435e+08, 2.68435e+08, 2.68435e+08, 2.68435e+08, 6, 7, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 5.36871e+08, 5.36871e+08, 5.36871e+08, 5.36871e+08, 5.36871e+08, 5.36871e+08, 7, 8, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.07374e+09, 1.07374e+09, 1.07374e+09, 1.07374e+09, 1.07374e+09, 1.07374e+09, 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 2.14748e+09, 2.14748e+09, 2.14748e+09, 2.14748e+09, 2.14748e+09, 2.14748e+09, 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 4.29497e+09, 4.29497e+09, 4.29497e+09, 4.29497e+09, 4.29497e+09, 4.29497e+09, 5, 6, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 8.58993e+09, 8.58993e+09, 8.58993e+09, 8.58993e+09, 8.58993e+09, 8.58993e+09, 6, 7, 6, 6
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.71799e+10, 1.71799e+10, 1.71799e+10, 1.71799e+10, 1.71799e+10, 1.71799e+10, 6, 7, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 3.43597e+10, 3.43597e+10, 3.43597e+10, 3.43597e+10, 3.43597e+10, 3.43597e+10, 7, 8, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 6.87195e+10, 6.87195e+10, 6.87195e+10, 6.87195e+10, 6.87195e+10, 6.87195e+10, 6, 7, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.37439e+11, 1.37439e+11, 1.37439e+11, 1.37439e+11, 1.37439e+11, 1.37439e+11, 7, 8, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 2.74878e+11, 2.74878e+11, 2.74878e+11, 2.74878e+11, 2.74878e+11, 2.74878e+11, 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 5.49756e+11, 5.49756e+11, 5.49756e+11, 5.49756e+11, 5.49756e+11, 5.49756e+11, 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.09951e+12, 1.09951e+12, 1.09951e+12, 1.09951e+12, 1.09951e+12, 1.09951e+12, 6, 7, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 2.19902e+12, 2.19902e+12, 2.19902e+12, 2.19902e+12, 2.19902e+12, 2.19902e+12, 7, 8, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 4.39805e+12, 4.39805e+12, 4.39805e+12, 4.39805e+12, 4.39805e+12, 4.39805e+12, 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 8.79609e+12, 8.79609e+12, 8.79609e+12, 8.79609e+12, 8.79609e+12, 8.79609e+12, 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.75922e+13, 1.75922e+13, 1.75922e+13, 1.75922e+13, 1.75922e+13, 1.75922e+13, 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 3.51844e+13, 3.51844e+13, 3.51844e+13, 3.51844e+13, 3.51844e+13, 3.51844e+13, 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 7.03687e+13, 7.03687e+13, 7.03687e+13, 7.03687e+13, 7.03687e+13, 7.03687e+13, 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.40737e+14, 1.40737e+14, 1.40737e+14, 1.40737e+14, 1.40737e+14, 1.40737e+14, 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 2.81475e+14, 2.81475e+14, 2.81475e+14, 2.81475e+14, 2.81475e+14, 2.81475e+14, 6, 7, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 5.6295e+14, 5.6295e+14, 5.6295e+14, 5.6295e+14, 5.6295e+14, 5.6295e+14, 7, 8, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.1259e+15, 1.1259e+15, 1.1259e+15, 1.1259e+15, 1.1259e+15, 1.1259e+15, 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 2.2518e+15, 2.2518e+15, 2.2518e+15, 2.2518e+15, 2.2518e+15, 2.2518e+15, 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 4.5036e+15, 4.5036e+15, 4.5036e+15, 4.5036e+15, 4.5036e+15, 4.5036e+15, 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 9.0072e+15, 9.0072e+15, 9.0072e+15, 9.0072e+15, 9.0072e+15, 9.0072e+15, 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.80144e+16, 1.80144e+16, 1.80144e+16, 1.80144e+16, 1.80144e+16, 1.80144e+16, 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 3.60288e+16, 3.60288e+16, 3.60288e+16, 3.60288e+16, 3.60288e+16, 3.60288e+16, 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 7.20576e+16, 7.20576e+16, 7.20576e+16, 7.20576e+16, 7.20576e+16, 7.20576e+16, 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.44115e+17, 1.44115e+17, 1.44115e+17, 1.44115e+17, 1.44115e+17, 1.44115e+17, 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 2.8823e+17, 2.8823e+17, 2.8823e+17, 2.8823e+17, 2.8823e+17, 2.8823e+17, 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 5.76461e+17, 5.76461e+17, 5.76461e+17, 5.76461e+17, 5.76461e+17, 5.76461e+17, 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.15292e+18, 1.15292e+18, 1.15292e+18, 1.15292e+18, 1.15292e+18, 1.15292e+18, 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 2.30584e+18, 2.30584e+18, 2.30584e+18, 2.30584e+18, 2.30584e+18, 2.30584e+18, 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 4.61169e+18, 4.61169e+18, 4.61169e+18, 4.61169e+18, 4.61169e+18, 4.61169e+18, 9, 10, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 9.22337e+18, 9.22337e+18, 9.22337e+18, 9.22337e+18, 9.22337e+18, 9.22337e+18, 10, 11, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.84467e+19, 1.84467e+19, 1.84467e+19, 1.84467e+19, 1.84467e+19, 1.84467e+19, 6, 7, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 3.68935e+19, 3.68935e+19, 3.68935e+19, 3.68935e+19, 3.68935e+19, 3.68935e+19, 7, 8, 7, 7
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 7.3787e+19, 7.3787e+19, 7.3787e+19, 7.3787e+19, 7.3787e+19, 7.3787e+19, 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.47574e+20, 1.47574e+20, 1.47574e+20, 1.47574e+20, 1.47574e+20, 1.47574e+20, 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 2.95148e+20, 2.95148e+20, 2.95148e+20, 2.95148e+20, 2.95148e+20, 2.95148e+20, 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 5.90296e+20, 5.90296e+20, 5.90296e+20, 5.90296e+20, 5.90296e+20, 5.90296e+20, 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.18059e+21, 1.18059e+21, 1.18059e+21, 1.18059e+21, 1.18059e+21, 1.18059e+21, 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 2.36118e+21, 2.36118e+21, 2.36118e+21, 2.36118e+21, 2.36118e+21, 2.36118e+21, 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 4.72237e+21, 4.72237e+21, 4.72237e+21, 4.72237e+21, 4.72237e+21, 4.72237e+21, 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 9.44473e+21, 9.44473e+21, 9.44473e+21, 9.44473e+21, 9.44473e+21, 9.44473e+21, 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.88895e+22, 1.88895e+22, 1.88895e+22, 1.88895e+22, 1.88895e+22, 1.88895e+22, 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 3.77789e+22, 3.77789e+22, 3.77789e+22, 3.77789e+22, 3.77789e+22, 3.77789e+22, 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 7.55579e+22, 7.55579e+22, 7.55579e+22, 7.55579e+22, 7.55579e+22, 7.55579e+22, 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.51116e+23, 1.51116e+23, 1.51116e+23, 1.51116e+23, 1.51116e+23, 1.51116e+23, 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 3.02231e+23, 3.02231e+23, 3.02231e+23, 3.02231e+23, 3.02231e+23, 3.02231e+23, 9, 10, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 6.04463e+23, 6.04463e+23, 6.04463e+23, 6.04463e+23, 6.04463e+23, 6.04463e+23, 10, 11, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.20893e+24, 1.20893e+24, 1.20893e+24, 1.20893e+24, 1.20893e+24, 1.20893e+24, 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 2.41785e+24, 2.41785e+24, 2.41785e+24, 2.41785e+24, 2.41785e+24, 2.41785e+24, 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 4.8357e+24, 4.8357e+24, 4.8357e+24, 4.8357e+24, 4.8357e+24, 4.8357e+24, 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 9.67141e+24, 9.67141e+24, 9.67141e+24, 9.67141e+24, 9.67141e+24, 9.67141e+24, 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.93428e+25, 1.93428e+25, 1.93428e+25, 1.93428e+25, 1.93428e+25, 1.93428e+25, 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 3.86856e+25, 3.86856e+25, 3.86856e+25, 3.86856e+25, 3.86856e+25, 3.86856e+25, 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 7.73713e+25, 7.73713e+25, 7.73713e+25, 7.73713e+25, 7.73713e+25, 7.73713e+25, 9, 10, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.54743e+26, 1.54743e+26, 1.54743e+26, 1.54743e+26, 1.54743e+26, 1.54743e+26, 10, 11, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 3.09485e+26, 3.09485e+26, 3.09485e+26, 3.09485e+26, 3.09485e+26, 3.09485e+26, 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 6.1897e+26, 6.1897e+26, 6.1897e+26, 6.1897e+26, 6.1897e+26, 6.1897e+26, 9, 10, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.23794e+27, 1.23794e+27, 1.23794e+27, 1.23794e+27, 1.23794e+27, 1.23794e+27, 9, 10, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 2.47588e+27, 2.47588e+27, 2.47588e+27, 2.47588e+27, 2.47588e+27, 2.47588e+27, 10, 11, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 4.95176e+27, 4.95176e+27, 4.95176e+27, 4.95176e+27, 4.95176e+27, 4.95176e+27, 9, 10, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 9.90352e+27, 9.90352e+27, 9.90352e+27, 9.90352e+27, 9.90352e+27, 9.90352e+27, 10, 11, 10, 10
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.9807e+28, 1.9807e+28, 1.9807e+28, 1.9807e+28, 1.9807e+28, 1.9807e+28, 10, 11, 11, 11
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 3.96141e+28, 3.96141e+28, 3.96141e+28, 3.96141e+28, 3.96141e+28, 3.96141e+28, 11, 12, 11, 11
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 7.92282e+28, 7.92282e+28, 7.92282e+28, 7.92282e+28, 7.92282e+28, 7.92282e+28, 7, 8, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 1.58456e+29, 1.58456e+29, 1.58456e+29, 1.58456e+29, 1.58456e+29, 1.58456e+29, 8, 9, 8, 8
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 3.16913e+29, 3.16913e+29, 3.16913e+29, 3.16913e+29, 3.16913e+29, 3.16913e+29, 8, 9, 9, 9
==> MM_UNIT_TEST_RESULT : OK | Test Name: mm::testPower
Validation Statement: AreAllValuesEqual(result1, result2, result3, result4, result5, result6)
Variables: result1, result2, result3, result4, result5, result6, mul1, mul2, mul3, mul4
Values   : 6.33825e+29, 6.33825e+29, 6.33825e+29, 6.33825e+29, 6.33825e+29, 6.33825e+29, 9, 10, 9, 9

Execution of unit tests is finished. All tests have passed unless any failure printed above!


1970-01-02_16-42-51.242,498,496 CONGRATULATIONS!!! End of program reached successfully.





*/
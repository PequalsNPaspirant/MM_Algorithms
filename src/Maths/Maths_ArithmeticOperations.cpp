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

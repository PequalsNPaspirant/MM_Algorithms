//Goal

#include <iostream>
#include <vector>
#include <iomanip> //for setw(n)
#include <cmath>
using namespace std;

#include "Others/Others_FloatingPoint.h"
#include "C++/C++_DataTypes.h"

namespace mm {

	/*
	Info:

	Binary representation:

	 seeeeeeeemmmmmmmmmmmmmmmmmmmmmmm    meaning
	31                               0    bit #

	s = 1 sign bit, e = 11 exponent bits, m = 52 mantissa bits

	Hex representation:

	EE EM MM MM MM MM MM MM = total 8 bytes

	Special Cases:
	Sr. No.	Exponent		Mantissa	Meaning
	1.		000				0			+zero (special case)
	2.		800				0			-zero (special case)
	3.		000 or 800		not 0		Subnormal or Denormal number
	4.		7ff				0			+infinity
	5.		fff				0			-infinity
	6.		7ff or fff		not zero	Nan

	Value:
	If  Exponent = The integer formed from all E bits
		Mantissa = The integer formed from all M bits
	Then
		For case 1 to 3 = (-1)^s * 2^(-1023) * (0 + Mantissa * 2^(-52))  // Exponent = 0 always
		For case 4 to 6 = infinity or NaN
		For rest		= (-1)^s * 2^(Exponent - 1023) * (1 + Mantissa * 2^(-52))

	In computer science, denormal numbers or denormalized numbers (now often called subnormal numbers) fill
	the underflow gap around zero in floating-point arithmetic. Any non-zero number with magnitude smaller
	than the smallest normal number is 'subnormal'.


	//  Float           Memory          Bias (unsigned)
	//  -----           ------          ---------------
	//   NaN            0xFFFFFFFF      0xFF800001
	//   NaN            0xFF800001      0xFFFFFFFF
	//  -Infinity       0xFF800000      0x00000000 ---
	//  -3.40282e+038   0xFF7FFFFF      0x00000001    |
	//  -1.40130e-045   0x80000001      0x7F7FFFFF    |
	//  -0.0            0x80000000      0x7F800000    |--- Valid <= 0xFF000000.
	//   0.0            0x00000000      0x7F800000    |    NaN > 0xFF000000
	//   1.40130e-045   0x00000001      0x7F800001    |
	//   3.40282e+038   0x7F7FFFFF      0xFEFFFFFF    |
	//   Infinity       0x7F800000      0xFF000000 ---
	//   NaN            0x7F800001      0xFF000001
	//   NaN            0x7FFFFFFF      0xFF7FFFFF
	//
	//   Either value of NaN returns false.
	//   -Infinity and +Infinity are not "close".
	//   -0 and +0 are equal.


	*/

	void FloatingPointExplained::analyseDouble(double value)
	{
		//in general: number = (sign ? -1 : 1) * 2^(exponent) * 1.(mantissa bits)

		bool compareResults = true;
		cout << "\n\n";
		cout << setw(10) << "Value: " << std::defaultfloat << value;
		cout << " or " << std::scientific << std::setprecision(std::numeric_limits<double>::digits) << value; // << " or " << std::scientific << value;

		cout << " ";
		switch (std::fpclassify(value)) {
		case FP_INFINITE:  cout << "Double Value Classification: Infinity"; compareResults = false; break;
		case FP_NAN:       cout << "Double Value Classification: Nan"; compareResults = false; break;
		case FP_NORMAL:    cout << "Double Value Classification: Normal"; break;
		case FP_SUBNORMAL: cout << "Double Value Classification: Subnormal"; break;
		case FP_ZERO:      cout << "Double Value Classification: Zero"; break;
		default:           cout << "Double Value Classification: Unknown"; MM_Assert::mmRunTimeAssert(false);
		}

		double myZero = 1.0E-10;

		//Hexadecimal representation
		cout << "\n";
		cout << setw(10) << "Hex: ";
		unsigned char* ch = (unsigned char*)&value + (sizeof(value) - 1);
		for (int i = sizeof(value); i > 0; i -= sizeof(char), --ch)
			std::cout << setw(2) << std::hex << std::left << std::setfill('0') << int(*ch) << " ";

		cout << std::defaultfloat << std::setfill('\0') << std::right;

		// METHOD 1 ==================================
		cout << "\n";
		string bitStr = DataTypes::GetBitString(value);
		string bitStr2 = DataTypes::GetBitString2(value);
		cout << setw(10) << "Bit Str: " << bitStr2;

		string::size_type n = 0;
		while ((n = bitStr2.find_first_of(" _", n)) != std::string::npos)
			bitStr2.erase(n++, 1);

		MM_Assert::mmRunTimeAssert(bitStr == bitStr2);

		// METHOD 2 ==================================
		cout << "\n";
		//Bitwise reprentation
		vector<short> bits(sizeof(value) * 8);
		DataTypes::ShowBitwiseMemoryRepresentation(value, bits, true);

		//Build double back from sign, exponent and significand
		const short signBitPosition = 0;
		const short lastExponentBitPosition = 11;

		//Calculate significand
		double significand = 0; //a.k.a. the mantissa or fraction
		for (short i = 63; i > lastExponentBitPosition; i--)
			significand += bits[i] * pow(2, -(i - lastExponentBitPosition));

		significand += 1.0;
		if (bits[0] == 1)
			significand = -significand;

		//Calculate exponent
		int exponentBase2 = 0;
		for (short i = lastExponentBitPosition; i > signBitPosition; i--)
			exponentBase2 += bits[i] * static_cast<int>(pow(2, lastExponentBitPosition - i));

		const int biasedComponent = 1023;
		int exponentBase2afterBiasAdjusted = exponentBase2 - biasedComponent;
		//cout << "\nIn other form: " << std::scientific << std::setprecision(std::numeric_limits<double>::digits) << significand << " x 2^ " << exponentBase2afterBiasAdjusted;
		double val4 = significand * pow(2, exponentBase2afterBiasAdjusted);
		if (compareResults)
			MM_Assert::mmRunTimeAssert(fabs(value - val4) < myZero, "Tally failed");
		//MM_Assert::mmRunTimeAssert(FloatngPointComparison::checkIfAlmostEqual(value, val4, myZero), "Tally failed");

		double base2To10conversionFactor = log(2.0) / log(10.0);
		double exponentBase10 = exponentBase2afterBiasAdjusted * base2To10conversionFactor;
		//cout << "\nIn other form: " << std::scientific << std::setprecision(std::numeric_limits<double>::digits) << significand << " E " << exponentBase10;
		double val5 = significand * pow(10.0, exponentBase10);
		//MM_Assert::mmRunTimeAssert(fabs(value - val5) < myZero, "Tally failed");
		if (compareResults)
			MM_Assert::mmRunTimeAssert(FloatngPointComparison::checkIfAlmostEqual(value, val5, myZero), "Tally failed");

		// METHOD 3 ==================================
		unsigned long long* ptr = (unsigned long long*)&value;
		int s = *ptr >> 63; //extract single sign bit
		int e = ((*ptr << 1) >> 53); //extract 11 exponent bits
		long long m = ((*ptr << 12) >> 12); //extract 52 mantissa bits
		double mntsa = (1 + m * pow(2, -52));
		double val2 = pow(-1, s) * mntsa * pow(2, (e - 1023));
		if (compareResults)
			MM_Assert::mmRunTimeAssert(fabs(value - val2) < myZero, "Tally failed");

		// METHOD 4 ==================================
		int exp;
		double mantissa = std::frexp(value, &exp);
		double val = std::ldexp(mantissa, exp);
		if (compareResults)
			MM_Assert::mmRunTimeAssert(fabs(value - val) < myZero, "Tally failed");

		// METHOD 5 ==================================
		//typedef union {
		//	double d;
		//	struct {
		//		//Following variables consider all adjacent bits, so need to evaluate it properly
		//		long long mantissa : 52;
		//		int exponent : 11;
		//		int sign : 1;
		//	} parts;
		//} double_cast;
		//double_cast d1;
		//d1.d = value;
		//double mantissa2 = (1 + d1.parts.mantissa * pow(2, -52));
		//double val3 = pow(-1, d1.parts.sign) * mantissa2 * pow(2, (d1.parts.exponent - 1023));
		//MM_Assert::mmRunTimeAssert(fabs(value - val3) < myZero, "Tally failed");

	}

	void FloatingPointExplained::testFloatingPointBitwiseMemoryRepresentation()
	{
		cout << "\nAnalysis of floating point number representation:";
		cout << "\n\nSpecial cases:";
		typedef std::numeric_limits<double> type;
		double specialDoubles[8] = {
			+0.0,				// Sign bit: 0, Exponent: All bits 0, Mantissa: All bits 0		//Zero
			-0.0,				// Sign bit: 1, Exponent: All bits 0, Mantissa: All bits 0		//-Zero
			type::min() / 2,	// Sign bit: 0, Exponent: All bits 0, Mantissa: All bits NOT 0	//Denormal
			-type::min() / 2,	// Sign bit: 1, Exponent: All bits 0, Mantissa: All bits NOT 0	//Denormal
			//1.0 / (-1.0 + 1.0),	// Sign bit: 0, Exponent: All bits 1, Mantissa: All bits 0		//+Infinity
			type::infinity(),	// Sign bit: 0, Exponent: All bits 1, Mantissa: All bits 0		//+Infinity
			-type::infinity(),	// Sign bit: 1, Exponent: All bits 1, Mantissa: All bits 0		//-Infinity
			type::quiet_NaN(),	// Sign bit: 0, Exponent: All bits 1, Mantissa: All bits NOT 0	//Nan
			//-0.0 / 0.0			// Sign bit: 1, Exponent: All bits 1, Mantissa: All bits NOT 0	//Nan	
			-type::quiet_NaN(),	// Sign bit: 1, Exponent: All bits 1, Mantissa: All bits NOT 0	//Nan	
		};
		for (int i = 0; i < 8; i++)
			FloatingPointExplained::analyseDouble(specialDoubles[i]);

		cout << "\n\nExtreme cases:";
		cout << "\nTesting numeric_limits<double>::max():";
		double maxVal = numeric_limits<double>::max();
		FloatingPointExplained::analyseDouble(maxVal);

		cout << "\nTesting numeric_limits<double>::min():";
		double minVal = numeric_limits<double>::min();
		FloatingPointExplained::analyseDouble(minVal);

		cout << "\nTesting numeric_limits<double>::lowest():";
		double lowest = numeric_limits<double>::lowest();
		FloatingPointExplained::analyseDouble(lowest);

		cout << "\n\nSome impossible Extreme cases:";

		for (double i = 2.0; i < 1025; i *= 2.0)
		{
			cout << "\nTesting numeric_limits<double>::min() / " << i << ":";
			double minVal = numeric_limits<double>::min();
			FloatingPointExplained::analyseDouble(minVal / i);
		}

		for (double i = 2.0; i < 1025; i *= 2.0)
		{
			cout << "\nTesting numeric_limits<double>::max() * " << i << ":";
			double maxVal = numeric_limits<double>::max();
			FloatingPointExplained::analyseDouble(maxVal * i);
		}

		cout << "\n\nOther test cases:";

		vector<double> testValues{
			1.2345, 123.456,
			-1, -2, -3, -4, -5, -10, -100, -1000, -10000,
			0, 1, 2, 3, 4, 5, 10, 100, 1000, 1000,
			0.1, 0.2, 0.3, 0.4, 0.5
		};
		for (size_t i = 0; i < testValues.size(); i++)
			FloatingPointExplained::analyseDouble(testValues[i]);

		//Test numbers between 0 and 1
		for (double d = 0; d <= 1; d += 0.0625)
			FloatingPointExplained::analyseDouble(d);

		//Type interval = (maxVal - minVal) / 50; it can cause overflow or underflow
		const int count = 200;
		double interval = maxVal / count - lowest / count;
		for (double start = lowest + interval; start < (maxVal - interval); start += interval)
			FloatingPointExplained::analyseDouble(start);
	}

	// =================================================================================================================

	void FloatngPointComparison::testFloatngPointComparison()
	{
		// TODO: Use all comparison function and see which is best
		double d1 = 0.2;
		double d2 = 1 / std::sqrt(5) / std::sqrt(5);

		if (d1 == d2)
			std::cout << "d1 == d2\n";
		else
			std::cout << "d1 != d2\n";

		if (FloatngPointComparison::almost_equal(d1, d2, 2))
			std::cout << "d1 almost equals d2\n";
		else
			std::cout << "d1 does not almost equal d2\n";
	}

	// =================================================================================================================

	void FloatingPointTest()
	{
		FloatingPointExplained::testFloatingPointBitwiseMemoryRepresentation();
		FloatngPointComparison::testFloatngPointComparison();
	}

}

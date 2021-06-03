//Goal
/*
Convert decimal fraction into binary fraction and then into double floating point format.
Convert decimal fraction directly into double floating point format.
Tally above bit pattern with actual bit pattern of double of equal value stored in memory.
Create a bit pattern from character array and cast it to double and compare its value with 
	actual double of equal value stored in memory.
*/

#include <cstdint>
#include <limits>
#include <climits>
#include <cmath>
#include <cstring>
#include <cmath>
using namespace std;

#include "Utils/Utils_MM_Assert.h"

namespace mm {

	template<typename From, typename To>
	struct alias_cast_t
	{
		union
		{
			From raw;
			To data;
		};
	};

	template<typename From, typename To>
	To alias_cast(From raw_data)
	{
		alias_cast_t<From, To> ac;
		ac.raw = raw_data;
		return ac.data;
	}

	class FloatingPointExplained
	{
	public:
		static void testFloatingPointBitwiseMemoryRepresentation();

		//Check if NaN
		inline bool IsNan_1(float f)
		{
			const std::uint32_t u = *(alias_cast<float*, std::uint32_t*>(&f));
			return (u & 0x7F800000) == 0x7F800000 && (u & 0x7FFFFF);    // Both NaN and qNan.
		}

		inline bool IsNan_2(double d)
		{
			const std::uint64_t u = *(alias_cast<double*, std::uint64_t*>(&d));
			return (u & 0x7FF0000000000000ULL) == 0x7FF0000000000000ULL && (u & 0xFFFFFFFFFFFFFULL);
		}
		static bool IsNan_3(double value)
		{
			return ((*(alias_cast<double*, unsigned long long*>(&value))) & 0x7fffffff) > 0x7f800000;
		}

		static bool IsNan_4(double value)
		{
			return value != value;
		}
		static bool IsNan_5(double x)
		{
			return (((x) < 0) == ((x) >= 0));
		}
		static bool IsNan_6(double x)
		{
			return (!((x) < 0) && !((x) >= 0));
		}

		union NaN
		{
			uint64_t bits;
			double num;
		};

		static bool IsNan_7(double x)
		{
			union NaN n;
			n.num = x;
			if ((n.bits | 0x800FFFFFFFFFFFFF) == 0xFFFFFFFFFFFFFFFF)
			{
				//printf("NaN: %f", x);
				return true;
			}
			return false;
		}

		static bool IsNan_8(double x)
		{
			return std::isnan(x);
		}

		static bool IsNan_9(double x)
		{
			return fpclassify(x) == FP_NAN;
		}

		using Fp_info = numeric_limits<double>;

		static bool IsNan_10(double x)
		{
			return x == Fp_info::quiet_NaN();
		}

		static bool IsNan_11(double x)
		{
			return ilogb(x) == FP_ILOGBNAN;
		}

		static bool IsNan_12(double x)
		{
			return isunordered(1.2345, x);
		}

		static bool IsNan_13(double x)
		{
			return is_ieee754_nan(x);
		}

		static inline auto is_ieee754_nan(double const x)
			-> bool
		{
			static constexpr bool   is_claimed_ieee754 = Fp_info::is_iec559;
			static constexpr int    n_bits_per_byte = CHAR_BIT;
			using Byte = unsigned char;

			static_assert(is_claimed_ieee754, "!");
			static_assert(n_bits_per_byte == 8, "!");
			static_assert(sizeof(x) == sizeof(uint64_t), "!");

#ifdef _MSC_VER
			uint64_t const bits = reinterpret_cast<uint64_t const&>(x);
#else
			Byte bytes[sizeof(x)];
			memcpy(bytes, &x, sizeof(x));
			uint64_t int_value;
			memcpy(&int_value, bytes, sizeof(x));
			uint64_t const& bits = int_value;
#endif

			static constexpr uint64_t   sign_mask = 0x8000000000000000;
			static constexpr uint64_t   exp_mask = 0x7FF0000000000000;
			static constexpr uint64_t   mantissa_mask = 0x000FFFFFFFFFFFFF;

			(void)sign_mask;
			return (bits & exp_mask) == exp_mask && (bits & mantissa_mask) != 0;
		}

	private:
		static void analyseDouble(double value);
	};

	// ==============================================================================================================




	// ==============================================================================================================================

	class FloatngPointComparison
	{
	public:
		//Test all comparison functions
		static void testFloatngPointComparison();

		//My own methods
		static bool checkIfAlmostEqual(double v1, double v2, double tolerance)
		{
			return
				(fabs(v1 - v2) < tolerance) //Handles anything other than extreme cases i.e. smallest and largest numbers
				|| checkIfClose(v1, v2, tolerance); //Handles extreme cases
		}

		static bool checkIfClose(double v1, double v2, double tolerance)
		{
			return fabs(v1 - v2) < (fabs(std::fmax(v1, v2)) * tolerance / 100.0);
		}

		//From cppreference.com
		template<class T>
		static typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
			almost_equal(T x, T y, int ulp)
		{
			// the machine epsilon has to be scaled to the magnitude of the values used
			// and multiplied by the desired precision in ULPs (unit in the last place or unit of least precision)
			return std::abs(x - y) < std::numeric_limits<T>::epsilon() * std::abs(x + y) * ulp
				// unless the result is subnormal
				|| std::abs(x - y) < std::numeric_limits<T>::min();
		}

		//From other source
		/*
		The difference between X and the next value of X varies according to X.
		DBL_EPSILON is only the difference between 1 and the next value of 1.

		You can use std::nextafter for testing two double with epsilon difference:
		*/
		bool nearly_equal(double a, double b)
		{
			return std::nextafter(a, std::numeric_limits<double>::lowest()) <= b
				&& std::nextafter(a, std::numeric_limits<double>::max()) >= b;
		}

		//If you would like to test two double with factor * epsilon difference, you can use:
		bool nearly_equal(double a, double b, int factor /* a factor of epsilon */)
		{
			double min_a = a - (a - std::nextafter(a, std::numeric_limits<double>::lowest())) * factor;
			double max_a = a + (std::nextafter(a, std::numeric_limits<double>::max()) - a) * factor;

			return min_a <= b && max_a >= b;
		}

		//From other source
		//implements relative method - do not use for comparing with zero
		//use this most of the time, tolerance needs to be meaningful in your context
		template<typename TReal>
		static bool isApproximatelyEqual(TReal a, TReal b, TReal tolerance = std::numeric_limits<TReal>::epsilon())
		{
			TReal diff = std::fabs(a - b);
			if (diff <= tolerance)
				return true;

			if (diff < std::fmax(std::fabs(a), std::fabs(b)) * tolerance)
				return true;

			return false;
		}

		//supply tolerance that is meaningful in your context
		//for example, default tolerance may not work if you are comparing double with float
		template<typename TReal>
		static bool isApproximatelyZero(TReal a, TReal tolerance = std::numeric_limits<TReal>::epsilon())
		{
			if (std::fabs(a) <= tolerance)
				return true;
			return false;
		}


		//use this when you want to be on safe side
		//for example, don't start rover unless signal is above 1
		template<typename TReal>
		static bool isDefinitelyLessThan(TReal a, TReal b, TReal tolerance = std::numeric_limits<TReal>::epsilon())
		{
			TReal diff = a - b;
			if (diff < tolerance)
				return true;

			if (diff < std::fmax(std::fabs(a), std::fabs(b)) * tolerance)
				return true;

			return false;
		}
		template<typename TReal>
		static bool isDefinitelyGreaterThan(TReal a, TReal b, TReal tolerance = std::numeric_limits<TReal>::epsilon())
		{
			TReal diff = a - b;
			if (diff > tolerance)
				return true;

			if (diff > std::fmax(std::fabs(a), std::fabs(b)) * tolerance)
				return true;

			return false;
		}

		//implements ULP method
		//use this when you are only concerned about floating point precision issue
		//for example, if you want to see if a is 1.0 by checking if its within
		//10 closest representable floating point numbers around 1.0.
		template<typename TReal>
		static bool isWithinPrecisionInterval(TReal a, TReal b, unsigned int interval_size = 1)
		{
			TReal min_a = a - (a - std::nextafter(a, std::numeric_limits<TReal>::lowest())) * interval_size;
			TReal max_a = a + (std::nextafter(a, std::numeric_limits<TReal>::max()) - a) * interval_size;

			return min_a <= b && max_a >= b;
		}

		//From other source
		float close_rel(float a, float b, float epsilon)
		{
			// assume small positive epsilon
			MM_Assert::mmRunTimeAssert(epsilon >= 0.0f && epsilon <= 1.0f);

			float diff = abs(a - b);
			float maxab = fmax(abs(a), abs(b));

			// if the multiply won't underflow then use a multiply
			if (maxab >= 1.0f)
			{
				return diff <= (epsilon * maxab);
			}
			// multiply could underflow so use a divide if nonzero denominator
			else if (maxab > 0.0f)
			{
				// correctly returns false on divide overflow
				// (inf < = epsilon is false), since overflow means the
				// relative difference is large and they are therefore not close
				return diff / maxab <= epsilon;
			}
			else
			{
				// both a and b are zero
				return false;
			}
		}

		//From other source
		/*
		#include <ieee754.h>

		int flt_equals(float a, float b, int sigfigs)
		{
		union ieee754_float *pa, *pb;
		unsigned int aexp, bexp;
		float sig_mag;

		if (a == b)
		return 1;
		pa = (union ieee754_float*)&a;
		pb = (union ieee754_float*)&b;
		aexp = pa->ieee.exponent;
		bexp = pb->ieee.exponent;
		if (aexp != bexp || pa->ieee.negative != pb->ieee.negative)
		return 0;
		pa->ieee.exponent = pb->ieee.exponent = IEEE754_FLOAT_BIAS;
		sig_mag = pow(10, -(float)sigfigs);
		if (fabs(a - b) < sig_mag / 2)
		return 1;
		return 0;
		}

		source: http://www.cprogramming.com/tutorial/floating_point/understanding_floating_point_representation.html
		How does this function work? First, if the numbers are totally equal, then we know the answer right away. Next, we check the exponents and signs; if the numbers have different exponents then they cannot be equal (because of the 1.m representation). In the rest of the function, we use the old fabs-less-than idiom, but validating its assumption by actually setting the exponents of the two numbers to zero. We compare against 0.5*10^-sigfigs, which provides for rounding. Imagine sigfigs==2—then we would want 0.01 to equal 0.008 and 0.012, but not 0.018. Hence we compare the difference to .005, which is 0.5*10^-2.
		In production code, you would want to avoid computing sig_mag every time, probably by doing so in another function that sets the desired precision. Also note that I can't make any guarantees regarding the correctness or performance of this code; like any code I present it is intended only for instructional purposes.

		You might have noticed that this function does not do a great job comparing numbers to zero (since zero's representation is a special case, zero is a special case here as well). Experience has shown that numbers which "should be" zero do not tend to have a zero exponent—they are not those special extra-small numbers. Instead they are just some random value around epsilon, like 6.12303e-17, which is what I get for the double-precision cosine of pi/2 (this is probably because algorithm designers often aim to get their results within epsilon). In this case it is better to compare the absolute value of a number to sig_mag/2 directly, without manipulating exponents.

		This function is also deficient for comparing those extra-small numbers. We would have to handle their different representation, in which significant digits begin wherever the first set bit is. The function will typically conclude that two such numbers are equal (since, effectively, it will just be comparing leading zeros), unless you ask for enough digits to reach all the way out to whatever bits are set.
		*/

		//From other source
		bool same(double a, double b)
		{
			return std::nextafter(a, std::numeric_limits<double>::lowest()) <= b
				&& std::nextafter(a, std::numeric_limits<double>::max()) >= b;
		}

		//From other source
		bool approximatelyEqual(float a, float b, float epsilon)
		{
			return fabs(a - b) <= ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
		}

		bool essentiallyEqual(float a, float b, float epsilon)
		{
			return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
		}

		bool definitelyGreaterThan(float a, float b, float epsilon)
		{
			return (a - b) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
		}

		bool definitelyLessThan(float a, float b, float epsilon)
		{
			return (b - a) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
		}

		//From other source
		// Usable AlmostEqual function    
		bool AlmostEqual2sComplement(float A, float B, int maxUlps)
		{
			// Make sure maxUlps is non-negative and small enough that the    
			// default NAN won't compare as equal to anything.    
			MM_Assert::mmRunTimeAssert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024, "");
			int aInt = *(alias_cast<float*, int*>(&A));
			// Make aInt lexicographically ordered as a twos-complement int    
			if (aInt < 0)
				aInt = 0x80000000 - aInt;
			// Make bInt lexicographically ordered as a twos-complement int    
			int bInt = *(alias_cast<float*, int*>(&B));
			if (bInt < 0)
				bInt = 0x80000000 - bInt;
			int intDiff = abs(aInt - bInt);
			if (intDiff <= maxUlps)
				return true;
			return false;
		}

		//From other source
		bool absoluteToleranceCompare(double x, double y)
		{
			return std::fabs(x - y) <= std::numeric_limits<double>::epsilon();
		}

		bool relativeToleranceCompare(double x, double y)
		{
			double maxXY = std::fmax(std::fabs(x), std::fabs(y));
			return std::fabs(x - y) <= std::numeric_limits<double>::epsilon()*maxXY;
		}

		bool combinedToleranceCompare(double x, double y)
		{
			double maxXYOne = std::fmax(std::fmax(1.0, std::fabs(x)), std::fabs(y));

			return std::fabs(x - y) <= std::numeric_limits<double>::epsilon()*maxXYOne;
		}

		//From other source
		template< typename A, typename B >
		inline
			bool close_enough(A const & a, B const & b,
				typename std::common_type< A, B >::type const & epsilon)
		{
			using std::isless;
			assert(isless(0, epsilon)); // epsilon is a part of the whole quantity
			assert(isless(epsilon, 1));
			using std::abs;
			auto const delta = abs(a - b);
			auto const x = abs(a);
			auto const y = abs(b);
			// comparable generally and |a - b| < eps * (|a| + |b|) / 2
			return isless(epsilon * y, x) && isless(epsilon * x, y) && isless((delta + delta) / (x + y), epsilon);
		}
	};

	//============================================================================================
	//Some other source

	typedef unsigned int   U32;

	class CompareFloat {
	public:
		union {
			float     m_f32;
			U32       m_u32;
		};
		static bool   IsClose(float A, float B, U32 unitsDelta = 4)
		{
			U32    a = GetBiased(A);
			U32    b = GetBiased(B);

			if ((a > 0xFF000000) || (b > 0xFF000000))
			{
				return(false);
			}
			return((static_cast<U32>(fabs(a - b))) < unitsDelta);
		}
	protected:
		static U32    GetBiased(float f)
		{
			U32    r = ((CompareFloat*)&f)->m_u32;

			if (r & 0x80000000)
			{
				return(~r - 0x007FFFFF);
			}
			return(r + 0x7F800000);
		}
	};



	//============================================================================================
	// From Google Test
	// ==============================================================================================================

	// Copyright 2005, Google Inc.
	// All rights reserved.
	//
	// Redistribution and use in source and binary forms, with or without
	// modification, are permitted provided that the following conditions are
	// met:
	//
	//     * Redistributions of source code must retain the above copyright
	// notice, this list of conditions and the following disclaimer.
	//     * Redistributions in binary form must reproduce the above
	// copyright notice, this list of conditions and the following disclaimer
	// in the documentation and/or other materials provided with the
	// distribution.
	//     * Neither the name of Google Inc. nor the names of its
	// contributors may be used to endorse or promote products derived from
	// this software without specific prior written permission.
	//
	// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	//
	// Authors: wan@google.com (Zhanyong Wan), eefacm@gmail.com (Sean Mcafee)
	//
	// The Google C++ Testing Framework (Google Test)


	// This template class serves as a compile-time function from size to
	// type.  It maps a size in bytes to a primitive type with that
	// size. e.g.
	//
	//   TypeWithSize<4>::UInt
	//
	// is typedef-ed to be unsigned int (unsigned integer made up of 4
	// bytes).
	//
	// Such functionality should belong to STL, but I cannot find it
	// there.
	//
	// Google Test uses this class in the implementation of floating-point
	// comparison.
	//
	// For now it only handles UInt (unsigned int) as that's all Google Test
	// needs.  Other types can be easily added in the future if need
	// arises.
	template <size_t size>
	class TypeWithSize {
	public:
		// This prevents the user from using TypeWithSize<N> with incorrect
		// values of N.
		typedef void UInt;
	};

	// The specialization for size 4.
	template <>
	class TypeWithSize<4> {
	public:
		// unsigned int has size 4 in both gcc and MSVC.
		//
		// As base/basictypes.h doesn't compile on Windows, we cannot use
		// uint32, uint64, and etc here.
		typedef int Int;
		typedef unsigned int UInt;
	};

	// The specialization for size 8.
	template <>
	class TypeWithSize<8> {
	public:
#if GTEST_OS_WINDOWS
		typedef __int64 Int;
		typedef unsigned __int64 UInt;
#else
		typedef long long Int;  // NOLINT
		typedef unsigned long long UInt;  // NOLINT
#endif  // GTEST_OS_WINDOWS
	};


	// This template class represents an IEEE floating-point number
	// (either single-precision or double-precision, depending on the
	// template parameters).
	//
	// The purpose of this class is to do more sophisticated number
	// comparison.  (Due to round-off error, etc, it's very unlikely that
	// two floating-points will be equal exactly.  Hence a naive
	// comparison by the == operation often doesn't work.)
	//
	// Format of IEEE floating-point:
	//
	//   The most-significant bit being the leftmost, an IEEE
	//   floating-point looks like
	//
	//     sign_bit exponent_bits fraction_bits
	//
	//   Here, sign_bit is a single bit that designates the sign of the
	//   number.
	//
	//   For float, there are 8 exponent bits and 23 fraction bits.
	//
	//   For double, there are 11 exponent bits and 52 fraction bits.
	//
	//   More details can be found at
	//   http://en.wikipedia.org/wiki/IEEE_floating-point_standard.
	//
	// Template parameter:
	//
	//   RawType: the raw floating-point type (either float or double)
	template <typename RawType>
	class FloatingPoint {
	public:
		// Defines the unsigned integer type that has the same size as the
		// floating point number.
		typedef typename TypeWithSize<sizeof(RawType)>::UInt Bits;

		// Constants.

		// # of bits in a number.
		static const size_t kBitCount = 8 * sizeof(RawType);

		// # of fraction bits in a number.
		static const size_t kFractionBitCount =
			std::numeric_limits<RawType>::digits - 1;

		// # of exponent bits in a number.
		static const size_t kExponentBitCount = kBitCount - 1 - kFractionBitCount;

		// The mask for the sign bit.
		static const Bits kSignBitMask = static_cast<Bits>(1) << (kBitCount - 1);

		// The mask for the fraction bits.
		static const Bits kFractionBitMask =
			~static_cast<Bits>(0) >> (kExponentBitCount + 1);

		// The mask for the exponent bits.
		static const Bits kExponentBitMask = ~(kSignBitMask | kFractionBitMask);

		// How many ULP's (Units in the Last Place) we want to tolerate when
		// comparing two numbers.  The larger the value, the more error we
		// allow.  A 0 value means that two numbers must be exactly the same
		// to be considered equal.
		//
		// The maximum error of a single floating-point operation is 0.5
		// units in the last place.  On Intel CPU's, all floating-point
		// calculations are done with 80-bit precision, while double has 64
		// bits.  Therefore, 4 should be enough for ordinary use.
		//
		// See the following article for more details on ULP:
		// http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm.
		static const size_t kMaxUlps = 4;

		// Constructs a FloatingPoint from a raw floating-point number.
		//
		// On an Intel CPU, passing a non-normalized NAN (Not a Number)
		// around may change its bits, although the new value is guaranteed
		// to be also a NAN.  Therefore, don't expect this constructor to
		// preserve the bits in x when x is a NAN.
		explicit FloatingPoint(const RawType& x) { u_.value_ = x; }

		// Static methods

		// Reinterprets a bit pattern as a floating-point number.
		//
		// This function is needed to test the AlmostEquals() method.
		static RawType ReinterpretBits(const Bits bits) {
			FloatingPoint fp(0);
			fp.u_.bits_ = bits;
			return fp.u_.value_;
		}

		// Returns the floating-point number that represent positive infinity.
		static RawType Infinity() {
			return ReinterpretBits(kExponentBitMask);
		}

		// Non-static methods

		// Returns the bits that represents this number.
		const Bits &bits() const { return u_.bits_; }

		// Returns the exponent bits of this number.
		Bits exponent_bits() const { return kExponentBitMask & u_.bits_; }

		// Returns the fraction bits of this number.
		Bits fraction_bits() const { return kFractionBitMask & u_.bits_; }

		// Returns the sign bit of this number.
		Bits sign_bit() const { return kSignBitMask & u_.bits_; }

		// Returns true iff this is NAN (not a number).
		bool is_nan() const {
			// It's a NAN if the exponent bits are all ones and the fraction
			// bits are not entirely zeros.
			return (exponent_bits() == kExponentBitMask) && (fraction_bits() != 0);
		}

		// Returns true iff this number is at most kMaxUlps ULP's away from
		// rhs.  In particular, this function:
		//
		//   - returns false if either number is (or both are) NAN.
		//   - treats really large numbers as almost equal to infinity.
		//   - thinks +0.0 and -0.0 are 0 DLP's apart.
		bool AlmostEquals(const FloatingPoint& rhs) const {
			// The IEEE standard says that any comparison operation involving
			// a NAN must return false.
			if (is_nan() || rhs.is_nan()) return false;

			return DistanceBetweenSignAndMagnitudeNumbers(u_.bits_, rhs.u_.bits_)
				<= kMaxUlps;
		}

	private:
		// The data type used to store the actual floating-point number.
		union FloatingPointUnion {
			RawType value_;  // The raw floating-point number.
			Bits bits_;      // The bits that represent the number.
		};

		// Converts an integer from the sign-and-magnitude representation to
		// the biased representation.  More precisely, let N be 2 to the
		// power of (kBitCount - 1), an integer x is represented by the
		// unsigned number x + N.
		//
		// For instance,
		//
		//   -N + 1 (the most negative number representable using
		//          sign-and-magnitude) is represented by 1;
		//   0      is represented by N; and
		//   N - 1  (the biggest number representable using
		//          sign-and-magnitude) is represented by 2N - 1.
		//
		// Read http://en.wikipedia.org/wiki/Signed_number_representations
		// for more details on signed number representations.
		static Bits SignAndMagnitudeToBiased(const Bits &sam) {
			if (kSignBitMask & sam) {
				// sam represents a negative number.
				return ~sam + 1;
			}
			else {
				// sam represents a positive number.
				return kSignBitMask | sam;
			}
		}

		// Given two numbers in the sign-and-magnitude representation,
		// returns the distance between them as an unsigned number.
		static Bits DistanceBetweenSignAndMagnitudeNumbers(const Bits &sam1,
			const Bits &sam2) {
			const Bits biased1 = SignAndMagnitudeToBiased(sam1);
			const Bits biased2 = SignAndMagnitudeToBiased(sam2);
			return (biased1 >= biased2) ? (biased1 - biased2) : (biased2 - biased1);
		}

		FloatingPointUnion u_;
	};

	// ==============================================================================================================

}

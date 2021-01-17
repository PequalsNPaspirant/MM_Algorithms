#pragma once

#include <iostream>
#include <iomanip>
//#include <bit> //C++20 only for std::endian
#include <cassert>
#include <climits>
using namespace std;

/*
References:
best source: https://graphics.stanford.edu/~seander/bithacks.html
https://stackoverflow.com/questions/671815/what-is-the-fastest-most-efficient-way-to-find-the-highest-set-bit-msb-in-an-i
https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
*/

namespace mm {

	
	class MM_BitHacks
	{
	public:
		static bool isLittleEndian_v1()
		{
			union {
				int i;
				char c[sizeof(int)];
			} x;
			x.i = 1;
			if (x.c[0] == 1)
				return true;
			else
				return false;
		}

		static bool isLittleEndian_v2()
		{
			union U {
				char c[sizeof(int)];
				int i;
			};
			bool retVal;
			constexpr U x = U{ 'l', '?', '?', 'b' };
			if (static_cast<char>(x.i) == 'l')
				retVal = true;
			else if (static_cast<char>(x.i) == 'b')
				retVal = false;

			return retVal;
		}

		static bool isLittleEndian_v3()
		{
			int n = 1;
			// little endian if true
			if (*(char *)&n == 1)
				return true;
			else
				return false;
		}

		//C++20 only
		static bool isLittleEndian_v4()
		{
			bool retVal = false;
			//if constexpr (std::endian::native == std::endian::little)
			//	retVal = true;
			//else if constexpr (std::endian::native == std::endian::big)
			//	retVal = false;
			//else
			//	throw std::runtime_error{ "May be mixed-endian!" };

			return retVal;
		}

		//Find MSB

		static int MSBPosition_v1(unsigned int v)
		{
			return MSBPosition_v1(static_cast<unsigned long long>(v));
		}

		static int MSBPosition_v1(unsigned long long v)
		{
			//Find the log base 2 of an integer with the MSB N set in O(N) operations(the obvious way)
			//unsigned int v; // 32-bit word to find the log base 2 of
			unsigned int r = 0; // r will be lg(v)

			while (v >>= 1) // unroll for more speed...
			{
				r++;
			}

			return r;
		}

		static int MSBPosition_v2(unsigned int v)
		{
			enum class Endianness
			{
				little_endian = 0,
				big_endian
			};
			Endianness LITTLE_ENDIAN = Endianness::little_endian;
			Endianness BIG_ENDIAN = Endianness::big_endian;
			Endianness __FLOAT_WORD_ORDER = isLittleEndian_v1() ? LITTLE_ENDIAN : BIG_ENDIAN;

			//Find the integer log base 2 of an integer with an 64 - bit IEEE float
			//int v; // 32-bit integer to find the log base 2 of
			int r; // result of log_2(v) goes here
			union { unsigned int u[2]; double d; } t; // temp

			t.u[__FLOAT_WORD_ORDER == LITTLE_ENDIAN] = 0x43300000;
			t.u[__FLOAT_WORD_ORDER != LITTLE_ENDIAN] = v;
			t.d -= 4503599627370496.0;
			r = (t.u[__FLOAT_WORD_ORDER == LITTLE_ENDIAN] >> 20) - 0x3FF;
			//The code above loads a 64 - bit(IEEE - 754 floating - point) double with a 32 - bit integer(with no paddding bits) by storing the integer in the mantissa while the exponent is set to 252. From this newly minted double, 252 (expressed as a double) is subtracted, which sets the resulting exponent to the log base 2 of the input value, v.All that is left is shifting the exponent bits into position(20 bits right) and subtracting the bias, 0x3FF (which is 1023 decimal).This technique only takes 5 operations, but many CPUs are slow at manipulating doubles, and the endianess of the architecture must be accommodated.
			//Eric Cole sent me this on January 15, 2006. Evan Felix pointed out a typo on April 4, 2006. Vincent Lefèvre told me on July 9, 2008 to change the endian check to use the float's endian, which could differ from the integer's endian.
			return r;
		}

		static int MSBPosition_v2(unsigned long long)
		{
			return 0;
		}

		static int MSBPosition_v3(unsigned int v)
		{
			//Find the log base 2 of an integer with a lookup table
			static const char LogTable256[256] =
			{
#define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
				- 1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
				LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
				LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
			};

			//unsigned int v; // 32-bit word to find the log of
			unsigned int r;     // r will be lg(v)
			register unsigned int t, tt; // temporaries

			if (tt = v >> 16)
			{
				r = (t = tt >> 8) ? 24 + LogTable256[t] : 16 + LogTable256[tt];
			}
			else
			{
				r = (t = v >> 8) ? 8 + LogTable256[t] : LogTable256[v];
			}
			//The lookup table method takes only about 7 operations to find the log of a 32 - bit value.If extended for 64 - bit quantities, it would take roughly 9 operations.Another operation can be trimmed off by using four tables, with the possible additions incorporated into each.Using int table elements may be faster, depending on your architecture.
			return r;
		}

		static int MSBPosition_v3(unsigned long long)
		{
			return 0;
		}

		static int MSBPosition_v4(unsigned int v)
		{
			//The code above is tuned to uniformly distributed output values.If your inputs are evenly distributed across all 32 - bit values, then consider using the following :

			unsigned r;     // r will be lg(v)
			register unsigned int tt; // temporaries

									  //To initially generate the log table algorithmically :
			static char LogTable256[256];
			static bool tableGenerated = false;
			if (!tableGenerated)
			{
				LogTable256[0] = LogTable256[1] = 0;
				for (int i = 2; i < 256; i++)
				{
					LogTable256[i] = 1 + LogTable256[i / 2];
				}
				LogTable256[0] = -1; // if you want log(0) to return -1
				tableGenerated = true;
			}

			if (tt = v >> 24)
			{
				r = 24 + LogTable256[tt];
			}
			else if (tt = v >> 16)
			{
				r = 16 + LogTable256[tt];
			}
			else if (tt = v >> 8)
			{
				r = 8 + LogTable256[tt];
			}
			else
			{
				r = LogTable256[v];
			}

			//Behdad Esfahbod and I shaved off a fraction of an operation(on average) on May 18, 2005. Yet another fraction of an operation was removed on November 14, 2006 by Emanuel Hoogeveen.The variation that is tuned to evenly distributed input values was suggested by David A.Butterfield on September 19, 2008. Venkat Reddy told me on January 5, 2009 that log(0) should return -1 to indicate an error, so I changed the first entry in the table to that.
			return r;
		}

		static int MSBPosition_v4(unsigned long long)
		{
			return 0;
		}

		static int MSBPosition_v5(unsigned int v)
		{
			//Find the log base 2 of an N - bit integer in O(lg(N)) operations
			//unsigned int v;  // 32-bit value to find the log2 of 
			const unsigned int b[] = { 0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000 };
			const unsigned int S[] = { 1, 2, 4, 8, 16 };
			int i;

			register unsigned int r = 0; // result of log2(v) will go here
			for (i = 4; i >= 0; i--) // unroll for speed...
			{
				if (v & b[i])
				{
					v >>= S[i];
					r |= S[i];
				}
			}

			return r;
		}

		static int MSBPosition_v5(unsigned long long)
		{
			return 0;
		}

		static int MSBPosition_v6(unsigned int v)
		{
			// OR (IF YOUR CPU BRANCHES SLOWLY):

			//unsigned int v;	         // 32-bit value to find the log2 of 
			register unsigned int r; // result of log2(v) will go here

			register unsigned int shift;

			r = (v > 0xFFFF) << 4; v >>= r;
			shift = (v > 0xFF) << 3; v >>= shift; r |= shift;
			shift = (v > 0xF) << 2; v >>= shift; r |= shift;
			shift = (v > 0x3) << 1; v >>= shift; r |= shift;
			r |= (v >> 1);

			return r;

			// OR (IF YOU KNOW v IS A POWER OF 2):
			if ((v & (v - 1)) == 0)
			{
				//unsigned int v;  // 32-bit value to find the log2 of 
				static const unsigned int b[] = { 0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0, 0xFF00FF00, 0xFFFF0000 };
				r = (v & b[0]) != 0;
				for (int i = 4; i > 0; i--) // unroll for speed...
				{
					r |= ((v & b[i]) != 0) << i;
				}
				return r;
			}

			//Of course, to extend the code to find the log of a 33 - to 64 - bit number, we would append another element, 0xFFFFFFFF00000000, to b, append 32 to S, and loop from 5 to 0. This method is much slower than the earlier table - lookup version, but if you don't want big table or your architecture is slow to access memory, it's a good choice.The second variation involves slightly more operations, but it may be faster on machines with high branch costs(e.g.PowerPC).
			//The second version was sent to me by Eric Cole on January 7, 2006. Andrew Shapira subsequently trimmed a few operations off of it and sent me his variation(above) on Sept. 1, 2007. The third variation was suggested to me by John Owens on April 24, 2002; it's faster, but it is only suitable when the input is known to be a power of 2. On May 25, 2003, Ken Raeburn suggested improving the general case by using smaller numbers for b[], which load faster on some architectures (for instance if the word size is 16 bits, then only one load instruction may be needed). These values work for the general version, but not for the special-case version below it, where v is a power of 2; Glenn Slayden brought this oversight to my attention on December 12, 2003.
		}

		static int MSBPosition_v6(unsigned long long)
		{
			return 0;
		}

		static int MSBPosition_v7(unsigned int v)
		{
			//Find the log base 2 of an N - bit integer in O(lg(N)) operations with multiply and lookup
			//uint32_t v; // find the log base 2 of 32-bit v
			int r;      // result goes here

			static const int MultiplyDeBruijnBitPosition[32] =
			{
				0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
				8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
			};

			v |= v >> 1; // first round down to one less than a power of 2 
			v |= v >> 2;
			v |= v >> 4;
			v |= v >> 8;
			v |= v >> 16;

			r = MultiplyDeBruijnBitPosition[(uint32_t)(v * 0x07C4ACDDU) >> 27];
			return r;

			//The code above computes the log base 2 of a 32 - bit integer with a small table lookup and multiply.
			//It requires only 13 operations, compared to(up to) 20 for the previous method.The purely table - based method 
			//requires the fewest operations, but this offers a reasonable compromise between table size and speed.

		}

		static int MSBPosition_v7(unsigned long long)
		{
			return 0;
		}

		static int MSBPosition_v8(unsigned int v)
		{
			//If you know that v is a power of 2, then you only need the following :
			//if its not power of 2, then you can make it using
			if ((v & (v - 1)) != 0)
			{
				/*
				// Suppose n is 273 (binary 
				// is 100010001). It does following 
				// 100010001 | 010001000 = 110011001 
				n |= n >> 1; 
  
				// This makes sure 4 bits 
				// (From MSB and including MSB) 
				// are set. It does following 
				// 110011001 | 001100110 = 111111111 
				n |= n >> 2; 
  
				n |= n >> 4; 
				n |= n >> 8; 
				n |= n >> 16; 
  
				// Increment n by 1 so that 
				// there is only one set bit 
				// which is just before original 
				// MSB. n now becomes 1000000000 
				n = n + 1; 
  
				// Return original MSB after shifting. 
				// n now becomes 100000000 
				return (n >> 1); 
				*/

				v |= v >> 1;
				v |= v >> 2;
				v |= v >> 4;
				v |= v >> 8;
				v |= v >> 16;
				v = (v >> 1) + 1;
				//or 
				unsigned int x = (v & ~(v >> 1));
				assert(v == x);
			}

			int r;      // result goes here
			static const int MultiplyDeBruijnBitPosition2[32] =
			{
				0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
				31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
			};
			r = MultiplyDeBruijnBitPosition2[(uint32_t)(v * 0x077CB531U) >> 27];
			return r;

			//Eric Cole devised this January 8, 2006 after reading about the entry below to round up to a power of 2 and 
			//the method below for computing the number of trailing bits with a multiply and lookup using a DeBruijn sequence.
			//On December 10, 2009, Mark Dickinson shaved off a couple operations by requiring v be rounded up to one less than 
			//the next power of 2 rather than the power of 2.
		}

		static int MSBPosition_v8(unsigned long long)
		{
			return 0;
		}

		static int MSBPosition_v9(unsigned int v)
		{
#define UINT_BIT (CHAR_BIT*sizeof(unsigned int))

			if (0 == v)
				return -1;

			int r = 0;

			for (unsigned int i = UINT_BIT >> 1; 0<i; i >>= 1)
				if (static_cast<unsigned int>(v >> i))
				{
					v >>= i;
					r |= i;
				}

			return r;
		}

		static int MSBPosition_v9(unsigned long long)
		{
			return 0;
		}

		static int LSBPosition_v9(unsigned int v)
		{
#define UINT_BIT (CHAR_BIT*sizeof(unsigned int))

			if (0 == v)
				return -1;

			int r = UINT_BIT - 1;

			for (unsigned int i = UINT_BIT >> 1; 0<i; i >>= 1)
				if (static_cast<unsigned int>(v << i))
				{
					v <<= i;
					r ^= i;
				}

			return r;
		}
	};
}
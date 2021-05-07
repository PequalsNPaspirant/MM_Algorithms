//Goal

#include <iostream> // std::cout, std::hex, std::endl, std::showbase, std::ios_base, std::ios
#include <iomanip> //for setw(n)
#include <limits>
#include <vector>
#include <bitset>
#include <cmath> //for std::frexp & std::ldexp
using namespace std;

#include "C++/C++_DataTypes.h"

namespace mm {

	void DataTypes::checkAllDataTypeLowerAndUpperLimits()
	{
#define PRINT(TYPE) cout << setw(22) << STRINGIFY(TYPE) << ": Size " << sizeof(TYPE) << " Min " << numeric_limits<TYPE>::min() << " Max " << numeric_limits<TYPE>::max() << endl;
#define PRINT_(TYPE) cout << setw(22) << STRINGIFY(TYPE) << ": Size " << sizeof(TYPE) << " Min " << (int)numeric_limits<TYPE>::min() << " Max " << (int)numeric_limits<TYPE>::max() << endl;

		//#define STRINGIFY(X) #X  //Only one macro does not work
#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

		PRINT(char)
			PRINT_(char)
			PRINT(unsigned char)
			PRINT_(unsigned char)
			PRINT(short)
			PRINT(unsigned short)

			std::cout << std::scientific;

		PRINT(int)
			PRINT(unsigned int)
			PRINT(long)
			PRINT(unsigned long)
			PRINT(long long)
			PRINT(unsigned long long)
			PRINT(float)
			//PRINT(unsigned float)
			PRINT(double)
			//PRINT(unsigned double)
			PRINT(long double)
			//PRINT(unsigned long double)
			// Typical floating point formats don't support unsigned numbers. See, for instance, this list of IEEE 754 formats.
			// Adding a numerical format that isn't supported by common hardware just makes life difficult for compiler writers, 
				// and is probably not considered worth the effort.

	}

	/*Output:

	char:					Size 1 Min Ç Max 
	char:					Size 1 Min -128 Max 127
	unsigned char:			Size 1 Min   Max  
	unsigned char:			Size 1 Min 0 Max 255
	short:					Size 2 Min -32768 Max 32767
	unsigned short:			Size 2 Min 0 Max 65535
	int:					Size 4 Min -2147483648 Max 2147483647
	unsigned int:			Size 4 Min 0 Max 4294967295
	long:					Size 4 Min -2147483648 Max 2147483647
	unsigned long:			Size 4 Min 0 Max 4294967295
	long long:				Size 8 Min -9223372036854775808 Max 9223372036854775807
	unsigned long long:		Size 8 Min 0 Max 18446744073709551615
	float:					Size 4 Min 1.175494e-38 Max 3.402823e+38
	unsigned float:			Size 4 Min 0 Max 4294967295
	double:					Size 8 Min 2.225074e-308 Max 1.797693e+308
	unsigned double:		Size 4 Min 0 Max 4294967295
	long double:			Size 8 Min 2.225074e-308 Max 1.797693e+308
	unsigned long double:	Size 8 Min 2.225074e-308 Max 1.797693e+308

	*/

	void DataTypes::tryVariousPrintingFormatsOfNumbers()
	{
		double a = 3.1415926534;
		double b = 2006.0;
		double c = 1.0e-10;

		std::cout.precision(5);

		std::cout << "default:\n";
		std::cout << a << '\n' << b << '\n' << c << '\n';

		std::cout << '\n';

		std::cout << "fixed:\n";
		std::cout << std::fixed << a << '\n' << b << '\n' << c << '\n';

		std::cout << '\n';

		std::cout << "scientific:\n";
		std::cout << std::scientific << a << '\n' << b << '\n' << c << '\n';

		/*Output:

		default:
		3.1416
		2006
		1e-10

		fixed:
		3.14159
		2006.00000
		0.00000

		scientific:
		3.14159e+00
		2.00600e+03
		1.00000e-10
		*/
		std::cout.setf(std::ios::hex, std::ios::basefield);  // set hex as the basefield
		std::cout.setf(std::ios::showbase);                  // activate showbase
		std::cout << 100 << '\n';
		std::cout.unsetf(std::ios::showbase);                // deactivate showbase
		std::cout << 100 << '\n';

		/*Outut
		0x64
		64
		*/
		std::cout.flags(std::ios::right | std::ios::hex | std::ios::showbase);
		std::cout.width(10);
		std::cout << 100 << '\n';

		/*Output
		0x64
		*/

		// using fmtflags as class member constants:
		std::cout.setf(std::ios_base::hex, std::ios_base::basefield);
		std::cout.setf(std::ios_base::showbase);
		std::cout << 100 << '\n';

		// using fmtflags as inherited class member constants:
		std::cout.setf(std::ios::hex, std::ios::basefield);
		std::cout.setf(std::ios::showbase);
		std::cout << 100 << '\n';

		// using fmtflags as object member constants:
		std::cout.setf(std::cout.hex, std::cout.basefield);
		std::cout.setf(std::cout.showbase);
		std::cout << 100 << '\n';

		// using fmtflags as a type:
		std::ios_base::fmtflags ff;
		ff = std::cout.flags();
		ff &= ~std::cout.basefield;   // unset basefield bits
		ff |= std::cout.hex;          // set hex
		ff |= std::cout.showbase;     // set showbase
		std::cout.flags(ff);
		std::cout << 100 << '\n';

		// not using fmtflags, but using manipulators:
		std::cout << std::hex << std::showbase << 100 << '\n';

		/*output:
		0x64
		0x64
		0x64
		0x64
		0x64
		*/
	}

	void DataTypes::testEndianess()
	{
		cout << "\n\nTest:";
		union {
			uint32_t i;
			char c[4];
		} bint = { 0x01020304 };
		if (bint.c[0] == 1)
			cout << "\nSystem is big endian";
		else
			cout << "\nSystem is little endian";

		//cout << "\n\nNext Test:";
		//if (htonl(47) == 47) {
		//	cout << "\nSystem is big endian";
		//}
		//else {
		//	cout << "\nSystem is little endian";
		//}

		cout << "\n\nNext Test:";
		int num = 1;
		if (*(char *)&num == 1)
		{
			printf("\nLittle-Endian");
		}
		else
		{
			printf("\nBig-Endian");
		}

		cout << "\n\nNext Test:";
		short int word = 0x0001;
		char *byte = (char *)&word;
		if (byte[0] == 1)
			cout << "\nSystem is little endian";
		else
			cout << "\nSystem is big endian";

		/*
		Long story short:

		Little Endian - Least Significant Bit (LSB) at Lower Memory Address
		Big Endian    - Most  Significant Bit (MSB) at Lower Memory Address

		Example 1:
		We read it as (MSB -> LSB):
		Decimal	: ‭1‬
		Hex		: 00 00 00 01
		Binary	: ‭0000_0000  0000_0000  0000_0000  0000_0001

		Little Endian -
		memory address	: 100        101        102        103
		Hex				: ‭01         00         00         00‬
		Binary			: ‭1000_0000  0000_0000  0000_0000  0000_0000‬

		Big Endian -
		memory address	: 100        101        102        103
		Hex				: ‭00         00         00         01
		Binary			: ‭0000_0000  0000_0000  0000_0000  0000_0001

		Example 2:
		We read it as (MSB -> LSB):
		Decimal	: ‭1985229328‬
		Hex		: 76 54 32 10
		Binary	: ‭0111_0110  0101_0100  0011_0010  0001_0000

		Little Endian -
		memory address	: 100        101        102        103
		Hex				: ‭10         32         54         76‬
		Binary			: ‭0000_1000  0100_1100  0010_1010  0110_1110‬

		Big Endian -
		memory address	: 100        101        102        103
		Hex				: ‭76         54         32         10
		Binary			: ‭0111_0110  0101_0100  0011_0010  0001_0000

		Details:

		In big endian format, the most significant byte, which is the byte containing the most significant bit (MSB, also called as high - order bit)
		i.e.the bit position in a binary number having the greatest value
		OR the sign bit of a signed binary number in one's or two's complement notation
		is stored first(at lower memory address) and the following bytes are stored in decreasing significance order,
		the least significant byte, which is the byte containing the least significant bit (LSB),
		thus being stored at last place (at higher memory address)
		i.e. if memory address is increaing from left to right, the integer (4 bytes long) 12345678 in hexadecimal form
		is stored as

		Example:
		Hex: ‭		12 34 56 78‬
		Decimal: ‭	305419896‬
		Binary:		‭0001 0010 0011 0100 0101 0110 0111 1000‬

		In big endian format: (the way we write numbers on paper, if memory in increasing from left to right)
		_________________________
		Number:			|  12 |  34 |  56 |  78 |
		memory address: 100   101   102   103

		The bits in first byte	i.e. the hexadecimal number 0x12 (i.e. decimal 18 = 2^1 + 2^4) is represented as:

		0 0 0 1   0 0 1 0

		The whole number can be constructed as:
		Binary:			 0   0 0 1   0 0 1  0  |  ‭0   0 1 1  0 1 0  0‬   | ‭ 0   1 0 1   0 1 1  0‬ |  ‭0  1 1 1    1 0 0  0 ‬
		Construction:	2^31       |      2^24 | 2^23       |      2^16 | 2^15       |      2^8 | 2^7        |       2^0
		Hex:			1          2	       3            4	        5            6			7            8
		Decimal:		18				       52					    86				     120

		In little endian format:
		_________________________
		Number :		|  78 |  56 |  34 |  12 |
		memory address: 100   101   102   103

		The bits in same byte where hexadecimal number 0x12 is stored is represented as :
		(NOTE THAT THE BIT ORDER IN EACH BYTE IS ALSO REVERSED)
		0 1 0 0   1 0 0 0

		The whole number can be constructed as:
		Binary:			 0   0 0 1   1 1 1  0  |  ‭0   1 1 0   1 0 1  0‬   | ‭ 0   0 1 0   1 1 0  0‬   |  ‭0   1 0 0   1 0 0  0 ‬
		Construction:	2^0        |       2^7 | 2^8        |       2^15 | 2^16       |       2^23 | 2^24       |       2^31
		Hex:			8          7		   6            5			 4            3			   2            1
		Decimal:		120 			       86					     52				           18

		*/
		/*
		More from Geeksforgeeks

		Little and Big Endian Mystery
		What are these?
		Little and big endian are two ways of storing multibyte data-types ( int, float, etc). In little endian machines,
		last byte of binary representation of the multibyte data-type is stored first. On the other hand, in big endian machines,
		first byte of binary representation of the multibyte data-type is stored first.

		Suppose integer is stored as 4 bytes (For those who are using DOS based compilers such as C++ 3.0 ,
		integer is 2 bytes) then a variable x with value 0x01234567 will be stored as following.



		Memory representation of integer ox01234567 inside Big and little endian machines
		How to see memory representation of multibyte data types on your machine?
		Here is a sample C code that shows the byte representation of int, float and pointer.

		#include <stdio.h>

		/* function to show bytes in memory, from location start to start+n
		void show_mem_rep(char *start, int n)
		{
		int i;
		for (i = 0; i < n; i++)
		printf(" %.2x", start[i]);
		printf("\n");
		}

		/*Main function to call above function for 0x01234567
		int main()
		{
		int i = 0x01234567;
		show_mem_rep((char *)&i, sizeof(i));
		getchar();
		return 0;
		}
		Run on IDE
		When above program is run on little endian machine, gives “67 45 23 01″ as output, while if it is run on endian machine,
		gives “01 23 45 67″ as output.

		Does endianness matter for programmers ?
		Most of the times compiler takes care of endianness, however, endianness becomes an issue in following cases.

		It matters in network programming : Suppose you write integers to file on a little endian machine and you transfer
		this file to a big endian machine.Unless there is little andian to big endian transformation, big endian machine will
		read the file in reverse order.You can find such a practical example here.

		Standard byte order for networks is big endian, also known as network byte order.Before transferring data on network,
		data is first converted to network byte order(big endian).

		Sometimes it matters when you are using type casting, below program is an example.

		#include <stdio.h>
		int main()
		{
		unsigned char arr[2] = { 0x01, 0x00 };
		unsigned short int x = *(unsigned short int *) arr;
		printf("%d", x);
		getchar();
		return 0;
		}
		Run on IDE
		In the above program, a char array is typecasted to an unsigned short integer type. When I run above program on little endian machine,
		I get 1 as output, while if I run it on a big endian machine I get 256. To make programs endianness independent,
		above programming style should be avoided.

		What are bi - endians ?
		Bi - endian processors can run in both modes little and big endian.

		What are the examples of little, big endian and bi - endian machines ?
		Intel based processors are little endians.ARM processors were little endians.Current generation ARM processors are bi - endian.

		Motorola 68K processors are big endians.PowerPC(by Motorola) and SPARK(by Sun) processors were big endian. Current version of
		these processors are bi - endians.

		Does endianness effects file formats ?
		File formats which have 1 byte as a basic unit are independent of endianness e..g., ASCII files.Other file formats use
		some fixed endianness forrmat e.g, JPEG files are stored in big endian format.

		Which one is better — little endian or big endian
		The term little and big endian came from Gulliver’s Travels by Jonathan Swift.Two groups could not agree by which
		end a egg should be opened - a - the little or the big.Just like the egg issue, there is no technological reason to
		choose one byte ordering convention over the other, hence the arguments degenerate into bickering about sociopolitical issues.
		As long as one of the conventions is selected and adhered to consistently, the choice is arbitrary.
		*/
	}

	long long DataTypes::convertToIntegralType(string str)
	{
		long long retVal = 0;
		//TODO
		return retVal;
	}

	long double DataTypes::convertToFloatingType(string str)
	{
		long double retVal = 0.0;
		//TODO
		return retVal;
	}

	void DataTypes::ShowHexMemoryRepresentation(char *start, int n)
	{
		int i;
		for (i = 0; i < n; i++)
			printf(" %.2x", start[i]);
		printf("\n");
	}

	void DataTypes::ShowBitwiseMemoryRepresentation(char *start, int n, vector<short>& bits, bool isFloating /*= false*/)
	{
		for (int i = 0; i < n; i++)
		{
			for (int k = 1; k <= 8; k++)
			{
				//The bits representation is generic, not specific to any type of endian-ness
				//bits[8 * n - 1] represents lowest memory address
				//bits[0] represents highest memory address
				bits[((n - i) * 8) - k] = (Bits::isBitON(*start, k) ? 1 : 0);
			}
			start++;
		}

		cout << setw(10) << "Binary: ";
		for (int i = 0; i < bits.size(); i++)
		{
			cout << bits[i];
			if (isFloating)
			{
				//if (i == 0)
				//	cout << "_s";
				//if (i == 0)
				//	cout << "_";

				//if (i == 11)
				//	cout << "_";
				//else if ((i + 1) % 4 == 0) 
				//	cout << " ";

				if ((i + 5) % 8 == 0)
					cout << "_";
			}
			//else
			//	if ((i + 1) % 4 == 0) cout << " ";
			if ((i + 1) % 8 == 0) cout << " ";
		}
	}

	void DataTypes::ShowBitwiseMemoryRepresentation(char *start, int n)
	{
		vector<short> bits(n * 8);
		ShowBitwiseMemoryRepresentation(start, n, bits);
	}

	void DataTypes::TestHexMemoryRepresentation()
	{
		int i = 0x12345678;
		cout << "\nMemory Representation of " << std::hex << i << " is: ";
		ShowHexMemoryRepresentation((char *)&i, sizeof(i));
	}

	void DataTypes::testDataTypeValuesAndBitwiseMemoryRepresentation()
	{
		char ch = 0;
		DataTypes::testDataTypeValuesAndBitwiseMemoryRepresentationOnAllTypes(ch);
		unsigned char uch = 0;
		DataTypes::testDataTypeValuesAndBitwiseMemoryRepresentationOnAllTypes(uch);
		short sh = 0;
		DataTypes::testDataTypeValuesAndBitwiseMemoryRepresentationOnAllTypes(sh);
		unsigned short ush = 0;
		DataTypes::testDataTypeValuesAndBitwiseMemoryRepresentationOnAllTypes(ush);
		int in = 0;
		DataTypes::testDataTypeValuesAndBitwiseMemoryRepresentationOnAllTypes(in);
		unsigned int uin = 0;
		DataTypes::testDataTypeValuesAndBitwiseMemoryRepresentationOnAllTypes(uin);
		long l = 0;
		DataTypes::testDataTypeValuesAndBitwiseMemoryRepresentationOnAllTypes(l);
		unsigned long ul = 0;
		DataTypes::testDataTypeValuesAndBitwiseMemoryRepresentationOnAllTypes(ul);
		long long ll = 0;
		DataTypes::testDataTypeValuesAndBitwiseMemoryRepresentationOnAllTypes(ll);
		unsigned long long ull = 0;
		DataTypes::testDataTypeValuesAndBitwiseMemoryRepresentationOnAllTypes(ull);
		float f = 0.0;
		DataTypes::testDataTypeValuesAndBitwiseMemoryRepresentationOnAllTypes(f, true);
		double d = 0.0;
		DataTypes::testDataTypeValuesAndBitwiseMemoryRepresentationOnAllTypes(d, true);
		long double ld = 0.0;
		DataTypes::testDataTypeValuesAndBitwiseMemoryRepresentationOnAllTypes(ld, true);
	}

	void DataTypes::testSTDBitSet()
	{
		char a = -58;
		std::bitset<8> x(a);
		std::cout << "\nValue: " << a << " Bits: " << x;

		short c = -315;
		std::bitset<16> y(c);
		std::cout << "\nValue: " << c << " Bits: " << y;
	}


	// ===========================================================================================================

	void Bits::testBitwiseOperations()
	{
		char ch = 0;
		Bits::testBitwiseOperationsOnAllTypes(ch);
		unsigned char uch = 0;
		Bits::testBitwiseOperationsOnAllTypes(uch);
		short sh = 0;
		Bits::testBitwiseOperationsOnAllTypes(sh);
		unsigned short ush = 0;
		Bits::testBitwiseOperationsOnAllTypes(ush);
		int in = 0;
		Bits::testBitwiseOperationsOnAllTypes(in);
		unsigned int uin = 0;
		Bits::testBitwiseOperationsOnAllTypes(uin);
		long l = 0;
		Bits::testBitwiseOperationsOnAllTypes(l);
		unsigned long ul = 0;
		Bits::testBitwiseOperationsOnAllTypes(ul);
		long long ll = 0;
		Bits::testBitwiseOperationsOnAllTypes(ll);
		unsigned long long ull = 0;
		Bits::testBitwiseOperationsOnAllTypes(ull);
		float f = 0.0;
		Bits::testBitwiseOperationsOnAllTypes(f, true);
		double d = 0.0;
		Bits::testBitwiseOperationsOnAllTypes(d, true);
		long double ld = 0.0;
		Bits::testBitwiseOperationsOnAllTypes(ld, true);

		myObject obj;
		Bits::testBitwiseOperationsOnAllTypes(obj, true);
	}

	// ===========================================================================================================

	void DataTypes()
	{
		//DataTypes::checkAllDataTypeLowerAndUpperLimits();
		//DataTypes::tryVariousPrintingFormatsOfNumbers();
		//DataTypes::testEndianess();
		//DataTypes::TestMemoryRepresentation();
		//DataTypes::testDataTypeValuesAndBitwiseMemoryRepresentation();
		//DataTypes::testSTDBitSet();

		//Bits::testBitwiseOperations();
	}

	// ===========================================================================================================

	/*
	void DataTypes::tryVariousPrintingFormatsOfNumbers()

	More information:
	http://www.cplusplus.com/reference/ios/ios_base/fmtflags/

	the function
	ios_base& scientific(ios_base& str);
	sets the floatfield format flag for the str stream to scientific.

	flag value	effect when set
	fixed	write floating-point values in fixed-point notation
	scientific	write floating-point values in scientific notation.
	(none)	write floating-point values in default floating-point notation.
	The default notation (none) is a different floatfield value than either fixed or scientific. The default notation can be selected by calling str.unsetf(ios_base::floatfield).

	For standard streams, no floatfield is set on initialization (default notation).

	The precision field can be modified using member precision.

	Notice that the treatment of the precision field differs between the default floating-point notation
	and the fixed and scientific notations (see precision). On the default floating-point notation,
	the precision field specifies the maximum number of meaningful digits to display both before
	and after the decimal point, while in both the fixed and scientific notations, the precision field specifies
	exactly how many digits to display after the decimal point, even if they are trailing decimal zeros.

	std::ios_base::fmtflags
	Type for stream format flags
	Bitmask type to represent stream format flags.
	This type is used as its parameter and/or return value by the member functions flags, setf and unsetf.

	The values passed and retrieved by these functions can be any valid combination of the following member constants:


	field				member constant		effect when set

	independent flags	boolalpha			read/write bool elements as alphabetic strings (true and false).
	showbase			write integral values preceded by their corresponding numeric base prefix.
	showpoint			write floating-point values including always the decimal point.
	showpos				write non-negative numerical values preceded by a plus sign (+).
	skipws				skip leading whitespaces on certain input operations.
	unitbuf				flush output after each inserting operation.
	uppercase			write uppercase letters replacing lowercase letters in certain insertion operations.
	numerical base
	(basefield)			dec					read/write integral values using decimal base format.
	hex					read/write integral values using hexadecimal base format.
	oct					read/write integral values using octal base format.
	float format
	(floatfield)		fixed				write floating point values in fixed-point notation.
	scientific			write floating-point values in scientific notation.
	adjustment
	(adjustfield)		internal			the output is padded to the field width by inserting fill characters at a specified internal point.
	left				the output is padded to the field width appending fill characters at the end.
	right				the output is padded to the field width by inserting fill characters at the beginning.

	Three additional bitmask constants made of the combination of the values of each of the three groups of selective flags can also be used:

	flag value		equivalent to

	adjustfield		left | right | internal
	basefield		dec | oct | hex
	floatfield		scientific | fixed

	The values of these constants can be combined into a single fmtflags value using the OR bitwise operator (|).

	These constants are defined as public members in the ios_base class. Therefore, they can be refered to either directly by their name as ios_base members (like ios_base::hex) or by using any of their inherited classes or instantiated objects, like for example ios::left or cout.oct.

	These values of type ios_base::fmtflags should not be confused with the manipulators that have the same name but in the global scope, because they are used in different circumstances. The manipulators cannot be used as values for ios_base::fmtflags, as well as these constants shouldn't be used instead of the manipulators. Notice the difference:
	1
	2
	ios_base::skipws     // constant value of type ios_base::fmtflags
	skipws               // manipulator (global function)


	Notice that several manipulators have the same name as these member constants (but as global functions instead) - see manipulators. The behavior of these manipulators generally corresponds to the same as setting or unsetting them with ios_base::setf or ios_base::unsetf, but they should not be confused! Manipulators are global functions and these constants are member constants. For example, showbase is a manipulator, while ios_base::showbase is a constant value that can be used as parameter with ios_base::setf.

	//More info:
	//http://www.cplusplus.com/reference/ostream/ostream/

	*/

}
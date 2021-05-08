//Goal
//All miscellaneous problems

#include <iostream>     // std::cout, std::hex, std::endl, std::showbase, std::ios_base, std::ios
#include <iomanip>      // std::setiosflags
#include <limits>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <windows.h>
#include <stdio.h>  /* defines FILENAME_MAX */
#include <direct.h>
#include <cmath>
using namespace std;

namespace mm {

void checkAllDataTypeLowerAndUpperLimits()
{
#define PRINT(TYPE) cout << endl << STRINGIFY(TYPE) << " Size " << sizeof(TYPE) << " Min " << numeric_limits<TYPE>::min() << " Max " << numeric_limits<TYPE>::max();
#define PRINT_(TYPE) cout << endl << STRINGIFY(TYPE) << " Size " << sizeof(TYPE) << " Min " << (int)numeric_limits<TYPE>::min() << " Max " << (int)numeric_limits<TYPE>::max();
//#define STRINGIFY(X) #X  //Only one macro does not work

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

#define TYPE char
	PRINT(TYPE)
		PRINT_(TYPE)
#define TYPE unsigned char
		PRINT(TYPE)
		PRINT_(TYPE)
#define TYPE short
		PRINT(TYPE)
#define TYPE unsigned short
		PRINT(TYPE)

	//double d = 1234.56789;
	//cout << "\n test: " << d;

	//std::cout.setf(std::scientific);
	std::cout << endl << std::scientific;
	//cout << "\n test: " << d;

#define TYPE int
		PRINT(TYPE)
#define TYPE unsigned int
		PRINT(TYPE)
#define TYPE long
		PRINT(TYPE)
#define TYPE unsigned long
		PRINT(TYPE)
#define TYPE long long
		PRINT(TYPE)
#define TYPE unsigned long long
		PRINT(TYPE)
#define TYPE float
		PRINT(TYPE)
#define TYPE unsigned float
		PRINT(TYPE)
#define TYPE double
		PRINT(TYPE)
#define TYPE unsigned double
		PRINT(TYPE)
#define TYPE long double
		PRINT(TYPE)
#define TYPE unsigned long double
		PRINT(TYPE)

/*Output:

char Size 1 Min Ç Max 
char Size 1 Min -128 Max 127
unsigned char Size 1 Min   Max  
unsigned char Size 1 Min 0 Max 255
short Size 2 Min -32768 Max 32767
unsigned short Size 2 Min 0 Max 65535

int Size 4 Min -2147483648 Max 2147483647
unsigned int Size 4 Min 0 Max 4294967295
long Size 4 Min -2147483648 Max 2147483647
unsigned long Size 4 Min 0 Max 4294967295
long long Size 8 Min -9223372036854775808 Max 9223372036854775807
unsigned long long Size 8 Min 0 Max 18446744073709551615
float Size 4 Min 1.175494e-38 Max 3.402823e+38
unsigned float Size 4 Min 0 Max 4294967295
double Size 8 Min 2.225074e-308 Max 1.797693e+308
unsigned double Size 4 Min 0 Max 4294967295
long double Size 8 Min 2.225074e-308 Max 1.797693e+308
unsigned long double Size 8 Min 2.225074e-308 Max 1.797693e+308

*/
}

void tryVariousPrintingFormatsOfNumbers()
{
/*
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

*/
	double a = 3.1415926534;
	double b = 2006.0;
	double c = 1.0e-10;

	std::cout.precision(5);

	std::cout << "default:\n";
	std::cout << a << '\n' << b << '\n' << c << '\n';

	std::cout << '\n';

	std::cout << "fixed:\n";
	std::cout << std::fixed  << a << '\n' << b << '\n' << c << '\n';

	std::cout << '\n';

	std::cout << "scientific:\n";
	std::cout << std::scientific  << a << '\n' << b << '\n' << c << '\n';

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

/*

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

/*More info:
http://www.cplusplus.com/reference/ostream/ostream/

std::ostream
typedef basic_ostream<char> ostream;
Output Stream
ios_base
ios
ostream
iostream
ofstream
ostringstream

Output stream objects can write sequences of characters and represent other kinds of data. Specific members are provided to perform these output operations (see functions below).

The standard objects cout, cerr and clog are objects of this type.

This is an instantiation of basic_ostream with the following template parameters:
template parameter	definition	comments
charT	char	Aliased as member char_type
traits	char_traits<char>	Aliased as member traits_type

Objects of this class keep a set of internal fields inherited from ios_base and ios:
field	member functions	description
Formatting	format flags	flags
setf
unsetf	A set of internal flags that affect how certain input/output operations are interpreted or generated.
See member type fmtflags.
field width	width	Width of the next formatted element to insert.
display precision	precision	Decimal precision for the next floating-point value inserted.
locale	getloc
imbue	The locale object used by the function for formatted input/output operations affected by localization properties.
fill character	fill	Character to pad a formatted field up to the field width (width).
State	error state	rdstate
setstate
clear	The current error state of the stream.
Individual values may be obtained by calling good, eof, fail and bad.
See member type iostate.
exception mask	exceptions	The state flags for which a failure exception is thrown.
See member type iostate.
Other	callback stack	register_callback	Stack of pointers to functions that are called when certain events occur.
extensible arrays	iword
pword
xalloc	Internal arrays to store objects of type long and void*.
tied stream	tie	Pointer to output stream that is flushed before each i/o operation on this stream.
stream buffer	rdbuf	Pointer to the associated streambuf object, which is charge of all input/output operations.

Member types
The class contains the following member class:
sentry
Prepare stream for output (public member class )

Along with the following member types:
member type	definition
char_type	char
traits_type	char_traits<char>
int_type	int
pos_type	streampos
off_type	streamoff
And these member types inherited from ios_base through ios:
event
Type to indicate event type (public member type )
event_callback
Event callback function type (public member type )
failure
Base class for stream exceptions (public member class )
fmtflags
Type for stream format flags (public member type )
Init
Initialize standard stream objects (public member class )
iostate
Type for stream state flags (public member type )
openmode
Type for stream opening mode flags (public member type )
seekdir
Type for stream seeking direction flag (public member type )

Public member functions
(constructor)
Construct object (public member function )
(destructor)
Destroy object (public member function )

Formatted output:
operator<<
Insert formatted output (public member function )

Unformatted output:
put
Put character (public member function )
write
Write block of data (public member function )

Positioning:
tellp
Get position in output sequence (public member function )
seekp
Set position in output sequence (public member function )

Synchronization:
flush
Flush output stream buffer (public member function )

Protected member functions
operator=
Move assignment (protected member function )
swap
Swap internals (protected member function )

Public member functions inherited from ios
good
Check whether state of stream is good (public member function )
eof
Check whether eofbit is set (public member function )
fail
Check whether either failbit or badbit is set (public member function )
bad
Check whether badbit is set (public member function )
operator!
Evaluate stream (not) (public member function )
operator bool
Evaluate stream (public member function )
rdstate
Get error state flags (public member function )
setstate
Set error state flag (public member function )
clear
Set error state flags (public member function )
copyfmt
Copy formatting information (public member function )
fill
Get/set fill character (public member function )
exceptions
Get/set exceptions mask (public member function )
imbue
Imbue locale (public member function )
tie
Get/set tied stream (public member function )
rdbuf
Get/set stream buffer (public member function )
narrow
Narrow character (public member function )
widen
Widen character (public member function )

Public member functions inherited from ios_base
flags
Get/set format flags (public member function )
setf
Set specific format flags (public member function )
unsetf
Clear specific format flags (public member function )
precision
Get/Set floating-point decimal precision (public member function )
width
Get/set field width (public member function )
imbue
Imbue locale (public member function )
getloc
Get current locale (public member function )
xalloc
Get new index for extensible array [static] (public static member function )
iword
Get integer element of extensible array (public member function )
pword
Get pointer element of extensible array (public member function )
register_callback
Register event callback function (public member function )
sync_with_stdio
Toggle synchronization with cstdio streams [static] (public static member function )


*/
}

//-----------------------------------------------------------------------------------------------------

//#include <Winsock2.h> //for htonl --> doesnot work unless required windows dll is linked
void testEndianess()
{
/*
In big endian format, the most significant byte, which is the byte containing the most significant bit (MSB, also called as high - order bit)
	i.e.the bit position in a binary number having the greatest value
	OR the sign bit of a signed binary number in one's or two's complement notation 
is stored first(at lower memory address) and the following bytes are stored in decreasing significance order,
the least significant byte, which is the byte containing the least significant bit (LSB), 
thus being stored at last place (at higher memory address)
i.e. if memory address is increaing from left to right, the integer (4 bytes long) 12345678 in hexadecimal form
is stored as

Example:
Hex: ‭		12345678‬
Decimal: ‭	305419896‬
Binary:		‭00010010001101000101011001111000‬

In big endian format: (the way we write numbers on paper, if memory in increasing from left to right)
				_________________________ 
Number:			|  12 |  34 |  56 |  78 | 
memory address: 100   101   102   103

The bits in first byte	i.e. the hexadecimal number 0x12 (i.e. decimal 18 = 2^1 + 2^4) is represented as:

0 0 0 1 0 0 1 0

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
0 1 0 0 1 0 0 0

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
Little and big endian are two ways of storing multibyte data-types ( int, float, etc). In little endian machines, last byte of binary representation of the multibyte data-type is stored first. On the other hand, in big endian machines, first byte of binary representation of the multibyte data-type is stored first.

Suppose integer is stored as 4 bytes (For those who are using DOS based compilers such as C++ 3.0 , integer is 2 bytes) then a variable x with value 0x01234567 will be stored as following.



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
		When above program is run on little endian machine, gives “67 45 23 01″ as output, while if it is run on endian machine, gives “01 23 45 67″ as output.

		Is there a quick way to determine endianness of your machine ?
		There are n no.of ways for determining endianness of your machine.Here is one quick way of doing the same.

#include <stdio.h>
		int main()
	{
		unsigned int i = 1;
		char *c = (char*)&i;
		if (*c)
			printf("Little endian");
		else
			printf("Big endian");
		getchar();
		return 0;
	}
	Run on IDE
		In the above program, a character pointer c is pointing to an integer i.Since size of character is 1 byte when the character pointer is de - referenced it will contain only first byte of integer.If machine is little endian then *c will be 1 (because last byte is stored first) and if machine is big endian then *c will be 0.

		Does endianness matter for programmers ?
		Most of the times compiler takes care of endianness, however, endianness becomes an issue in following cases.

		It matters in network programming : Suppose you write integers to file on a little endian machine and you transfer this file to a big endian machine.Unless there is little andian to big endian transformation, big endian machine will read the file in reverse order.You can find such a practical example here.

		Standard byte order for networks is big endian, also known as network byte order.Before transferring data on network, data is first converted to network byte order(big endian).

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
		In the above program, a char array is typecasted to an unsigned short integer type.When I run above program on little endian machine, I get 1 as output, while if I run it on a big endian machine I get 256. To make programs endianness independent, above programming style should be avoided.

		What are bi - endians ?
		Bi - endian processors can run in both modes little and big endian.

		What are the examples of little, big endian and bi - endian machines ?
		Intel based processors are little endians.ARM processors were little endians.Current generation ARM processors are bi - endian.

		Motorola 68K processors are big endians.PowerPC(by Motorola) and SPARK(by Sun) processors were big endian.Current version of these processors are bi - endians.

		Does endianness effects file formats ?
		File formats which have 1 byte as a basic unit are independent of endianness e..g., ASCII files.Other file formats use some fixed endianness forrmat e.g, JPEG files are stored in big endian format.


		Which one is better — little endian or big endian
		The term little and big endian came from Gulliver’s Travels by Jonathan Swift.Two groups could not agree by which end a egg should be opened - a - the little or the big.Just like the egg issue, there is no technological reason to choose one byte ordering convention over the other, hence the arguments degenerate into bickering about sociopolitical issues.As long as one of the conventions is selected and adhered to consistently, the choice is arbitrary.
*/
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
	byte[0] == 1 ? cout << "\nSystem is little endian" : cout << "\nSystem is big endian";
}

//-----------------------------------------------------------------------------------------------------
//Algorithm to check if sequence of brackets is valid

bool checkIfBracketSeqValid(const string& str)
{
	int score = 0;
	for (int i = 0; score >= 0 && i < str.size(); i++)
	{
		if (str[i] == '(')
			score++;
		else if (str[i] == ')')
			score--;
	}
	if (score == 0)
		return true;
	else
		return false;
}

//Assume precedence is {, [ and then (, so {[()]}, {{[[((()))]]}}, {()}, [()], (()) are correct but [{()}] is incorrect
bool checkIfAllBracketsSeqValid(string str)
{
	int CurlyBracketCounter = 0, SquareBracketCounter = 0, RoundBracketCounter = 0;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == '(') RoundBracketCounter++;
		else if (str[i] == ')') RoundBracketCounter--;
		else if (str[i] == '{') CurlyBracketCounter++;
		else if (str[i] == '}') CurlyBracketCounter--;
		else if (str[i] == '[') SquareBracketCounter++;
		else if (str[i] == ']') SquareBracketCounter--;
		if (RoundBracketCounter == -1 || CurlyBracketCounter == -1 || SquareBracketCounter == -1)
			return false;
		if (RoundBracketCounter > 0 && (str[i] == '{' || str[i] == '}' || str[i] == '[' || str[i] == ']'))
			return false;
		if (SquareBracketCounter > 0 && (str[i] == '{' || str[i] == '}'))
			return false;
	}
	if (RoundBracketCounter == 0 && CurlyBracketCounter == 0 && SquareBracketCounter == 0)
		return true;
	else
		return false;
}

void testCheckIfBracketSeqValid()
{
	cout << "\n\n------------- checkIfBracketSeqValid ------------------------";
	string str = "anything(means any) as (EXAMPLE(example))";
	cout << endl << str << ((checkIfBracketSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
	str = "dad(dad))adna(dsd)dadd";
	cout << endl << str << ((checkIfBracketSeqValid(str) == true) ? "=> VALID" : "=> INVALID");

	cout << "\n\n------------- checkIfAllBracketsSeqValid ------------------------";
	str = "anything(means any) as (EXAMPLE(example))";
	cout << endl << str << ((checkIfAllBracketsSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
	str = "dad(dad))adna(dsd)dadd";
	cout << endl << str << ((checkIfAllBracketsSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
	str = "{ [()] }";
	cout << endl << str << ((checkIfAllBracketsSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
	str = "{ { [[((()))]] } }";
	cout << endl << str << ((checkIfAllBracketsSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
	str = "{ () }";
	cout << endl << str << ((checkIfAllBracketsSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
	str = "[()]";
	cout << endl << str << ((checkIfAllBracketsSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
	str = "(())";
	cout << endl << str << ((checkIfAllBracketsSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
	str = "[{()}]";
	cout << endl << str << ((checkIfAllBracketsSeqValid(str) == true) ? "=> VALID" : "=> INVALID");
}

//-----------------------------------------------------------------------------------------------------

//Arrange all zeros & negative elements at left side, and positive at right side

//Traversing from both ends. It does not maintains the order
void clustorArray1(int arrayInt[], int size)
{
	int noOfSwaps = 0;
	cout << "\nInput: ";
	for (int i = 0; i < size; i++)
		cout << arrayInt[i] << " ";

	int leftPos = 0;
	int rightPos = size - 1;
	while(leftPos < rightPos)
	{
		while (leftPos < size && arrayInt[leftPos] <= 0) leftPos++;
		while (rightPos >= 0 && arrayInt[rightPos] > 0) rightPos--;
		if (leftPos < rightPos)
		{
			swap(arrayInt[leftPos], arrayInt[rightPos]);
			noOfSwaps++;
		}
	}

	cout << " || Output: ";
	for (int i = 0; i < size; i++)
		cout << arrayInt[i] << " ";
	cout << "\nNo. of Swaps: " << noOfSwaps;
}

//Traversing from left end. It maintains the order of only left elements. No. of Swaps are more.
void clustorArray2(int arrayInt[], int size)
{
	int noOfSwaps = 0;
	cout << "\nInput: ";
	for (int i = 0; i < size; i++)
		cout << arrayInt[i] << " ";

	int separatingWall = 0;
	for (int i = 0; i < size; i++)
	{
		if (arrayInt[i] <= 0) 
		{
			if (i != separatingWall) //No. of swaps are reduced by this check, but need to increase separatingWall anyway
			{
				swap(arrayInt[separatingWall], arrayInt[i]);
				noOfSwaps++;
			}
			separatingWall++;
		}
	}

	cout << " || Output: ";
	for (int i = 0; i < size; i++)
		cout << arrayInt[i] << " ";
	cout << "\nNo. of Swaps: " << noOfSwaps;
}

//Rotate anticlockwise by one element
void rightRotate(int arrayInt[], int startIndex, int endIndex, double& noOfSwaps)
{
	int backup = arrayInt[endIndex];
	for (int i = endIndex; i > startIndex; i--)
	{
		arrayInt[i] = arrayInt[i - 1];
		noOfSwaps += 0.5;
	}
	arrayInt[startIndex] = backup;
	noOfSwaps += 0.5;
}

//Traversing from left end. It maintains the order in left and right groups. Needs most no. of swaps
void clustorArray3(int arrayInt[], int size)
{
	double noOfSwaps = 0;
	cout << "\nInput: ";
	for (int i = 0; i < size; i++)
		cout << arrayInt[i] << " ";

	int separatingWall = 0;
	for (int i = 0; i < size; i++)
	{
		if (arrayInt[i] > 0)
		{
			for (int j = i + 1; j < size; j++)
			{
				if (arrayInt[j] <= 0)
				{
					rightRotate(arrayInt, i, j, noOfSwaps);
					break;
				}
			}
		}
	}

	cout << " || Output: ";
	for (int i = 0; i < size; i++)
		cout << arrayInt[i] << " ";
	cout << "\nNo. of Swaps: " << noOfSwaps;
}

/*
--A Juggling Algorithm
Rotate anticlockwise
Instead of moving one by one, divide the array in different sets
where number of sets is equal to GCD of n and d and move the elements within sets.
If GCD is 1 as is for the above example array(n = 7 and d = 2), then elements will be moved within one set only, 
we just start with temp = arr[0] and keep moving arr[I + d] to arr[I] and finally store temp at the right place.
*/
int gcd(int a, int b)
{
	int max = a;
	int min = b;
	if (b > a)
	{
		max = b;
		min = a;
	}

	while (min != 0)
	{
		int lastMax = max;
		max = min;
		min = lastMax % min;
	}
	return max;
}

void rightRotateBy(int arrayInt[], int startIndex, int endIndex, int rotateby, double& noOfSwaps)
{
	if (rotateby <= 0) return;

	int size = endIndex - startIndex + 1;
	int gcdVal = gcd(size, rotateby);
	for (int i = 0; i < gcdVal; i++)
	{
		int startWith = startIndex + i;
		int j = startWith;
		int temp = arrayInt[j];
		noOfSwaps += 0.5;
		while (1)
		{
			int k = j - rotateby;
			if (k < startIndex)
				k = (endIndex + 1) - (startIndex - k);
			if (startWith == k)
				break;
			arrayInt[j] = arrayInt[k];
			noOfSwaps += 0.5;
			j = k;
		}
		arrayInt[j] = temp;
		noOfSwaps += 0.5;
	}	
}

void leftShift(int arrayInt[], int startIndex, int endIndex, int shiftBy, double& noOfSwaps)
{
	int rotateBy = endIndex - startIndex + 1;
	rightRotateBy(arrayInt, startIndex - shiftBy, endIndex, rotateBy, noOfSwaps);
	//Reverse the first (endIndex - startIndex) elements
	//-----No need to reverse the elements
	//int start = startIndex - shiftBy;
	//int end = start + rotateBy - 1;
	//for (int i = start; i < end / 2; i++)
	//{
	//	swap(arrayInt[i], arrayInt[end - i]);
	//	noOfSwaps += 1;
	//}
}

//Traversing from left end. It maintains the order in left and right groups. Needs most no. of swaps
void clustorArray4(int arrayInt[], int size)
{
	double noOfSwaps = 0;
	cout << "\nInput: ";
	for (int i = 0; i < size; i++)
		cout << arrayInt[i] << " ";

	int separatingWall = 0;
	for (int i = 0; i < size;)
	{
		if (arrayInt[i] > 0)
		{
			int shiftBy = 0;
			int shiftStart = -1;
			int j;
			//Find next first 0 or negative element
			for (j = i + 1; j < size; j++)
			{
				shiftBy++;
				if (arrayInt[j] <= 0)
				{
					shiftStart = j;
					break;
				}
			}
			//no 0 or negative element found
			if (shiftStart == -1)
				break;
			//Find how many successive 0 or negative elements
			int k = j + 1;
			for (; k < size; k++)
			{
				if (arrayInt[k] > 0)
					break;
			}
			leftShift(arrayInt, shiftStart, k - 1, shiftBy, noOfSwaps);
			int totalElementsToShift = k - shiftStart;
			i += totalElementsToShift;
		}
		else
			i++;
	}

	cout << " || Output: ";
	for (int i = 0; i < size; i++)
		cout << arrayInt[i] << " ";
	cout << "\nNo. of Swaps: " << noOfSwaps;
}

// Traversing from left end.It maintains the order in left and right groups.Needs most no.of swaps
void clustorArray4Simplified(int arrayInt[], int size)
{
	double noOfSwaps = 0;
	cout << "\nInput: ";
	for (int i = 0; i < size; i++)
		cout << arrayInt[i] << " ";

	int separatingWall = 0;
	for (int i = 0; i < size;)
	{
		if (arrayInt[i] > 0)
		{
			int rotateStart = i;
			//Find next first 0 or negative element
			int j = i + 1;			
			while (j < size && arrayInt[j] > 0) j++;
			//Find how many successive 0 or negative elements
			if (j == size)
				break;
			int k = j + 1;
			int rotateBy = 1;
			for (; k < size && arrayInt[k] <= 0; k++)
				rotateBy++;

			rightRotateBy(arrayInt, rotateStart, k - 1, rotateBy, noOfSwaps);
			i += rotateBy;
		}
		else
			i++;
	}

	cout << " || Output: ";
	for (int i = 0; i < size; i++)
		cout << arrayInt[i] << " ";
	cout << "\nNo. of Swaps: " << noOfSwaps;
}

void testClustorArray()
{
	{
		cout << "\n-----------------------------------------------------";
		int arrayInt1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		int arrayInt2[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		int arrayInt3[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		int arrayInt4[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		int arrayInt5[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		clustorArray1(arrayInt1, 10);
		clustorArray2(arrayInt2, 10);
		clustorArray3(arrayInt3, 10);
		clustorArray4(arrayInt4, 10);
		clustorArray4Simplified(arrayInt5, 10);
	}
	{
		cout << "\n-----------------------------------------------------";
		int arrayInt1[] = { -1, 2, -3, 4, 5, -6, 7, 8, -9, -10 };
		int arrayInt2[] = { -1, 2, -3, 4, 5, -6, 7, 8, -9, -10 };
		int arrayInt3[] = { -1, 2, -3, 4, 5, -6, 7, 8, -9, -10 };
		int arrayInt4[] = { -1, 2, -3, 4, 5, -6, 7, 8, -9, -10 };
		int arrayInt5[] = { -1, 2, -3, 4, 5, -6, 7, 8, -9, -10 };
		clustorArray1(arrayInt1, 10);
		clustorArray2(arrayInt2, 10);
		clustorArray3(arrayInt3, 10);
		clustorArray4(arrayInt4, 10);
		clustorArray4Simplified(arrayInt5, 10);
	}
	{
		cout << "\n-----------------------------------------------------";
		int arrayInt1[] = { 1, 2, -3, 4, -5, -6, -7, 8, 9, 10 };
		int arrayInt2[] = { 1, 2, -3, 4, -5, -6, -7, 8, 9, 10 };
		int arrayInt3[] = { 1, 2, -3, 4, -5, -6, -7, 8, 9, 10 };
		int arrayInt4[] = { 1, 2, -3, 4, -5, -6, -7, 8, 9, 10 };
		int arrayInt5[] = { 1, 2, -3, 4, -5, -6, -7, 8, 9, 10 };
		clustorArray1(arrayInt1, 10);
		clustorArray2(arrayInt2, 10);
		clustorArray3(arrayInt3, 10);
		clustorArray4(arrayInt4, 10);
		clustorArray4Simplified(arrayInt5, 10);
	}
	{
		cout << "\n-----------------------------------------------------";
		int arrayInt1[] = { 1, 2, 3, -4, -5, -6, -7, -8, -9, -10 };
		int arrayInt2[] = { 1, 2, 3, -4, -5, -6, -7, -8, -9, -10 };
		int arrayInt3[] = { 1, 2, 3, -4, -5, -6, -7, -8, -9, -10 };
		int arrayInt4[] = { 1, 2, 3, -4, -5, -6, -7, -8, -9, -10 };
		int arrayInt5[] = { 1, 2, 3, -4, -5, -6, -7, -8, -9, -10 };
		clustorArray1(arrayInt1, 10);
		clustorArray2(arrayInt2, 10);
		clustorArray3(arrayInt3, 10);
		clustorArray4(arrayInt4, 10);
		clustorArray4Simplified(arrayInt5, 10);
	}
	{
		cout << "\n-----------------------------------------------------";
		int arrayInt1[] = { -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 };
		int arrayInt2[] = { -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 };
		int arrayInt3[] = { -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 };
		int arrayInt4[] = { -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 };
		int arrayInt5[] = { -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 };
		clustorArray1(arrayInt1, 10);
		clustorArray2(arrayInt2, 10);
		clustorArray3(arrayInt3, 10);
		clustorArray4(arrayInt4, 10);
		clustorArray4Simplified(arrayInt5, 10);
	}
	{
		cout << "\n-----------------------------------------------------";
		int arrayInt1[] = { 1, 2, 3, 0, 0, 0, 0, 8, 9, 10 };
		int arrayInt2[] = { 1, 2, 3, 0, 0, 0, 0, 8, 9, 10 };
		int arrayInt3[] = { 1, 2, 3, 0, 0, 0, 0, 8, 9, 10 };
		int arrayInt4[] = { 1, 2, 3, 0, 0, 0, 0, 8, 9, 10 };
		int arrayInt5[] = { 1, 2, 3, 0, 0, 0, 0, 8, 9, 10 };
		clustorArray1(arrayInt1, 10);
		clustorArray2(arrayInt2, 10);
		clustorArray3(arrayInt3, 10);
		clustorArray4(arrayInt4, 10);
		clustorArray4Simplified(arrayInt5, 10);
	}
	{
		cout << "\n-----------------------------------------------------";
		int arrayInt1[] = { 0, 0, 0, 4, -5, -3, 7, 0, 0, 0 };
		int arrayInt2[] = { 0, 0, 0, 4, -5, -3, 7, 0, 0, 0 };
		int arrayInt3[] = { 0, 0, 0, 4, -5, -3, 7, 0, 0, 0 };
		int arrayInt4[] = { 0, 0, 0, 4, -5, -3, 7, 0, 0, 0 };
		int arrayInt5[] = { 0, 0, 0, 4, -5, -3, 7, 0, 0, 0 };
		clustorArray1(arrayInt1, 10);
		clustorArray2(arrayInt2, 10);
		clustorArray3(arrayInt3, 10);
		clustorArray4(arrayInt4, 10);
		clustorArray4Simplified(arrayInt5, 10);
	}

/*
	-----------------------------------------------------
	Input: 1 2 3 4 5 6 7 8 9 10  || Output: 1 2 3 4 5 6 7 8 9 10
	No. of Swaps: 0
	Input: 1 2 3 4 5 6 7 8 9 10  || Output: 1 2 3 4 5 6 7 8 9 10
	No. of Swaps: 0
	Input: 1 2 3 4 5 6 7 8 9 10  || Output: 1 2 3 4 5 6 7 8 9 10
	No. of Swaps: 0
	Input: 1 2 3 4 5 6 7 8 9 10  || Output: 1 2 3 4 5 6 7 8 9 10
	No. of Swaps: 0
	Input: 1 2 3 4 5 6 7 8 9 10  || Output: 1 2 3 4 5 6 7 8 9 10
	No. of Swaps: 0
	-----------------------------------------------------
	Input: -1 2 -3 4 5 -6 7 8 -9 -10  || Output: -1 -10 -3 -9 -6 5 7 8 4 2
	No. of Swaps: 3
	Input: -1 2 -3 4 5 -6 7 8 -9 -10  || Output: -1 -3 -6 -9 -10 2 7 8 4 5
	No. of Swaps: 4
	Input: -1 2 -3 4 5 -6 7 8 -9 -10  || Output: -1 -3 -6 -9 -10 2 4 5 7 8
	No. of Swaps: 9
	Input: -1 2 -3 4 5 -6 7 8 -9 -10  || Output: -1 -3 -6 -9 -10 2 4 5 7 8
	No. of Swaps: 8
	Input: -1 2 -3 4 5 -6 7 8 -9 -10  || Output: -1 -3 -6 -9 -10 2 4 5 7 8
	No. of Swaps: 8
	-----------------------------------------------------
	Input: 1 2 -3 4 -5 -6 -7 8 9 10  || Output: -7 -6 -3 -5 4 2 1 8 9 10
	No. of Swaps: 3
	Input: 1 2 -3 4 -5 -6 -7 8 9 10  || Output: -3 -5 -6 -7 2 1 4 8 9 10
	No. of Swaps: 4
	Input: 1 2 -3 4 -5 -6 -7 8 9 10  || Output: -3 -5 -6 -7 1 2 4 8 9 10
	No. of Swaps: 7.5
	Input: 1 2 -3 4 -5 -6 -7 8 9 10  || Output: -3 -5 -6 -7 1 2 4 8 9 10
	No. of Swaps: 6.5
	Input: 1 2 -3 4 -5 -6 -7 8 9 10  || Output: -3 -5 -6 -7 1 2 4 8 9 10
	No. of Swaps: 6.5
	-----------------------------------------------------
	Input: 1 2 3 -4 -5 -6 -7 -8 -9 -10  || Output: -10 -9 -8 -4 -5 -6 -7 3 2 1
	No. of Swaps: 3
	Input: 1 2 3 -4 -5 -6 -7 -8 -9 -10  || Output: -4 -5 -6 -7 -8 -9 -10 2 3 1
	No. of Swaps: 7
	Input: 1 2 3 -4 -5 -6 -7 -8 -9 -10  || Output: -4 -5 -6 -7 -8 -9 -10 1 2 3
	No. of Swaps: 14
	Input: 1 2 3 -4 -5 -6 -7 -8 -9 -10  || Output: -4 -5 -6 -7 -8 -9 -10 1 2 3
	No. of Swaps: 5.5
	Input: 1 2 3 -4 -5 -6 -7 -8 -9 -10  || Output: -4 -5 -6 -7 -8 -9 -10 1 2 3
	No. of Swaps: 5.5
	-----------------------------------------------------
	Input: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10  || Output: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10
	No. of Swaps: 0
	Input: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10  || Output: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10
	No. of Swaps: 0
	Input: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10  || Output: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10
	No. of Swaps: 0
	Input: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10  || Output: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10
	No. of Swaps: 0
	Input: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10  || Output: -1 -2 -3 -4 -5 -6 -7 -8 -9 -10
	No. of Swaps: 0
	-----------------------------------------------------
	Input: 1 2 3 0 0 0 0 8 9 10  || Output: 0 0 0 0 3 2 1 8 9 10
	No. of Swaps: 3
	Input: 1 2 3 0 0 0 0 8 9 10  || Output: 0 0 0 0 2 3 1 8 9 10
	No. of Swaps: 4
	Input: 1 2 3 0 0 0 0 8 9 10  || Output: 0 0 0 0 1 2 3 8 9 10
	No. of Swaps: 8
	Input: 1 2 3 0 0 0 0 8 9 10  || Output: 0 0 0 0 1 2 3 8 9 10
	No. of Swaps: 4
	Input: 1 2 3 0 0 0 0 8 9 10  || Output: 0 0 0 0 1 2 3 8 9 10
	No. of Swaps: 4
	-----------------------------------------------------
	Input: 0 0 0 4 -5 -3 7 0 0 0  || Output: 0 0 0 0 -5 -3 0 0 7 4
	No. of Swaps: 2
	Input: 0 0 0 4 -5 -3 7 0 0 0  || Output: 0 0 0 -5 -3 0 0 0 7 4
	No. of Swaps: 5
	Input: 0 0 0 4 -5 -3 7 0 0 0  || Output: 0 0 0 -5 -3 0 0 0 4 7
	No. of Swaps: 6.5
	Input: 0 0 0 4 -5 -3 7 0 0 0  || Output: 0 0 0 -5 -3 0 0 0 4 7
	No. of Swaps: 5
	Input: 0 0 0 4 -5 -3 7 0 0 0  || Output: 0 0 0 -5 -3 0 0 0 4 7
	No. of Swaps: 5

*/
}

//------------------------------------------------------------------------------------------------
//produce all combinations of words from given string
void doProduceCombinations(string& str, const string& charSet, const int index, int& totalCombincations)
{
	if (index == charSet.length())
	{
		cout << endl << totalCombincations++ << " : " << str;
		return;
	}

	for (int i = 0; i < charSet.length(); i++)
	{
		str[index] = charSet[i];
		doProduceCombinations(str, charSet, index + 1, totalCombincations);
	}
}

void produceCombinations(const string& charSet)
{
	cout << "\n\nTest for : " << charSet << "------------------------------";
	string temp(charSet);
	int totalCombincations = 0;
	doProduceCombinations(temp, charSet, 0, totalCombincations);
}

void testCombinations()
{
	produceCombinations("");
	produceCombinations("A");
	produceCombinations("AB");
	produceCombinations("ABC");
	produceCombinations("ABCD");
	//produceCombinations("ABCDE");
	//produceCombinations("ABCDEF");
}

//-----------------------------------------------------------------------------------------------------
//Given an array and a number n, find all the pairs from array that sum upto n

struct Pair
{
	int first;
	int second;
};

//Assuming the numbers provided are non-negative and non repeating
void getPairs(vector<Pair>& result, vector<int>& input, int sum)
{
	vector<bool> cache(sum + 1);
	for (int i = 0; i <= sum; i++)
		cache[i] = false;

	for (int i = 0; i < input.size(); i++)
		if (input[i] <= sum)
			cache[input[i]] = true;

	for (int i = 0; i <= sum / 2; i++)
		if (cache[i] == true && cache[sum - i] == true)
		{
			Pair newPair;
			newPair.first = i;
			newPair.second = sum - i;
			result.push_back(newPair);
		}
}

void testFindPairsFromArrayThatSumUptoNumberFirstApproach()
{
	cout << "\n --------- testFindPairsFromArrayThatSumUptoNumberFirstApproach --------";

	vector<Pair> result;
	vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int sum = 15;
	getPairs(result, input, sum);
	for (int i = 0; i < result.size(); i++)
		cout << "\nPair: " << result[i].first << " ," << result[i].second;
}

// ---------------- Second approach

/* FOLLOWING FUNCTIONS ARE ONLY FOR SORTING
PURPOSE */
void exchange(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

int partition(int A[], int si, int ei)
{
	int x = A[ei];
	int i = (si - 1);
	int j;

	for (j = si; j <= ei - 1; j++)
	{
		if (A[j] <= x)
		{
			i++;
			exchange(&A[i], &A[j]);
		}
	}
	exchange(&A[i + 1], &A[ei]);
	return (i + 1);
}

//For better implementation of quicksort, refer "sorting.cpp"

/* Implementation of Quick Sort
A[] --> Array to be sorted
si  --> Starting index
ei  --> Ending index
*/
void quickSort(int A[], int si, int ei)
{
	int pi;    /* Partitioning index */
	if (si < ei)
	{
		pi = partition(A, si, ei);
		quickSort(A, si, pi - 1);
		quickSort(A, pi + 1, ei);
	}
}

bool hasArrayTwoCandidates(int A[], int arr_size, int sum)
{
	int l, r;

	/* Sort the elements */
	quickSort(A, 0, arr_size - 1);

	/* Now look for the two candidates in the sorted
	array*/
	l = 0;
	r = arr_size - 1;
	while (l < r)
	{
		if (A[l] + A[r] == sum)
			return 1;
		else if (A[l] + A[r] < sum)
			l++;
		else // A[i] + A[j] > sum
			r--;
	}
	return 0;
}

void testFindPairsFromArrayThatSumUptoNumberSecondApproach()
{
	cout << "\n --------- testFindPairsFromArrayThatSumUptoNumberSecondApproach --------";

	int A[] = { 1, 4, 45, 6, 10, -8 };
	int n = 16;
	int arr_size = 6;

	if (hasArrayTwoCandidates(A, arr_size, n))
		printf("\nArray has two elements with sum 16");
	else
		printf("\nArray doesn't have two elements with sum 16 ");

	getchar();
}

// ----------------------- Third approach - Best approach


bool has_pair(vector<int>& data, int sum)
{
	if (data.size() == 0)
	{
		return (sum == 0);
	}
	unordered_multiset <int> mys; // To make it work with repeated numbers/pairs

	for (int i = 0; i < data.size(); i++)
	{
		unordered_set<int>::const_iterator got = mys.find(sum - data[i]);
		if (got != mys.end())
		{
			cout << "\nThe pair found and they are " << data[i] << " and " << sum - data[i];
			mys.erase(got);
		}
		else
			mys.insert(data[i]);
	}
	return false;
}

void testFindPairsFromArrayThatSumUptoNumberThirdApproach()
{
	cout << "\n --------- testFindPairsFromArrayThatSumUptoNumberThirdApproach --------";
	vector<int> data = { 1, 4, 45, 7, 7, 6, 6, -8, 8, 5, 5};
	int sum = 12;
	has_pair(data, sum);
}

void testFindPairsFromArrayThatSumUptoNumber()
{
	testFindPairsFromArrayThatSumUptoNumberFirstApproach();
	testFindPairsFromArrayThatSumUptoNumberSecondApproach();
	testFindPairsFromArrayThatSumUptoNumberThirdApproach();
}

//-----------------------------------------------------------------------------------------------
//Implement power function

double getPowerOfNum(double base, int index)
{
	if (base == 0) return 0;

	bool isNegativeIndex = false;
	if (index < 0)
	{
		isNegativeIndex = true;
		index = -index;
	}
	double result = 1;
	for (int i = 0; i < index; i++)
		result *= base;
	if (isNegativeIndex)
		return 1 / result;
	else
		return result;
}

double getPowerOfNumSecondMethod(double base, int index)
{
	if (base == 0) return 0;

	bool isNegativeIndex = false;
	if (index < 0)
	{
		isNegativeIndex = true;
		index = -index;
	}
	double result = 1;
	if (index == 0)
		return result;
	else if (index == 1)
		return base;
	else if (index % 2 == 0)
		result = getPowerOfNumSecondMethod(base * base, index / 2);
	else
		result = base * getPowerOfNumSecondMethod(base * base, index / 2);

	if (isNegativeIndex)
		return 1 / result;
	else
		return result;
}

double getPowerOfNumThirdMethod(double base, int index)
{	
	if (base == 0) return 0;
	if (index == 0) return 1; //This case is NOT handled below, and this shoudl be after base = 0, because 0^0 = 0
	if (base == 1) return 1;
	if (base == -1) return (index & 1) ? -1 : 1;

	index = abs(index);

	double result = 1;
	if (index % 2 != 0)
		result = base;
	for (; index > 1; index /= 2)
		base = base * base;

	result = result * base;

	return (index > 0) ? result : 1 / result;
}

void testPowerFunction()
{
	double base;
	int index;
	
	for (double base : {-0.12, -1.0, 0.0, 1.0, 1.5, 2.345})
		for (int index : {-3, -2, -1, 0, 1, 2, 3})
		{
			cout << "\n\nbase = " << base << " index = " << index;
			cout << "\nFirst method: " << getPowerOfNum(base, index);
			cout << "\nSecond method: " << getPowerOfNum(base, index);
			cout << "\nThird method: " << getPowerOfNum(base, index);
		}
}

//-----------------------------------------------------------------------------------------------
//Merge the set of timings/numbers
//Find the free time slot of friends when busy time sets are given

struct BusyTime
{
	BusyTime(double startIn, double endIn)
		:start(startIn), end(endIn)
	{}
	double start, end;
};

void mergeBusyTimes(map<double, double>& total, vector<vector<BusyTime>>& individuals)
{
	for (int numFriends = 0; numFriends < individuals.size(); numFriends++)
	{
		for (int friendBusyTimeIndex = 0; friendBusyTimeIndex < individuals[numFriends].size(); friendBusyTimeIndex++)
		{
			BusyTime current = individuals[numFriends][friendBusyTimeIndex];
			//Find the entry in map for current.start,
			//If found, compare end time and update if current.end > mapEntry.end
			//else get closest entry and merge if there is any common time
			//else add a new entry
			//We need to iterate through map if we dont find entry with same start point
		}
	}
}

void testInviteFriendsForParty()
{
	//Convert the time into 24 hr format for easy comparison
	//Assume that these timings belong to same day and the problem boundaries are limited to that day
	//BusyTime busyTime1[] = { {8, 9}, {10.30, 11}, {5, 6} };
	vector<BusyTime> busyTime1 = { { 8, 9 }, { 10.30, 11 }, { 17, 18 } };
	//BusyTime busyTime2[] = { { 7, 8 },{ 3, 4 },{ 1, 2 }, {12.30, 1} };
	vector<BusyTime> busyTime2 = { { 7, 8 },{ 15, 16 },{ 13, 14 },{ 12.30, 13 } };
	//BusyTime busyTime3[] = { { 5, 6 }, { 1, 2 }, { 8, 9 } };
	vector<BusyTime> busyTime3 = { { 5, 6 },{ 13, 14 },{ 20, 21 } };
	
	
	vector<vector<BusyTime>> individuals;
	individuals.push_back(busyTime2);
	individuals.push_back(busyTime3);
	
	//use map of start time to end time to ensure sorting of start time to reduce the search complexity => O(nlogn)
	map<double, double> total;
	for (int i = 0; i < busyTime1.size(); i++)
	{
		BusyTime time = busyTime1[i];
		total[time.start] = time.end;
	}

	mergeBusyTimes(total, individuals);	
}

//------------------------------------------------

void scratchOutBusyTime(map<double, int>& result, int size, double shortestInterval, vector<vector<BusyTime>>& individuals)
{
	for (int numFriends = 0; numFriends < individuals.size(); numFriends++)
	{
		for (int friendBusyTimeIndex = 0; friendBusyTimeIndex < individuals[numFriends].size(); friendBusyTimeIndex++)
		{
			BusyTime current = individuals[numFriends][friendBusyTimeIndex];
			for (double start = current.start; start < current.end; start += shortestInterval)
				result[int(start / shortestInterval)] = 1;
		}
	}
}

double findShortestInterval(vector<vector<BusyTime>>& individuals)
{
	double shortestInterval = 24; //initialize with total hrs to find minimum
	for (int numFriends = 0; numFriends < individuals.size(); numFriends++)
	{
		for (int friendBusyTimeIndex = 0; friendBusyTimeIndex < individuals[numFriends].size(); friendBusyTimeIndex++)
		{
			BusyTime current = individuals[numFriends][friendBusyTimeIndex];
			double duration = current.end - current.start;
			if (shortestInterval > duration)
				shortestInterval = duration;
		}
	}

	return shortestInterval;
}

double findGCD(double first, double second)
{
	const double accuracy = 0.000001;
	double min = first;
	if (min > second)
		min = second;
	double ratio1 = first / min;
	double ratio2 = second / min;
	if (fabs(ratio1 - int(ratio1)) < accuracy && fabs(ratio2 - int(ratio2)) < accuracy)
		return min;
	else
		return findGCD(min, fabs(first - second));
}

double findShortestIntervalCorrectWay(vector<vector<BusyTime>>& individuals)
{
	double shortestInterval = 24.0; //initialize with total hrs to find minimum
	for (int numFriends = 0; numFriends < individuals.size(); numFriends++)
	{
		for (int friendBusyTimeIndex = 0; friendBusyTimeIndex < individuals[numFriends].size(); friendBusyTimeIndex++)
		{
			BusyTime current = individuals[numFriends][friendBusyTimeIndex];
			double duration = current.start - int(current.start);
			shortestInterval = findGCD(shortestInterval, duration);
			duration = current.end - int(current.end);
			shortestInterval = findGCD(shortestInterval, duration);
		}
	}

	return shortestInterval;
}

void testScratchOutBusyTime()
{
	//Convert the time into 24 hr format for easy comparison
	//Assume that these timings belong to same day and the problem boundaries are limited to that day
	//Another best way is to convert it into minutes to avoid floating point operation errors
	//BusyTime busyTime1[] = { {8, 9}, {10.30, 11}, {5, 6} };
	vector<BusyTime> busyTime1 = { { 8, 9 },{ 10.30, 11 },{ 17, 18 } };
	//BusyTime busyTime2[] = { { 7, 8 },{ 3, 4 },{ 1, 2 }, {12.30, 1} };
	vector<BusyTime> busyTime2 = { { 7, 8 },{ 15, 16 },{ 13, 14 },{ 12.30, 13 } };
	//BusyTime busyTime3[] = { { 5, 6 }, { 1, 2 }, { 8, 9 } };
	vector<BusyTime> busyTime3 = { { 5, 6 },{ 13, 14 },{ 20, 21 } };

	vector<vector<BusyTime>> individuals;
	individuals.push_back(busyTime2);
	individuals.push_back(busyTime3);

	//Shortest duration finding wont work if some friend in above set has busy duration like 20.10 - 21.35
	//In this case, the shortest duration may be 
	double shortestInterval = findShortestIntervalCorrectWay(individuals);
	const int size = 24.0 / shortestInterval;
	double* timePoints = new double[size];
	timePoints[0] = 0.0;
	for (int i = 1; i < size; i++)
	{
		timePoints[i] = timePoints[i - 1] + shortestInterval;
	}
	map<double, int> result;
	for (int i = 0; i < size; i++)
	{
		result[timePoints[i]] = 0;
	}
	scratchOutBusyTime(result, size, shortestInterval, individuals);
}

void arrayOfReferences()
{
/*

C++ Standard 8.3.2/4:
There shall be no references to references, no arrays of references, and no pointers to references.

A reference is analogous to an alias.
References are not objects. They don't have storage of their own, they just reference existing objects. 
For this reason it doesn't make sense to have arrays of references.

struct cintref
{
cintref(const int & ref) : ref(ref) {}
operator const int &() { return ref; }
private:
const int & ref;
void operator=(const cintref &);
};

int main()
{
int a=1,b=2,c=3;
//typedef const int &  cintref;
cintref arr[] = {a,b,c,8};
}


	
*/
}

//#define MACRO_COMPILE_TIME_ASSERT_IF_EQUAL(a, b) {int x[((a - b) == 0 ? 0 : 1)];}
//#define MACRO_COMPILE_TIME_ASSERT_IF_NOT_EQUAL(a, b) {int x[((a - b) != 0 ? 0 : 1)];}
#define MACRO_COMPILE_TIME_ASSERT_IF_EQUAL(a, b) {int x[((a == b) ? 0 : 1)];}
#define MACRO_COMPILE_TIME_ASSERT_IF_NOT_EQUAL(a, b) {int x[((a == b) ? 1 : 0)];}

#define MACRO_COMPILE_TIME_ASSERT(EXPRESSION) {int x[((EXPRESSION) ? 0 : 1)];} //Asserts when expression is true

//inline void inlinefun_compile_time_assert_if_equal(const int a, const int b)
//{
//	int x[((a - b) == 0 ? 0 : 1)];
//}
//
//inline void inlinefun_compile_time_assert_if_not_equal(const int a, const int b)
//{
//	int x[((a - b) != 0 ? 0 : 1)];
//}

void testStaticAssert()
{
	//inlinefun_compile_time_assert_if_equal(sizeof(double), sizeof(int));
	//inlinefun_compile_time_assert_if_not_equal(sizeof(double), sizeof(int));

	MACRO_COMPILE_TIME_ASSERT_IF_EQUAL(sizeof(double), sizeof(int));
	//MACRO_COMPILE_TIME_ASSERT_IF_EQUAL(4, sizeof(int));			//error C2466: cannot allocate an array of constant size 0
	//MACRO_COMPILE_TIME_ASSERT_IF_NOT_EQUAL(sizeof(double), sizeof(int));		 //error C2466: cannot allocate an array of constant size 0
	MACRO_COMPILE_TIME_ASSERT_IF_NOT_EQUAL(4, sizeof(int));

	MACRO_COMPILE_TIME_ASSERT(sizeof(double) == sizeof(int));
	//MACRO_COMPILE_TIME_ASSERT(4 == sizeof(int));		//error C2466: cannot allocate an array of constant size 0
}


//Take a vector of 1000 randon integers, and convert it into min heap (in place) (How to heapify?)

class A
{
public:
	static void fun()
	{
		A a;
		a.one();
		a.two();
	}

private:
	void one() {}
	void two() {}
};

void testBasicEncapsulation()
{
	//fun() is static function, but its member function, so it can access A's private members
	A::fun();
}


void printCurrentProgramName()
{
	const int len = 256;
	WCHAR pBuf[len];
	int bytes = GetModuleFileName(NULL, pBuf, len);
	if (bytes == 0)
		return;
	else
		wcout << "\nCurrent Program: " << pBuf << endl;
}


void printCurrentDirectory()
{
	char cCurrentPath[FILENAME_MAX];
	char* returnString = _getcwd(cCurrentPath, sizeof(cCurrentPath));
	cout << "The current working directory is: " << cCurrentPath << endl;
	cout << "The retval is: " << returnString << endl;
}


//------------------------------------------------------------------------------------------------
void miscellaneous()
{
	//checkAllDataTypeLowerAndUpperLimits();
	//tryVariousPrintingFormatsOfNumbers();
	//testEndianess();
	//testCheckIfBracketSeqValid();
	testClustorArray();
	//testCombinations();
	//testFindPairsFromArrayThatSumUptoNumber();
	//testPowerFunction();
	//printCurrentProgramName();
	//printCurrentDirectory();
}

}
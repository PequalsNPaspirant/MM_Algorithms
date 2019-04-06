//Goal:
//Implement all operator overloading in one

#include <iostream>
#include <initializer_list>
#include <cmath>
using namespace std;

namespace mm {

	class ComplexNumber
	{
	public:
		ComplexNumber(double realPart, double imaginaryPart)
			:m_realPart(realPart), m_imaginaryPart(imaginaryPart)
		{
		}

		~ComplexNumber()
		{
		}

		//Copy constructor
		ComplexNumber(const ComplexNumber& num)
			:m_realPart(num.m_realPart), m_imaginaryPart(num.m_imaginaryPart)
		{

		}

		//Arithmetic operators
		//	+ (addition)
		//	-(subtraction)
		//	* (multiplication)
		//	/ (division)
		//	% (modulus)

		//implement it as non-member function to make it posible cn1 = 2 + cn2. There must be an implicit contructor that takes one argument.
		//Return by value to make this possible: cn1 + cn2 + cn3 or cn1 + cn2 * cn3 
		//(Precedence will be followed by compiler. * will be executed before +)
		//Return constant to avoid cn1 + cn2 = cn3
		friend const ComplexNumber operator+(const ComplexNumber& lhs, const ComplexNumber& rhs)
		{
			return ComplexNumber(lhs.m_realPart + rhs.m_realPart, lhs.m_imaginaryPart + rhs.m_imaginaryPart);
		}

		//Similarly operators -, /, *, % can be implemented

		//unary - operator
		//return const to avoid -cn1 = cn2
		const ComplexNumber operator-()
		{
			return ComplexNumber(-m_realPart, -m_imaginaryPart);
		}

		//All of the bitwise operators are binary, except complement, which is unary.
		//It should be noted that these operators have a lower precedence than the arithmetic operators, 
		//so if ^ were to be overloaded for exponentiation, x ^ y + z may not work as intended.
		//Bitwise operators: 
		// ^ (XOR)
		// | (OR)
		// & (AND)
		// ~(complement)
		// << (shift left, insertion to stream)
		// >> (shift right, extraction from stream)

		//Left/right shift operator
		//Return non const to make this possible(cn << 2) >> 1
		ComplexNumber operator<<(const unsigned int n)
		{
			//This is just a dummy implementation (It may be meaningless)
			ComplexNumber(m_realPart * pow(2, n), m_imaginaryPart * pow(2, n));
		}

		//Implement operator<< to print complex number
		friend std::ostream& operator<<(std::ostream& out, const ComplexNumber& num) // output
		{
			out << "( " << num.m_realPart << (num.m_imaginaryPart < 0 ? " - " : " + ") << fabs(num.m_imaginaryPart) << " i )";
			return out;
		}

		friend std::istream& operator >> (std::istream& in, ComplexNumber& num) // input
		{
			double realPart, imaginaryPart;
			// skip opening paranthesis
			in.ignore(1);

			// read realPart
			in >> realPart;
			num.m_realPart = realPart;

			// skip delimiter + 
			in.ignore(3);

			// read imaginaryPart
			in >> imaginaryPart;
			num.m_imaginaryPart = imaginaryPart;

			// skip i and closing paranthesis
			in.ignore(2);

			return in;
		}

		//Assignment operator
		//Return reference to this to chain assignment work: cn1 = cn2 = cn3
		ComplexNumber& operator=(const ComplexNumber& num)
		{
			//Avoid self asssignment
			if (this == &num)
				return *this;

			//Deallocate old resources if any and allocate new resources if required

			//Do assignment
			m_realPart = num.m_realPart;
			m_imaginaryPart = num.m_imaginaryPart;

			return *this;
		}

		//Relational operators[edit]
		//	== (equality)
		//	!= (inequality)
		//	> (greater - than)
		//	< (less - than)
		//	>= (greater - than - or - equal - to)
		//	<= (less - than - or - equal - to)
		//There are, some templates defined in the header <utility>; if this header is included, 
		//then it suffices to just overload operator== and operator<, and the other operators will be provided by the STL.

		//Logical operators[edit]
		//	!(NOT)
		//	&& (AND)
		//	|| (OR)
		//The ! operator is unary, && and || are binary. 
		//It should be noted that in normal use, && and || have "short-circuit" behavior, where the right operand may not be evaluated, 
		//depending on the left operand. When overloaded, these operators get function call precedence, and this short circuit behavior is lost. 
		//It is best to leave these operators alone.

		//Compound assignment operators[edit]
		//	+= (addition - assignment)
		//	-= (subtraction - assignment)
		//	*= (multiplication - assignment)
		//	/= (division - assignment)
		//	%= (modulus - assignment)
		//	&= (AND - assignment)
		//	|= (OR - assignment)
		//	^= (XOR - assignment)
		//	<<= (shift - left - assignment)
		//	>>= (shift - right - assignment)

		//	Compound assignment operators should be overloaded as member functions, as they change the left - hand operand.

		//Increment and decrement operators[edit]
		//	++(increment)
		//	--(decrement)
		ComplexNumber& operator++() // prefix
		{
			++m_realPart;
			return *this;
		}

		//ComplexNumber operator++(int unused) // postfix
		//{
		//	SomeValue result = *this;
		//	++data;
		//	return result;
		//}

		//Often one operator is defined in terms of the other for ease in maintenance, especially if the function call is complex.
		ComplexNumber operator++(int unused) // postfix
		{
			ComplexNumber result = *this;
			++(*this); // call ComplexNumber::operator++() prefix
			return result;
		}

	private:
		double m_realPart;
		double m_imaginaryPart;
	};

	void testComplexNumbers()
	{
		ComplexNumber n1(2, 3), n2(5, 8), n3(7, 12);
		cout << "\n n1 : " << n1;
		cout << "\n n2 : " << n2;
		cout << "\n n3 : " << n3;

		n3 = n1 + n2;
		cout << "\n n3 : " << n3;

		ComplexNumber n4(n3);
		cout << "\n n3 : " << n3;

		n4 = -n3;
		cout << "\n n4 : " << n4;
	}

	template <typename T>
	class ReferenceCountingSmartPointer
	{
	public:
		ReferenceCountingSmartPointer(int size = 10)
			:m_pData(new T[size])
		{
		}
		~ReferenceCountingSmartPointer()
		{
			delete[] m_pData;
		}

		//Subscript operator

		//Address of, Reference, and Pointer operators

		//Function call operator
		//Used to implement Functors
		//Functors are used to encapsulate C and C++ Function Pointers
		//Functors are functions with a state.

	private:
		T* m_pData;

	};

	//Distinguish between read and write

	template<typename T>
	class Array
	{
	public:
		//forward declaration
		class Proxy;

		Array(unsigned const int size = 1)
			:m_size(size),
			m_pData(new T[m_size])
		{
		}

		Array(const std::initializer_list<T>& list)
			:m_size(list.size()),
			m_pData(new T[m_size])
		{
			int i = 0;
			//for each (T var in list)
			for (T var : list)
			{
				m_pData[i++] = var;
			}
		}

		~Array()
		{
			delete[] m_pData;
		}

		//Read or Write (called for non-const)
		//T& operator[](const unsigned int index)
		//This function must return by value, because *this is const and 
		//returned object can be used to write, so it should be non const in such case
		Proxy operator[](const unsigned int index)
		{
			cout << "\nCalling Array::operator[] for read or write";
			//return m_pData[index];
			return Proxy(*this, index);
		}

		//Read or Write (called for const)
		//const T& operator[](const unsigned int index) const
		//Function must return local or temporory object by value
		const Proxy operator[](const unsigned int index) const // warning C4172: returning address of local variable or temporary
		{
			cout << "\nCalling Array::operator[] const for read (this can not be called for write)";
			//return m_pData[index];
			return Proxy(*this, index);

			//OR this function can directly do following as it is called only for read
			//callMeWhileReading();
			//return m_pData[index];
		}

		class Proxy
		{
		public:
			Proxy(const Array& refObject, const unsigned int index)
				:m_refData(refObject),
				m_index(index)
			{
			}

			//Write
			//Obviously, this should be non-const function as it is used for writing
			const T& operator=(const T& rval)
			{
				cout << "\nCalling Proxy::operator= for write";
				m_refData.callMeWhileWriting();
				m_refData.m_pData[m_index] = rval;
				return rval; //To allow Array<int>[0] = Array<int>[1] = Array<int>[2] = 10
			}

			//Read
			operator const T&() const
			{
				cout << "\nCalling Proxy::operator const SameType&() for read";
				m_refData.callMeWhileReading();
				return m_refData.m_pData[m_index];
			}

		private:
			const Array<T>& m_refData;
			const unsigned int m_index;
		};

	private:
		unsigned const int m_size; //Declare this member before m_pData to get it initialised first in initialization list
		T* m_pData;

		//Reading can be called on const or non-const, so make it const to handle both cases
		void callMeWhileReading() const
		{
			cout << "\n Log: debug -> Somebody is reading the value";
		}

		//Writing can not be called on const object, so no need to make this function const
		//But the data stored in class Proxy is: const Array<T>& m_refData;
		//So we have to make it const
		void callMeWhileWriting() const
		{
			cout << "\n Log: debug -> Somebody is writing the value";
		}
	};

	void testDiffReadWrite()
	{
		const unsigned int index = 4;

		cout << "\nTesting non-const Array of int";
		Array<int> intArray(10);

		//Write
		cout << "\nWriting...";
		intArray[index] = 50;

		cout << "\nTesting chain assignment";
		intArray[0] = intArray[1] = intArray[2] = 10;

		//Read
		cout << "\nReading...";
		int i = intArray[index];
		cout << "\nThe expected value (shoudl be 50): " << i;

		cout << "\nTesting const Array of int";
		const Array<int> constIntArray({ 1, 2, 3, 4, 5 });
		//Write: This should be error
		cout << "\nWriting...(this should give compilation error)";
		//constIntArray[index] = 50; //error C2678: binary '=': no operator found which takes a left-hand operand 
									//of type 'const Array<int>::Proxy' (or there is no acceptable conversion)
		//Read
		cout << "\nReading...";
		i = constIntArray[index];
		cout << "\nThe expected value (shoudl be 5): " << i;
	}

	void OperatorOverloadingAllinOne(void)
	{
		//testComplexNumbers();
		testDiffReadWrite();
	}

}
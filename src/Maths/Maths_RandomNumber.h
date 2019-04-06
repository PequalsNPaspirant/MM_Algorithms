#pragma once

#include <cstdlib>
#include <cmath>
using namespace std;

namespace mm {

	template <typename Type>
	class RandomNumber
	{
	public:
		static Type getRandomNumber(unsigned int bitCount = 8);
		static Type getnDigitRandomNumber(unsigned int numDigits);

	private:
		RandomNumber();
		~RandomNumber();
	};

	template <typename Type>
	RandomNumber<Type>::RandomNumber()
	{
	}


	template <typename Type>
	RandomNumber<Type>::~RandomNumber()
	{
	}

	template <typename Type>
	Type RandomNumber<Type>::getRandomNumber(unsigned int bitCount /*= 8*/)
	{
		//unsigned short numberOfDigits = m_securityBitLength * log10(2.0);
		//return (rand() % Type(pow(10, numberOfDigits)));

		Type num = rand() % Type(pow(2, bitCount));
		return num + (1 << (bitCount - 1));  //Ensure that leftmost bit is always on so that the number has digits equal to "bitCount" (in base 2)
	}

	template <typename Type>
	Type RandomNumber<Type>::getnDigitRandomNumber(unsigned int numDigits)
	{
		return (rand() % Type(pow(10, numDigits))) + Type(pow(10, numDigits));  //Ensure than it has numDigits
	}

}
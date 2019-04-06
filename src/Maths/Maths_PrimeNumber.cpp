//#include "..\Header Files\PrimeNumber.h"
#include "Maths/Maths_PrimeNumber.h"

namespace mm {

	void UseAllPrimalityTests()
	{
		unsigned int primeNumber = PrimeNumber<unsigned int>::generateRandomPrimeNumber(8);
		for (int i = 0; i < MaxPrimalityTest; i++)
		{
			MyAssert::myRunTimeAssert(PrimeNumber<unsigned int>::checkIfPrimeNumber(primeNumber, ePrimalityTest(i)) == true);
		}
	}

	void TestPrimeNumber()
	{
		UseAllPrimalityTests();
	}

}

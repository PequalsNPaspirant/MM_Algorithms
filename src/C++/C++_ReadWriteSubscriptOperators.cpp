//Goal
// Implement subscript oprators for a vector/array to separate read write operations

#include <iostream>
using namespace std;

#include "C++\C++_ReadWriteSubscriptOperators.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	void testReadWriteSubscriptOperators()
	{
		unsigned int readCount = 0;
		unsigned int writeCount = 0;

		//Testing non-const Array of int
		Array<int> intArray(5);

		//Write
		intArray[0] = 50;
		readCount = intArray.getReadCount(0);
		writeCount = intArray.getWriteCount(0);
		MM_EXPECT_TRUE(readCount == 0 && writeCount == 1, readCount, writeCount);

		//Read
		intArray.setReadCount(0, 0);
		intArray.setWriteCount(0, 0);
		int i = intArray[0];
		readCount = intArray.getReadCount(0);
		writeCount = intArray.getWriteCount(0);
		MM_EXPECT_TRUE(readCount == 1 && writeCount == 0, readCount, writeCount);

		//Test chain assignment
		intArray[1] = intArray[2];
		readCount = intArray.getReadCount(2);
		writeCount = intArray.getWriteCount(2);
		MM_EXPECT_TRUE(readCount == 1 && writeCount == 0, readCount, writeCount);
		readCount = intArray.getReadCount(1);
		writeCount = intArray.getWriteCount(1);
		MM_EXPECT_TRUE(readCount == 0 && writeCount == 1, readCount, writeCount);

		intArray.setReadCount(1, 0);
		intArray.setWriteCount(1, 0);
		intArray.setReadCount(2, 0);
		intArray.setWriteCount(2, 0);
		intArray[1] = intArray[2] = 10;
		readCount = intArray.getReadCount(2);
		writeCount = intArray.getWriteCount(2);
		MM_EXPECT_TRUE(readCount == 1 && writeCount == 1, readCount, writeCount);
		readCount = intArray.getReadCount(1);
		writeCount = intArray.getWriteCount(1);
		MM_EXPECT_TRUE(readCount == 0 && writeCount == 1, readCount, writeCount);

		intArray.setReadCount(1, 0);
		intArray.setWriteCount(1, 0);
		intArray.setReadCount(2, 0);
		intArray.setWriteCount(2, 0);
		intArray[1] = intArray[2] = intArray[4];
		readCount = intArray.getReadCount(4);
		writeCount = intArray.getWriteCount(4);
		MM_EXPECT_TRUE(readCount == 1 && writeCount == 0, readCount, writeCount);
		readCount = intArray.getReadCount(2);
		writeCount = intArray.getWriteCount(2);
		MM_EXPECT_TRUE(readCount == 1 && writeCount == 1, readCount, writeCount);
		readCount = intArray.getReadCount(1);
		writeCount = intArray.getWriteCount(1);
		MM_EXPECT_TRUE(readCount == 0 && writeCount == 1, readCount, writeCount);

		//Testing const Array of int
		const Array<int> constIntArray({ 1, 2, 3, 4, 5 });
		//Write
		//constIntArray[index] = 50; //error C2678: binary '=': no operator found which takes a left-hand operand 
		//of type 'const Array<int>::Proxy' (or there is no acceptable conversion)
		//Read
		i = constIntArray[0];
		readCount = constIntArray.getReadCount(0);
		writeCount = constIntArray.getWriteCount(0);
		MM_EXPECT_TRUE(readCount == 0 && writeCount == 0, readCount, writeCount);

		//Test chain assignment
		//constIntArray[1] = constIntArray[2]; //Error: Can assign value to const object
	}

	MM_DECLARE_FLAG(ReadWriteSubscriptOperators);

	MM_UNIT_TEST(ReadWriteSubscriptOperators_test_1, ReadWriteSubscriptOperators)
	{
		testReadWriteSubscriptOperators();
	}

}


#include "Binary/Binary_ReverseBits.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	unsigned int reverseBits(unsigned int value)
	{
		unsigned int result = 0;
		for (int i = 0; ; )
		{
			if ((value & 1) == 1)
				result += 1;

			if (++i == sizeof(unsigned int) * 8)
				break;

			value = value >> 1;
			result = result << 1;
		}

		return result;
	}

	/*
	bool isOnlyOneBitON(); // or isPowerOf2()
	bool isnthBitON();
	bool isnthBitOFF();
	void setnthBitON();
	void setnthBitOFF();
	unsigned int getCountOfONBits();
	getRightmostONBit();
	getLeftmostONBit();
	keepOnlyRightmostBitON() { lhs[0] = (currentDigit & -currentDigit); }
	getCountOfONBits()
	*/

	MM_DECLARE_FLAG(Binary_reverseBitsInInteger);

	MM_UNIT_TEST(Binary_reverseBitsInInteger_test_1, Binary_reverseBitsInInteger)
	{
		struct testDataStruct
		{
			unsigned int value;
			unsigned int reverse;
		};

		std::vector<testDataStruct> testData{
			{ 1, 2147483648 },
			{ 2, 1073741824 }
		};

		for (int i = 0; i < testData.size(); ++i)
		{
			unsigned int actualResult;
			MM_EXPECT_TRUE((actualResult = reverseBits(testData[i].value)) == testData[i].reverse, testData[i].value, testData[i].reverse, actualResult);
		}

		unsigned int value[20] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
		for (int i = 0; i < 20; ++i)
		{
			unsigned int temp = reverseBits(value[i]);
			unsigned int temp2 = reverseBits(temp);
			MM_EXPECT_TRUE(value[i] == temp2, value[i], temp2)
		}
	}
}
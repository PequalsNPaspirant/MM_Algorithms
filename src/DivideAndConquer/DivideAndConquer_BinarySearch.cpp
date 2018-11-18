//Goal
//Solve puzzles using dynamic programming

#include <iostream>
#include <array>
#include <vector>
#include <string>
using namespace std;

#include "DivideAndConquer/DivideAndConquer_BinarySearch.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
//#include <gtest/gtest.h>

namespace mm {

	const std::array<int, 8> testSet8 = { 10, 20, 30, 40, 50, 60, 70, 80 };
	const std::array<int, 9> testSet9 = { 10, 20, 30, 40, 50, 60, 70, 80, 90 };
	const std::array<int, 10> testSet10 = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
	const std::array<int, 11> testSet11 = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110 };
	const std::array<int, 12> testSet12 = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120 };
	const std::array<int, 13> testSet13 = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130 };

	template<typename ObjectType, typename ContainerType>
	int BinarySearchTestCaseFormat(const ContainerType& container, ObjectType target)
	{
		//int index = binarySearch_1(container, target);
		//cout << "\nTarget " << target << " is present at index " << index << " in container";
		return binarySearch_1(container, target);
	}

	MM_DEFINE_FLAG(false, binarySearch_sanityTest);

	MM_UNIT_TEST(binarySearch_sanityTest8_1, binarySearch_sanityTest) { MM_EXPECT_EQUAL(0, BinarySearchTestCaseFormat(testSet8, 10)); }
	MM_UNIT_TEST(binarySearch_sanityTest8_2, binarySearch_sanityTest) { MM_EXPECT_EQUAL(1, BinarySearchTestCaseFormat(testSet8, 20)); }
	MM_UNIT_TEST(binarySearch_sanityTest8_3, binarySearch_sanityTest) { MM_EXPECT_EQUAL(2, BinarySearchTestCaseFormat(testSet8, 30)); }
	MM_UNIT_TEST(binarySearch_sanityTest8_4, binarySearch_sanityTest) { MM_EXPECT_EQUAL(3, BinarySearchTestCaseFormat(testSet8, 40)); }
	MM_UNIT_TEST(binarySearch_sanityTest8_5, binarySearch_sanityTest) { MM_EXPECT_EQUAL(4, BinarySearchTestCaseFormat(testSet8, 50)); }
	MM_UNIT_TEST(binarySearch_sanityTest8_6, binarySearch_sanityTest) { MM_EXPECT_EQUAL(5, BinarySearchTestCaseFormat(testSet8, 60)); }
	MM_UNIT_TEST(binarySearch_sanityTest8_7, binarySearch_sanityTest) { MM_EXPECT_EQUAL(6, BinarySearchTestCaseFormat(testSet8, 70)); }
	MM_UNIT_TEST(binarySearch_sanityTest8_8, binarySearch_sanityTest) { MM_EXPECT_EQUAL(7, BinarySearchTestCaseFormat(testSet8, 80)); }
	MM_UNIT_TEST(binarySearch_sanityTest8_n1, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet8, 5)); }
	MM_UNIT_TEST(binarySearch_sanityTest8_n2, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet8, 15)); }
	MM_UNIT_TEST(binarySearch_sanityTest8_n3, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet8, 25)); }
	MM_UNIT_TEST(binarySearch_sanityTest8_n4, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet8, 35)); }
	MM_UNIT_TEST(binarySearch_sanityTest8_n5, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet8, 45)); }
	MM_UNIT_TEST(binarySearch_sanityTest8_n6, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet8, 55)); }
	MM_UNIT_TEST(binarySearch_sanityTest8_n7, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet8, 65)); }
	MM_UNIT_TEST(binarySearch_sanityTest8_n8, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet8, 75)); }
	MM_UNIT_TEST(binarySearch_sanityTest8_n9, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet8, 85)); }

	MM_UNIT_TEST(binarySearch_sanityTest9_1, binarySearch_sanityTest) { MM_EXPECT_EQUAL(0, BinarySearchTestCaseFormat(testSet9, 10)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_2, binarySearch_sanityTest) { MM_EXPECT_EQUAL(1, BinarySearchTestCaseFormat(testSet9, 20)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_3, binarySearch_sanityTest) { MM_EXPECT_EQUAL(2, BinarySearchTestCaseFormat(testSet9, 30)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_4, binarySearch_sanityTest) { MM_EXPECT_EQUAL(3, BinarySearchTestCaseFormat(testSet9, 40)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_5, binarySearch_sanityTest) { MM_EXPECT_EQUAL(4, BinarySearchTestCaseFormat(testSet9, 50)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_6, binarySearch_sanityTest) { MM_EXPECT_EQUAL(5, BinarySearchTestCaseFormat(testSet9, 60)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_7, binarySearch_sanityTest) { MM_EXPECT_EQUAL(6, BinarySearchTestCaseFormat(testSet9, 70)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_8, binarySearch_sanityTest) { MM_EXPECT_EQUAL(7, BinarySearchTestCaseFormat(testSet9, 80)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_9, binarySearch_sanityTest) { MM_EXPECT_EQUAL(8, BinarySearchTestCaseFormat(testSet9, 90)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_n1, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet9, 5)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_n2, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet9, 15)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_n3, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet9, 25)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_n4, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet9, 35)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_n5, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet9, 45)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_n6, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet9, 55)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_n7, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet9, 65)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_n8, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet9, 75)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_n9, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet9, 85)); }
	MM_UNIT_TEST(binarySearch_sanityTest9_n10, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet9, 95)); }

	MM_UNIT_TEST(binarySearch_sanityTest10_1, binarySearch_sanityTest) { MM_EXPECT_EQUAL(0, BinarySearchTestCaseFormat(testSet10, 10)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_2, binarySearch_sanityTest) { MM_EXPECT_EQUAL(1, BinarySearchTestCaseFormat(testSet10, 20)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_3, binarySearch_sanityTest) { MM_EXPECT_EQUAL(2, BinarySearchTestCaseFormat(testSet10, 30)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_4, binarySearch_sanityTest) { MM_EXPECT_EQUAL(3, BinarySearchTestCaseFormat(testSet10, 40)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_5, binarySearch_sanityTest) { MM_EXPECT_EQUAL(4, BinarySearchTestCaseFormat(testSet10, 50)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_6, binarySearch_sanityTest) { MM_EXPECT_EQUAL(5, BinarySearchTestCaseFormat(testSet10, 60)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_7, binarySearch_sanityTest) { MM_EXPECT_EQUAL(6, BinarySearchTestCaseFormat(testSet10, 70)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_8, binarySearch_sanityTest) { MM_EXPECT_EQUAL(7, BinarySearchTestCaseFormat(testSet10, 80)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_9, binarySearch_sanityTest) { MM_EXPECT_EQUAL(8, BinarySearchTestCaseFormat(testSet10, 90)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_10, binarySearch_sanityTest) { MM_EXPECT_EQUAL(9, BinarySearchTestCaseFormat(testSet10, 100)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_n1, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet10, 5)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_n2, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet10, 15)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_n3, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet10, 25)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_n4, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet10, 35)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_n5, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet10, 45)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_n6, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet10, 55)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_n7, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet10, 65)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_n8, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet10, 75)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_n9, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet10, 85)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_n10, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet10, 95)); }
	MM_UNIT_TEST(binarySearch_sanityTest10_n11, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet10, 105)); }

	MM_UNIT_TEST(binarySearch_sanityTest11_1, binarySearch_sanityTest) { MM_EXPECT_EQUAL(0, BinarySearchTestCaseFormat(testSet11, 10)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_2, binarySearch_sanityTest) { MM_EXPECT_EQUAL(1, BinarySearchTestCaseFormat(testSet11, 20)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_3, binarySearch_sanityTest) { MM_EXPECT_EQUAL(2, BinarySearchTestCaseFormat(testSet11, 30)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_4, binarySearch_sanityTest) { MM_EXPECT_EQUAL(3, BinarySearchTestCaseFormat(testSet11, 40)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_5, binarySearch_sanityTest) { MM_EXPECT_EQUAL(4, BinarySearchTestCaseFormat(testSet11, 50)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_6, binarySearch_sanityTest) { MM_EXPECT_EQUAL(5, BinarySearchTestCaseFormat(testSet11, 60)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_7, binarySearch_sanityTest) { MM_EXPECT_EQUAL(6, BinarySearchTestCaseFormat(testSet11, 70)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_8, binarySearch_sanityTest) { MM_EXPECT_EQUAL(7, BinarySearchTestCaseFormat(testSet11, 80)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_9, binarySearch_sanityTest) { MM_EXPECT_EQUAL(8, BinarySearchTestCaseFormat(testSet11, 90)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_10, binarySearch_sanityTest) { MM_EXPECT_EQUAL(9, BinarySearchTestCaseFormat(testSet11, 100)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_11, binarySearch_sanityTest) { MM_EXPECT_EQUAL(10, BinarySearchTestCaseFormat(testSet11, 110)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_n1, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet11, 5)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_n2, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet11, 15)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_n3, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet11, 25)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_n4, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet11, 35)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_n5, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet11, 45)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_n6, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet11, 55)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_n7, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet11, 65)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_n8, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet11, 75)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_n9, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet11, 85)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_n10, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet11, 95)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_n11, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet11, 105)); }
	MM_UNIT_TEST(binarySearch_sanityTest11_n12, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet11, 115)); }

	MM_UNIT_TEST(binarySearch_sanityTest12_1, binarySearch_sanityTest) { MM_EXPECT_EQUAL(0, BinarySearchTestCaseFormat(testSet12, 10)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_2, binarySearch_sanityTest) { MM_EXPECT_EQUAL(1, BinarySearchTestCaseFormat(testSet12, 20)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_3, binarySearch_sanityTest) { MM_EXPECT_EQUAL(2, BinarySearchTestCaseFormat(testSet12, 30)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_4, binarySearch_sanityTest) { MM_EXPECT_EQUAL(3, BinarySearchTestCaseFormat(testSet12, 40)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_5, binarySearch_sanityTest) { MM_EXPECT_EQUAL(4, BinarySearchTestCaseFormat(testSet12, 50)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_6, binarySearch_sanityTest) { MM_EXPECT_EQUAL(5, BinarySearchTestCaseFormat(testSet12, 60)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_7, binarySearch_sanityTest) { MM_EXPECT_EQUAL(6, BinarySearchTestCaseFormat(testSet12, 70)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_8, binarySearch_sanityTest) { MM_EXPECT_EQUAL(7, BinarySearchTestCaseFormat(testSet12, 80)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_9, binarySearch_sanityTest) { MM_EXPECT_EQUAL(8, BinarySearchTestCaseFormat(testSet12, 90)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_10, binarySearch_sanityTest) { MM_EXPECT_EQUAL(9, BinarySearchTestCaseFormat(testSet12, 100)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_11, binarySearch_sanityTest) { MM_EXPECT_EQUAL(10, BinarySearchTestCaseFormat(testSet12, 110)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_12, binarySearch_sanityTest) { MM_EXPECT_EQUAL(11, BinarySearchTestCaseFormat(testSet12, 120)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_n1, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet12, 5)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_n2, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet12, 15)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_n3, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet12, 25)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_n4, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet12, 35)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_n5, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet12, 45)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_n6, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet12, 55)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_n7, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet12, 65)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_n8, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet12, 75)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_n9, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet12, 85)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_n10, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet12, 95)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_n11, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet12, 105)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_n12, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet12, 115)); }
	MM_UNIT_TEST(binarySearch_sanityTest12_n13, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet12, 125)); }

	MM_UNIT_TEST(binarySearch_sanityTest13_1, binarySearch_sanityTest) { MM_EXPECT_EQUAL(0, BinarySearchTestCaseFormat(testSet13, 10)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_2, binarySearch_sanityTest) { MM_EXPECT_EQUAL(1, BinarySearchTestCaseFormat(testSet13, 20)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_3, binarySearch_sanityTest) { MM_EXPECT_EQUAL(2, BinarySearchTestCaseFormat(testSet13, 30)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_4, binarySearch_sanityTest) { MM_EXPECT_EQUAL(3, BinarySearchTestCaseFormat(testSet13, 40)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_5, binarySearch_sanityTest) { MM_EXPECT_EQUAL(4, BinarySearchTestCaseFormat(testSet13, 50)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_6, binarySearch_sanityTest) { MM_EXPECT_EQUAL(5, BinarySearchTestCaseFormat(testSet13, 60)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_7, binarySearch_sanityTest) { MM_EXPECT_EQUAL(6, BinarySearchTestCaseFormat(testSet13, 70)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_8, binarySearch_sanityTest) { MM_EXPECT_EQUAL(7, BinarySearchTestCaseFormat(testSet13, 80)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_9, binarySearch_sanityTest) { MM_EXPECT_EQUAL(8, BinarySearchTestCaseFormat(testSet13, 90)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_10, binarySearch_sanityTest) { MM_EXPECT_EQUAL(9, BinarySearchTestCaseFormat(testSet13, 100)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_11, binarySearch_sanityTest) { MM_EXPECT_EQUAL(10, BinarySearchTestCaseFormat(testSet13, 110)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_12, binarySearch_sanityTest) { MM_EXPECT_EQUAL(11, BinarySearchTestCaseFormat(testSet13, 120)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_13, binarySearch_sanityTest) { MM_EXPECT_EQUAL(12, BinarySearchTestCaseFormat(testSet13, 130)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_n1, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet13, 5)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_n2, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet13, 15)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_n3, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet13, 25)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_n4, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet13, 35)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_n5, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet13, 45)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_n6, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet13, 55)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_n7, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet13, 65)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_n8, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet13, 75)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_n9, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet13, 85)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_n10, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet13, 95)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_n11, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet13, 105)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_n12, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet13, 115)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_n13, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet13, 125)); }
	MM_UNIT_TEST(binarySearch_sanityTest13_n14, binarySearch_sanityTest) { MM_EXPECT_EQUAL(-1, BinarySearchTestCaseFormat(testSet13, 135)); }
}

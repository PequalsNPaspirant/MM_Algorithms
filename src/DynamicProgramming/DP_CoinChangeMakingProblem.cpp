#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // for std::min()
#include <unordered_map>
using namespace std;

#include "DynamicProgramming/DP_CoinChangeMakingProblem.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	// ============================= Problem Statement =============================
	/*
		Given an unlimited supply of coins of
given denominations, Find
the
minimum number of coins required to
get a desired change
	*/

	// ============================= Top-down (Recursive) =============================
	/*
	Complexity: O(n^n)
	Auxiliary space: O(1)

	This can be converted into general (not 1-0) knapsack problem, 
	to pick the coins of different denominations (we can pick multiple coins of same denominations),
	so that total value is less than or equal to given target and number of coins is minimum

	recurrence relation:
	count(0, n, targetValue) = min(1 + count(0, n - 1, targetValue - coinValue(n),  // include last coin, but consider adding the same coin again since we can take multiple coins of same denomination
								count(0, n, targetValue)) // exclude last coin
	*/
	int getMinCoinsChange_recursive_1(const vector<int>& coins, const int startIndex, const int endIndex, const int totalValue)
	{
		if (totalValue == 0)
			return 0;

		if (totalValue < 0 || endIndex < startIndex)
			return numeric_limits<int>::max();

		// Include last coin, and recurse on all coins to collect remaining value
		int numberOfCoinsIncludingThis = getMinCoinsChange_recursive_1(coins, startIndex, endIndex, totalValue - coins[endIndex]);
		if (numberOfCoinsIncludingThis != numeric_limits<int>::max())
			numberOfCoinsIncludingThis += 1;

		//Exclude last coin and recurse on the remaining coins to collect target value
		int numberOfCoinsExcludingThis = getMinCoinsChange_recursive_1(coins, startIndex, endIndex - 1, totalValue);

		return std::min(numberOfCoinsIncludingThis, numberOfCoinsExcludingThis);
	}

	/*
	recurrence relation :
	count(targetValue) = min(1 + count(targetValue - coinValue(1),  // include first coin
							 1 + count(targetValue - coinValue(2),  // include second coin
							 ...
							 1 + count(targetValue - coinValue(n)) // exclude last coin
	*/

	int getMinCoinsChange_recursive_2(const vector<int>& coins, const int totalValue)
	{
		if (totalValue == 0)
			return 0;

		if (totalValue < 0)
			return numeric_limits<int>::max();

		int minCoins = numeric_limits<int>::max();
		for (int coinIndex = 0; coinIndex < coins.size(); ++coinIndex)
		{
			// TODO: this algo gives repeated results, so to avoid it, pass startIndex to recursive function, so that it can not pick coins of less denominations again (since we have already considered those coins before)
			int numberOfCoins = getMinCoinsChange_recursive_2(coins, totalValue - coins[coinIndex]);
			if (numberOfCoins != numeric_limits<int>::max()) // If we could cover totalValue
				minCoins = std::min(minCoins, numberOfCoins + 1);
			//else
			//	continue; // If we could not cover totalValue, skip this coin and try next coin for target value remaining at this stage
		}

		return minCoins;
	}

	// ============================= Top-down dynamic programming =============================
	/*
	Complexity: O(n*n)
	Auxiliary space: O(n + 1)
	Recurrence relation:
		minCount(targetValue) = min( 1 + minCount(targetValue - coinValue(1)),
									 1 + minCount(targetValue - coinValue(2)),
									 ...
									 1 + minCount(targetValue - coinValue(n))) ...where targetValue < coinValue(n)
	*/
	int getMinCoinsChange_DP_TopDown_1(unordered_map<int, int>& minCoinsHashTable, const vector<int>& coins, const int startIndex, const int endIndex, const int totalValue)
	{
		if (totalValue == 0)
			return 0;

		if (totalValue < 0 || endIndex < startIndex)
			return numeric_limits<int>::max();

		// Include last coin, and recurse on all coins to collect remaining value
		int numberOfCoinsIncludingThis = numeric_limits<int>::max();
		if (minCoinsHashTable.find(totalValue) == minCoinsHashTable.end())
		{
			int numberOfCoinsIncludingThis = getMinCoinsChange_DP_TopDown_1(minCoinsHashTable, coins, startIndex, endIndex, totalValue - coins[endIndex]);
			if (numberOfCoinsIncludingThis != numeric_limits<int>::max())
				numberOfCoinsIncludingThis = minCoinsHashTable[totalValue - coins[endIndex]] + 1;

			//Exclude last coin and recurse on the remaining coins to collect target value
			int numberOfCoinsExcludingThis = getMinCoinsChange_DP_TopDown_1(minCoinsHashTable, coins, startIndex, endIndex - 1, totalValue);
			minCoinsHashTable[totalValue] = std::min(numberOfCoinsIncludingThis, numberOfCoinsExcludingThis);
		}
		else
			int n = 10;
		
		return minCoinsHashTable[totalValue];
	}

	int getMinCoinsChange_DP_TopDown_1(const vector<int>& coins, const int startIndex, const int endIndex, const int totalValue)
	{
		// Create a hash table to store results of subproblems
		unordered_map<int, int> minCoinsHashTable;
		return getMinCoinsChange_DP_TopDown_1(minCoinsHashTable, coins, startIndex, endIndex, totalValue);
	}


	int getMinCoinsChange_DP_TopDown_2(unordered_map<int, int>& minCoinsHashTable, const vector<int>& coins, const int totalValue)
	{
		if (totalValue == 0)
			return 0;

		if (totalValue < 0)
			return numeric_limits<int>::max();


		if (minCoinsHashTable.find(totalValue) == minCoinsHashTable.end())
		{
			int minCoins = numeric_limits<int>::max();
			for (int coinIndex = 0; coinIndex < coins.size(); ++coinIndex)
			{
				int numberOfCoins = getMinCoinsChange_DP_TopDown_2(minCoinsHashTable, coins, totalValue - coins[coinIndex]);
				if (numberOfCoins != numeric_limits<int>::max()) // If we could cover totalValue
					minCoins = std::min(minCoins, numberOfCoins + 1);
				//else
				//	continue; // If we could not cover totalValue, skip this coin and try next coin for target value remaining at this stage
				
			}
			minCoinsHashTable[totalValue] = minCoins;
		}
		else
			int n = 10; // to debug and check if we are really reusing the subproblems results

		return minCoinsHashTable[totalValue];
	}

	int getMinCoinsChange_DP_TopDown_2(const vector<int>& coins, const int totalValue)
	{
		// Create a hash table to store results of subproblems
		unordered_map<int, int> minCoinsHashTable;
		return getMinCoinsChange_DP_TopDown_2(minCoinsHashTable, coins, totalValue);
	}

	// ============================= bottom-up dynamic programming =============================
	/*
	Complexity: O(n*n)
	Auxiliary space: O(n + 1)
	Recurrence relation:
		maxRevenue(n) = max( price(1) + maxRevenue(n - 1),
							 price(2) + maxRevenue(n - 2),
							 ...
							 price(n))
	*/

	//int getMinCoinsChange_DP_BottomUp(const vector<int>& price, const int rodLength)
	int getMinCoinsChange_DP_BottomUp(const vector<int>& coins, const int totalValue)
	{
		vector<int> minCoinsHashTable(totalValue + 1);
		// initial condition:
		minCoinsHashTable[0] = 0; // is total value is 0, we need 0 coins, initialize it with max()
		for (int totalCurrentValue = 1; totalCurrentValue <= totalValue; ++totalCurrentValue)
		{
			minCoinsHashTable[totalCurrentValue] = numeric_limits<int>::max();
			for (int coinIndex = 0; coinIndex < coins.size(); ++coinIndex)
			{
				if (totalCurrentValue >= coins[coinIndex])
				{
					// Include current coin
					int currentMinCoins = minCoinsHashTable[totalCurrentValue - coins[coinIndex]];
					if (currentMinCoins != numeric_limits<int>::max())
						minCoinsHashTable[totalCurrentValue] = std::min(minCoinsHashTable[totalCurrentValue], currentMinCoins + 1);
				}
			}
		}

		return minCoinsHashTable[totalValue];
	}

	// ============================= Unit Tests =============================

	MM_DECLARE_FLAG(DP_MinCoinsChange)

	MM_UNIT_TEST(DP_MinCoinsChange_test_1, DP_MinCoinsChange)
	{
		const vector<int> coins1 = { 1, 2, 3, 4 };
		int numCoins = 0;

		MM_EXPECT_TRUE((numCoins = getMinCoinsChange_recursive_1(coins1, 0, static_cast<int>(coins1.size() - 1), 15)) == 4, numCoins);
		MM_EXPECT_TRUE((numCoins = getMinCoinsChange_recursive_2(coins1, 15)) == 4, numCoins);
		MM_EXPECT_TRUE((numCoins = getMinCoinsChange_DP_TopDown_1(coins1, 0, static_cast<int>(coins1.size() - 1), 15)) == 4, numCoins);
		MM_EXPECT_TRUE((numCoins = getMinCoinsChange_DP_TopDown_2(coins1, 15)) == 4, numCoins);
		MM_EXPECT_TRUE((numCoins = getMinCoinsChange_DP_BottomUp(coins1, 15)) == 4, numCoins);

		const vector<int> coins2 = { 1, 3, 5, 7 };
		MM_EXPECT_TRUE((numCoins = getMinCoinsChange_recursive_1(coins2, 0, static_cast<int>(coins2.size() - 1), 15)) == 3, numCoins);
		MM_EXPECT_TRUE((numCoins = getMinCoinsChange_recursive_2(coins2, 15)) == 3, numCoins);
		MM_EXPECT_TRUE((numCoins = getMinCoinsChange_DP_TopDown_1(coins2, 0, static_cast<int>(coins2.size() - 1), 15)) == 3, numCoins);
		MM_EXPECT_TRUE((numCoins = getMinCoinsChange_DP_TopDown_2(coins2, 15)) == 3, numCoins);
		MM_EXPECT_TRUE((numCoins = getMinCoinsChange_DP_BottomUp(coins2, 15)) == 3, numCoins);
	}
	
}

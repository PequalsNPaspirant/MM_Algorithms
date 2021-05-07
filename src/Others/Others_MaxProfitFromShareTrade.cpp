#include <tuple>

#include "Others/Others_MaxProfitFromShareTrade.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	/*
	 Given the closing prices of shares for last few days, find the days on which the share could have been 
	 bought and sold to get max profit.

	*/

	int getMaxProfitFromShareTrading_v1(const vector<int> &closingPricesIn, int& buyIndex, int& sellIndex)
	{
		buyIndex = 0, sellIndex = 0;
		int maxProfit = 0;
		int tempBuyIndex = 0, tempSellIndex = 0;
		for (int i = 1; i < closingPricesIn.size(); ++i)
		{
			if (closingPricesIn[i] > closingPricesIn[tempSellIndex])
			{
				tempSellIndex = i;
				// if we see more profit than before, preserve the data
				int currentProfit = closingPricesIn[tempSellIndex] - closingPricesIn[tempBuyIndex];
				if (currentProfit > maxProfit)
				{
					buyIndex = tempBuyIndex;
					sellIndex = tempSellIndex;
					maxProfit = currentProfit;
				}
			}

			if (closingPricesIn[i] < closingPricesIn[tempBuyIndex])
			{
				// update new buy index, but we can not use previous sell index, we need to look for new sell index in future
				// in short, start over again
				tempBuyIndex = i;
				tempSellIndex = tempBuyIndex;
			}
		}

		return maxProfit;
	}

	int getMaxProfitFromShareTrading_v2(const vector<int> &closingPricesIn, int& buyIndex, int& sellIndex)
	{
		buyIndex = 0, sellIndex = 0;
		int maxProfit = 0;
		int tempBuyIndex = 0;
		for (int i = 1; i < closingPricesIn.size(); ++i)
		{
			// if we see more profit than before, preserve the data
			int currentProfit = closingPricesIn[i] - closingPricesIn[tempBuyIndex];
			if (currentProfit > maxProfit)
			{
				buyIndex = tempBuyIndex;
				sellIndex = i;
				maxProfit = currentProfit;
			}

			if (closingPricesIn[i] < closingPricesIn[tempBuyIndex])
			{
				// update new buy index, but we can not use previous sell index, we need to look for new sell index in future
				// in short, start over again
				tempBuyIndex = i;
			}
		}

		return maxProfit;
	}
	
	MM_DECLARE_FLAG(getMaxProfitFromShareTradingUnitTest);

	MM_UNIT_TEST(getMaxProfitFromShareTrading_v1_UnitTest_sanity, getMaxProfitFromShareTradingUnitTest)
	{
		vector<std::tuple< vector<int>, int, int, int>> testSet = {
			{ { 1, 3, 6, 4, 1, 2 }, 0, 2, 5 },
			{ { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }, 0, 9, 9 },
			{ { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 }, 0, 0, 0 },
			{ { 4, 5, 6, 5, 4, 3, 2, 3, 4 }, 0, 2, 2 },
			{ { 4, 5, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7 }, 6, 11, 5 },
			{ { 4, 5, 6, 5, 4, 3, 2, 3, 4, 4, 5, 6, 5, 4, 3, 2, 3, 4, 4, 5, 6, 5, 4, 3, 2, 3, 4 }, 6, 11, 4 },
			{ { 4, 5, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7, 4, 5, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7, 4, 5, 6, 5, 4, 3, 2, 3, 4, 5, 6, 7 }, 6, 11, 5 },
			{ { 18, 10, 18, 12, 14, 4, 6, 18, 6, 9, 13, 13, 13, 11, 2, 1, 1, 19, 10, 8 }, 15, 17, 18 },
			{ { 17, 3, 13, 2, 8, 11, 0, 19, 6, 14, 3, 4, 4, 5, 7, 13, 11, 16, 2, 0 }, 6, 7, 19 },
			{ { 18, 3, 1, 1, 13, 14, 0, 8, 9, 11, 3, 8, 13, 17, 6, 4, 14, 12, 15, 3 }, 6, 13, 17 },
			{ { 16, 10, 1, 6, 6, 14, 4, 5, 16, 6, 12, 0, 0, 19, 16, 6, 15, 7, 1, 3 }, 11, 13, 19 },
			{ { 3, 2, 13, 8, 9, 14, 9, 16, 16, 4, 7, 18, 4, 1, 3, 6, 5, 0, 8, 6 }, 1, 11, 16 },
			{ { 12, 13, 2, 8, 7, 12, 10, 1, 7, 6, 10, 4, 2, 6, 16, 19, 6, 18, 10, 7 }, 7, 15, 18 },
			{ { 2, 3, 3, 2, 10, 1, 15, 15, 5, 17, 10, 0, 5, 13, 10, 8, 6, 19, 15, 4 }, 11, 17, 19 },
			{ { 19, 3, 10, 3, 0, 3, 1, 14, 3, 18, 8, 8, 14, 6, 9, 2, 9, 16, 18, 3 }, 4, 9, 18 },
			{ { 18, 3, 8, 9, 4, 18, 17, 3, 10, 19, 15, 16, 10, 7, 2, 4, 1, 16, 4, 9 }, 1, 9, 16 },
			{ { 19, 9, 13, 19, 13, 1, 5, 18, 8, 7, 10, 16, 4, 12, 0, 17, 8, 16, 9, 2 }, 5, 7, 17 }
		};

		for (size_t i = 0; i < testSet.size(); ++i)
		{
			int buyIndex, sellIndex, maxProfit = 0;
			MM_EXPECT_TRUE((maxProfit = getMaxProfitFromShareTrading_v1(std::get<0>(testSet[i]), buyIndex, sellIndex)) 
				== std::get<3>(testSet[i]) && buyIndex == std::get<1>(testSet[i]) && sellIndex == std::get<2>(testSet[i]), 
				i, buyIndex, sellIndex, maxProfit);
			MM_EXPECT_TRUE((maxProfit = getMaxProfitFromShareTrading_v2(std::get<0>(testSet[i]), buyIndex, sellIndex))
				== std::get<3>(testSet[i]) && buyIndex == std::get<1>(testSet[i]) && sellIndex == std::get<2>(testSet[i]),
				i, buyIndex, sellIndex, maxProfit);
		}
	}

}
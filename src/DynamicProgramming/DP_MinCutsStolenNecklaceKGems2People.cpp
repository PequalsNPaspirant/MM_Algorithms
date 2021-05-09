#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <random>
#include <utility>
#include <algorithm>
using namespace std;

#include "DynamicProgramming/DP_MinCutsStolenNecklaceKGems2People.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	// ============================= Problem Statement =============================
	/*
		Two thieves stole a necklace made up of k types of gems. Each type of gem has even number of gems in necklace.
		Take a minimum number of cuts on necklace to divide it between the two thieves such that both gets equal number of gems of each type.
	*/

	// ============================= Top-down (Recursive) =============================
	/*
	Complexity: O(n^n)
	Auxiliary space: O(1)

	This can be converted into general (not 1-0) knapsack problem,
	to pick the rod pieces of different length (we can pick multiple pieces of same length),
	so that total length is less than or equal to given max rod length and total price is maximum
	*/

	using GemType = int;
	using Count = int;

	struct MinCutsStolenNecklaceResults
	{
		std::vector<int> owners;
		int minCuts;

		void calculateMinCuts()
		{
			minCuts = 0;
			for (size_t i = 1; i < owners.size(); ++i)
				if (owners[i - 1] != owners[i])
					++minCuts;
		}
	};
	
	int getMinCutsStolenNecklaceKGems2People_recursive(int numPeople, const vector<int>& necklace, int gemIndex, int totalGemsToDistribute,
		vector<unordered_map<GemType, Count>>& expectedDistribution, MinCutsStolenNecklaceResults& currentResults, vector<MinCutsStolenNecklaceResults>& results)
	{
		//If this result is good, store it
		if (gemIndex == 0)
		{
			if (totalGemsToDistribute == 0)
			{
				currentResults.calculateMinCuts();
				//Push to results if the current results are same or better
				if(!results.empty() && currentResults.minCuts < results.front().minCuts)
					results.clear(); //current result is better, so remove old results

				if (results.empty() || currentResults.minCuts == results.front().minCuts)
					results.push_back(currentResults);
			}
			
			return currentResults.minCuts;
		}

		int minCutsSoFar = numeric_limits<int>::max();

		//skip the current gem and try
		int minCutsSkipCurrent = getMinCutsStolenNecklaceKGems2People_recursive(numPeople, necklace, gemIndex - 1, 
			totalGemsToDistribute, expectedDistribution, currentResults, results);

		GemType type = necklace[gemIndex - 1];
		for (int personIndex = 0; personIndex < numPeople; ++personIndex)
		{
			unordered_map<GemType, Count>& currPerDist = expectedDistribution[personIndex];
			Count& c = currPerDist[type];
			if (c == 0)
				continue;

			currentResults.owners[gemIndex - 1] = personIndex;
			--c;
			//either backtrack i.e. reduce it here and increase after recursive call 
			//OR do not change local value and pass a new value to recursive call
			//--totalGemsToDistribute; 

			int minCutsForSubinstance = getMinCutsStolenNecklaceKGems2People_recursive(numPeople, necklace, gemIndex - 1, 
				totalGemsToDistribute - 1, expectedDistribution, currentResults, results);

			if (minCutsSoFar > minCutsForSubinstance)
				minCutsSoFar = minCutsForSubinstance;

			//backtrack
			currentResults.owners[gemIndex - 1] = -1;
			++c;
			//++totalGemsToDistribute
		}

		if (minCutsSoFar > minCutsSkipCurrent)
			minCutsSoFar = minCutsSkipCurrent;

		return minCutsSoFar;
	}

	int getMinCutsStolenNecklaceKGems2People_recursive(int numPeople, const vector<int>& necklace,
		vector<unordered_map<GemType, Count>>& expectedDistribution, vector<MinCutsStolenNecklaceResults>& results)
	{
		MinCutsStolenNecklaceResults currentResultsTemp;
		currentResultsTemp.owners.resize(necklace.size(), -1);
		currentResultsTemp.minCuts = numeric_limits<int>::max();
		int totalGemsToDistribute = 0;
		for (size_t personIndex = 0; personIndex < expectedDistribution.size(); ++personIndex)
		{
			const unordered_map<GemType, Count>& currPerDist = expectedDistribution[personIndex];
			for (auto it = currPerDist.begin(); it != currPerDist.end(); ++it)
				totalGemsToDistribute += it->second;
		}

		return getMinCutsStolenNecklaceKGems2People_recursive(numPeople, necklace, static_cast<int>(necklace.size()), 
			totalGemsToDistribute, expectedDistribution, currentResultsTemp, results);
	}


	// ============================= Top-down dynamic programming =============================
	/*
	Complexity: O(n*n)
	Auxiliary space: O(n + 1)
	Recurrence relation:
		maxRevenue(n) = max( price(1) + maxRevenue(n - 1),
								price(2) + maxRevenue(n - 2),
								...
								price(n))
	*/
	//int getMinCutsStolenNecklaceKGems2People_DP_TopDown(vector<int>& maxRevenueHashTable, const vector<int>& price, const int rodLength)
	//{
	//	if (rodLength == 0)
	//		return 0;

	//	int maxRevenue = 0;
	//	for (int rodLen = 1; rodLen <= rodLength; ++rodLen)
	//	{
	//		if (maxRevenueHashTable[rodLength - rodLen] == -1)
	//			maxRevenueHashTable[rodLength - rodLen] = getMaxRevenueRodCutting_DP_TopDown(maxRevenueHashTable, price, rodLength - rodLen);

	//		int currentRevenue = price[rodLen - 1] + maxRevenueHashTable[rodLength - rodLen];
	//		if (maxRevenue < currentRevenue)
	//			maxRevenue = currentRevenue;
	//	}

	//	return maxRevenue;
	//}

	//int getMinCutsStolenNecklaceKGems2People_DP_TopDown(const vector<int>& price, const int rodLength)
	//{
	//	// Create a hash table to store results of subproblems
	//	vector<int> maxRevenueHashTable(rodLength, -1);
	//	return getMaxRevenueRodCutting_DP_TopDown(maxRevenueHashTable, price, rodLength);
	//}

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

	//int getMinCutsStolenNecklaceKGems2People_DP_BottomUp(const vector<int>& price, const int rodLength)
	//{
	//	vector<int> maxRevenueHashTable(rodLength + 1);
	//	// initial condition:
	//	maxRevenueHashTable[0] = 0; // Rod of length 0 will earn 0 revenue
	//	for (int totalRodLen = 1; totalRodLen <= rodLength; ++totalRodLen)
	//	{
	//		for (int rodLen = 1; rodLen <= totalRodLen; ++rodLen)
	//		{
	//			int currentRevenue = price[rodLen - 1] + maxRevenueHashTable[totalRodLen - rodLen];
	//			if (maxRevenueHashTable[totalRodLen] < currentRevenue)
	//				maxRevenueHashTable[totalRodLen] = currentRevenue;
	//		}

	//	}

	//	return maxRevenueHashTable[rodLength];
	//}



	// ============================= Unit Tests =============================

	bool isFulfilled(const vector<unordered_map<GemType, Count>>& expectedDistribution)
	{
		for (size_t personIndex = 0; personIndex < expectedDistribution.size(); ++personIndex)
		{
			const unordered_map<GemType, Count>& currPerDist = expectedDistribution[personIndex];
			for (auto it = currPerDist.begin(); it != currPerDist.end(); ++it)
			{
				if (it->second != 0)
					return false;
			}
		}

		return true;
	}

	struct MinCutsStolenNecklaceTestData
	{
		int numPeople;
		int numGemTypes;
		int numGemsInNecklace;
		int numGemsToDistribute;
		vector<int> necklace;
		vector<unordered_map<GemType, Count>> expectedDistribution;
		vector<MinCutsStolenNecklaceResults> results;

		void createRandomNecklace()
		{
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_int_distribution<int> dist(0, numGemTypes - 1);
			necklace.reserve(numGemsInNecklace);
			for (int i = 0; i < numGemsInNecklace; ++i)
			{
				necklace.push_back(dist(mt));
			}
		}

		void createRandomExpectedDistribution(int numGemsToDistribute, int increaseCountBy)
		{
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_int_distribution<int> distPersonIndex(0, numPeople - 1);
			std::uniform_int_distribution<int> distSkipGems(0, 100);
			expectedDistribution.resize(numPeople);
			//distribute the gems randomly
			int gemsToSkip = numGemsInNecklace - numGemsToDistribute;
			for (int gemIndex = 0; gemIndex < numGemsInNecklace && numGemsToDistribute > 0; ++gemIndex)
			{
				if (gemsToSkip > 0)
				{
					if (distSkipGems(mt) % 2 == 0)
					{
						--gemsToSkip;
						continue;
					}
				}

				int personIndex = distPersonIndex(mt);
				unordered_map<GemType, Count>& currPerDist = expectedDistribution[personIndex];
				GemType type = necklace[gemIndex];
				Count& c = currPerDist[type];
				c += increaseCountBy;
				--numGemsToDistribute;
			}
		}

		bool isValidResult()
		{
			for (size_t resIndex = 0; resIndex < results.size(); ++resIndex)
			{
				vector<unordered_map<GemType, Count>> copy = expectedDistribution;
				for (size_t i = 0; i < results[resIndex].owners.size(); ++i)
				{
					GemType type = necklace[i];
					int ownerIndex = results[resIndex].owners[i];
					if (ownerIndex == -1)
						continue;
					unordered_map<GemType, Count>& currPerDist = copy[ownerIndex];
					Count& c = currPerDist[type];
					--c;
				}
				
				if (!isFulfilled(copy))
					return false;
			}

			return true;
		}

		void printResults()
		{
			cout << "\n\n";
			cout << "numPeople: " << numPeople;
			cout << "  numGemTypes: " << numGemTypes;
			cout << "  numGemsInNecklace: " << numGemsInNecklace;
			cout << "  numGemsToDistribute: " << numGemsToDistribute;

			constexpr const int colWidth = 8;
			cout << "\n" << "expectedDistribution:";
			cout << "\n" << setw(colWidth) << "GemTypes" << " : ";
			for (GemType type = 0; type < numGemTypes; ++type)
				cout << setw(colWidth) << type;
			cout << "\n" << setw(colWidth) << "PersonIndex" << " : ";
			for (size_t personIndex = 0; personIndex < expectedDistribution.size(); ++personIndex)
			{
				cout << "\n" << setw(colWidth) << personIndex << " : ";
				for (GemType type = 0; type < numGemTypes; ++type)
				{
					unordered_map<GemType, Count>& currPerDist = expectedDistribution[personIndex];
					Count& c = currPerDist[type];
					cout << setw(colWidth) << c;
				}
			}

			constexpr const int width = 3;
			cout << "\n" << "necklace: ";
			cout << "\n{";
			for (size_t i = 0; i < necklace.size(); ++i)
			{
				cout << setw(width) << necklace[i];
			}
			cout << setw(width) << "}";

			cout << "\n" << "Results: " << results.size() << " minCuts: " << (results.size() > 0 ? results[0].minCuts : -1) << "    owners: ";
			for (size_t i = 0; i < results.size(); ++i)
			{
				cout << "\n{";
				for (size_t j = 0; j < results[i].owners.size(); ++j)
				{
					cout << setw(width) << results[i].owners[j];
				}
				cout << setw(width) << "}";
			}
		}
	};

	MM_DECLARE_FLAG(DP_MinCutsStolenNecklaceKGems2People);

	MM_UNIT_TEST(DP_MinCutsStolenNecklaceKGems2People_test_1, DP_MinCutsStolenNecklaceKGems2People)
	{
		using TestDataShortName = MinCutsStolenNecklaceTestData;
		vector<TestDataShortName> testData{
			TestDataShortName{2, 1, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{2, 2, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{2, 3, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{2, 4, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{2, 5, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{3, 1, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{3, 2, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{3, 3, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{3, 4, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{3, 5, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{4, 1, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{4, 2, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{4, 3, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{4, 4, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{4, 5, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{5, 1, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{5, 2, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{5, 3, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{5, 4, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{5, 5, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},

			TestDataShortName{2, 1, 10, 5, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{2, 2, 10, 6, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{2, 3, 10, 7, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{2, 4, 10, 8, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{2, 5, 10, 9, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{3, 1, 10, 5, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{3, 2, 10, 6, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{3, 3, 10, 7, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{3, 4, 10, 8, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{3, 5, 10, 9, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{4, 1, 10, 5, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{4, 2, 10, 6, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{4, 3, 10, 7, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{4, 4, 10, 8, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{4, 5, 10, 9, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{5, 1, 10, 5, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{5, 2, 10, 6, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{5, 3, 10, 7, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{5, 4, 10, 8, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
			TestDataShortName{5, 5, 10, 9, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		};

		for (size_t i = 0; i < testData.size(); ++i)
		{
			testData[i].createRandomNecklace();
			int minCuts;
			bool result;

			//Create positive test cases
			testData[i].createRandomExpectedDistribution(testData[i].numGemsToDistribute, 1);
			minCuts = getMinCutsStolenNecklaceKGems2People_recursive(testData[i].numPeople, testData[i].necklace, testData[i].expectedDistribution, testData[i].results);
			result = testData[i].isValidResult();
			//MM_EXPECT_TRUE(result == true, result);
			testData[i].printResults();

			//Create negative test cases
			testData[i].expectedDistribution.clear();
			testData[i].results.clear();
			testData[i].createRandomExpectedDistribution(testData[i].numGemsToDistribute, 3);
			minCuts = getMinCutsStolenNecklaceKGems2People_recursive(testData[i].numPeople, testData[i].necklace, testData[i].expectedDistribution, testData[i].results);
			result = testData[i].isValidResult();
			//MM_EXPECT_TRUE(result == true, result);
			testData[i].printResults();
		}
	}
}

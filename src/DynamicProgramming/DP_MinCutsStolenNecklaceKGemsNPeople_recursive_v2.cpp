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

#include "DynamicProgramming/DP_MinCutsStolenNecklaceKGemsNPeople_recursive_v2.h"

namespace mm {

	namespace MinCutsStolenNecklaceKGemsNPeople_recursive_v2 {

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

		int getMinCutsStolenNecklaceKGemsNPeople(int numPeople, const vector<int>& necklace, int gemIndex, int totalGemsToDistribute,
			vector<unordered_map<GemType, Count>>& expectedDistribution, MinCutsStolenNecklaceResults& currentResults, vector<MinCutsStolenNecklaceResults>& results)
		{
			if (gemIndex < totalGemsToDistribute)
				return numeric_limits<int>::max();

			//If this result is good, store it
			if (totalGemsToDistribute == 0)
			{
				currentResults.calculateMinCuts();
				//Push to results if the current results are same or better
				if (!results.empty() && currentResults.minCuts < results.front().minCuts)
					results.clear(); //current result is better, so remove old results

				if (results.empty() || currentResults.minCuts == results.front().minCuts)
					results.push_back(currentResults);

				return currentResults.minCuts;
			}

			if (gemIndex == 0)
				return numeric_limits<int>::max();

			int minCutsSoFar = numeric_limits<int>::max();

			//skip the current gem and try
			int minCutsSkipCurrent = getMinCutsStolenNecklaceKGemsNPeople(numPeople, necklace, gemIndex - 1,
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

				int minCutsForSubinstance = getMinCutsStolenNecklaceKGemsNPeople(numPeople, necklace, gemIndex - 1,
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

		int getMinCutsStolenNecklaceKGemsNPeople(int numPeople, const vector<int>& necklace,
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

			int minCuts = getMinCutsStolenNecklaceKGemsNPeople(numPeople, necklace, static_cast<int>(necklace.size()),
				totalGemsToDistribute, expectedDistribution, currentResultsTemp, results);

			return minCuts == numeric_limits<int>::max() ? -1 : minCuts;
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

	}
}

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

#include "DynamicProgramming/DP_MinCutsStolenNecklaceKGemsNPeople_branchAndBounds_v1.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "DataStructures/DS_MM_Heap.h"

namespace mm {

	namespace MinCutsStolenNecklaceKGemsNPeople_branchAndBounds_v1 {

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

		struct StolenNecklaceDistributionState
		{
			int gemIndex;
			int gemsToDistribute;
			vector<unordered_map<GemType, Count>> expectedDistribution;
			MinCutsStolenNecklaceResults currentResults;
			int maxPossibleCuts;

			struct compare
			{
				bool operator()(const StolenNecklaceDistributionState& lhs, const StolenNecklaceDistributionState& rhs) const
				{
					return lhs.maxPossibleCuts > rhs.maxPossibleCuts; //build min heap
				}
			};
		};

		MM_Heap<StolenNecklaceDistributionState, StolenNecklaceDistributionState::compare> states;

		int getMinCutsStolenNecklaceKGemsNPeople_Greedy(int numPeople, const vector<GemType>& necklace, int gemIndex, int gemsToDistribute,
			vector<unordered_map<GemType, Count>> expectedDistribution, int lastGemOwner)
		{
			int minCutsGreedy = 0;
			for (; gemIndex > 0 && gemsToDistribute > 0; --gemIndex)
			{
				GemType type = necklace[gemIndex - 1];
				const int indexOffset = lastGemOwner != -1 ? lastGemOwner : 0;
				for (int k = 0; k < numPeople; ++k)
				{
					int personIndex = k + indexOffset;
					if (personIndex >= numPeople)
						personIndex -= numPeople; //start with lastGemOwner

					unordered_map<GemType, Count>& currPerDist = expectedDistribution[personIndex];
					Count& c = currPerDist[type];
					if (c != 0)
					{
						--c;
						--gemsToDistribute;
						if (personIndex != lastGemOwner && gemIndex < necklace.size())
							++minCutsGreedy;

						lastGemOwner = personIndex;
						break;
					}
				}
			}

			return minCutsGreedy;
		}

		int getMinCutsStolenNecklaceKGemsNPeople(int numPeople, const vector<GemType>& necklace, int gemIndex, int gemsToDistribute,
			vector<unordered_map<GemType, Count>>& expectedDistribution, const MinCutsStolenNecklaceResults& currentResults, 
			vector<MinCutsStolenNecklaceResults>& results, int maxSolutions)
		{
			if (gemIndex <= 0 || gemsToDistribute <= 0)
				return 0;

			StolenNecklaceDistributionState s{ gemIndex, gemsToDistribute, expectedDistribution, currentResults, numeric_limits<int>::max() };
			s.maxPossibleCuts = s.currentResults.minCuts + 
				MinCutsStolenNecklaceKGemsNPeople_branchAndBounds_v1::getMinCutsStolenNecklaceKGemsNPeople_Greedy(numPeople, necklace, gemIndex, gemsToDistribute, expectedDistribution, -1);
			states.push(std::move(s));

			//int minCutsSoFar = numeric_limits<int>::max();
			while (!states.empty())
			{
				if (MM_Measure<int>::getInstance().isTimedOut())
					return results.empty() ? numeric_limits<int>::max() : results.front().minCuts;

				if (results.size() >= maxSolutions)
					return results.front().minCuts;

				StolenNecklaceDistributionState curr = states.top();
				states.pop();

				int lastGemOwner = curr.gemIndex < necklace.size() ? curr.currentResults.owners[curr.gemIndex] : -1;

				//skip the current gem and try if there are any excess gems
				if (curr.gemIndex > curr.gemsToDistribute)
				{
					//if (gemIndex < necklace.size() && currentResults.owners[gemIndex] != -1) //if last gem is not skipped
					//	++currentResults.minCuts;
					//minCutsSoFar = getMinCutsStolenNecklaceKGemsNPeople(numPeople, necklace, gemIndex - 1,
					//	gemsToDistribute, expectedDistribution, currentResults, results, maxSolutions);
					StolenNecklaceDistributionState s = curr;
					--s.gemIndex;
					if (curr.gemIndex < necklace.size() && curr.currentResults.owners[curr.gemIndex] != -1) //if last gem is not skipped
						++s.currentResults.minCuts;

					bool pushToStates = true;
					if (!results.empty() && results.front().minCuts < s.currentResults.minCuts)
						pushToStates = false;

					if (s.gemIndex < s.gemsToDistribute)
						pushToStates = false;

					if (s.gemIndex == 0)
						pushToStates = false;

					if (pushToStates)
					{
						s.maxPossibleCuts = s.currentResults.minCuts + 
							MinCutsStolenNecklaceKGemsNPeople_branchAndBounds_v1::getMinCutsStolenNecklaceKGemsNPeople_Greedy(numPeople, necklace, s.gemIndex, s.gemsToDistribute, s.expectedDistribution, lastGemOwner);
						states.push(std::move(s));
					}

					//if (gemIndex < necklace.size() && currentResults.owners[gemIndex] != -1) //if last gem is not skipped
					//	--currentResults.minCuts;
				}

				GemType type = necklace[curr.gemIndex - 1];
				//int lastGemOwner = (gemIndex < necklace.size()) ? currentResults.owners[gemIndex] : 0;
				//int skipPersonIndex = -1;
				for (int personIndex = 0; personIndex < numPeople; ++personIndex)
				{
					//int personIndex = i + lastGemOwner;
					//if (personIndex >= numPeople)
					//	personIndex -= numPeople; //start with lastGemOwner

					unordered_map<GemType, Count>& currPerDist = curr.expectedDistribution[personIndex];
					Count& c = currPerDist[type];
					if (c == 0)
						continue;

					StolenNecklaceDistributionState s = curr;
					--s.gemIndex;
					--s.gemsToDistribute;
					s.currentResults.owners[s.gemIndex] = personIndex;
					if (curr.gemIndex < necklace.size() && curr.currentResults.owners[curr.gemIndex] != personIndex)
						++s.currentResults.minCuts;
					unordered_map<GemType, Count>& sPerDist = s.expectedDistribution[personIndex];
					Count& sc = sPerDist[type];
					--sc;

					//If this result is good, store it
					if (s.gemsToDistribute == 0)
					{
						if (s.gemIndex != 0 && s.gemIndex < necklace.size() && s.currentResults.owners[s.gemIndex] != -1) //some excess gems are still remaining in necklace, we need to throw them away
							++s.currentResults.minCuts;

						//currentResults.calculateMinCuts();
						//Push to results if the current results are same or better
						if (!results.empty() && s.currentResults.minCuts < results.front().minCuts)
							results.clear(); //current result is better, so remove old results

						if (results.empty() || s.currentResults.minCuts == results.front().minCuts)
							results.push_back(s.currentResults);

						//int retVal = currentResults.minCuts;
						//if (gemIndex != 0 && gemIndex < necklace.size() && currentResults.owners[gemIndex] != -1)
						//	--currentResults.minCuts;

						//return retVal;
					}
					else
					{
						bool pushToStates = true;
						if (!results.empty() && results.front().minCuts < s.currentResults.minCuts)
							pushToStates = false;

						if (s.gemIndex < s.gemsToDistribute)
							pushToStates = false;

						if (s.gemIndex == 0)
							pushToStates = false;

						if (pushToStates)
						{
							s.maxPossibleCuts = s.currentResults.minCuts + 
								MinCutsStolenNecklaceKGemsNPeople_branchAndBounds_v1::getMinCutsStolenNecklaceKGemsNPeople_Greedy(numPeople, necklace, s.gemIndex, s.gemsToDistribute, s.expectedDistribution, lastGemOwner);
							states.push(std::move(s));
						}
					}
					//either backtrack i.e. reduce it here and increase after recursive call 
					//OR do not change local value and pass a new value to recursive call
					//--gemsToDistribute; 

					//int minCutsIncludeCurrent = getMinCutsStolenNecklaceKGemsNPeople(numPeople, necklace, gemIndex - 1,
					//	gemsToDistribute - 1, expectedDistribution, currentResults, results, maxSolutions);

					//if (minCutsSoFar > minCutsIncludeCurrent)
					//	minCutsSoFar = minCutsIncludeCurrent;

					//backtrack
					//currentResults.owners[gemIndex - 1] = -1;
					//if (gemIndex < necklace.size() && currentResults.owners[gemIndex] != personIndex)
					//	--currentResults.minCuts;
					//++c;
					//++gemsToDistribute
				}
			}

			return results.empty() ? numeric_limits<int>::max() : results.front().minCuts;
		}

		int getMinCutsStolenNecklaceKGemsNPeople(int numPeople, const vector<GemType>& necklace,
			const vector<unordered_map<GemType, Count>>& expectedDistribution, vector<MinCutsStolenNecklaceResults>& results)
		{
			MinCutsStolenNecklaceResults currentResultsTemp;
			currentResultsTemp.owners.resize(necklace.size(), -1);
			currentResultsTemp.minCuts = 0;
			int gemsToDistribute = 0;
			for (size_t personIndex = 0; personIndex < expectedDistribution.size(); ++personIndex)
			{
				const unordered_map<GemType, Count>& currPerDist = expectedDistribution[personIndex];
				for (auto it = currPerDist.begin(); it != currPerDist.end(); ++it)
					gemsToDistribute += it->second;
			}

			auto expectedDistributionCopy = expectedDistribution;
			int maxSolutions = necklace.size() > gemsToDistribute ? 1 : numeric_limits<int>::max();
			int minCuts = getMinCutsStolenNecklaceKGemsNPeople(numPeople, necklace, static_cast<int>(necklace.size()),
				gemsToDistribute, expectedDistributionCopy, currentResultsTemp, results, maxSolutions);

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

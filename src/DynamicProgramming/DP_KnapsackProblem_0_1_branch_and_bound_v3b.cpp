//=======================================================================================================//
//   Copyright (c) 2018 Maruti Mhetre                                                                    //
//   All rights reserved.                                                                                //
//=======================================================================================================//
//   Redistribution and use of this software in source and binary forms, with or without modification,   //
//   are permitted for personal, educational or non-commercial purposes provided that the following      //
//   conditions are met:                                                                                 //
//   1. Redistributions of source code must retain the above copyright notice, this list of conditions   //
//      and the following disclaimer.                                                                    //
//   2. Redistributions in binary form must reproduce the above copyright notice, this list of           //
//      conditions and the following disclaimer in the documentation and/or other materials provided     //
//      with the distribution.                                                                           //
//   3. Neither the name of the copyright holder nor the names of its contributors may be used to        //
//      endorse or promote products derived from this software without specific prior written            //
//      permission.                                                                                      //
//=======================================================================================================//
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR      //
//   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND    //
//   FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR          //
//   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL   //
//   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   //
//   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER  //
//   IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT   //
//   OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                     //
//=======================================================================================================//

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <locale> // For printing number as thousand separated string
#include <random>
#include <functional> //std::bind
#include <map>
#include <queue>
#include <deque>
#include <memory>
#include <numeric> //For std::iota to generate the sequence of numbers
using namespace std;

//#include "Utils/Utils_MM_Assert.h"
#include "Utils/Utils_PrintUtils.h"
//#include "DynamicProgramming/DP_Common.h"
#include "DynamicProgramming/DP_KnapsackProblem_0_1.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Utils/Utils_MM_Assert.h"

namespace mm {

	// ============================= Problem Statement: =============================
	/*
	Given a set of items, each with a weight and a value, determine the number of each item to include in a collection
	so that the total weight is less than or equal to a given limit and the total value is as large as possible.
	In short:
	maximize = sumof(ViXi)
	subject to sumof(WiXi) <= W
				Xi = 0 or 1 ...For 0-1 knapsack problem
	Vi = value of ith item
	Wi = weight of ith item
	Xi = number of items

	In case of bounded knapsack problem, 0 < Xi < Ci where Ci = number of ith items available
	In case of unbounded knapsack problem, 0 < Xi where there is no upper bound on number of ith items available
	*/

	// ============================= Solution using branch and bound approach v3b =============================
	/*
	The solution using branch and bound approach. It does not depend upon capacity.
	Optimization over last version v3a:
	Instead of starting with maxValue = 0, start with maxValue = exact greedy max value.
	n = number of objects
	Time Complexity  :  worst case: O(2^n)
	Space Complexity :  O(2^n * 4) //heap object size is 4 and max heap size can be 2^n
	+ O(n)     //to store sortedIndices
	+ O(n)     //to store valueToWeightsRatio used for sorting and then it is reused to store cumulativeValues
	+ O(n)     //to store cumulativeWeights
	+ O(n)     //to store values
	+ O(n)     //to store weights
	The recursive call stack : NOT APPLICABLE
	*/
	struct decisionTreeNode_v3b
	{
		mutable unsigned int level;
		mutable double maxValue;
		mutable unsigned long long totalWeight; //or it can be called as knapsack capacity till now
		double upperbound;

		void calculateAndSetUpperBound(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity,
			const vector<unsigned int>& sortedIndices, const vector<unsigned long long>& cumulativeWeights, const vector<double>& cumulativeValues)
		{
			upperbound = maxValue;
			if (level == values.size()) // no more items left
				return;

			unsigned long long cumulativeWeightTillNow = 0;
			double cumulativeValueTillNow = 0;
			if (level > 0)
			{
				cumulativeWeightTillNow = cumulativeWeights[sortedIndices[level - 1]];
				cumulativeValueTillNow = cumulativeValues[sortedIndices[level - 1]];
			}

			unsigned long long remainingKnapsackCapacity = knapsackCapacity - totalWeight;
			if (remainingKnapsackCapacity <= 0)
				return;

			unsigned long long targetCumulativeWeight = cumulativeWeightTillNow + remainingKnapsackCapacity;

			vector<unsigned int>::const_iterator iter = std::upper_bound(sortedIndices.begin() + level, sortedIndices.begin() + values.size(), targetCumulativeWeight,
				[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned int indexIn) { return targetCumulativeWeightIn < cumulativeWeights[indexIn]; });

			unsigned long long currentKnapsackCapacity = totalWeight;
			upperbound = maxValue;
			if (iter != sortedIndices.begin() + level)
			{
				vector<unsigned int>::const_iterator iterPrev = iter - 1;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
				currentKnapsackCapacity += (cumulativeWeights[*iterPrev] - cumulativeWeightTillNow);
				upperbound += (cumulativeValues[*iterPrev] - cumulativeValueTillNow);
			}

			if (iter != sortedIndices.end() && currentKnapsackCapacity < knapsackCapacity)
			{
				unsigned long long remainingCapacity = knapsackCapacity - currentKnapsackCapacity;
				upperbound += (remainingCapacity * values[*iter] / weights[*iter]);
			}
		}

		friend bool operator<(const decisionTreeNode_v3b& lhs, const decisionTreeNode_v3b& rhs)
		{
			return lhs.upperbound < rhs.upperbound;
		}
	};

	std::priority_queue<decisionTreeNode_v3b, vector<decisionTreeNode_v3b>, std::less<decisionTreeNode_v3b>> maxHeap_v3b;

	double DP_KnapsackProblem_0_1_branch_and_bound_v3b(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights)
	{
		unsigned long long smallestWeight = weights[0];
		//calculate value to weight ratio
		for (int i = 0; i < values.size(); ++i)
		{
			valueToWeightsRatio[i] = values[i] / weights[i];
			if (smallestWeight > weights[i])
				smallestWeight = weights[i];
		}

		//initialize the indices
		for (int i = 0; i < values.size(); ++i)
			sortedIndices[i] = i;

		std::sort(sortedIndices.begin(), sortedIndices.begin() + values.size(),
			[&valueToWeightsRatio](const unsigned int lhs, const unsigned int rhs) -> bool { return valueToWeightsRatio[lhs] > valueToWeightsRatio[rhs]; });

		//Store cumulative weights
		//unsigned long long sumOfWeightsTillNow = 0;
		//for (int i = 0; i < values.size(); ++i)
		//{
		//	sumOfWeightsTillNow += weights[ sortedIndices[i] ];
		//	cumulativeWeights[ sortedIndices[i] ] = sumOfWeightsTillNow;
		//}

		cumulativeWeights[sortedIndices[0]] = weights[sortedIndices[0]];
		for (int i = 1; i < values.size(); ++i)
			cumulativeWeights[sortedIndices[i]] = cumulativeWeights[sortedIndices[i - 1]] + weights[sortedIndices[i]];

		//Store cumulative values
		//We do not need valueToWeightsRatio now, store cumulative values in it
		vector<double>& cumulativeValues = valueToWeightsRatio;
		cumulativeValues[sortedIndices[0]] = values[sortedIndices[0]];
		for (int i = 1; i < values.size(); ++i)
			cumulativeValues[sortedIndices[i]] = cumulativeValues[sortedIndices[i - 1]] + values[sortedIndices[i]];

		decisionTreeNode_v3b prev;
		prev.level = 0;
		prev.maxValue = 0.0;
		prev.totalWeight = 0;
		//prev.upperbound = 0;
		prev.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
		maxHeap_v3b.push(prev);

		double maxValue = 0.0;
		vector<unsigned int>::iterator iter = std::upper_bound(sortedIndices.begin(), sortedIndices.end(), knapsackCapacity,
			[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned int indexIn) { return targetCumulativeWeightIn < cumulativeWeights[indexIn]; });
		if (iter != sortedIndices.begin())
		{
			--iter;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
			maxValue = cumulativeValues[*iter];
			unsigned long long remainingKnapsackCapacity = knapsackCapacity - cumulativeWeights[*iter];
			for (++iter; iter != sortedIndices.end() && remainingKnapsackCapacity >= smallestWeight; ++iter)
			{
				if (remainingKnapsackCapacity >= weights[*iter])
				{
					maxValue += values[*iter];
					remainingKnapsackCapacity -= weights[*iter];
				}
			}
		}

		while (!maxHeap_v3b.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < maxHeap_v3b.size())
				sizeOfHeap = maxHeap_v3b.size();

			const decisionTreeNode_v3b& top = maxHeap_v3b.top();

			if (top.upperbound <= maxValue)
				break;

			unsigned int itemIndex = sortedIndices[top.level];

			top.level += 1;

			prev = top;
			//include this item
			unsigned long long totalWeight = top.totalWeight + weights[itemIndex];
			if (totalWeight <= knapsackCapacity)
			{
				top.totalWeight = totalWeight;
				top.maxValue += values[itemIndex];
				//No need to recalculate upper bound, it will be same as current item was considered before as well while calculating upper bound.
				//current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
				//current.upperbound = prev.upperbound;
				if (top.maxValue > maxValue)
					maxValue = top.maxValue;

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				//if (top.upperbound > maxValue)
				//	maxHeap_v3b.push(current);
				if (top.upperbound <= maxValue)
					break;

				if (top.level == values.size())
					maxHeap_v3b.pop();
			}
			else
				maxHeap_v3b.pop();

			//exclude this item
			prev.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (top.level < values.size() && prev.upperbound > maxValue)
				maxHeap_v3b.push(prev);
		}

		while (!maxHeap_v3b.empty())
			maxHeap_v3b.pop();

		return maxValue;
	}
}

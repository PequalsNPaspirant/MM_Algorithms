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

	// ============================= Solution using branch and bound approach v7 =============================
	/*
	The solution using branch and bound approach. It does not depend upon capacity.
	Optimization over last version v6:
	Instead of std::queue use mm::MM_Queue.
	n = number of objects
	Time Complexity  :  worst case: O(2^n)
	Space Complexity :  O(2^n * 1)		//size of unique_ptr is 1 and max heap size can be 2^n
	+ O(2^n * 4)	//growing memory pool - the heap object size is 4
	+ O(n)			//to store next available index of pool
	+ O(n)			//to store sortedIndices
	+ O(n)			//to store valueToWeightsRatio used for sorting and then it is reused to store cumulativeValues
	+ O(n)			//to store cumulativeWeights
	+ O(n)			//to store values
	+ O(n)			//to store weights
	The recursive call stack : NOT APPLICABLE
	*/

	struct Comparator_v7
	{
		bool operator()(const decisionTreeNode_v7* lhs, const decisionTreeNode_v7* rhs)
		{
			return lhs->upperbound < rhs->upperbound;
		}
	};

	std::priority_queue<decisionTreeNode_v7*, vector<decisionTreeNode_v7*>, Comparator_v7> maxHeap_v7;

	double DP_KnapsackProblem_0_1_branch_and_bound_v7(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& valueToWeightsRatio, vector<unsigned int>& sortedIndices, vector<unsigned long long>& cumulativeWeights,
		vector<vector<decisionTreeNode_v7>>& heapObjectsGrowingPool, MM_Queue<decisionTreeNode_v7*>& poolIndices)
	{
		//calculate value to weight ratio
		for (int i = 0; i < values.size(); ++i)
			valueToWeightsRatio[i] = values[i] / weights[i];

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

		decisionTreeNode_v7* pObj = poolIndices.front();
		poolIndices.pop();
		pObj->level = 0;
		pObj->maxValue = 0.0;
		pObj->totalWeight = 0;
		//prev.upperbound = 0;
		pObj->calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
		maxHeap_v7.push(pObj);

		double maxValue = 0.0; // set max value by greedy

		while (!maxHeap_v7.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < maxHeap_v7.size())
				sizeOfHeap = maxHeap_v7.size();

			decisionTreeNode_v7* top = maxHeap_v7.top();

			if (top->upperbound <= maxValue)
				break;

			unsigned int itemIndex = sortedIndices[top->level];

			top->level += 1;

			decisionTreeNode_v7* prev = top;
			//unique_ptr<decisionTreeNode_v7> prev = make_unique<decisionTreeNode_v7>(*top);
			//prev = top;
			//include this item
			unsigned long long totalWeight = top->totalWeight + weights[itemIndex];
			if (totalWeight <= knapsackCapacity)
			{
				if (poolIndices.empty()) //need to grow pool
				{
					heapObjectsGrowingPool.push_back(vector<decisionTreeNode_v7>(values.size()));
					size_t lastIndex = heapObjectsGrowingPool.size() - 1;
					for (int i = 0; i < values.size(); ++i)
						poolIndices.push(&heapObjectsGrowingPool[lastIndex][i]);
				}

				prev = poolIndices.front();
				poolIndices.pop();
				*prev = *top;

				top->totalWeight = totalWeight;
				top->maxValue += values[itemIndex];
				//No need to recalculate upper bound, it will be same as current item was considered before as well while calculating upper bound.
				//current.calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
				//current.upperbound = prev.upperbound;
				if (top->maxValue > maxValue)
					maxValue = top->maxValue;

				// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
				//if (top.upperbound > maxValue)
				//	maxHeap_v3.push(current);
				if (top->upperbound <= maxValue)
					break;

				if (top->level == values.size())
					maxHeap_v7.pop();
			}
			else
				maxHeap_v7.pop();

			//exclude this item
			prev->calculateAndSetUpperBound(values, weights, knapsackCapacity, sortedIndices, cumulativeWeights, cumulativeValues);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (prev->level < values.size() && prev->upperbound > maxValue)
				maxHeap_v7.push(prev);
			else
				poolIndices.push(prev);
		}

		while (!maxHeap_v7.empty())
			maxHeap_v7.pop();

		return maxValue;
	}

	
}

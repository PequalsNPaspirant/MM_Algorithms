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

	// ============================= Solution using branch and bound approach v9b =============================
	/*
	The solution using branch and bound approach. It does not depend upon capacity.
	Optimization over last version v8b:
	Use in place sorting and discard sortedIndices
	n = number of objects
	Time Complexity  :  worst case: O(2^n)
	Space Complexity :  O(2^n * 1)		//size of unique_ptr is 1 and max heap size can be 2^n
	+ O(2^n * 4)	//growing memory pool - the heap object size is 4
	+ O(n)			//to store valueToWeightsRatio used for sorting and then it is reused to store cumulativeValues
	+ O(n)			//to store cumulativeWeights and also to store sortedIndices
	+ O(n)			//to store input values
	+ O(n)			//to store input weights
	The recursive call stack : NOT APPLICABLE
	*/

	double DP_KnapsackProblem_0_1_branch_and_bound_v9b(vector<double>& values, vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems,
		vector<double>& valueToWeightsRatio, vector<unsigned long long>& cumulativeWeights,
		MM_Heap<decisionTreeNode_v9b*, Comparator_v9b>& maxHeap_v9b, vector<vector<decisionTreeNode_v9b>>& heapObjectsGrowingPool)
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
		vector<unsigned long long>&sortedIndices = cumulativeWeights;
		std::iota(sortedIndices.begin(), sortedIndices.end(), 0);
		//for (int i = 0; i < values.size(); ++i)
		//	sortedIndices[i] = i;

		std::sort(sortedIndices.begin(), sortedIndices.begin() + values.size(),
			[&valueToWeightsRatio](const unsigned int lhs, const unsigned int rhs) -> bool { return valueToWeightsRatio[lhs] > valueToWeightsRatio[rhs]; });

		//Testing
		//vector<double> oldValues = values;
		//vector<unsigned long long> oldWeights = weights;
		//vector<unsigned long long> oldSortedIndices = sortedIndices;

		//Apply sortedIndices to weights and values
		for (unsigned int i = 0; i < sortedIndices.size(); ++i)
		{
			unsigned long long curr_i = i;
			unsigned long long sourceIndex = sortedIndices[curr_i];

			if (curr_i == sourceIndex || sourceIndex == numeric_limits<unsigned long long>::max())
				continue;

			double value = values[curr_i]; //back up
			unsigned long long weight = weights[curr_i]; //back up
			sortedIndices[curr_i] = numeric_limits<unsigned long long>::max();
			while (sortedIndices[sourceIndex] != numeric_limits<unsigned long long>::max()) //value at sourceIndex is NOT yet moved to its right sorted position
			{
				values[curr_i] = values[sourceIndex];
				weights[curr_i] = weights[sourceIndex];
				unsigned long long oldSourceIndex = sourceIndex;
				curr_i = sourceIndex;
				sourceIndex = sortedIndices[curr_i];
				sortedIndices[oldSourceIndex] = numeric_limits<unsigned long long>::max(); //value at sourceIndex is NOW moved to its right sorted position
			}
			values[curr_i] = value;
			weights[curr_i] = weight;
		}

		//Testing
		//for (int i = 0; i < values.size(); ++i)
		//{
		//	MyAssert::myRunTimeAssert(values[i] == oldValues[ oldSortedIndices[i] ]);
		//	MyAssert::myRunTimeAssert(weights[i] == oldWeights[ oldSortedIndices[i] ]);
		//}

		//Store cumulative weights
		cumulativeWeights[0] = weights[0];
		for (int i = 1; i < weights.size(); ++i)
			cumulativeWeights[i] = cumulativeWeights[i - 1] + weights[i];

		//Store cumulative values
		//We do not need valueToWeightsRatio now, store cumulative values in it
		vector<double>& cumulativeValues = valueToWeightsRatio;
		cumulativeValues[0] = values[0];
		for (int i = 1; i < values.size(); ++i)
			cumulativeValues[i] = cumulativeValues[i - 1] + values[i];

		MyAssert::myRunTimeAssert(maxHeap_v9b.capacity() > maxHeap_v9b.size());
		decisionTreeNode_v9b* pObj = maxHeap_v9b.getNextAvailableElement();
		pObj->level = 0;
		pObj->maxValue = 0.0;
		pObj->totalWeight = 0;
		//prev.upperbound = 0;
		pObj->calculateAndSetUpperBound(values, weights, knapsackCapacity, cumulativeWeights, cumulativeValues);
		maxHeap_v9b.push(pObj);

		double maxValue = 0.0; // set max value by greedy
		vector<unsigned long long>::iterator iter = std::upper_bound(cumulativeWeights.begin(), cumulativeWeights.end(), knapsackCapacity);
		//[&cumulativeWeights](unsigned long long targetCumulativeWeightIn, unsigned long long currentCumulativeWeight) { return targetCumulativeWeightIn < currentCumulativeWeight; });
		unsigned int index = iter - cumulativeWeights.begin();
		if (index != 0)
		{
			--index;  //iter does not point to sortedIndices.begin(), so it is safe to go one step towards left
			maxValue = cumulativeValues[index];
			unsigned long long remainingKnapsackCapacity = knapsackCapacity - cumulativeWeights[index];
			for (++index; index < values.size() && remainingKnapsackCapacity >= smallestWeight; ++index)
			{
				if (remainingKnapsackCapacity >= weights[index])
				{
					maxValue += values[index];
					remainingKnapsackCapacity -= weights[index];
				}
			}
		}

		while (!maxHeap_v9b.empty())
		{
			++numberOfFunctionCalls;
			if (sizeOfHeap < maxHeap_v9b.size())
				sizeOfHeap = maxHeap_v9b.size();

			decisionTreeNode_v9b* top = maxHeap_v9b.top();

			if (top->upperbound <= maxValue)
				break;

			unsigned int itemIndex = top->level;

			top->level += 1;

			decisionTreeNode_v9b* prev = top;
			//unique_ptr<decisionTreeNode_v6> prev = make_unique<decisionTreeNode_v6>(*top);
			//prev = top;
			//include this item
			unsigned long long totalWeight = top->totalWeight + weights[itemIndex];
			if (totalWeight <= knapsackCapacity)
			{
				if (maxHeap_v9b.capacity() == maxHeap_v9b.size()) //need to grow pool
				{
					heapObjectsGrowingPool.push_back(vector<decisionTreeNode_v9b>(values.size()));
					maxHeap_v9b.reserve(maxHeap_v9b.capacity() + values.size());
					int lastIndex = heapObjectsGrowingPool.size() - 1;
					for (int i = 0; i < values.size(); ++i)
						maxHeap_v9b.addToData(&heapObjectsGrowingPool[lastIndex][i]);
				}

				prev = maxHeap_v9b.getNextAvailableElement();
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
					maxHeap_v9b.pop();
			}
			else
				maxHeap_v9b.pop();

			//exclude this item
			prev->calculateAndSetUpperBound(values, weights, knapsackCapacity, cumulativeWeights, cumulativeValues);
			// maxValue is kind of lower bound so far, so avoid the decision tree nodes having upper bound less than maxValue
			if (prev->level < values.size() && prev->upperbound > maxValue)
				maxHeap_v9b.push(prev);
		}

		maxHeap_v9b.clear();

		return maxValue;
	}
	
}

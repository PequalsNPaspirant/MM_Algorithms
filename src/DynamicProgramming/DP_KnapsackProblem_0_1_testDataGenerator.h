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

#pragma once

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
using namespace std;

//#include "Utils/Utils_MM_Assert.h"
#include "Utils/Utils_PrintUtils.h"
//#include "DynamicProgramming/DP_Common.h"
#include "DynamicProgramming/DP_KnapsackProblem_0_1.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Utils/Utils_MM_Assert.h"

namespace mm {

	struct Kanpsack_0_1_testCaseType
	{
		static const string sanityTestCases;
		static const string performanceTestCases_1K;
		static const string performanceTestCases_10K;
		static const string performanceTestCases_100K;
		static const string performanceTestCases_1M;
		static const string performanceTestCases_10M;
		static const string performanceTestCases_100M;
		//static const string highPerformanceTestCases;
		//static const string crazyPerformanceTestCases;
		//static const string superCrazyPerformanceTestCases;
	};

	struct Kanpsack_0_1_tags
	{
		static const string result_exact;
		static const string result_greedy;
		static const string knapsack_capacity;
		static const string num_items;
		static const string value_weight_pairs;
	};

	struct knapsack_0_1_Results
	{
		KnapsackProblemAlgo algoType;
		string skipReason;
		double actualMaxValue;
		unsigned long long time;
		unsigned long long space;
		unsigned long long numFunctionCalls;
		unsigned long long sizeOfHeap;
	};

	struct Kanpsack_0_1_DataSet
	{
		Kanpsack_0_1_DataSet(const vector<double>& valuesIn = {}, 
			const vector<unsigned long long>& weightsIn = {}, 
			unsigned long long knapsackCapacityIn = 0,
			vector<unsigned int> expectedSelectedItemsIn = {},
			double expectedMaxValueIn = 0.0,
			double expectedMaxValueByGreedyIn = 0.0)
			: values(valuesIn),
			weights(weightsIn),
			knapsackCapacity(knapsackCapacityIn),
			expectedSelectedItems(expectedSelectedItemsIn),
			expectedMaxValue(expectedMaxValueIn),
			expectedMaxValueByGreedy(expectedMaxValueByGreedyIn),
			results(int(KnapsackProblemAlgo::max_approaches))
		{}

		string testFileName;
		vector<double> values;
		vector<unsigned long long> weights;
		unsigned long long knapsackCapacity;
		vector<unsigned int> expectedSelectedItems; //stored indices of selected items
		double expectedMaxValue;
		double expectedMaxValueByGreedy;

		vector<knapsack_0_1_Results> results;
	};

	class DP_KnapsackProblem_0_1_testDataGenerator
	{
	public:
		static void generateAndWriteTestCasesToFile();
		static void writeTestCaseToFile(const Kanpsack_0_1_DataSet& testData, const string& testCaseName);
		static vector<Kanpsack_0_1_DataSet> readAllTestCasesFromFile(const string& testCaseNamePrefix);
		static Kanpsack_0_1_DataSet readOneTestCaseFromFile(const string& testCaseFullFileName);

		static vector<Kanpsack_0_1_DataSet> getHardecodedSanityTestCases();

		static void generateRandomTestCases(
			unsigned long long minNumObjects, unsigned long long maxNumObjects, int increment, int numTestCasesOfEachObjectSize,
			double minValue, double maxValue,
			unsigned long long minWeight, unsigned long long maxWeight,
			int minKnapsackCapacityPercentage, int maxKnapsackCapacityPercentage,
			bool findSolution, bool greedyEqualsExact, const string& testCaseNamePrefix);

		static const string testCaseDirectory;

	private:
		
	};

}
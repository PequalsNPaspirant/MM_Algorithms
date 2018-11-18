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
#include <fstream> //for file read/write
#include <sstream> // for std::stringstream
#include <algorithm>
#include <queue>
using namespace std;

//#include "DynamicProgramming/DP_Common.h"
#include "DynamicProgramming/DP_KnapsackProblem_0_1.h"
#include "DynamicProgramming/DP_KnapsackProblem_0_1_testDataGenerator.h"
//#include "Assert/MyAssert.h"
#include "Others/PrintUtils.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Assert/MyAssert.h"
#include "Timer/Timer.h"

namespace mm {

	// MaxNumberOfItems * MaxKnapsackCapacity = 10 million // 
	//unsigned int MaxNumberOfItems = 1000;
	
	
	 // 100 million
	unsigned long long max_nW = 1000000000; // 1 billion - dynamic programming takes 

	unsigned long long knapsack_duration = 0;
	unsigned long long knapsack_spaceComplexity = 0;
	unsigned long long numberOfFunctionCalls = 0;
	unsigned long long sizeOfHeap = 0;

	
	const unsigned long long one_thousand = 1000;
	const unsigned long long ten_thousand = 10 * one_thousand;
	const unsigned long long hundred_thousand = 100 * one_thousand;
	const unsigned long long one_million = 1000000; // 1K * 1K * 8 bytes = 8 MB
	const unsigned long long ten_million = 10 * one_million;  // 10 * 1K * 1K * 8 bytes = 80 MB
	const unsigned long long hundred_million = 100 * one_million; // 100 * 1K * 1K * 8 bytes = 800 MB
	const unsigned long long one_billion = one_thousand * one_million; // 1K * 1K * 1K * 8 bytes = 8 GB
	const unsigned long long ten_billion = 10 * one_billion; // 10 * 1K * 1K * 1K * 8 bytes = 80 GB
	const unsigned long long maxULL = numeric_limits<unsigned long long>::max();

	vector<KnapsackProblemControlStation> controlStationData{
//{implemented?,maxValues,			maxKnapsackCapacity,	max_nW,			mask,																							algoName														}
{true,			30,					maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::naive_v1),														"naive_v1"														}, // Takes 6 seconds for 30 values
{true,			31,					maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::naive_v2),														"naive_v2"														}, // Takes 6 seconds for 31 values
{false,			maxULL,				maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::naive_tail_recursive),											"naive_tail_recursive" 											},
{true,			30,					maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::naive_non_recursive_v1),											"naive_non_recursive_v1" 										}, // Takes 12 seconds for 30 values
{true,			30,					maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::naive_non_recursive_v2),											"naive_non_recursive_v2" 										}, // Takes 6 seconds for 30 values
{true,			30,					maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::naive_non_recursive_v3),											"naive_non_recursive_v3" 										}, // Takes 5 seconds for 30 values
{true,			maxULL,				maxULL,					ten_million,	1 << int(KnapsackProblemAlgo::top_down_using_vector),											"top_down_using_vector" 										},
{true,			maxULL,				maxULL,					ten_million,	1 << int(KnapsackProblemAlgo::top_down_using_hashmap),											"top_down_using_hashmap" 										},
{false,			maxULL,				maxULL,					ten_million,	1 << int(KnapsackProblemAlgo::top_down_using_map_tail_recursive),								"top_down_using_map_tail_recursive" 							},
{true,			maxULL,				maxULL,					ten_million,	1 << int(KnapsackProblemAlgo::top_down_using_map_non_recursive),								"top_down_using_map_non_recursive" 								},
{true,			maxULL,				maxULL,					hundred_million,1 << int(KnapsackProblemAlgo::bottom_up_v1),													"bottom_up_v1" 													},
{true,			maxULL,				maxULL,					hundred_million,1 << int(KnapsackProblemAlgo::bottom_up_v2),													"bottom_up_v2" 													},
{true,			maxULL,				ten_million,			ten_billion,	1 << int(KnapsackProblemAlgo::bottom_up_object_count_independent_space_efficient_v1),			"bottom_up_object_count_independent_space_efficient_v1" 		},
{true,			maxULL,				hundred_million,		ten_billion,	1 << int(KnapsackProblemAlgo::bottom_up_object_count_independent_space_efficient_v2),			"bottom_up_object_count_independent_space_efficient_v2" 		},
{true,			maxULL,				hundred_million,		ten_billion,	1 << int(KnapsackProblemAlgo::bottom_up_object_count_independent_space_efficient_v3),			"bottom_up_object_count_independent_space_efficient_v3" 		},
{false,			maxULL,				maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::bottom_up_object_count_independent_space_efficient_using_set),	"bottom_up_object_count_independent_space_efficient_using_set"	},
{false,			maxULL,				maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::bottom_up_capacity_independent_space_efficient),					"bottom_up_capacity_independent_space_efficient" 				},
{false,			maxULL,				maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::bottom_up_capacity_independent), 									"bottom_up_capacity_independent" 								},
{true,			ten_thousand,		maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::branch_and_bound_naive_v1),										"branch_and_bound_naive_v1" 									},
{true,			ten_million,		maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::branch_and_bound_cumulative_sum_v2),								"branch_and_bound_cumulative_sum_v2" 							},
{true,			ten_million,		maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::branch_and_bound_no_recalc_upperbound_v3),						"branch_and_bound_no_recalc_upperbound_v3" 						},
{true,			ten_million,		maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::branch_and_bound_use_greedylike_maxValue_v3a),					"branch_and_bound_use_greedylike_maxValue_v3a" 					},
{true,			ten_million,		maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::branch_and_bound_use_greedy_maxValue_v3b),						"branch_and_bound_use_greedy_maxValue_v3b" 						},
{true,			ten_million,		maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::branch_and_bound_use_deque_v4),									"branch_and_bound_use_deque_v4" 								},
{true,			ten_million,		maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::branch_and_bound_unique_ptr_heap_v5),								"branch_and_bound_unique_ptr_heap_v5" 							},
{true,			ten_million,		maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::branch_and_bound_heap_preallocate_v6),							"branch_and_bound_ptr_heap_preallocate_v6" 						},
{true,			ten_million,		maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::branch_and_bound_heap_preallocate_MM_Queue_v7),					"branch_and_bound_ptr_heap_preallocate_MM_Queue_v7" 			},
{true,			ten_million,		maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::branch_and_bound_MM_Heap_v8),										"branch_and_bound_MM_Heap_v8" 									},
{true,			ten_million,		maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::branch_and_bound_MM_Heap_use_greedy_maxValue_v8b),				"branch_and_bound_MM_Heap_use_greedy_maxValue_v8b" 				},
{true,			ten_million,		maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::branch_and_bound_MM_Heap_use_greedy_maxValue_sort_inplace_v9b),	"branch_and_bound_MM_Heap_use_greedy_maxValue_sort_inplace_v9b" },
{true,			maxULL,				maxULL,					maxULL,			1 << int(KnapsackProblemAlgo::greedy),															"greedy" 														}
	};

	ofstream& createResultFile()
	{
		string fileName("../../../test/data/knapsack_0_1/results/knapsack_results_" + Timer::getCurrentLocalTimeInNanoSeconds2() + ".csv");
		cout << "\nWriting results to file: " << fileName;
		static ofstream resultsFile;

		try
		{
			resultsFile.open(fileName);
		}
		catch (std::ofstream::failure &writeErr)
		{
			cout << "\nERROR: Can not open file: " << fileName << endl;
			return resultsFile;
		}
		catch (...)
		{
			cout << "\nUNKNOWN ERROR while opening file: " << fileName << endl;
			return resultsFile;
		}

		std::stringstream buffer;
		constexpr int maxPrecision = 4;
		buffer.precision(maxPrecision);
		buffer << std::fixed;

		//Write columns
		buffer << "Test File Name,Number of Items,Knapsack Capacity,Approach,Max Value,Duration (nano-seconds),Space Complexity,NumFunCalls,sizeOfHeap" << "\n";
		string str = buffer.str();
		resultsFile.write(str.c_str(), str.length());
		resultsFile.flush();
		buffer.str(std::string());
		buffer.clear();

		return resultsFile;
	}

	void writePerformanceTestResultsToCSVFile(const vector<Kanpsack_0_1_DataSet>& testDataVector, ofstream& resultsFile)
	{
		std::stringstream buffer;
		constexpr int maxPrecision = 4;
		buffer.precision(maxPrecision);
		buffer << std::fixed;

		if (resultsFile.is_open())
		{
			for (int i = 0; i < testDataVector.size(); ++i)
			{
				const Kanpsack_0_1_DataSet& refData = testDataVector[i];
				for (int j = int(KnapsackProblemAlgo::max_approaches) - 1; j > 0; --j)
				{
					//Columns: "Test File Name, Number of Items,Knapsack Capacity,Approach,Max Value,Duration (nano-seconds),Space Complexity,NumFunCalls"
					buffer << refData.testFileName << ","
						<< refData.values.size() << ","
						<< refData.knapsackCapacity << ","
						<< controlStationData[j].algoName << ",";

					if (refData.results[j].skipReason.empty())
					{
						buffer << refData.results[j].actualMaxValue << ","
							<< refData.results[j].time << ","
							<< refData.results[j].space << ","
							<< refData.results[j].numFunctionCalls << ","
							<< refData.results[j].sizeOfHeap
							<< "\n";
					}
					else
						buffer << refData.results[j].skipReason << "\n";
				}
			}
			string str = buffer.str();
			resultsFile.write(str.c_str(), str.length());
			resultsFile.flush();
			buffer.str(std::string());
			buffer.clear();
		}
	}

	double DP_KnapsackProblem_0_1(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, KnapsackProblemAlgo approach)
	{
		unsigned long long spaceComplexity = pow(2, values.size()); // Default/Worst case space complexity
		std::chrono::steady_clock::time_point startTime, endTime;
		double maxValue = 0;
		switch (approach)
		{
		case KnapsackProblemAlgo::naive_v1:
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_naive_v1(values, values.size(), weights, knapsackCapacity, selectedItems);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		case KnapsackProblemAlgo::naive_v2:
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_naive_v2(values, values.size(), weights, knapsackCapacity, selectedItems);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		case KnapsackProblemAlgo::naive_tail_recursive:
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_naive_tail_recursive(0, values, values.size(), weights, knapsackCapacity, selectedItems);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = 0; //it is less than values.size() + 1;
			break;
		case KnapsackProblemAlgo::naive_non_recursive_v1:
		{
			vector<StackMember> functionStack(values.size() + 1);
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_naive_non_recursive_v1(values, values.size(), weights, knapsackCapacity, selectedItems, functionStack);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		}
		case KnapsackProblemAlgo::naive_non_recursive_v2:
		{
			vector<StackMember> functionStack(values.size() + 1);
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_naive_non_recursive_v2(values, values.size(), weights, knapsackCapacity, selectedItems, functionStack);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		}
		case KnapsackProblemAlgo::naive_non_recursive_v3:
		{
			vector<StackMember> functionStack(values.size() + 1);
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_naive_non_recursive_v3(values, values.size(), weights, knapsackCapacity, selectedItems, functionStack);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		}
		case KnapsackProblemAlgo::top_down_using_vector:
		{
			//unsigned long long MaxKnapsackCapacity = *std::max_element(weights.begin(), weights.end());
			vector<vector<double>> cache_bottom_up_2d(values.size() + 1, vector<double>(knapsackCapacity + 1));
			//vector<vector<unsigned long long>> lookupVector(MaxNumberOfItems, vector<unsigned long long>(MaxKnapsackCapacity, 0));
			//for (int i = 0; i < values.size(); ++i)
			//	for (unsigned long long j = 0; j < knapsackCapacity; ++j)
			//		cache_bottom_up_2d[i][j] = 0;
			startTime = std::chrono::steady_clock::now();
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_top_down_using_vector(values, values.size(), weights, knapsackCapacity, selectedItems, cache_bottom_up_2d);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() * knapsackCapacity;
			break;
		}
		case KnapsackProblemAlgo::top_down_using_hashmap:
		{
			unordered_map<size_t, double> lookupHashMap(values.size() * knapsackCapacity);
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_top_down_using_hashmap(values, values.size(), weights, knapsackCapacity, selectedItems, lookupHashMap);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = lookupHashMap.size();
			break;
		}
		case KnapsackProblemAlgo::top_down_using_map_tail_recursive:
		{
			unordered_map<size_t, double> lookup(values.size() * knapsackCapacity);
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_top_down_using_map_tail_recursive(values, values.size(), weights, knapsackCapacity, selectedItems, lookup);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = lookup.size();
			break;
		}
		case KnapsackProblemAlgo::top_down_using_map_non_recursive:
		{
			vector<StackMember> functionStack(values.size() + 1);
			unordered_map<size_t, double> lookup(values.size() * knapsackCapacity);
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_top_down_using_map_non_recursive(values, values.size(), weights, knapsackCapacity, selectedItems, lookup, functionStack);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = lookup.size();
			break;
		}
		case KnapsackProblemAlgo::bottom_up_v1:
		{
			//unsigned long long MaxKnapsackCapacity = *std::max_element(weights.begin(), weights.end());
			vector<vector<double>> cache_bottom_up_2d(values.size() + 1, vector<double>(knapsackCapacity + 1));
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_bottom_up_v1(values, weights, knapsackCapacity, selectedItems, cache_bottom_up_2d);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = (values.size() + 1) * (knapsackCapacity + 1);
			break;
		}
		case KnapsackProblemAlgo::bottom_up_v2:
		{
			vector<double> cache_bottom_up((values.size() + 1) * (knapsackCapacity + 1));
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_bottom_up_v2(values, weights, knapsackCapacity, selectedItems, cache_bottom_up);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = (values.size() + 1) * (knapsackCapacity + 1);
			break;
		}
		case KnapsackProblemAlgo::bottom_up_object_count_independent_space_efficient_v1:
		{
			vector<double> cache_bottom_up_prev(knapsackCapacity + 1);
			vector<double> cache_bottom_up_current(knapsackCapacity + 1);
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_bottom_up_object_count_independent_space_efficient_v1(values, weights, knapsackCapacity, selectedItems,
				cache_bottom_up_prev, cache_bottom_up_current);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = 2 * (knapsackCapacity + 1);
			break;
		}
		case KnapsackProblemAlgo::bottom_up_object_count_independent_space_efficient_v2:
		{
			vector<double> cache_bottom_up(knapsackCapacity + 1);
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_bottom_up_object_count_independent_space_efficient_v2(values, weights, knapsackCapacity, selectedItems, cache_bottom_up);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = (knapsackCapacity + 1);
			break;
		}
		case KnapsackProblemAlgo::bottom_up_object_count_independent_space_efficient_v3:
		{
			vector<double> cache_bottom_up(knapsackCapacity + 1);
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_bottom_up_object_count_independent_space_efficient_v3(values, weights, knapsackCapacity, selectedItems, cache_bottom_up);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = (knapsackCapacity + 1);
			break;
		}
		case KnapsackProblemAlgo::bottom_up_capacity_independent:
		{
			//unsigned long long MaxKnapsackCapacity = *std::max_element(weights.begin(), weights.end());
			vector<vector<double>> cache_bottom_up_2d(values.size() + 1, vector<double>(knapsackCapacity + 1));
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_bottom_up_capacity_independent(values, weights, knapsackCapacity, selectedItems, cache_bottom_up_2d);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = (values.size() + 1) * (knapsackCapacity + 1);
			break;
		}
		case KnapsackProblemAlgo::bottom_up_capacity_independent_space_efficient:
		{
			vector<double> cache_bottom_up_prev(knapsackCapacity + 1);
			vector<double> cache_bottom_up_current(knapsackCapacity + 1);
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_bottom_up_capacity_independent_space_efficient(values, weights, knapsackCapacity, selectedItems,
				cache_bottom_up_prev, cache_bottom_up_current);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = 2 * (values.size() + 1); // Hope to see this
			break;
		}
		case KnapsackProblemAlgo::bottom_up_object_count_independent_space_efficient_using_set:
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_bottom_up_object_count_independent_space_efficient_using_set(values, weights, knapsackCapacity, selectedItems);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		case KnapsackProblemAlgo::branch_and_bound_naive_v1:
		{
			vector<double> valueToWeightsRatio(values.size(), 0.0); // 8 MB
			vector<unsigned int> sortedIndices(values.size(), 0); // 4 MB
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_branch_and_bound_v1(values, weights, knapsackCapacity, selectedItems, valueToWeightsRatio, sortedIndices);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		}
		case KnapsackProblemAlgo::branch_and_bound_cumulative_sum_v2:
		{
			vector<double> valueToWeightsRatio(values.size(), 0.0); // 8 MB
			vector<unsigned int> sortedIndices(values.size(), 0); // 4 MB
			vector<unsigned long long> cumulativeWeights(values.size(), 0); // 8 MB
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_branch_and_bound_v2(values, weights, knapsackCapacity, selectedItems, valueToWeightsRatio, sortedIndices, cumulativeWeights);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		}
		case KnapsackProblemAlgo::branch_and_bound_no_recalc_upperbound_v3:
		{
			vector<double> valueToWeightsRatio(values.size(), 0.0); // 8 MB
			vector<unsigned int> sortedIndices(values.size(), 0); // 4 MB
			vector<unsigned long long> cumulativeWeights(values.size(), 0); // 8 MB
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_branch_and_bound_v3(values, weights, knapsackCapacity, selectedItems, valueToWeightsRatio, sortedIndices, cumulativeWeights);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		}
		case KnapsackProblemAlgo::branch_and_bound_use_greedylike_maxValue_v3a:
		{
			vector<double> valueToWeightsRatio(values.size(), 0.0); // 8 MB
			vector<unsigned int> sortedIndices(values.size(), 0); // 4 MB
			vector<unsigned long long> cumulativeWeights(values.size(), 0); // 8 MB
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_branch_and_bound_v3a(values, weights, knapsackCapacity, selectedItems, valueToWeightsRatio, sortedIndices, cumulativeWeights);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		}
		case KnapsackProblemAlgo::branch_and_bound_use_greedy_maxValue_v3b:
		{
			vector<double> valueToWeightsRatio(values.size(), 0.0); // 8 MB
			vector<unsigned int> sortedIndices(values.size(), 0); // 4 MB
			vector<unsigned long long> cumulativeWeights(values.size(), 0); // 8 MB
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_branch_and_bound_v3b(values, weights, knapsackCapacity, selectedItems, valueToWeightsRatio, sortedIndices, cumulativeWeights);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		}
		case KnapsackProblemAlgo::branch_and_bound_use_deque_v4:
		{
			//maxHeap_v4.clear();
			//maxHeap_v4.resize(values.size());
			vector<double> valueToWeightsRatio(values.size(), 0.0); // 8 MB
			vector<unsigned int> sortedIndices(values.size(), 0); // 4 MB
			vector<unsigned long long> cumulativeWeights(values.size(), 0); // 8 MB
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_branch_and_bound_v4(values, weights, knapsackCapacity, selectedItems, valueToWeightsRatio, sortedIndices, cumulativeWeights);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		}
		case KnapsackProblemAlgo::branch_and_bound_unique_ptr_heap_v5:
		{
			vector<double> valueToWeightsRatio(values.size(), 0.0); // 8 MB
			vector<unsigned int> sortedIndices(values.size(), 0); // 4 MB
			vector<unsigned long long> cumulativeWeights(values.size(), 0); // 8 MB
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_branch_and_bound_v5(values, weights, knapsackCapacity, selectedItems, valueToWeightsRatio, sortedIndices, cumulativeWeights);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		}
		case KnapsackProblemAlgo::branch_and_bound_heap_preallocate_v6:
		{
			vector<vector<decisionTreeNode_v6>> heapObjectsGrowingPool(1, vector<decisionTreeNode_v6>(values.size()));
			deque<decisionTreeNode_v6*> containerForQueue(values.size(), nullptr);
			//initialize the pool indices
			for (int i = 0; i < values.size(); ++i)
				containerForQueue[i] = &heapObjectsGrowingPool[0][i];
			queue<decisionTreeNode_v6*, deque<decisionTreeNode_v6*>> poolIndices(containerForQueue);
			vector<double> valueToWeightsRatio(values.size(), 0.0); // 8 MB
			vector<unsigned int> sortedIndices(values.size(), 0); // 4 MB
			vector<unsigned long long> cumulativeWeights(values.size(), 0); // 8 MB
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_branch_and_bound_v6(values, weights, knapsackCapacity, selectedItems, valueToWeightsRatio, sortedIndices, cumulativeWeights, heapObjectsGrowingPool, poolIndices);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		}
		case KnapsackProblemAlgo::branch_and_bound_heap_preallocate_MM_Queue_v7:
		{
			vector<vector<decisionTreeNode_v7>> heapObjectsGrowingPool(1, vector<decisionTreeNode_v7>(values.size()));
			MM_Queue<decisionTreeNode_v7*> poolIndices(values.size());
			//initialize the pool indices
			for (int i = 0; i < values.size(); ++i)
				poolIndices.push(&heapObjectsGrowingPool[0][i]);
			vector<double> valueToWeightsRatio(values.size(), 0.0); // 8 MB
			vector<unsigned int> sortedIndices(values.size(), 0); // 4 MB
			vector<unsigned long long> cumulativeWeights(values.size(), 0); // 8 MB
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_branch_and_bound_v7(values, weights, knapsackCapacity, selectedItems, valueToWeightsRatio, sortedIndices, cumulativeWeights, heapObjectsGrowingPool, poolIndices);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		}
		case KnapsackProblemAlgo::branch_and_bound_MM_Heap_v8:
		{
			vector<vector<decisionTreeNode_v8>> heapObjectsGrowingPool(1, vector<decisionTreeNode_v8>(values.size()));
			MM_Heap<decisionTreeNode_v8*, Comparator_v8> maxHeap_v8(values.size());
			//initialize the pool indices
			for (int i = 0; i < values.size(); ++i)
				maxHeap_v8.addToData(&heapObjectsGrowingPool[0][i]);
			vector<double> valueToWeightsRatio(values.size(), 0.0); // 8 MB
			vector<unsigned int> sortedIndices(values.size(), 0); // 4 MB
			vector<unsigned long long> cumulativeWeights(values.size(), 0); // 8 MB
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_branch_and_bound_v8(values, weights, knapsackCapacity, selectedItems, valueToWeightsRatio, sortedIndices, cumulativeWeights, maxHeap_v8, heapObjectsGrowingPool);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		}
		case KnapsackProblemAlgo::branch_and_bound_MM_Heap_use_greedy_maxValue_v8b:
		{
			vector<vector<decisionTreeNode_v8b>> heapObjectsGrowingPool(1, vector<decisionTreeNode_v8b>(values.size()));
			MM_Heap<decisionTreeNode_v8b*, Comparator_v8b> maxHeap_v8b(values.size());
			//initialize the pool indices
			for (int i = 0; i < values.size(); ++i)
				maxHeap_v8b.addToData(&heapObjectsGrowingPool[0][i]);
			vector<double> valueToWeightsRatio(values.size(), 0.0); // 8 MB
			vector<unsigned int> sortedIndices(values.size(), 0); // 4 MB
			vector<unsigned long long> cumulativeWeights(values.size(), 0); // 8 MB
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_branch_and_bound_v8b(values, weights, knapsackCapacity, selectedItems, valueToWeightsRatio, sortedIndices, cumulativeWeights, maxHeap_v8b, heapObjectsGrowingPool);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		}
		case KnapsackProblemAlgo::branch_and_bound_MM_Heap_use_greedy_maxValue_sort_inplace_v9b:
		{
			vector<double> valuesCopy = values;
			vector<unsigned long long> weightsCopy = weights;
			vector<vector<decisionTreeNode_v9b>> heapObjectsGrowingPool(1, vector<decisionTreeNode_v9b>(values.size()));
			MM_Heap<decisionTreeNode_v9b*, Comparator_v9b> maxHeap_v9b(values.size());
			//initialize the pool indices
			for (int i = 0; i < values.size(); ++i)
				maxHeap_v9b.addToData(&heapObjectsGrowingPool[0][i]);
			vector<double> valueToWeightsRatio(values.size(), 0.0); // 8 MB
			vector<unsigned long long> cumulativeWeights(values.size(), 0); // 8 MB
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_branch_and_bound_v9b(valuesCopy, weightsCopy, knapsackCapacity, selectedItems, valueToWeightsRatio, cumulativeWeights, maxHeap_v9b, heapObjectsGrowingPool);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size() + 1;
			break;
		}
		case KnapsackProblemAlgo::greedy:
			startTime = std::chrono::steady_clock::now();
			maxValue = DP_KnapsackProblem_0_1_greedy(values, weights, knapsackCapacity, selectedItems);
			endTime = std::chrono::steady_clock::now();
			spaceComplexity = values.size();
			break;
		default:
			MyAssert::myRunTimeAssert(false, "Unknown DP approach");
		}

		knapsack_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
		knapsack_spaceComplexity = spaceComplexity;

		return maxValue;
	}

	void KnapsackTest(vector<Kanpsack_0_1_DataSet>& testData, int mask)
	{
		std::cout.imbue(std::locale(""));
		vector<unsigned int> actualSelectedItems(0);
		double actualMaxValue;
		for (int i = 0; i < testData.size(); ++i)
		{
			cout << "\n";
			cout << "\nTest file name: " << testData[i].testFileName;
			cout << "\nNumber of Values: " << testData[i].values.size();
			if (testData[i].values.size() <= 100) //Avoid printing large vector on screen
			{
				cout << "\nValues: " << testData[i].values;
				cout << "\nWeights: " << testData[i].weights;
			}
			cout << "\nKnapsack capacity: " << testData[i].knapsackCapacity;
			cout << "\nExpected Max Value (exact ): " << testData[i].expectedMaxValue;
			cout << "\nExpected Max Value (greedy): " << testData[i].expectedMaxValueByGreedy;
			cout << "\n";

			//Print columns
			const int widthCol1 = 65;
			const int widthCol2 = 20;
			const int widthCol3 = 20;
			const int widthCol4 = 18;
			const int widthCol5 = 16;
			cout << setw(widthCol1) << std::right << "Algo Name";
			cout << setw(widthCol2) << std::right << "Time (nano-sec)";
			cout << setw(widthCol3) << std::right << "Space Complexity";
			cout << setw(widthCol4) << std::right << "FunctionCalls";
			cout << setw(widthCol5) << std::right << "sizeOfHeap";
			string separator(widthCol1 + widthCol2 + widthCol3 + widthCol4 + widthCol5, '-');
			cout << "\n" << separator;

			for (int j = int(KnapsackProblemAlgo::max_approaches) - 1; j > 0; --j)
			{
				if (!controlStationData[j].isImplemented || testData[i].values.empty())
					continue;

				cout << "\n" << setw(widthCol1) << std::right << controlStationData[j].algoName;

				std::stringstream buffer;
				if ((mask & (1 << j)) == 0)
					buffer << "   skipping as mask is not set";

				if(controlStationData[j].maxNumItems < testData[i].values.size())
					buffer << "   skipping due to max values: " << controlStationData[j].maxNumItems;

				if(controlStationData[j].maxKnapsackCapacity < testData[i].knapsackCapacity)
					buffer << "   skipping due to max capacity: " << controlStationData[j].maxKnapsackCapacity;

				if(controlStationData[j].max_nW < testData[i].values.size() * testData[i].knapsackCapacity)
					buffer << "   skipping due to max_nW: " << controlStationData[j].max_nW << " < " << testData[i].values.size() * testData[i].knapsackCapacity;

				string str = buffer.str();
				if (!str.empty())
				{
					cout << str;
					testData[i].results[j].skipReason = str;
					buffer.str(std::string());
					buffer.clear();
					continue;
				}

				numberOfFunctionCalls = 0;
				sizeOfHeap = 0;
				actualMaxValue = DP_KnapsackProblem_0_1(testData[i].values, testData[i].weights, testData[i].knapsackCapacity, actualSelectedItems, KnapsackProblemAlgo(j));
				double expectedMaxValue = testData[i].expectedMaxValue;
				if (j == int(KnapsackProblemAlgo::greedy))
					expectedMaxValue = testData[i].expectedMaxValueByGreedy;

				testData[i].results[j].algoType = KnapsackProblemAlgo(j);
				testData[i].results[j].actualMaxValue = actualMaxValue;
				testData[i].results[j].time = knapsack_duration;
				testData[i].results[j].space = knapsack_spaceComplexity;
				testData[i].results[j].numFunctionCalls = numberOfFunctionCalls;
				testData[i].results[j].sizeOfHeap = sizeOfHeap;

				//cout << "\n" << setw(55) << std::left << controlStationData[j].algoName;
				cout << setw(widthCol2) << std::right << knapsack_duration;
				cout << setw(widthCol3) << std::right << knapsack_spaceComplexity;
				cout << setw(widthCol4) << std::right << numberOfFunctionCalls;
				cout << setw(widthCol5) << std::right << sizeOfHeap;
				//cout << "\n";

				double tolerance = 0.0001;
				MM_EXPECT_TRUE(actualSelectedItems == testData[i].expectedSelectedItems && fabs(actualMaxValue - expectedMaxValue) < tolerance, 
					//testData[i].values, testData[i].weights, 
					testData[i].knapsackCapacity,
					actualSelectedItems, testData[i].expectedSelectedItems, actualMaxValue, expectedMaxValue);
			}

			cout << "\n\n";
		}
	}
	
	void runHardecodedSanityTest(ofstream& resultFile)
	{
		vector<Kanpsack_0_1_DataSet> testData = DP_KnapsackProblem_0_1_testDataGenerator::getHardecodedSanityTestCases();
		KnapsackTest(testData, (1 << int(KnapsackProblemAlgo::max_approaches)) - 1);
		writePerformanceTestResultsToCSVFile(testData, resultFile);
	}

	void runSanityTest(ofstream& resultFile)
	{
		vector<Kanpsack_0_1_DataSet> testData = DP_KnapsackProblem_0_1_testDataGenerator::readTestCasesFromFile(Kanpsack_0_1_testCaseType::sanityTestCases, 1, 200);
		KnapsackTest(testData, (1 << int(KnapsackProblemAlgo::max_approaches)) - 1);
		writePerformanceTestResultsToCSVFile(testData, resultFile);
	}
	
	void runPerformanceTest_1K(ofstream& resultFile)
	{
		vector<Kanpsack_0_1_DataSet> testData = DP_KnapsackProblem_0_1_testDataGenerator::readTestCasesFromFile(Kanpsack_0_1_testCaseType::performanceTestCases_1K, 1, 100);
		KnapsackTest(testData, (1 << int(KnapsackProblemAlgo::max_approaches)) - 1);
		writePerformanceTestResultsToCSVFile(testData, resultFile);
	}

	void runPerformanceTest_10K(ofstream& resultFile)
	{
		// n = 1000, W = 5,000,000, O(5 billion)    Takes 7 seconds
		vector<Kanpsack_0_1_DataSet> testData = DP_KnapsackProblem_0_1_testDataGenerator::readTestCasesFromFile(Kanpsack_0_1_testCaseType::performanceTestCases_10K, 1, 100);
		KnapsackTest(testData, (1 << int(KnapsackProblemAlgo::max_approaches)) - 1);

		writePerformanceTestResultsToCSVFile(testData, resultFile);
	}

	void runPerformanceTest_100K(ofstream& resultFile)
	{
		// n = 10,000, W = 5,000,000, O(50 billion)  
		// n = 10,000, W = 10,000,000, O(100 billion)   
		vector<Kanpsack_0_1_DataSet> testData = DP_KnapsackProblem_0_1_testDataGenerator::readTestCasesFromFile(Kanpsack_0_1_testCaseType::performanceTestCases_100K, 1, 100);
		KnapsackTest(testData, (1 << int(KnapsackProblemAlgo::max_approaches)) - 1);

		writePerformanceTestResultsToCSVFile(testData, resultFile);
	}

	void runPerformanceTest_1M(ofstream& resultFile)
	{
		for(int i = 0; i < 99; ++i)
		{
			vector<Kanpsack_0_1_DataSet> testData = DP_KnapsackProblem_0_1_testDataGenerator::readTestCasesFromFile(Kanpsack_0_1_testCaseType::performanceTestCases_1M, i, i);
			if (testData.empty())
				continue;
			
			KnapsackTest(testData, (1 << int(KnapsackProblemAlgo::max_approaches)) - 1);

			writePerformanceTestResultsToCSVFile(testData, resultFile);
			testData.clear();
		}
	}

	void runPerformanceTest_10M(ofstream& resultFile)
	{
		for(int i = 0; i < 99; ++i)
		{
			vector<Kanpsack_0_1_DataSet> testData = DP_KnapsackProblem_0_1_testDataGenerator::readTestCasesFromFile(Kanpsack_0_1_testCaseType::performanceTestCases_10M, i, i);
			if (testData.empty())
				continue;

			KnapsackTest(testData, (1 << int(KnapsackProblemAlgo::max_approaches)) - 1);

			writePerformanceTestResultsToCSVFile(testData, resultFile);
			testData.clear();
		}
	}

	void runPerformanceTest_100M(ofstream& resultFile)
	{
		for(int i = 0; i < 99; ++i)
		{
			vector<Kanpsack_0_1_DataSet> testData = DP_KnapsackProblem_0_1_testDataGenerator::readTestCasesFromFile(Kanpsack_0_1_testCaseType::performanceTestCases_100M, i, i);
			if (testData.empty())
				continue;

			KnapsackTest(testData, (1 << int(KnapsackProblemAlgo::max_approaches)) - 1);

			writePerformanceTestResultsToCSVFile(testData, resultFile);
			testData.clear();
		}
	}

	MM_DECLARE_FLAG(DP_KnapsackProblem_0_1);
	
	MM_UNIT_TEST(DP_KnapsackProblem_0_1_test_1, DP_KnapsackProblem_0_1)
	{
		MM_SET_PAUSE_ON_ERROR(true);
		MM_PRINT_TEST_CASE_NUMBER(false);
		
		constexpr int maxPrecision = 4;
		cout.precision(maxPrecision);
		cout << std::fixed;
		cout.imbue(std::locale(""));

		//Uncomment this only when generating new test cases
		//DP_KnapsackProblem_0_1_testDataGenerator::generateAndWriteTestCasesToFile();
		//return;

		ofstream& resultFile = createResultFile();

		runHardecodedSanityTest(resultFile);
		runSanityTest(resultFile);

		runPerformanceTest_1K(resultFile);
		runPerformanceTest_10K(resultFile);
		runPerformanceTest_100K(resultFile);

		runPerformanceTest_1M(resultFile);
		runPerformanceTest_10M(resultFile);
		runPerformanceTest_100M(resultFile);

		resultFile.close();
	}
	
}

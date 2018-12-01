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
#include <filesystem>
using namespace std;

#include "DynamicProgramming/DP_KnapsackProblem_0_1_testDataGenerator.h"
//#include "Assert/MyAssert.h"
#include "Others/PrintUtils.h"
//#include "DynamicProgramming/DP_Common.h"
#include "DynamicProgramming/DP_KnapsackProblem_0_1.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Assert/MyAssert.h"

namespace mm {

	const string Kanpsack_0_1_testCaseType::sanityTestCases{ "sanityTestCases" };
	const string Kanpsack_0_1_testCaseType::performanceTestCases_1K{ "performanceTestCases_1K" };
	const string Kanpsack_0_1_testCaseType::performanceTestCases_10K{ "performanceTestCases_10K" };
	const string Kanpsack_0_1_testCaseType::performanceTestCases_100K{ "performanceTestCases_100K" };
	const string Kanpsack_0_1_testCaseType::performanceTestCases_1M{ "performanceTestCases_1M" };
	const string Kanpsack_0_1_testCaseType::performanceTestCases_10M{ "performanceTestCases_10M" };
	const string Kanpsack_0_1_testCaseType::performanceTestCases_100M{ "performanceTestCases_100M" };
	//const string Kanpsack_0_1_testCaseType::highPerformanceTestCases{ "highPerformanceTestCases" };
	//const string Kanpsack_0_1_testCaseType::crazyPerformanceTestCases{ "crazyPerformanceTestCases" };
	//const string Kanpsack_0_1_testCaseType::superCrazyPerformanceTestCases{ "superCrazyPerformanceTestCases" };

	const string Kanpsack_0_1_tags::result_exact{"result_exact: "};
	const string Kanpsack_0_1_tags::result_greedy{"result_greedy: "};
	const string Kanpsack_0_1_tags::knapsack_capacity{"knapsack_capacity: "};
	const string Kanpsack_0_1_tags::num_items{"num_items: "};
	const string Kanpsack_0_1_tags::value_weight_pairs{"value_weight_pairs: "};

	void DP_KnapsackProblem_0_1_testDataGenerator::generateAndWriteTestCasesToFile()
	{
		int numTestCases = 10;
		DP_KnapsackProblem_0_1_testDataGenerator::generateRandomTestCases(1, 20, 1, 1, 100, 99999, 10, 500, 75, 98, 
			true, false, Kanpsack_0_1_testCaseType::performanceTestCases_1K);
		DP_KnapsackProblem_0_1_testDataGenerator::generateRandomTestCases(25, 100, 5, 1, 100, 99999, 10, 500, 50, 75, 
			true, false, Kanpsack_0_1_testCaseType::performanceTestCases_1K);
		DP_KnapsackProblem_0_1_testDataGenerator::generateRandomTestCases(100, 100, 0, numTestCases, 100.0, 99999.0, 1, 1000, 75, 98, 
			true, false, Kanpsack_0_1_testCaseType::performanceTestCases_1K);
		
		DP_KnapsackProblem_0_1_testDataGenerator::generateRandomTestCases(1000, 1000, 0, numTestCases, 100.0, 99999.0, 1, 10000, 50, 75, 
			true, false, Kanpsack_0_1_testCaseType::performanceTestCases_1K);

		DP_KnapsackProblem_0_1_testDataGenerator::generateRandomTestCases(10000, 10000, 0, numTestCases/2, 100.0, 99999.0, 1, 1000, 75, 98, 
			true, false, Kanpsack_0_1_testCaseType::performanceTestCases_10K);
		DP_KnapsackProblem_0_1_testDataGenerator::generateRandomTestCases(10000, 10000, 0, numTestCases/2, 100.0, 99999.0, 1, 5000, 50, 75, 
			true, false, Kanpsack_0_1_testCaseType::performanceTestCases_10K);

		DP_KnapsackProblem_0_1_testDataGenerator::generateRandomTestCases(100000, 100000, 0, numTestCases/2, 100.0, 99999.0, 1, 250, 75, 98, 
			true, false, Kanpsack_0_1_testCaseType::performanceTestCases_100K);
		DP_KnapsackProblem_0_1_testDataGenerator::generateRandomTestCases(100000, 100000, 0, numTestCases/2, 100.0, 99999.0, 1, 2000, 50, 75, 
			true, false, Kanpsack_0_1_testCaseType::performanceTestCases_100K);

		DP_KnapsackProblem_0_1_testDataGenerator::generateRandomTestCases(1000000, 1000000, 0, numTestCases/2, 100.0, 99999.0, 1, 250, 75, 98, 
			true, false, Kanpsack_0_1_testCaseType::performanceTestCases_1M);
		DP_KnapsackProblem_0_1_testDataGenerator::generateRandomTestCases(1000000, 1000000, 0, numTestCases/2, 100.0, 99999.0, 1, 2000, 50, 75, 
			true, false, Kanpsack_0_1_testCaseType::performanceTestCases_1M);
		
		DP_KnapsackProblem_0_1_testDataGenerator::generateRandomTestCases(10000000, 10000000, 0, numTestCases / 2, 100.0, 99999.0, 1, 250, 75, 98, 
			true, false, Kanpsack_0_1_testCaseType::performanceTestCases_10M);
		DP_KnapsackProblem_0_1_testDataGenerator::generateRandomTestCases(10000000, 10000000, 0, numTestCases / 2, 100.0, 99999.0, 1, 2000, 50, 75, 
			true, false, Kanpsack_0_1_testCaseType::performanceTestCases_10M);
	}

	void printOrWriteData(std::ostream& base, const Kanpsack_0_1_DataSet& data)
	{
		base << Kanpsack_0_1_tags::result_exact << data.expectedMaxValue;
		base << "\n";
		base << Kanpsack_0_1_tags::result_greedy << data.expectedMaxValueByGreedy;
		base << "\n";
		base << Kanpsack_0_1_tags::knapsack_capacity << data.knapsackCapacity;
		base << "\n";
		base << Kanpsack_0_1_tags::num_items << data.values.size();
		base << "\n";
		base << Kanpsack_0_1_tags::value_weight_pairs;
		base << "\n";
		for (int i = 0; i < data.values.size(); ++i)
			base << data.values[i] << ", " << data.weights[i] << "\n";
	}

	void DP_KnapsackProblem_0_1_testDataGenerator::writeTestCaseToFile(const Kanpsack_0_1_DataSet& testData, const string& testCaseName)
	{
		string fileName("../../../test/data/knapsack_0_1/" + testCaseName + "_" + to_string(0) + ".data");
		cout << "\nWriting test case to file: " << fileName;
		ofstream testDataFile;

		try
		{
			testDataFile.open(fileName);
		}
		catch (std::ofstream::failure &writeErr)
		{
			cout << "\nERROR: Can not open file: " << fileName << endl;
			return;
		}
		catch (...)
		{
			cout << "\nUNKNOWN ERROR while opening file: " << fileName << endl;
			return;
		}

		std::stringstream buffer;
		constexpr int maxPrecision = 4;
		buffer.precision(maxPrecision);
		buffer << std::fixed;

		string str;

		if (testDataFile.is_open())
		{
			printOrWriteData(buffer, testData);
			str = buffer.str();
			testDataFile.write(str.c_str(), str.length());
			testDataFile.flush();
			buffer.str(std::string());
			buffer.clear();

			testDataFile.close();
		}
	}

	vector<Kanpsack_0_1_DataSet> DP_KnapsackProblem_0_1_testDataGenerator::readTestCasesFromFile(const string& testCaseNamePrefix)
	{
		vector<Kanpsack_0_1_DataSet> testData;
		const string path("../../../test/data/knapsack_0_1");
		for (const auto & fileOrDirectory : std::experimental::filesystem::directory_iterator(path))
		//for (int i = start; i <= end; ++i)
		{
			if (std::experimental::filesystem::is_directory(fileOrDirectory))
				continue;
			
			string fullFileName(fileOrDirectory.path().generic_u8string());
			string fileName(fullFileName.substr(fullFileName.find_last_of("/") + 1));
			if (fullFileName.find(testCaseNamePrefix) == string::npos)
				continue;

			ifstream testDataFile;

			try
			{
				testDataFile.open(fullFileName);
			}
			catch (std::ifstream::failure &readErr)
			{
				cout << "\nERROR: Can not open file: " << fullFileName << endl;
				return testData;
			}
			catch (...)
			{
				cout << "\nUNKNOWN ERROR while opening file: " << fullFileName << endl;
				return testData;
			}

			if (testDataFile.is_open())
			{
				Kanpsack_0_1_DataSet element;
				element.testFileName = fileName;
				string line;
				int numItems = 0;
				while (std::getline(testDataFile, line, '\n'))
				{
					if (line.empty())
						continue;
					else if (line.substr(0, Kanpsack_0_1_tags::result_exact.length()) == Kanpsack_0_1_tags::result_exact)
						element.expectedMaxValue = stod(line.substr(Kanpsack_0_1_tags::result_exact.length()));
					else if (line.substr(0, Kanpsack_0_1_tags::result_greedy.length()) == Kanpsack_0_1_tags::result_greedy)
						element.expectedMaxValueByGreedy = stod(line.substr(Kanpsack_0_1_tags::result_greedy.length()));
					else if (line.substr(0, Kanpsack_0_1_tags::knapsack_capacity.length()) == Kanpsack_0_1_tags::knapsack_capacity)
						element.knapsackCapacity = stoull(line.substr(Kanpsack_0_1_tags::knapsack_capacity.length()));
					else if (line.substr(0, Kanpsack_0_1_tags::num_items.length()) == Kanpsack_0_1_tags::num_items)
						numItems = stoi(line.substr(Kanpsack_0_1_tags::num_items.length()));
					else if (line == Kanpsack_0_1_tags::value_weight_pairs)
					{
						while (std::getline(testDataFile, line, '\n'))
						{
							if (line.empty())
								continue;
							size_t mid = line.find_first_of(',', 0);
							element.values.push_back(stod(line.substr(0, mid)));
							element.weights.push_back(stoull(line.substr(mid + 1)));
						}
					}
				}

				MyAssert::myRunTimeAssert(numItems == element.values.size() && numItems == element.weights.size());
				testData.push_back(element);
			}
			//else
			//	break;
		}

		return testData;
	}

	//Function declaration
	double DP_KnapsackProblem_0_1(const vector<double>& values, const vector<unsigned long long>& weights, unsigned long long knapsackCapacity, vector<unsigned int>& selectedItems, KnapsackProblemAlgo approach);

	void DP_KnapsackProblem_0_1_testDataGenerator::generateRandomTestCases(
		unsigned long long minNumObjects, unsigned long long maxNumObjects, int increment, int numTestCasesOfEachObjectSize,
		double minValue, double maxValue,
		unsigned long long minWeight, unsigned long long maxWeight,
		int minKnapsackCapacityPercentage, int maxKnapsackCapacityPercentage,
		bool findSolution, bool greedyEqualsExact, const string& testCaseNamePrefix)
	{
		//vector<Kanpsack_0_1_DataSet> testDataVector(0);

		if (minValue == minWeight && maxValue == maxWeight) // random number distribution would be same
			--maxValue;

		std::default_random_engine generator;
		std::uniform_int_distribution<unsigned long long> valueDistribution(minValue, maxValue);
		auto valueGenerator = std::bind(valueDistribution, generator);
		std::uniform_int_distribution<unsigned long long> weightDistribution(minWeight, maxWeight);
		auto weightGenerator = std::bind(weightDistribution, generator);
		std::uniform_int_distribution<unsigned long long> capacityDistribution(minKnapsackCapacityPercentage, maxKnapsackCapacityPercentage);
		auto capacityGenerator = std::bind(capacityDistribution, generator);

		//int nextIndex = testDataVector.size();
		//testDataVector.resize(testDataVector.size() + (maxNumObjects - minNumObjects + 1) * numTestCasesOfEachObjectSize);
		
		//Generate test data
		if (increment == 0)
			increment = 1; //Make sure the loop terminates
		for (unsigned long long numObjects = minNumObjects; numObjects <= maxNumObjects; numObjects += increment)
		{
			for (int i = 0; i < numTestCasesOfEachObjectSize; ++i)
			{
				cout << "\nGenerating test case- numObejcts: " << numObjects << " test case no.: " << i + 1;
				vector<double> values(numObjects);
				vector<unsigned long long> weights(numObjects);
				
				//Generate weights & values
				unsigned long long knapsackCapacity = 0;
				for (unsigned long long j = 0; j < numObjects; ++j)
				{
					values[j] = valueGenerator() * 0.99; //Make a fraction from ULL
					weights[j] = weightGenerator();
					knapsackCapacity += weights[j];
				}
				knapsackCapacity = knapsackCapacity * capacityGenerator() / 100;
				//testDataVector[nextIndex] = Kanpsack_0_1_DataSet(values, weights, knapsackCapacity);
				Kanpsack_0_1_DataSet newDataSet(values, weights, knapsackCapacity);

				if (findSolution)
				{
					vector<unsigned int> actualSelectedItems;
					newDataSet.expectedMaxValue = DP_KnapsackProblem_0_1(newDataSet.values, newDataSet.weights, newDataSet.knapsackCapacity, actualSelectedItems, KnapsackProblemAlgo::branch_and_bound_MM_Heap_use_greedy_maxValue_sort_inplace_v9b);
					newDataSet.expectedSelectedItems = actualSelectedItems;

					newDataSet.expectedMaxValueByGreedy = DP_KnapsackProblem_0_1(newDataSet.values, newDataSet.weights, newDataSet.knapsackCapacity, actualSelectedItems, KnapsackProblemAlgo::greedy);

					// Decide what king of test case data you want to generate: e.g. always defeating greedy algo, consistent with gredy algo, or random
					//if (testDataVector[nextIndex].maxValue != testDataVector[nextIndex].maxValueByGreedy)
					if (greedyEqualsExact == (newDataSet.expectedMaxValue != newDataSet.expectedMaxValueByGreedy))
					{
						--i;
						continue;
					}
				}

				writeTestCaseToFile(newDataSet, testCaseNamePrefix);
			}
		}

		// Print results:
		//for (int i = 0; i < testDataVector.size(); ++i)
		//{
		//	if (i > 0 && testDataVector[i - 1].values.size() < testDataVector[i].values.size())
		//		cout << "\n// number of objects = " << testDataVector[i].values.size();
		//	cout << "\n{ " << testDataVector[i].values << ", " << testDataVector[i].weights << ", " << testDataVector[i].knapsackCapacity << ", {}, " << testDataVector[i].maxValue << ", " << testDataVector[i].maxValueByGreedy << " },";
		//}
		//cout << "\n";
		//return testDataVector;
	}

	vector<Kanpsack_0_1_DataSet> DP_KnapsackProblem_0_1_testDataGenerator::getHardecodedSanityTestCases()
	{
		//generateRandomTestCases(3, 10, 10, 1, 50, 1, 50, 75, 98, true);

		vector<Kanpsack_0_1_DataSet> testDataVector = {

			//{{Values}, {Weights}, knapsackCapacity, {selected}, maxValue, maxValueByGreedy}

			// number of objects = 1
			{ { 6 }, { 12 }, 19, {}, 6, 6 },
			{ { 6 }, { 12 }, 12, {}, 6, 6 },
			{ { 6 }, { 12 }, 10, {}, 0, 0 },
			// number of objects = 2
			{ { 16, 12 }, { 8, 6 }, 15, {}, 28, 28 },
			{ { 16, 12 }, { 8, 6 }, 14, {}, 28, 28 },
			{ { 16, 12 }, { 8, 6 }, 13, {}, 16, 16 },
			{ { 16, 12 }, { 8, 6 }, 7, {}, 12, 12 },
			{ { 16, 12 }, { 8, 5 }, 10, {}, 16, 12 },
			{ { 16, 12 }, { 8, 7 }, 10, {}, 16, 16 },

			// Test cases which do NOT match greedy algo results:
			// number of objects = 3
			{ { 6, 10, 12 }, { 1, 2, 3 }, 5, {}, 22, 16 },
			{ { 12, 12, 10 }, { 7, 7, 5 }, 17, {}, 24, 22 },
			{ { 6, 12, 11 }, { 5, 9, 10 }, 21, {}, 23, 18 },
			{ { 42, 10, 14 }, { 4, 11, 22 }, 34, {}, 56, 52 },
			{ { 18, 36, 10 }, { 29, 45, 7 }, 75, {}, 54, 46 },
			{ { 10, 5, 14 }, { 46, 21, 48 }, 112, {}, 24, 19 },
			{ { 47, 1, 41 }, { 44, 11, 7 }, 48, {}, 47, 42 },
			{ { 31, 40, 2 }, { 10, 45, 5 }, 52, {}, 42, 33 },
			{ { 29, 30, 43 }, { 46, 48, 37 }, 125, {}, 73, 72 },
			{ { 23, 27, 24 }, { 9, 6, 45 }, 57, {}, 51, 50 },
			{ { 18, 37, 26 }, { 4, 21, 30 }, 51, {}, 63, 55 },
			{ { 8, 40, 4 }, { 18, 42, 4 }, 60, {}, 48, 44 },
			{ { 19, 36, 43 }, { 15, 1, 38 }, 41, {}, 79, 55 },
			// number of objects = 4
			{ { 43, 27, 25, 32 }, { 40, 23, 4, 41 }, 104, {}, 102, 95 },
			{ { 25, 46, 49, 11 }, { 5, 20, 45, 4 }, 72, {}, 120, 82 },
			{ { 44, 36, 40, 5 }, { 47, 20, 49, 4 }, 102, {}, 89, 85 },
			{ { 9, 14, 36, 11 }, { 16, 7, 5, 32 }, 58, {}, 61, 59 },
			{ { 20, 17, 32, 30 }, { 12, 3, 13, 43 }, 65, {}, 79, 69 },
			{ { 42, 38, 34, 3 }, { 43, 50, 12, 22 }, 102, {}, 80, 79 },
			{ { 10, 18, 33, 43 }, { 13, 11, 38, 1 }, 49, {}, 76, 71 },
			{ { 32, 47, 48, 38 }, { 31, 15, 3, 48 }, 84, {}, 133, 127 },
			{ { 6, 26, 44, 19 }, { 1, 45, 36, 25 }, 86, {}, 76, 69 },
			{ { 46, 23, 17, 15 }, { 27, 27, 1, 9 }, 62, {}, 86, 78 },
			// number of objects = 5
			{ { 28, 27, 49, 25, 25 }, { 18, 23, 44, 18, 20 }, 108, {}, 129, 105 },
			{ { 35, 45, 40, 44, 36 }, { 34, 32, 46, 25, 10 }, 123, {}, 165, 160 },
			{ { 39, 41, 45, 31, 24 }, { 3, 26, 6, 50, 11 }, 86, {}, 156, 149 },
			{ { 28, 43, 36, 45, 41 }, { 10, 24, 7, 20, 32 }, 85, {}, 165, 152 },
			{ { 40, 44, 40, 33, 21 }, { 43, 16, 20, 26, 15 }, 109, {}, 157, 138 },
			{ { 34, 29, 22, 21, 30 }, { 8, 42, 38, 33, 6 }, 120, {}, 115, 114 },
			{ { 14, 18, 30, 40, 15 }, { 32, 6, 25, 47, 46 }, 143, {}, 103, 102 },
			{ { 18, 37, 32, 40, 16 }, { 28, 23, 27, 5, 6 }, 83, {}, 127, 125 },
			{ { 20, 16, 31, 43, 26 }, { 49, 29, 27, 42, 8 }, 145, {}, 120, 116 },
			{ { 49, 18, 8, 40, 31 }, { 32, 4, 17, 12, 17 }, 64, {}, 120, 97 },
			// number of objects = 6
			{ { 1, 16, 14, 15, 32, 37 }, { 3, 37, 26, 11, 31, 14 }, 115, {}, 101, 99 },
			{ { 14, 35, 48, 19, 1, 14 }, { 12, 29, 14, 33, 7, 22 }, 88, {}, 116, 112 },
			{ { 36, 16, 37, 39, 34, 41 }, { 28, 24, 24, 33, 25, 30 }, 123, {}, 153, 148 },
			{ { 49, 33, 46, 32, 22, 9 }, { 29, 37, 18, 9, 24, 9 }, 118, {}, 182, 158 },
			{ { 26, 33, 12, 6, 30, 8 }, { 4, 21, 48, 2, 23, 31 }, 117, {}, 107, 103 },
			{ { 38, 37, 48, 41, 34, 27 }, { 31, 28, 31, 1, 13, 19 }, 113, {}, 198, 187 },
			{ { 29, 18, 12, 18, 27, 23 }, { 1, 44, 22, 48, 28, 32 }, 161, {}, 115, 109 },
			{ { 42, 5, 17, 30, 20, 1 }, { 27, 5, 15, 46, 36, 26 }, 124, {}, 109, 95 },
			{ { 22, 1, 3, 4, 10, 8 }, { 35, 7, 39, 10, 43, 46 }, 174, {}, 47, 45 },
			{ { 7, 20, 39, 28, 24, 37 }, { 2, 11, 18, 45, 2, 4 }, 72, {}, 135, 127 },
			// number of objects = 7
			{ { 20, 21, 1, 2, 17, 36, 19 }, { 50, 40, 20, 11, 16, 47, 21 }, 170, {}, 98, 96 },
			{ { 32, 48, 27, 18, 36, 29, 20 }, { 16, 17, 6, 9, 10, 10, 2 }, 60, {}, 181, 178 },
			{ { 28, 43, 16, 18, 44, 5, 35 }, { 40, 20, 33, 46, 32, 8, 20 }, 191, {}, 184, 171 },
			{ { 4, 13, 43, 11, 35, 14, 37 }, { 23, 25, 34, 25, 37, 36, 15 }, 154, {}, 142, 139 },
			{ { 20, 22, 39, 27, 20, 2, 25 }, { 38, 4, 48, 23, 32, 14, 50 }, 190, {}, 135, 130 },
			{ { 11, 45, 23, 24, 23, 1, 6 }, { 9, 9, 43, 47, 33, 8, 19 }, 129, {}, 110, 109 },
			{ { 17, 7, 14, 19, 3, 38, 13 }, { 11, 21, 11, 22, 25, 15, 39 }, 118, {}, 101, 98 },
			{ { 45, 21, 1, 7, 7, 12, 29 }, { 42, 40, 18, 13, 23, 18, 37 }, 145, {}, 107, 100 },
			{ { 14, 23, 40, 34, 22, 9, 15 }, { 6, 40, 48, 18, 35, 16, 4 }, 140, {}, 135, 134 },
			{ { 42, 18, 40, 8, 12, 32, 24 }, { 24, 7, 6, 14, 19, 42, 48 }, 131, {}, 156, 152 },
			// number of objects = 8
			{ { 46, 46, 20, 3, 8, 5, 5, 12 }, { 9, 25, 44, 10, 3, 19, 5, 24 }, 136, {}, 142, 140 },
			{ { 28, 34, 42, 19, 49, 20, 30, 9 }, { 2, 34, 36, 29, 5, 46, 21, 15 }, 174, {}, 222, 211 },
			{ { 15, 39, 24, 12, 48, 21, 18, 9 }, { 23, 49, 48, 23, 25, 2, 40, 6 }, 198, {}, 174, 168 },
			{ { 29, 21, 23, 2, 15, 47, 1, 25 }, { 41, 9, 25, 6, 7, 48, 19, 43 }, 154, {}, 139, 137 },
			{ { 31, 30, 27, 27, 29, 7, 49, 36 }, { 32, 47, 35, 20, 43, 24, 39, 24 }, 213, {}, 202, 199 },
			{ { 41, 20, 43, 42, 22, 9, 8, 17 }, { 36, 48, 13, 49, 23, 35, 31, 48 }, 266, {}, 194, 193 },
			{ { 20, 22, 18, 32, 48, 47, 15, 41 }, { 5, 38, 32, 29, 21, 12, 11, 37 }, 146, {}, 210, 203 },
			{ { 9, 27, 36, 43, 48, 49, 37, 23 }, { 7, 41, 26, 14, 33, 34, 33, 26 }, 188, {}, 249, 245 },
			{ { 46, 17, 9, 22, 30, 36, 3, 27 }, { 24, 46, 24, 29, 32, 34, 4, 24 }, 212, {}, 181, 173 },
			{ { 38, 30, 47, 24, 26, 9, 13, 1 }, { 34, 43, 49, 38, 20, 18, 31, 5 }, 178, {}, 154, 151 },
			// number of objects = 9
			{ { 49, 31, 28, 12, 36, 29, 9, 11, 4 }, { 31, 1, 4, 34, 5, 25, 12, 38, 38 }, 142, {}, 196, 194 },
			{ { 20, 20, 20, 19, 35, 24, 38, 43, 47 }, { 20, 22, 33, 43, 24, 40, 9, 25, 7 }, 171, {}, 227, 223 },
			{ { 17, 11, 49, 22, 33, 5, 8, 2, 18 }, { 17, 50, 5, 19, 16, 11, 28, 14, 8 }, 144, {}, 158, 154 },
			{ { 26, 2, 21, 2, 2, 18, 4, 41, 26 }, { 7, 7, 47, 11, 12, 1, 43, 34, 46 }, 201, {}, 140, 138 },
			{ { 34, 8, 24, 29, 36, 15, 28, 37, 46 }, { 27, 4, 19, 38, 32, 23, 13, 45, 11 }, 173, {}, 220, 213 },
			{ { 6, 2, 48, 8, 3, 35, 12, 3, 32 }, { 2, 33, 13, 22, 50, 14, 35, 15, 50 }, 212, {}, 147, 146 },
			{ { 29, 40, 20, 25, 19, 19, 27, 29, 20 }, { 39, 20, 28, 27, 15, 24, 39, 10, 19 }, 209, {}, 209, 201 },
			{ { 42, 49, 42, 17, 2, 15, 13, 23, 46 }, { 9, 27, 13, 16, 1, 25, 5, 48, 43 }, 175, {}, 234, 226 },
			{ { 16, 8, 20, 38, 1, 8, 21, 10, 29 }, { 43, 19, 8, 19, 3, 17, 34, 7, 11 }, 156, {}, 143, 135 },
			{ { 6, 13, 38, 26, 25, 36, 4, 22, 21 }, { 8, 27, 2, 16, 18, 8, 4, 48, 15 }, 118, {}, 174, 169 },
			// number of objects = 10
			{ { 6, 36, 46, 48, 49, 17, 12, 49, 44, 29 }, { 15, 21, 31, 24, 25, 47, 49, 24, 23, 36 }, 280, {}, 330, 324 },
			{ { 8, 46, 45, 7, 11, 22, 48, 39, 34, 35 }, { 16, 37, 14, 13, 12, 20, 28, 46, 22, 38 }, 238, {}, 288, 287 },
			{ { 37, 46, 24, 16, 18, 33, 10, 35, 6, 42 }, { 34, 26, 40, 46, 7, 1, 16, 12, 15, 18 }, 199, {}, 257, 251 },
			{ { 5, 23, 38, 8, 25, 38, 11, 9, 25, 49 }, { 27, 9, 28, 46, 34, 31, 15, 4, 46, 36 }, 262, {}, 226, 223 },
			{ { 15, 43, 30, 8, 37, 28, 12, 19, 46, 30 }, { 35, 44, 21, 12, 17, 45, 37, 41, 5, 26 }, 234, {}, 248, 241 },
			{ { 26, 39, 7, 7, 13, 36, 14, 41, 32, 9 }, { 13, 35, 13, 34, 48, 21, 10, 50, 38, 29 }, 238, {}, 208, 204 },
			{ { 47, 19, 44, 46, 47, 3, 13, 15, 42, 9 }, { 40, 24, 50, 14, 15, 44, 13, 35, 20, 15 }, 216, {}, 273, 267 },
			{ { 4, 14, 7, 7, 8, 23, 47, 7, 13, 26 }, { 33, 43, 16, 11, 38, 33, 11, 28, 43, 39 }, 238, {}, 145, 144 },
			{ { 13, 17, 11, 38, 46, 27, 48, 28, 6, 17 }, { 31, 36, 18, 20, 16, 42, 6, 4, 3, 29 }, 192, {}, 240, 238 },
			{ { 18, 38, 13, 40, 48, 46, 23, 18, 48, 8 }, { 30, 21, 47, 31, 25, 45, 18, 45, 15, 24 }, 291, {}, 292, 287 },

			// Test cases which do ALWAYS match greedy algo results:
			// number of objects = 3
			{ { 6, 11, 6 }, { 12, 5, 5 }, 19, {}, 17, 17 },
			{ { 2, 2, 8 }, { 4, 12, 3 }, 17, {}, 10, 10 },
			{ { 5, 10, 3 }, { 11, 9, 12 }, 28, {}, 15, 15 },
			{ { 44, 36, 16 }, { 13, 3, 35 }, 48, {}, 80, 80 },
			{ { 37, 3, 21 }, { 36, 5, 42 }, 67, {}, 40, 40 },
			{ { 10, 48, 1 }, { 30, 36, 49 }, 102, {}, 58, 58 },
			{ { 33, 35, 11 }, { 4, 36, 16 }, 51, {}, 68, 68 },
			{ { 26, 8, 14 }, { 41, 27, 40 }, 85, {}, 40, 40 },
			{ { 43, 35, 25 }, { 21, 20, 5 }, 37, {}, 68, 68 },
			{ { 37, 34, 23 }, { 48, 7, 10 }, 52, {}, 57, 57 },
			{ { 32, 24, 3 }, { 10, 44, 40 }, 86, {}, 56, 56 },
			{ { 21, 22, 24 }, { 47, 27, 37 }, 103, {}, 46, 46 },
			{ { 40, 43, 16 }, { 13, 10, 30 }, 49, {}, 83, 83 },
			// number of objects = 4
			{ { 42, 45, 18, 36 }, { 44, 38, 29, 45 }, 135, {}, 123, 123 },
			{ { 2, 39, 29, 19 }, { 27, 46, 35, 8 }, 113, {}, 87, 87 },
			{ { 41, 10, 32, 23 }, { 7, 23, 37, 17 }, 75, {}, 96, 96 },
			{ { 26, 8, 15, 19 }, { 28, 35, 7, 40 }, 86, {}, 60, 60 },
			{ { 5, 2, 13, 15 }, { 12, 27, 9, 3 }, 38, {}, 33, 33 },
			{ { 25, 45, 49, 34 }, { 1, 13, 3, 2 }, 16, {}, 108, 108 },
			{ { 7, 25, 31, 2 }, { 24, 26, 43, 10 }, 94, {}, 63, 63 },
			{ { 48, 20, 43, 46 }, { 30, 42, 27, 10 }, 82, {}, 137, 137 },
			{ { 34, 20, 21, 22 }, { 31, 39, 9, 29 }, 92, {}, 77, 77 },
			{ { 15, 12, 29, 16 }, { 24, 50, 1, 37 }, 91, {}, 60, 60 },
			// number of objects = 5
			{ { 16, 8, 46, 2, 34 }, { 24, 33, 34, 21, 20 }, 122, {}, 104, 104 },
			{ { 43, 18, 41, 44, 22 }, { 37, 43, 12, 1, 45 }, 125, {}, 150, 150 },
			{ { 40, 32, 45, 18, 11 }, { 32, 30, 29, 26, 3 }, 100, {}, 128, 128 },
			{ { 46, 29, 18, 23, 36 }, { 1, 22, 49, 42, 18 }, 104, {}, 134, 134 },
			{ { 48, 47, 3, 48, 30 }, { 47, 9, 48, 29, 7 }, 120, {}, 173, 173 },
			{ { 11, 2, 16, 31, 25 }, { 25, 33, 50, 29, 1 }, 120, {}, 83, 83 },
			{ { 11, 3, 5, 33, 10 }, { 5, 26, 8, 24, 14 }, 73, {}, 59, 59 },
			{ { 48, 4, 43, 45, 21 }, { 10, 6, 36, 43, 36 }, 99, {}, 140, 140 },
			{ { 21, 24, 22, 32, 3 }, { 32, 31, 21, 7, 39 }, 100, {}, 99, 99 },
			{ { 16, 2, 1, 23, 27 }, { 2, 25, 21, 9, 6 }, 51, {}, 68, 68 },
			// number of objects = 6
			{ { 20, 5, 10, 40, 15, 25 }, { 1, 2, 3, 8, 7, 4 }, 10, {}, 60, 60 },
			{ { 24, 31, 34, 38, 18, 37 }, { 45, 47, 43, 1, 4, 21 }, 132, {}, 158, 158 },
			{ { 26, 43, 17, 32, 40, 28 }, { 30, 7, 36, 16, 26, 39 }, 117, {}, 158, 158 },
			{ { 39, 2, 39, 22, 8, 40 }, { 46, 19, 39, 46, 18, 42 }, 168, {}, 128, 128 },
			{ { 33, 13, 6, 22, 14, 25 }, { 35, 3, 16, 10, 44, 49 }, 117, {}, 99, 99 },
			{ { 8, 1, 37, 13, 6, 4 }, { 34, 19, 25, 35, 47, 50 }, 182, {}, 65, 65 },
			{ { 7, 14, 6, 38, 20, 11 }, { 18, 1, 30, 18, 10, 17 }, 87, {}, 90, 90 },
			{ { 8, 26, 10, 3, 49, 17 }, { 1, 7, 34, 21, 27, 11 }, 98, {}, 110, 110 },
			{ { 6, 10, 46, 27, 48, 39 }, { 43, 47, 2, 6, 42, 16 }, 124, {}, 170, 170 },
			{ { 5, 40, 17, 27, 7, 9 }, { 38, 23, 26, 29, 14, 1 }, 113, {}, 100, 100 },
			{ { 28, 9, 12, 30, 3, 22 }, { 34, 19, 4, 34, 45, 29 }, 158, {}, 101, 101 },
			// number of objects = 7
			{ { 21, 3, 23, 31, 31, 20, 43 }, { 9, 32, 25, 13, 31, 29, 16 }, 150, {}, 169, 169 },
			{ { 9, 35, 30, 22, 35, 21, 3 }, { 39, 36, 4, 28, 34, 42, 49 }, 218, {}, 152, 152 },
			{ { 5, 43, 48, 44, 33, 13, 45 }, { 49, 1, 1, 35, 49, 42, 43 }, 195, {}, 226, 226 },
			{ { 44, 18, 13, 12, 19, 7, 1 }, { 1, 26, 19, 40, 45, 47, 15 }, 164, {}, 107, 107 },
			{ { 4, 1, 34, 28, 39, 19, 36 }, { 37, 17, 22, 8, 12, 15, 1 }, 106, {}, 160, 160 },
			{ { 43, 25, 11, 45, 8, 43, 27 }, { 38, 21, 48, 19, 41, 40, 23 }, 186, {}, 191, 191 },
			{ { 25, 32, 25, 46, 49, 11, 15 }, { 4, 41, 5, 20, 45, 4, 6 }, 101, {}, 171, 171 },
			{ { 21, 30, 20, 19, 5, 14, 48 }, { 31, 40, 36, 29, 12, 5, 4 }, 146, {}, 152, 152 },
			{ { 17, 44, 36, 40, 5, 29, 12 }, { 46, 47, 20, 49, 4, 33, 26 }, 193, {}, 166, 166 },
			{ { 8, 11, 35, 17, 40, 38, 33 }, { 11, 49, 35, 42, 1, 39, 37 }, 181, {}, 171, 171 },
			// number of objects = 8
			{ { 4, 1, 9, 9, 14, 36, 11, 14 }, { 46, 15, 46, 16, 7, 5, 32, 26 }, 171, {}, 94, 94 },
			{ { 2, 4, 13, 19, 46, 33, 45, 34 }, { 24, 18, 4, 38, 34, 48, 31, 15 }, 173, {}, 190, 190 },
			{ { 24, 29, 20, 20, 17, 32, 30, 26 }, { 10, 9, 46, 12, 3, 13, 43, 45 }, 152, {}, 178, 178 },
			{ { 1, 17, 20, 12, 34, 29, 7, 6 }, { 40, 20, 28, 50, 15, 23, 41, 37 }, 215, {}, 125, 125 },
			{ { 26, 39, 10, 42, 38, 34, 3, 38 }, { 6, 47, 35, 43, 50, 12, 22, 24 }, 217, {}, 227, 227 },
			{ { 21, 37, 24, 36, 47, 47, 22, 14 }, { 48, 49, 26, 49, 9, 17, 42, 48 }, 267, {}, 234, 234 },
			{ { 23, 37, 48, 37, 8, 19, 24, 32 }, { 48, 34, 30, 29, 24, 40, 32, 14 }, 198, {}, 201, 201 },
			{ { 32, 48, 8, 41, 2, 35, 6, 44 }, { 44, 38, 30, 46, 45, 15, 47, 47 }, 271, {}, 214, 214 },
			{ { 5, 25, 46, 28, 42, 2, 10, 15 }, { 44, 3, 5, 28, 36, 36, 46, 4 }, 169, {}, 171, 171 },
			{ { 27, 20, 32, 15, 7, 43, 8, 19 }, { 25, 26, 15, 46, 2, 43, 35, 28 }, 184, {}, 156, 156 },
			// number of objects = 9
			{ { 27, 37, 19, 17, 8, 31, 39, 29, 10 }, { 26, 37, 21, 5, 43, 38, 25, 44, 9 }, 228, {}, 209, 209 },
			{ { 35, 47, 2, 5, 18, 45, 35, 37, 6 }, { 43, 48, 49, 29, 13, 20, 5, 11, 2 }, 187, {}, 228, 228 },
			{ { 11, 43, 13, 26, 15, 19, 2, 33, 42 }, { 42, 1, 43, 41, 27, 14, 32, 26, 8 }, 196, {}, 193, 193 },
			{ { 34, 16, 38, 23, 6, 19, 23, 18, 47 }, { 3, 32, 20, 3, 6, 38, 34, 45, 30 }, 173, {}, 206, 206 },
			{ { 17, 10, 26, 30, 11, 10, 16, 48, 23 }, { 8, 21, 14, 20, 3, 48, 27, 13, 37 }, 148, {}, 181, 181 },
			{ { 2, 21, 42, 15, 30, 1, 13, 22, 1 }, { 46, 15, 50, 30, 14, 32, 49, 2, 36 }, 243, {}, 146, 146 },
			{ { 32, 47, 48, 38, 15, 33, 8, 32, 37 }, { 31, 15, 3, 48, 34, 30, 37, 17, 13 }, 189, {}, 267, 267 },
			{ { 33, 11, 3, 13, 48, 44, 18, 38, 27 }, { 45, 28, 30, 49, 34, 29, 31, 44, 38 }, 259, {}, 219, 219 },
			{ { 33, 28, 4, 28, 3, 24, 40, 14, 41 }, { 11, 13, 38, 47, 31, 36, 9, 14, 14 }, 193, {}, 212, 212 },
			{ { 14, 46, 49, 27, 45, 7, 14, 43, 46 }, { 8, 48, 35, 50, 14, 18, 11, 45, 27 }, 199, {}, 257, 257 },
			// number of objects = 10
			{ { 23, 17, 15, 30, 9, 32, 12, 14, 1, 4 }, { 27, 1, 9, 20, 36, 35, 14, 22, 35, 46 }, 230, {}, 156, 156 },
			{ { 49, 25, 25, 30, 14, 41, 13, 3, 35, 45 }, { 44, 18, 20, 37, 12, 4, 26, 47, 34, 32 }, 210, {}, 264, 264 },
			{ { 11, 13, 6, 31, 3, 27, 9, 21, 47, 26 }, { 31, 5, 24, 20, 44, 39, 42, 20, 15, 20 }, 252, {}, 191, 191 },
			{ { 49, 23, 8, 49, 43, 29, 5, 38, 39, 41 }, { 10, 10, 17, 24, 49, 13, 50, 1, 3, 26 }, 152, {}, 311, 311 },
			{ { 45, 31, 24, 8, 18, 26, 44, 42, 10, 15 }, { 6, 50, 11, 37, 44, 26, 25, 40, 39, 15 }, 246, {}, 245, 245 },
			{ { 23, 27, 15, 42, 15, 21, 29, 35, 17, 35 }, { 36, 9, 40, 26, 39, 27, 8, 14, 25, 12 }, 202, {}, 244, 244 },
			{ { 23, 25, 5, 41, 1, 36, 29, 33, 8, 47 }, { 8, 19, 12, 27, 43, 32, 50, 18, 42, 10 }, 247, {}, 247, 247 },
			{ { 47, 20, 9, 31, 9, 47, 7, 47, 46, 40 }, { 46, 23, 33, 40, 23, 12, 33, 39, 11, 37 }, 252, {}, 287, 287 },
			{ { 28, 33, 28, 28, 28, 26, 3, 16, 28, 43 }, { 9, 4, 38, 20, 13, 17, 46, 20, 10, 24 }, 156, {}, 258, 258 },
			{ { 36, 45, 41, 15, 23, 3, 13, 38, 35, 24 }, { 7, 20, 32, 23, 14, 32, 48, 8, 6, 22 }, 190, {}, 270, 270 },

			// Following examples added in GPC-3
			{{10, 9, 16}, {2, 3, 4}, 6, {}, 26, 26},
			{{12, 15, 16}, {2, 3, 4}, 6, {}, 28, 27},
			{{10, 15, 12}, {2, 3, 4}, 6, {}, 25, 25},
			{{43, 27, 25, 32}, {40, 23, 4, 41}, 105, {}, 102, 95},
			{{35, 45, 40, 44, 36}, {34, 32, 46, 25, 10}, 123, {}, 165, 160},

			//Test cases that failed during dev testing
			{
				{ 29352, 8415, 97059, 60093, 91498, 98133, 26568, 5724, 90284, 82296, 85149, 38167, 40100, 16679, 10567, 17909, 266, 46000, 19706, 40233, 7970 },
				{ 552, 315, 759, 693, 598, 933, 468, 324, 284, 396, 549, 367, 500, 479, 667, 809, 266, 100, 806, 633, 770 },
				9014,
				{},
				879226,
				875456
			},

			{
				{ 17255, 63385, 30642, 99635, 49581, 87630, 25520, 31756, 83438, 85551, 54774, 2370, 96608, 64705, 67594, 17946, 26942, 83800, 27603 },
				{ 155, 385, 942, 635, 981, 330, 320, 256, 638, 951, 774, 570, 308, 805, 994, 846, 842, 100, 603 },
				10748,
				{},
				998789,
				998789
			}

		};

		return testDataVector;
	}
}
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

#include "DynamicProgramming/DP_MinCutsStolenNecklaceKGemsNPeople_recursive_v1.h"
#include "DynamicProgramming/DP_MinCutsStolenNecklaceKGemsNPeople_recursive_v2.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

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

	template<typename Fun>
	void executeTest(const std::string& str, size_t testCaseNo, Fun fun, MinCutsStolenNecklaceTestData& data)
	{
		//minCuts = MinCutsStolenNecklaceKGemsNPeople_v1::getMinCutsStolenNecklaceKGemsNPeople_recursive(data.numPeople, data.necklace, data.expectedDistribution, data.results);
			//MM_TIMED_EXPECT_TRUE(MinCutsStolenNecklaceKGemsNPeople_v1::getMinCutsStolenNecklaceKGemsNPeople_recursive(data.numPeople, data.necklace, data.expectedDistribution, data.results));
		long long timens;
		int minCuts = MM_Measure<int>::time(timens, fun,
			data.numPeople, data.necklace, data.expectedDistribution, data.results);
		cout << "\n" << setw(15) << str
			<< "   Test case: " << setw(4) << testCaseNo
			<< "   time(ns): " << setw(20) << timens << " ns"
			<< "   minCuts: " << (data.results.empty() ? -1 : data.results[0].minCuts);
		bool result = data.isValidResult();
		MM_EXPECT_TRUE(result == true, result);
		//data.printResults();
	}

	MM_DECLARE_FLAG(DP_MinCutsStolenNecklaceKGems2People);

	MM_UNIT_TEST(DP_MinCutsStolenNecklaceKGems2People_test_1, DP_MinCutsStolenNecklaceKGems2People)
	{
		MM_PRINT_TEST_CASE_NUMBER(false);
		cout.imbue(std::locale(""));

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

			//Create positive test cases
			testData[i].createRandomExpectedDistribution(testData[i].numGemsToDistribute, 1);
			executeTest("recursive_v1", i, MinCutsStolenNecklaceKGemsNPeople_recursive_v1::getMinCutsStolenNecklaceKGemsNPeople, testData[i]);
			executeTest("recursive_v2", i, MinCutsStolenNecklaceKGemsNPeople_recursive_v2::getMinCutsStolenNecklaceKGemsNPeople, testData[i]);

			//Create negative test cases
			testData[i].expectedDistribution.clear();
			testData[i].results.clear();
			testData[i].createRandomExpectedDistribution(testData[i].numGemsToDistribute, 3);
			executeTest("recursive_v1", i, MinCutsStolenNecklaceKGemsNPeople_recursive_v1::getMinCutsStolenNecklaceKGemsNPeople, testData[i]);
			executeTest("recursive_v2", i, MinCutsStolenNecklaceKGemsNPeople_recursive_v2::getMinCutsStolenNecklaceKGemsNPeople, testData[i]);
		}
	}
}

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <random>
#include <utility>
#include <algorithm>
//#include <cwchar>
using namespace std;

#include "DynamicProgramming/DP_MinCutsStolenNecklaceKGemsNPeople_recursive_v1.h"
#include "DynamicProgramming/DP_MinCutsStolenNecklaceKGemsNPeople_recursive_v2.h"
#include "DynamicProgramming/DP_MinCutsStolenNecklaceKGemsNPeople_recursive_v3.h"
#include "DynamicProgramming/DP_MinCutsStolenNecklaceKGemsNPeople_recursive_v4.h"

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

	GemType getGemType(int index)
	{
		auto fillIn = []() {
			vector<GemType> gemTypes;
			gemTypes.reserve(100);

			for (GemType i = 'a'; i <= 'z'; ++i)
				gemTypes.push_back(i);
			for (GemType i = 'A'; i <= 'Z'; ++i)
				gemTypes.push_back(i);

			return gemTypes;
		};

		static vector<GemType> gemTypes = fillIn();
		if (index >= gemTypes.size())
			throw std::out_of_range{ "index out of range" };

		return gemTypes[index];
	}

	struct MinCutsStolenNecklaceTestData
	{
		int numPeople;
		int numGemTypes;
		int numGemsInNecklace;
		int numGemsToDistribute;
		vector<GemType> necklace;
		vector<unordered_map<GemType, Count>> expectedDistribution;
		bool symmetricDistribution;
		vector<MinCutsStolenNecklaceResults> results;

		void createRandomNecklaceForSymmetricDistribution()
		{
			if (!necklace.empty())
				return;

			int numGemsPerPerson = numGemsToDistribute / numPeople;
			numGemsToDistribute = numGemsPerPerson * numPeople;
			numGemsInNecklace = numGemsToDistribute;
			unordered_map<GemType, Count> individualDist;
			//vector<int> numGemsOfEachType;
			//numGemsOfEachType.reserve(numGemTypes);
			std::random_device rd;
			std::mt19937 mt(rd());
			int totalGems = 0;
			int i = 0;
			for (; i < numGemTypes - 1; ++i)
			{
				int numGems = 0;
				int remainingGems = numGemTypes - i - 1;
				int availableGems = numGemsPerPerson - remainingGems - totalGems;
				if (availableGems >= 1)
				{
					std::uniform_int_distribution<int> dist(1, availableGems);
					numGems = dist(mt);
				}
				totalGems += numGems;
				individualDist[getGemType(i)] = numGems;
			}
			individualDist[getGemType(i)] = numGemsPerPerson - totalGems;

			for (int i = 0; i < numGemTypes; ++i)
			{
				for (int j = 0; j < individualDist[getGemType(i)] * numPeople; ++j)
				{
					necklace.push_back(getGemType(i));
				}
			}

			if (necklace.size() != numGemsToDistribute)
				throw std::runtime_error{ "necklace.size() != numGemsToDistribute" };

			std::uniform_int_distribution<int> dist(0, numGemTypes - 1);
			for (int i = 0; i < numGemsInNecklace - numGemsToDistribute; ++i)
			{
				necklace.push_back(getGemType(dist(mt)));
			}

			//Generate random permutation
			for (int i = 0; i < necklace.size(); ++i)
			{
				std::uniform_int_distribution<int> dist(i, static_cast<int>(necklace.size()) - 1);
				int j = dist(mt);
				std::swap(necklace[i], necklace[j]);
			}

			//Create symmetric distribution
			expectedDistribution.reserve(numPeople);
			for (int i = 0; i < numPeople; ++i)
				expectedDistribution.push_back(individualDist);
		}

		void createRandomNecklace()
		{
			if (!necklace.empty())
				return;

			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_int_distribution<int> dist(0, numGemTypes - 1);
			necklace.reserve(numGemsInNecklace);
			for (int i = 0; i < numGemsInNecklace; ++i)
			{
				necklace.push_back(getGemType(dist(mt)));
			}
		}
		
		void createRandomExpectedDistribution(int numGemsToDistribute, int increaseCountBy)
		{
			if (!expectedDistribution.empty())
				return;

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
			for (int i = 0; i < numGemTypes; ++i)
			{
				GemType type = getGemType(i);
				cout << setw(colWidth) << type;
			}
			cout << "\n" << setw(colWidth) << "PersonIndex" << " : ";
			for (size_t personIndex = 0; personIndex < expectedDistribution.size(); ++personIndex)
			{
				cout << "\n" << setw(colWidth) << personIndex << " : ";
				for (int i = 0; i < numGemTypes; ++i)
				{
					GemType type = getGemType(i);
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
	int executeTest(const std::string& str, size_t testCaseNo, Fun fun, MinCutsStolenNecklaceTestData& data, long long& timens)
	{
		//minCuts = MinCutsStolenNecklaceKGemsNPeople_v1::getMinCutsStolenNecklaceKGemsNPeople_recursive(data.numPeople, data.necklace, data.expectedDistribution, data.results);
			//MM_TIMED_EXPECT_TRUE(MinCutsStolenNecklaceKGemsNPeople_v1::getMinCutsStolenNecklaceKGemsNPeople_recursive(data.numPeople, data.necklace, data.expectedDistribution, data.results));
		data.results.clear();
		long long timeoutmillisec = 5 * 1000; //10 sec
		int minCuts = MM_Measure<int>::getInstance().time(timens, timeoutmillisec, fun,
			data.numPeople, data.necklace, data.expectedDistribution, data.results);
		int maxCuts = (data.numPeople - 1) * data.numGemTypes; //(k - 1) * t
		//wchar_t microsec = L'μ';
		cout << "\n"
			<< "Test#" << setw(3) << testCaseNo
			<< setw(15) << str
			<< "  People: " << setw(4) << data.numPeople
			<< "  GemTypes: " << setw(4) << data.numGemTypes
			<< "  necklaceLen: " << setw(4) << data.necklace.size()
			<< "  distribute: " << setw(4) << data.numGemsToDistribute
			<< "  time: " << setw(14) << timens / 1000 << " ms"
		//std::wcout << microsec; cout << "s";
			<< "  minCuts: " << setw(3) << (data.results.empty() ? -1 : data.results[0].minCuts)
			<< "  minCuts(funRet): " << setw(3) << minCuts
			<< "  maxCuts = (k-1)*t = " << setw(3) << maxCuts
			<< "  Solutions: " << setw(3) << data.results.size();
		bool result = data.isValidResult();
		if(!result || (data.symmetricDistribution && minCuts > maxCuts))
			data.printResults();
		MM_EXPECT_TRUE(result == true, result);
		if (data.symmetricDistribution && data.necklace.size() == data.numGemsToDistribute)
		{
			MM_EXPECT_TRUE(minCuts <= maxCuts, minCuts, maxCuts);
		}
		
		return minCuts;
	}

	void testAlgo()
	{
		//int t = 2;
		//std::vector<int> n{4, 6};
		std::vector< std::vector<int> > testData{
			{0,0,1,1},
			{0,1,2,2,0,1}
		};

		//int t;
		//cin >> t;
		for (int ti = 0; ti < testData.size(); ++ti)
		{
			const std::vector<int>& data = testData[ti];
			int n = static_cast<int>(data.size());
			//cin >> n;
			std::vector<int> cnt(500, 0), cut;
			for (int i = 0, k = 0; i < n; ++i)
			{
				int s = data[i];
				//cin >> s;
				if ((cnt[s]&1) != (k&1))
				{
					cut.push_back(i);
					++k;
				}
				++cnt[s];
			}

			cout << "\nCuts: " << cut.size() << "\n";

			for (int i = 0; i < cut.size(); ++i)
				cout << cut[i] << ' ';
			cout << "\n";
		}
		
	}

	MM_DECLARE_FLAG(DP_MinCutsStolenNecklaceKGems2People);

	MM_UNIT_TEST(DP_MinCutsStolenNecklaceKGems2People_test_1, DP_MinCutsStolenNecklaceKGems2People)
	{
		MM_PRINT_TEST_CASE_NUMBER(false);
		cout.imbue(std::locale(""));

		testAlgo();

		using MCSNTD = MinCutsStolenNecklaceTestData;
		vector<int> necklaceLenVec{ 10, 20, 50, 75, 100, 150, 200, 300, 500 };
		vector<int> numPeopleVec{ 2, 3, 4, 5, 8, 10, 15, 20, 30, 50, 75, 100 };
		vector<GemType> numGemTypesVec{ 1, 2, 3, 4, 5, 10, 20, 50 };
		vector<MCSNTD> testData;

		//Special cases found during dev testing
		testData.push_back({ 2, 2, 10, 10, vector<GemType>{'b',  'a',  'a',  'b',  'a',  'a',  'b',  'b',  'a',  'b'},
			vector<unordered_map<GemType, Count>>{ { {'a', 2 }, {'b', 5 } }, { {'a', 3 }, {'b', 0 } } }, false, vector<MinCutsStolenNecklaceResults>{} });

		for (int n : necklaceLenVec)
		{
			for (int k : numPeopleVec)
			{
				if (k >= n)
					continue;

				//Make n divisible by k
				n = (n / k) * k;

				for (int t : numGemTypesVec)
				{
					if (t >= n)
						continue;

					testData.push_back({ k, t, n, n, vector<GemType>{}, vector<unordered_map<GemType, Count>>{}, false, vector<MinCutsStolenNecklaceResults>{} });
					//testData.push_back({ k, t, n, n/2, vector<GemType>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{} });
					//testData.push_back({ k, t, n, n/3, vector<GemType>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{} });
				}
			}
		}

		//vector<MCSNTD> testData{
		//	//numPeople, numGemTypes, necklace length, gems from necklace to distribute
		//	MCSNTD{2, 1, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{2, 2, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{2, 3, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{2, 4, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{2, 5, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{3, 1, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{3, 2, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{3, 3, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{3, 4, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{3, 5, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{4, 1, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{4, 2, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{4, 3, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{4, 4, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{4, 5, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{5, 1, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{5, 2, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{5, 3, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{5, 4, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{5, 5, 10, 10, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},

		//	MCSNTD{2, 1, 10, 5, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{2, 2, 10, 6, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{2, 3, 10, 7, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{2, 4, 10, 8, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{2, 5, 10, 9, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{3, 1, 10, 5, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{3, 2, 10, 6, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{3, 3, 10, 7, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{3, 4, 10, 8, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{3, 5, 10, 9, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{4, 1, 10, 5, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{4, 2, 10, 6, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{4, 3, 10, 7, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{4, 4, 10, 8, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{4, 5, 10, 9, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{5, 1, 10, 5, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{5, 2, 10, 6, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{5, 3, 10, 7, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{5, 4, 10, 8, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//	MCSNTD{5, 5, 10, 9, vector<int>{}, vector<unordered_map<GemType, Count>>{}, vector<MinCutsStolenNecklaceResults>{}},
		//};

		bool needToInitialize = true;
		auto compareResultsWithPrevRun = [&needToInitialize](const vector<MinCutsStolenNecklaceResults>& results, int minCuts) {
			static int prevMinCuts = -1;
			static vector<MinCutsStolenNecklaceResults> prevRes{};
			if (needToInitialize)
			{
				needToInitialize = false;
				prevRes = results;
				prevMinCuts = minCuts;
				return;
			}

			MM_EXPECT_TRUE(results == prevRes, results, prevRes);
			MM_EXPECT_TRUE(minCuts == prevMinCuts, minCuts, prevMinCuts);
		};

		for (size_t i = 0; i < testData.size(); ++i)
		{
			for (int n = 0; n < 4; ++n)
			{
				switch (n)
				{
				case 0: //If the necklace and expectedDistribution is already defined while adding test case
					if (testData[i].necklace.empty() || testData[i].expectedDistribution.empty())
						continue;
					break;
				case 1:
					//Create symmetric distribution
					testData[i].createRandomNecklaceForSymmetricDistribution();
					break;
				case 2:
					//Create asymmetric distribution
					testData[i].createRandomNecklace();
					testData[i].createRandomExpectedDistribution(testData[i].numGemsToDistribute, 1);
					break;
				case 3:
					//Create negative test cases
					testData[i].createRandomNecklace();
					testData[i].createRandomExpectedDistribution(testData[i].numGemsToDistribute, 3);
					break;
				default:
					break;
				}
				
				needToInitialize = true; //initialize results on next first successful solution
				int minCuts = -1;
				long long timens;
				minCuts = executeTest("recursive_v1", i + 1, MinCutsStolenNecklaceKGemsNPeople_recursive_v1::getMinCutsStolenNecklaceKGemsNPeople, testData[i], timens);
				if(timens > 0) compareResultsWithPrevRun(testData[i].results, minCuts);
				testData[i].results.clear();

				minCuts = executeTest("recursive_v2", i + 1, MinCutsStolenNecklaceKGemsNPeople_recursive_v2::getMinCutsStolenNecklaceKGemsNPeople, testData[i], timens);
				if (timens > 0) compareResultsWithPrevRun(testData[i].results, minCuts);
				testData[i].results.clear();

				minCuts = executeTest("recursive_v3", i + 1, MinCutsStolenNecklaceKGemsNPeople_recursive_v3::getMinCutsStolenNecklaceKGemsNPeople, testData[i], timens);
				if (timens > 0) compareResultsWithPrevRun(testData[i].results, minCuts);
				testData[i].results.clear();

				minCuts = executeTest("recursive_v4", i + 1, MinCutsStolenNecklaceKGemsNPeople_recursive_v4::getMinCutsStolenNecklaceKGemsNPeople, testData[i], timens);
				if (timens > 0) compareResultsWithPrevRun(testData[i].results, minCuts);
				testData[i].results.clear();

				testData[i].necklace.clear();
				testData[i].expectedDistribution.clear();
			}
		}
	}
}

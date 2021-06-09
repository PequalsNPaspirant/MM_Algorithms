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
#include "DynamicProgramming/DP_MinCutsStolenNecklaceKGemsNPeople_branchAndBounds_v1.h"
#include "DynamicProgramming/DP_MinCutsStolenNecklaceKGemsNPeople_branchAndBounds_v2.h"

#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Utils/Utils_MM_Assert.h"

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

	int getGemIndex(GemType gem)
	{
		if (gem >= 'a')
			return gem - 'a';
		else if (gem >= 'A')
			return gem - 'A';

		return -1;
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

			symmetricDistribution = true;
			int numGemsPerPerson = numGemsToDistribute / numPeople;
			MM_Assert::mmRunTimeAssert(numGemsToDistribute == numGemsPerPerson * numPeople);
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

			symmetricDistribution = false;
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_int_distribution<int> dist(0, numGemTypes - 1);
			necklace.reserve(numGemsInNecklace);
			for (int i = 0; i < numGemsInNecklace; ++i)
			{
				necklace.push_back(getGemType(dist(mt)));
			}
		}
		
		void createRandomExpectedDistribution(int numGemsToDistributeIn, int increaseCountBy)
		{
			if (!expectedDistribution.empty())
				return;

			numGemsToDistribute = numGemsToDistributeIn * increaseCountBy;

			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_int_distribution<int> distPersonIndex(0, numPeople - 1);
			std::uniform_int_distribution<int> distSkipGems(0, 100);
			expectedDistribution.resize(numPeople);
			//distribute the gems randomly
			int gemsToSkip = numGemsInNecklace - numGemsToDistributeIn;
			for (int gemIndex = 0; gemIndex < numGemsInNecklace && numGemsToDistributeIn > 0; ++gemIndex)
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
				--numGemsToDistributeIn;
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

		void printResults() const
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
					const unordered_map<GemType, Count>& currPerDist = expectedDistribution[personIndex];
					GemType type = getGemType(i);
					auto it = currPerDist.find(type);
					if (it != currPerDist.end())
					{
						const Count& c = it->second;
						cout << setw(colWidth) << c;
					}
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



	int getMinCuts_v1(int numPeople, const vector<GemType>& necklace,
		const vector<unordered_map<GemType, Count>>& expectedDistribution, vector<MinCutsStolenNecklaceResults>& results)
	{
		//int t;
		//cin >> t;
		//for (int i = 0; i < t; ++i) 
		{
			int n;
			//cin >> n;
			n = static_cast<int>(necklace.size());
			vector<int> cnt(500, 0), cut;
			for (int i = 0, k = 0; i < n; ++i)
			{
				int s;
				//cin >> s;
				s = getGemIndex(necklace[i]);
				if ((cnt[s] & 1) != (k & 1))
				{
					cut.push_back(i);
					++k;
				}
				++cnt[s];
			}
			//cout << cut.size() << endl;
			results.resize(1);
			results[0].minCuts = static_cast<int>(cut.size());
			int owner = 0;
			int j = 0;
			for (int i = 0; i < cut.size(); ++i)
			{
				//cout << cut[i] << (i + 1 == cut.size() ? '\n' : ' ');
				for (; j < cut[i]; ++j)
					results[0].owners.push_back(owner);

				owner = 1 - owner;
			}
			for (; j < necklace.size(); ++j)
				results[0].owners.push_back(owner);
		}

		return results[0].minCuts;
	}

	int getMinCuts_v2(int numPeople, const vector<GemType>& necklace,
		const vector<unordered_map<GemType, Count>>& expectedDistribution, vector<MinCutsStolenNecklaceResults>& results)
	{
		const int MAXN = 1002;
		int n, a[MAXN], b[MAXN], need[2][MAXN];
		int sol[MAXN];

		int runs;
		//scanf("%d", &runs);
		runs = 1;
		//assert(1 <= runs && runs <= 30);
		MM_Assert::mmRunTimeAssert(1 <= runs && runs <= 30);
		while (runs--)
		{
			//scanf("%d", &n);
			n = static_cast<int>(necklace.size());
			//assert(2 <= n && n <= 1000 && n % 2 == 0);
			MM_Assert::mmRunTimeAssert(2 <= n && n <= 1000 && n % 2 == 0);
			for (int i = 0; i < n; i++)
			{
				//scanf("%d", &a[i]);
				a[i] = getGemIndex(necklace[i]) + 1;
				//assert(1 <= a[i] && a[i] <= 500);
				MM_Assert::mmRunTimeAssert(1 <= a[i] && a[i] <= 500);
			}
			memset(b, 0, sizeof b);
			for (int i = 0; i < n; i++) b[a[i]]++;
			for (int i = 1; i <= 500; i++) need[0][i] = need[1][i] = b[i] / 2;
			int p = 0, sz = 0;
			for (int i = 0; i < n; i++)
			{
				if (need[p][a[i]] > 0) need[p][a[i]]--;
				else
				{
					sol[sz++] = i;
					p = 1 - p;
					i--;
				}
			}
			//printf("%d\n", sz);
			results.resize(1);
			results[0].minCuts = sz;
			int owner = 0;
			int j = 0;
			for (int i = 0; i < sz; i++)
			{
				//if (i > 0) printf(" ");
				//printf("%d", sol[i]);
				for (; j < sol[i]; ++j)
					results[0].owners.push_back(owner);

				owner = 1 - owner;
			}
			for (; j < necklace.size(); ++j)
				results[0].owners.push_back(owner);
			//printf("\n");
		}

		return results[0].minCuts;
	}



	template<typename Fun>
	int executeTest(const std::string& str, size_t testCaseNo, Fun fun, MinCutsStolenNecklaceTestData& data, long long& timens, bool exactSolution)
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
			<< "  time: " << setw(14) << timens / 1000 << " us"
		//std::wcout << microsec; cout << "s";
			<< "  minCuts: " << setw(3) << (data.results.empty() ? -1 : data.results[0].minCuts)
			<< "  minCuts(funRet): " << setw(3) << minCuts
			<< "  maxCuts = (k-1)*t = " << setw(3) << maxCuts
			<< "  Solutions: " << setw(3) << data.results.size();
		
		if (timens > 0)
		{
			bool result = data.isValidResult();
			if (!result)
				data.printResults();
			MM_EXPECT_TRUE(result == true, result);

			if (data.symmetricDistribution && exactSolution && data.necklace.size() == data.numGemsToDistribute)
			{
				if (minCuts > maxCuts)
					data.printResults();
				MM_EXPECT_TRUE(minCuts <= maxCuts,
					data.symmetricDistribution, exactSolution, data.necklace.size(), data.numGemsToDistribute, minCuts, maxCuts);
			}

			if (!data.results.empty())
			{
				MM_EXPECT_TRUE(minCuts == data.results[0].minCuts, minCuts, data.results[0].minCuts);
				int minCutsBackup = data.results[0].minCuts;
				data.results[0].calculateMinCuts();
				MM_EXPECT_TRUE(minCutsBackup == data.results[0].minCuts, minCutsBackup, data.results[0].minCuts);
				data.results[0].minCuts = minCutsBackup; //restore it back in case its different
			}
			//data.printResults();
		}
		
		return minCuts;
	}

	MM_DECLARE_FLAG(DP_MinCutsStolenNecklaceKGems2People);

	MM_UNIT_TEST(DP_MinCutsStolenNecklaceKGems2People_test_1, DP_MinCutsStolenNecklaceKGems2People)
	{
		MM_PRINT_TEST_CASE_NUMBER(false);
		MM_SET_PAUSE_ON_ERROR(false);
		cout.imbue(std::locale(""));

		using MCSNTD = MinCutsStolenNecklaceTestData;
		vector<int> numPeopleVec{ 2, 3, 4, 5, 8, 10, 15, 20, 30, 50, 75, 100 };
		vector<GemType> numGemTypesVec{ 1, 2, 3, 4, 5, 10, 20, 50 };
		vector<int> necklaceLenVec{ 10, 20, 50, 75, 100, 150, 200, 300, 500 };
		vector<MCSNTD> testData;

		//Special cases found during dev testing
		testData.push_back({ 2, 1, 20, 6, vector<GemType>{'a',  'a',  'a',  'a',  'a',  'a',  'a',  'a',  'a',  'a',  'a',  'a',  'a',  'a',  'a',  'a',  'a',  'a',  'a',  'a'},
			vector<unordered_map<GemType, Count>>{ { {'a', 4 } }, { {'a', 2 } } }, false, vector<MinCutsStolenNecklaceResults>{} });

		testData.push_back({ 2, 2, 10, 10, vector<GemType>{'b',  'a',  'a',  'b',  'a',  'a',  'b',  'b',  'a',  'b'},
			vector<unordered_map<GemType, Count>>{ { {'a', 2 }, {'b', 5 } }, { {'a', 3 }, {'b', 0 } } }, false, vector<MinCutsStolenNecklaceResults>{} });

		for (int k : numPeopleVec)
		{
			for (int t : numGemTypesVec)
			{
				for (int n : necklaceLenVec)
				{
					if (n <= k)
						continue;

					//Make n divisible by k
					n = (n / k) * k;

					if (n <= t)
						continue;

					testData.push_back({ k, t, n, n, vector<GemType>{}, vector<unordered_map<GemType, Count>>{}, false, vector<MinCutsStolenNecklaceResults>{} });
					int gemsToDistribute = n / 2;
					//Make gemsToDustribute divisible by k
					gemsToDistribute = (gemsToDistribute / k) * k;
					testData.push_back({ k, t, n, gemsToDistribute, vector<GemType>{}, vector<unordered_map<GemType, Count>>{}, false, vector<MinCutsStolenNecklaceResults>{} });
					gemsToDistribute = n / 3;
					//Make gemsToDustribute divisible by k
					gemsToDistribute = (gemsToDistribute / k) * k;
					testData.push_back({ k, t, n, gemsToDistribute, vector<GemType>{}, vector<unordered_map<GemType, Count>>{}, false, vector<MinCutsStolenNecklaceResults>{} });
				}
			}
		}

		bool needToInitialize = true;
		auto compareResultsWithPrevRun = [&needToInitialize](const MinCutsStolenNecklaceTestData& data, int minCuts) {
			static int prevMinCuts = -1;
			static vector<MinCutsStolenNecklaceResults> prevRes{};
			vector<MinCutsStolenNecklaceResults> results = data.results;
			std::sort(results.begin(), results.end());
			if (needToInitialize)
			{
				needToInitialize = false;
				prevRes = results;
				prevMinCuts = minCuts;
				return;
			}

			if (results != prevRes || minCuts != prevMinCuts)
				data.printResults();

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

				std::cout << "\n" << "-------------------------------------------------------------------------------------";

				//Recursive (exponential)
				minCuts = executeTest("recursive_v1", i + 1, MinCutsStolenNecklaceKGemsNPeople_recursive_v1::getMinCutsStolenNecklaceKGemsNPeople, testData[i], timens, true);
				if(timens > 0 && testData[i].numGemsInNecklace == testData[i].numGemsToDistribute) compareResultsWithPrevRun(testData[i], minCuts);

				minCuts = executeTest("recursive_v2", i + 1, MinCutsStolenNecklaceKGemsNPeople_recursive_v2::getMinCutsStolenNecklaceKGemsNPeople, testData[i], timens, true);
				if (timens > 0 && testData[i].numGemsInNecklace == testData[i].numGemsToDistribute) compareResultsWithPrevRun(testData[i], minCuts);

				minCuts = executeTest("recursive_v3", i + 1, MinCutsStolenNecklaceKGemsNPeople_recursive_v3::getMinCutsStolenNecklaceKGemsNPeople, testData[i], timens, true);
				if (timens > 0 && testData[i].numGemsInNecklace == testData[i].numGemsToDistribute) compareResultsWithPrevRun(testData[i], minCuts);

				minCuts = executeTest("recursive_v4", i + 1, MinCutsStolenNecklaceKGemsNPeople_recursive_v4::getMinCutsStolenNecklaceKGemsNPeople, testData[i], timens, true);
				if (timens > 0 && testData[i].numGemsInNecklace == testData[i].numGemsToDistribute) compareResultsWithPrevRun(testData[i], minCuts);

				//Branch and bound (exponential)
				minCuts = executeTest("brnch&Bnd_v1", i + 1, MinCutsStolenNecklaceKGemsNPeople_branchAndBounds_v1::getMinCutsStolenNecklaceKGemsNPeople, testData[i], timens, true);
				if (timens > 0 && testData[i].numGemsInNecklace == testData[i].numGemsToDistribute) compareResultsWithPrevRun(testData[i], minCuts);

				minCuts = executeTest("brnch&Bnd_v2", i + 1, MinCutsStolenNecklaceKGemsNPeople_branchAndBounds_v2::getMinCutsStolenNecklaceKGemsNPeople, testData[i], timens, true);
				if (timens > 0 && testData[i].numGemsInNecklace == testData[i].numGemsToDistribute) compareResultsWithPrevRun(testData[i], minCuts);

				//Greedy_v1 splitting for 2 persons
				if (testData[i].numPeople == 2 
					&& testData[i].numGemsInNecklace == testData[i].numGemsToDistribute
					&& testData[i].symmetricDistribution)
				{
					minCuts = executeTest("getMinCuts_v1", i + 1, getMinCuts_v1, testData[i], timens, false);
					//if (timens > 0) compareResultsWithPrevRun(testData[i], minCuts); //can not compare since this is not exact solution
				}

				//Greedy_v2 (much better than Greedy_v1) splitting for 2 persons
				if (testData[i].numPeople == 2
					&& testData[i].numGemsInNecklace == testData[i].numGemsToDistribute
					&& testData[i].symmetricDistribution)
				{
					minCuts = executeTest("getMinCuts_v2", i + 1, getMinCuts_v2, testData[i], timens, false);
					//if (timens > 0) compareResultsWithPrevRun(testData[i], minCuts); //can not compare since this is not exact solution
				}

				testData[i].necklace.clear();
				testData[i].expectedDistribution.clear();
			}
		}
	}
}

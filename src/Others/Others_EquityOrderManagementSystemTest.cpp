#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <locale> // For printing number as thousand separated string
using namespace std;

//#include "Utils/Utils_MM_Assert.h"
#include "Utils/Utils_PrintUtils.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Others/Others_EquityOrderManagementSystemTestData.h"
#include "Others/Others_EquityOrderManagementSystemTestDataGenerator.h"
#include "Others/Others_EquityOrderManagementSystemTestDataReader.h"
#include "Others/Others_EquityOrderManagementSystem_v1a.h"
#include "Others/Others_EquityOrderManagementSystem_v1b.h"
#include "Others/Others_EquityOrderManagementSystem_v1c.h"
#include "Others/Others_EquityOrderManagementSystem_v2a.h"
#include "Others/Others_EquityOrderManagementSystem_v3a.h"
#include "Others/Others_EquityOrderManagementSystem_v3b.h"
#include "Others/Others_EquityOrderManagementSystem_v3c.h"
#include "Others/Others_EquityOrderManagementSystem_v4a.h"
#include "Others/Others_EquityOrderManagementSystem_v4b.h"
#include "Others/Others_EquityOrderManagementSystem_v4c.h"
#include "Others/Others_EquityOrderManagementSystem_v4d.h"
#include "Others/Others_EquityOrderManagementSystem_v4e.h"
#include "Others/Others_EquityOrderManagementSystem_v4f.h"
#include "Others/Others_EquityOrderManagementSystem_v4g.h"

namespace mm {

	struct Durations
	{
		Durations()
			: api0(0),
			api1(0),
			api2v1(0),
			api2v2(0),
			api2v3(0),
			totalv1(0),
			totalv2(0),
			totalv3(0)
		{}
		unsigned long long api0;
		unsigned long long api1;
		unsigned long long api2v1;
		unsigned long long api2v2;
		unsigned long long api2v3;
		unsigned long long totalv1;
		unsigned long long totalv2;
		unsigned long long totalv3;
	};
	//durationsOMS[algo version][test case number]
	typedef map<string, vector<Durations>> algoVersionMap;
	algoVersionMap durationsOMS;

	// ============================= Unit Testing =============================

	void test_Others_EquityOrderManagementSystem()
	{
#ifdef READ_TEST_DATA_FROM_FILE
		readTestEquityOMSDataFromFile();

		size_t numClientsLoaded = listOfClients.size();
		size_t numSymbolsLoaded = listOfSymbols.size();
		size_t numOperationsLoaded = operations.size();
		size_t numClientNameToIdMap = clientNameToIdMap.size();
		size_t numsymbolNameToIdMap = symbolNameToIdMap.size();

		cout << "\nTest case no.: " << vecTestDataStatsIndex + 1;
		cout << "\nNumber of clients: " << vecTestDataStats[vecTestDataStatsIndex].numClients;
		cout << "\nNumber of symbols: " << vecTestDataStats[vecTestDataStatsIndex].numSymbols;
		cout << "\nNumber of Operations: " << vecTestDataStats[vecTestDataStatsIndex].numOperations;
		cout << "\nNumber of calls to AddOrder: " << vecTestDataStats[vecTestDataStatsIndex].numCallsToAddOrderActual;
		cout << "\nNumber of calls to UpdateMarketPrice: " << vecTestDataStats[vecTestDataStatsIndex].numCallsToUpdateMarketPriceActual;
		cout << "\nNumber of calls to GetTop100: " << vecTestDataStats[vecTestDataStatsIndex].numCallsToGetTop100Actual;
		cout << "\n";
#endif

#ifdef GENERATE_TEST_DATA
		listOfClients = listOfClientsGenerated;
		listOfSymbols = listOfSymbolsGenerated;
		operations = operationsGenerated;
		clientNameToIdMap = clientNameToIdMapGenerated;
		symbolNameToIdMap = symbolNameToIdMapGenerated;

		MM_EXPECT_TRUE(vecTestDataStats[vecTestDataStatsIndex].numClients == numClientsLoaded
			&& vecTestDataStats[vecTestDataStatsIndex].numSymbols == numSymbolsLoaded
			&& vecTestDataStats[vecTestDataStatsIndex].numOperationsActual == numOperationsLoaded,
			vecTestDataStats[vecTestDataStatsIndex].numClients, numClientsLoaded,
			vecTestDataStats[vecTestDataStatsIndex].numSymbols, numSymbolsLoaded,
			vecTestDataStats[vecTestDataStatsIndex].numOperationsActual, numOperationsLoaded);

		//MM_EXPECT_TRUE(listOfClientsGenerated == listOfClients);
		//MM_EXPECT_TRUE(listOfSymbolsGenerated == listOfSymbols);
		//MM_EXPECT_TRUE(operationsGenerated == operations);
		//if (operationsGenerated != operations)
		//{
		//	for(int i = 0; i < operationsGenerated.size(); ++i)
		//		MM_EXPECT_TRUE(operationsGenerated[i] == operations[i], i, operationsGenerated[i], operations[i], operationsGenerated[i].price - operations[i].price);
		//}
		//MM_EXPECT_TRUE(finalSortedListsGenerated == finalSortedLists);
		//if (finalSortedListsGenerated != finalSortedLists)
		//{
		//	for (int i = 0; i < finalSortedListsGenerated.size(); ++i)
		//		MM_EXPECT_TRUE(finalSortedListsGenerated[i] == finalSortedLists[i], i, finalSortedListsGenerated[i], finalSortedLists[i]);
		//}
		//MM_EXPECT_TRUE(clientNameToIdMapGenerated == clientNameToIdMap);
		//if (clientNameToIdMapGenerated != clientNameToIdMap)
		//{
		//	auto it1 = clientNameToIdMapGenerated.begin();
		//	auto it2 = clientNameToIdMap.begin();
		//	for(; it1 != clientNameToIdMapGenerated.end() && it2 != clientNameToIdMap.end(); ++it1, ++it2)
		//		MM_EXPECT_TRUE(*it1 == *it2, *it1, *it2);
		//}
		//MM_EXPECT_TRUE(symbolNameToIdMapGenerated == symbolNameToIdMap);
		//if (symbolNameToIdMapGenerated != symbolNameToIdMap)
		//{
		//	auto it1 = symbolNameToIdMapGenerated.begin();
		//	auto it2 = symbolNameToIdMap.begin();
		//	for(; it1 != symbolNameToIdMapGenerated.end() && it2 != symbolNameToIdMap.end(); ++it1, ++it2)
		//		MM_EXPECT_TRUE(*it1 == *it2, *it1, *it1);
		//}
		cout << "\n";
#endif
		MM_SET_PAUSE_ON_ERROR(true);

		typedef void (*addOrderFunctionType)(const string& client, const string& symbol, double newOrderPrice);
		typedef void (*updateMarketPriceFunctionType)(const string& symbol, double currentMarketPrice);
		typedef vector<string> (*getTop100v1FunctionType)();
		typedef void (*getTop100v2FunctionType)(vector<const string*>& retVal);
		//typedef void (*getTop100v3FunctionType)(vector<std::pair<const string*, const string*>>& top100, unsigned int& count);
		typedef void(*clearAllDataFunctionType)();

		struct funData
		{
			string name;
			addOrderFunctionType addOrderFunction;
			updateMarketPriceFunctionType updateMarketPriceFunction;
			getTop100v1FunctionType getTop100v1Function;
			getTop100v2FunctionType getTop100v2Function;
			//getTop100v3FunctionType getTop100v3Function;
			clearAllDataFunctionType clearAllDataFunction;
			clearAllDataFunctionType allocateMemoryFunction;
			clearAllDataFunctionType deallocateMemoryFunction;
		};

		vector<funData> algoVersions = {
				//{ "v1a", Others_OMS_v1a::Others_OMS_addOrder, Others_OMS_v1a::Others_OMS_updateMarketPrice, Others_OMS_v1a::Others_OMS_getTop100v1, Others_OMS_v1a::Others_OMS_getTop100v2, /*Others_OMS_v1a::Others_OMS_getTop100v3,*/ Others_OMS_v1a::clearAllData, Others_OMS_v1a::allocateAllMemory, Others_OMS_v1a::deallocateAllMemory },
				//{ "v1b", Others_OMS_v1b::Others_OMS_addOrder, Others_OMS_v1b::Others_OMS_updateMarketPrice, Others_OMS_v1b::Others_OMS_getTop100v1, Others_OMS_v1b::Others_OMS_getTop100v2, Others_OMS_v1b::clearAllData, Others_OMS_v1b::allocateAllMemory, Others_OMS_v1b::deallocateAllMemory },
				//{ "v1c", Others_OMS_v1c::Others_OMS_addOrder, Others_OMS_v1c::Others_OMS_updateMarketPrice, Others_OMS_v1c::Others_OMS_getTop100v1, Others_OMS_v1c::Others_OMS_getTop100v2, Others_OMS_v1c::clearAllData, Others_OMS_v1c::allocateAllMemory, Others_OMS_v1c::deallocateAllMemory },
				//{ "v2a", Others_OMS_v2a::Others_OMS_addOrder, Others_OMS_v2a::Others_OMS_updateMarketPrice, Others_OMS_v2a::Others_OMS_getTop100v1, Others_OMS_v2a::Others_OMS_getTop100v2, Others_OMS_v2a::clearAllData, Others_OMS_v2a::allocateAllMemory, Others_OMS_v2a::deallocateAllMemory },
				//{ "v3a", Others_OMS_v3a::Others_OMS_addOrder, Others_OMS_v3a::Others_OMS_updateMarketPrice, Others_OMS_v3a::Others_OMS_getTop100v1, Others_OMS_v3a::Others_OMS_getTop100v2, Others_OMS_v3a::clearAllData, Others_OMS_v3a::allocateAllMemory, Others_OMS_v3a::deallocateAllMemory },
				//{ "v3b", Others_OMS_v3b::Others_OMS_addOrder, Others_OMS_v3b::Others_OMS_updateMarketPrice, Others_OMS_v3b::Others_OMS_getTop100v1, Others_OMS_v3b::Others_OMS_getTop100v2, Others_OMS_v3b::clearAllData, Others_OMS_v3b::allocateAllMemory, Others_OMS_v3b::deallocateAllMemory },
				//{ "v3c", Others_OMS_v3c::Others_OMS_addOrder, Others_OMS_v3c::Others_OMS_updateMarketPrice, Others_OMS_v3c::Others_OMS_getTop100v1, Others_OMS_v3c::Others_OMS_getTop100v2, Others_OMS_v3c::clearAllData, Others_OMS_v3c::allocateAllMemory, Others_OMS_v3c::deallocateAllMemory },
				//{ "v4a", Others_OMS_v4a::Others_OMS_addOrder, Others_OMS_v4a::Others_OMS_updateMarketPrice, Others_OMS_v4a::Others_OMS_getTop100v1, Others_OMS_v4a::Others_OMS_getTop100v2, Others_OMS_v4a::clearAllData, Others_OMS_v4a::allocateAllMemory, Others_OMS_v4a::deallocateAllMemory },
				//{ "v4b", Others_OMS_v4b::Others_OMS_addOrder, Others_OMS_v4b::Others_OMS_updateMarketPrice, Others_OMS_v4b::Others_OMS_getTop100v1, Others_OMS_v4b::Others_OMS_getTop100v2, Others_OMS_v4b::clearAllData, Others_OMS_v4b::allocateAllMemory, Others_OMS_v4b::deallocateAllMemory },
				//{ "v4c", Others_OMS_v4c::Others_OMS_addOrder, Others_OMS_v4c::Others_OMS_updateMarketPrice, Others_OMS_v4c::Others_OMS_getTop100v1, Others_OMS_v4c::Others_OMS_getTop100v2, Others_OMS_v4c::clearAllData, Others_OMS_v4c::allocateAllMemory, Others_OMS_v4c::deallocateAllMemory },
				//{ "v4d", Others_OMS_v4d::Others_OMS_addOrder, Others_OMS_v4d::Others_OMS_updateMarketPrice, Others_OMS_v4d::Others_OMS_getTop100v1, Others_OMS_v4d::Others_OMS_getTop100v2, /*Others_OMS_v4d::Others_OMS_getTop100v3,*/ Others_OMS_v4d::clearAllData, Others_OMS_v4d::allocateAllMemory, Others_OMS_v4d::deallocateAllMemory },
				{ "v4e", Others_OMS_v4e::Others_OMS_addOrder, Others_OMS_v4e::Others_OMS_updateMarketPrice, Others_OMS_v4e::Others_OMS_getTop100v1, Others_OMS_v4e::Others_OMS_getTop100v2, Others_OMS_v4e::clearAllData, Others_OMS_v4e::allocateAllMemory, Others_OMS_v4e::deallocateAllMemory },
				{ "v4f", Others_OMS_v4f::Others_OMS_addOrder, Others_OMS_v4f::Others_OMS_updateMarketPrice, Others_OMS_v4f::Others_OMS_getTop100v1, Others_OMS_v4f::Others_OMS_getTop100v2, Others_OMS_v4f::clearAllData, Others_OMS_v4f::allocateAllMemory, Others_OMS_v4f::deallocateAllMemory },
				{ "v4g", Others_OMS_v4g::Others_OMS_addOrder, Others_OMS_v4g::Others_OMS_updateMarketPrice, Others_OMS_v4g::Others_OMS_getTop100v1, Others_OMS_v4g::Others_OMS_getTop100v2, Others_OMS_v4g::clearAllData, Others_OMS_v4g::allocateAllMemory, Others_OMS_v4g::deallocateAllMemory }
			};

		std::cout.imbue(std::locale(""));
		std::chrono::steady_clock::time_point startTime, endTime;
		vector<ClientSymbolPair> actualFinalSortedList(100, ClientSymbolPair(0, 0));
		for (int k = 0; k < algoVersions.size(); ++k)
		{
			cout << "\nTesting Version...: " << algoVersions[k].name << "\n";

			Durations durations;
			int finalSortedListsIndex = -1;
			(*algoVersions[k].allocateMemoryFunction)();
			(*algoVersions[k].clearAllDataFunction)();

			for (int i = 0; i < operations.size(); ++i)
			{
				vector<string> actualTop100;
				switch (operations[i].fun)
				{
				case addOrder:
					startTime = std::chrono::steady_clock::now();
					(*algoVersions[k].addOrderFunction)(listOfClients[operations[i].clientId], listOfSymbols[operations[i].symbolId], operations[i].price);
					endTime = std::chrono::steady_clock::now();
					durations.api0 += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
					break;
				case updateMarketPrice:
					startTime = std::chrono::steady_clock::now();
					(*algoVersions[k].updateMarketPriceFunction)(listOfSymbols[operations[i].symbolId], operations[i].price);
					endTime = std::chrono::steady_clock::now();
					durations.api1 += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
					break;
				case getTop100:
					bool result = true;

#ifdef UNDEFINED
					//Call first API
					startTime = std::chrono::steady_clock::now();
					actualTop100 = (*algoVersions[k].getTop100v1Function)();
					endTime = std::chrono::steady_clock::now();
					durations.api2v1 += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

					++finalSortedListsIndex;
					result = true;
					if (actualTop100.size() == operations[i].top100.size())
					{
						for (int j = 0; j < actualTop100.size(); ++j)
						{
							size_t separator = actualTop100[j].find_first_of('-');
							int clientId = clientNameToIdMap[actualTop100[j].substr(0, separator)];
							int symbolId = symbolNameToIdMap[actualTop100[j].substr(separator + 1)];
#ifdef MM_DEBUG_OTHERS_EOMS
							actualFinalSortedList.push_back(ClientSymbolPair(clientId, symbolId, 0.0, 0.0, 0.0));
#else
							actualFinalSortedList[j] = ClientSymbolPair(clientId, symbolId);
							if (operations[i].top100[j] != actualFinalSortedList[j])
							{
								result = false;
								break;
							}
#endif
						}
					}
					else
						result = false;
					MM_EXPECT_TRUE(result, operations[i].top100, actualFinalSortedList);
					durations.totalv1 = durations.api0 + durations.api1 + durations.api2v1;
#endif

					//Call second API
					vector<const string*> actualTop100v2(100, nullptr);
					startTime = std::chrono::steady_clock::now();
					(*algoVersions[k].getTop100v2Function)(actualTop100v2);
					endTime = std::chrono::steady_clock::now();
					durations.api2v2 += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

					++finalSortedListsIndex;
					result = true;
					//if (actualTop100v2.size() == operations[i].top100.size())
					//{
					int j = 0;
						for (; j < actualTop100v2.size() && actualTop100v2[j] != nullptr; ++j)
						{
							size_t separator = actualTop100v2[j]->find_first_of('-');
							size_t clientId = clientNameToIdMap[actualTop100v2[j]->substr(0, separator)];
							size_t symbolId = symbolNameToIdMap[actualTop100v2[j]->substr(separator + 1)];
							actualFinalSortedList[j] = ClientSymbolPair(clientId, symbolId);
							if (operations[i].top100[j] != actualFinalSortedList[j])
							{
								result = false;
								//break; // Do not break it, otherwise actualFinalSortedList will not have all current results and debugging it difficult
							}
						}
					//}
					//else
					//	result = false;
					MM_EXPECT_TRUE(operations[i].top100.size() == j && result, i, operations[i].top100, actualFinalSortedList);
					durations.totalv2 = durations.api0 + durations.api1 + durations.api2v2;
#ifdef UNDEFINED
					//Call third API
					unsigned int count = 0;
					static vector<std::pair<const string*, const string*>> actualSortedListv3(100);
					startTime = std::chrono::steady_clock::now();
					(*algoVersions[k].getTop100v3Function)(actualSortedListv3, count);
					endTime = std::chrono::steady_clock::now();
					durations.api2v3 += std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();

					++finalSortedListsIndex;
					result = true;
					if (count == operations[i].top100.size())
					{
						for (int j = 0; j < count; ++j)
						{
							int clientId = clientNameToIdMap[*actualSortedListv3[j].first];
							int symbolId = symbolNameToIdMap[*actualSortedListv3[j].second];
							if (operations[i].top100[j].clientId != clientId
								|| operations[i].top100[j].symbolId != symbolId)
							{
								result = false;
								break;
							}
						}
					}
					else
						result = false;
					MM_EXPECT_TRUE(result, count, operations[i].top100, actualSortedListv3);
					durations.totalv3 = durations.api0 + durations.api1 + durations.api2v3;
#endif
				}
			}
			
			(*algoVersions[k].deallocateMemoryFunction)();
			cout << "\nTime API 0: " << durations.api0 << " nanoseconds";
			cout << "\nTime API 1: " << durations.api1 << " nanoseconds";
			//cout << "\nTime API 2v1: " << durations.api2v1 << " nanoseconds";
			cout << "\nTime API 2v2: " << durations.api2v2 << " nanoseconds";
			//cout << "\nTime API 2v3: " << durations.api2v3 << " nanoseconds";
			//cout << "\nTotal Time v1: " << durations.totalv1;
			cout << "\nTotal Time v2: " << durations.totalv2;
			//cout << "\nTotal Time v3: " << durations.totalv3;

			durationsOMS[algoVersions[k].name].push_back(std::move(durations));
			cout << "\n";
		}
	}

	//MM_DECLARE_FLAG(Others_EquityOrderManagementSystem);

	MM_UNIT_TEST(Others_EquityOrderManagementSystem_test_1, Others_EquityOrderManagementSystem)
	{
		MM_PRINT_TEST_CASE_NUMBER(false);

#ifdef GENERATE_TEST_DATA
		for (int i = 0; i < vecTestDataStats.size(); ++i)
		{
			vecTestDataStatsIndex = i;
			generateAndPrintTestEquityOMSData();
			vecTestDataStats[vecTestDataStatsIndex].numOperationsActual = operationsGenerated.size();
			int numClientsGenerated = listOfClientsGenerated.size();
			int numSymbolsGenerated = listOfSymbolsGenerated.size();
			int numClientNameToIdMapGenerated = clientNameToIdMapGenerated.size();
			int numsymbolNameToIdMapGenerated = symbolNameToIdMapGenerated.size();
			cout << "\nTest Data Generated - version: " << i + 1;
			cout << "\n numOperations:" << vecTestDataStats[vecTestDataStatsIndex].numOperationsActual;
			cout << "\n numCallsToAddOrderExpected:" << vecTestDataStats[vecTestDataStatsIndex].numCallsToAddOrderExpected;
			cout << "\n numCallsToAddOrderActual:" << vecTestDataStats[vecTestDataStatsIndex].numCallsToAddOrderActual;
			cout << "\n numCallsToUpdateMarketPriceExpected:" << vecTestDataStats[vecTestDataStatsIndex].numCallsToUpdateMarketPriceExpected;
			cout << "\n numCallsToUpdateMarketPriceActual:" << vecTestDataStats[vecTestDataStatsIndex].numCallsToUpdateMarketPriceActual;
			cout << "\n numCallsToGetTop100Expected:" << vecTestDataStats[vecTestDataStatsIndex].numCallsToGetTop100Expected;
			cout << "\n numCallsToGetTop100Actual:" << vecTestDataStats[vecTestDataStatsIndex].numCallsToGetTop100Actual;
			cout << "\n";
			MM_EXPECT_TRUE(vecTestDataStats[vecTestDataStatsIndex].numClients == numClientsGenerated
				&& vecTestDataStats[vecTestDataStatsIndex].numSymbols == numSymbolsGenerated
				&& vecTestDataStats[vecTestDataStatsIndex].numClients == numClientNameToIdMapGenerated
				&& vecTestDataStats[vecTestDataStatsIndex].numSymbols == numsymbolNameToIdMapGenerated,
				numClientsGenerated, numSymbolsGenerated, numClientNameToIdMapGenerated, numsymbolNameToIdMapGenerated);
		}
		//return;
#endif

		size_t numTestToExecute = vecTestDataStats.size();
		for (size_t i = 4; i < numTestToExecute; ++i)
		{
			vecTestDataStatsIndex = i;
			test_Others_EquityOrderManagementSystem();
		}

		// Print all results
		/*
					test1								test2					test3				test4...
					num clients
					num symbols
					API1, API2, API3, total_calls

		version1	API1, API2, API3, total_calls

		*/
		
		int firstColumnWith = 12;
		int minorColumnWidth = 17;
		int majorColumnWidth = minorColumnWidth * 4;
		
		//Print column headers
		cout << setw(firstColumnWith) << " ";
		for (int j = 0; j < vecTestDataStats.size(); ++j)
			cout << setw(majorColumnWidth) << std::right << "Test " + to_string(j + 1);
		cout << endl;
		cout << setw(firstColumnWith) << " ";
		for (int j = 0; j < vecTestDataStats.size(); ++j)
			cout << setw(majorColumnWidth) << std::right << "Clients: " + to_string(vecTestDataStats[j].numClients);
		cout << endl;
		cout << setw(firstColumnWith) << " ";
		for (int j = 0; j < vecTestDataStats.size(); ++j)
			cout << setw(majorColumnWidth) << std::right << "Symbols: " + to_string(vecTestDataStats[j].numSymbols);
		cout << endl;

		cout << setw(firstColumnWith) << setfill('-') << "";
		for (int j = 0; j < vecTestDataStats.size(); ++j)
			cout << setw(majorColumnWidth) << setfill('-') << "";
		cout << setfill(' ') << endl;

		cout << setw(firstColumnWith) << " ";
		for (int j = 0; j < vecTestDataStats.size(); ++j)
			cout << setw(minorColumnWidth) << std::right << "API 1" 
			<< setw(minorColumnWidth) << std::right << "API 2" 
			//<< setw(minorColumnWidth) << std::right << "API 3v1" 
			<< setw(minorColumnWidth) << std::right << "API 3v2" 
			//<< setw(minorColumnWidth) << std::right << "API 3v3"
			//<< setw(minorColumnWidth) << std::right << "Total calls v1" 
			<< setw(minorColumnWidth) << std::right << "Total calls v2" 
			//<< setw(minorColumnWidth) << std::right << "Total calls v3"
			;
		cout << endl;
		cout << setw(firstColumnWith) << " ";
		for (int j = 0; j < vecTestDataStats.size(); ++j)
			cout << setw(minorColumnWidth) << std::right << vecTestDataStats[j].numCallsToAddOrderActual
			<< setw(minorColumnWidth) << std::right << vecTestDataStats[j].numCallsToUpdateMarketPriceActual
			//<< setw(minorColumnWidth) << std::right << vecTestDataStats[j].numCallsToGetTop100Actual
			<< setw(minorColumnWidth) << std::right << vecTestDataStats[j].numCallsToGetTop100Actual
			//<< setw(minorColumnWidth) << std::right << vecTestDataStats[j].numCallsToGetTop100Actual
			//<< setw(minorColumnWidth) << std::right << vecTestDataStats[j].numOperationsActual
			<< setw(minorColumnWidth) << std::right << vecTestDataStats[j].numOperationsActual
			//<< setw(minorColumnWidth) << std::right << vecTestDataStats[j].numOperationsActual
			;
		cout << endl;

		cout << setw(firstColumnWith) << setfill('-') << "";
		for (int j = 0; j < vecTestDataStats.size(); ++j)
			cout << setw(majorColumnWidth) << setfill('-') << "";
		cout << setfill(' ') << endl;

		
		for (auto it = durationsOMS.begin(); it != durationsOMS.end(); ++it)
		{
			cout << setw(firstColumnWith) << std::right << "Version " + it->first + ":";
			vector<Durations>& data = it->second;
			for (int testCase = 0; testCase < data.size(); ++testCase)
			{
				//for (int api = 0; api < data[testCase].size(); ++api)
				//{
					//cout << setw(minorColumnWidth) << std::right << data[testCase][api];
					cout << setw(minorColumnWidth) << std::right << data[testCase].api0;
					cout << setw(minorColumnWidth) << std::right << data[testCase].api1;
					//cout << setw(minorColumnWidth) << std::right << data[testCase].api2v1;
					cout << setw(minorColumnWidth) << std::right << data[testCase].api2v2;
					//cout << setw(minorColumnWidth) << std::right << data[testCase].api2v3;
					//cout << setw(minorColumnWidth) << std::right << data[testCase].totalv1;
					cout << setw(minorColumnWidth) << std::right << data[testCase].totalv2;
					//cout << setw(minorColumnWidth) << std::right << data[testCase].totalv3;
				//}
			}
			cout << endl;
		}
	}
}

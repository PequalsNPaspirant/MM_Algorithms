#pragma once

#include <vector>
#include <unordered_map>
#include <fstream> //for file read/write
#include <sstream> // for std::stringstream
using namespace std;

#include "Others/Others_EquityOrderManagementSystemTestData.h"

namespace mm {

	vector<testDataStats> vecTestDataStats = {
		{200, 200, 100000, 33, 33, 33},
		{200, 200, 100000, 10, 10, 80},
		{200, 200, 100000, 10, 80, 10},
		{200, 200, 100000, 80, 10, 10},
		{500, 500, 500000, 40, 40, 20},
		{1000, 1000, 1000000, 40, 50, 10}
	};
	int vecTestDataStatsIndex = 0;

	vector<string> listOfClients(0);
	vector<string> listOfSymbols(0);
	vector<OperationData> operations(0);
	vector<vector<ClientSymbolPair>> finalSortedLists(0);	
	unordered_map<string, int> clientNameToIdMap(0);
	unordered_map<string, int> symbolNameToIdMap(0);

	vector<string> listOfClientsGenerated(0);
	vector<string> listOfSymbolsGenerated(0);
	vector<OperationData> operationsGenerated(0);
	vector<vector<ClientSymbolPair>> finalSortedListsGenerated(0);	
	unordered_map<string, int> clientNameToIdMapGenerated(0);
	unordered_map<string, int> symbolNameToIdMapGenerated(0);

	const string filePath("../../../test/data/OMS_data/Others_EquityOrderManagementSystemTestData.data");
	const string clientsEntry("----- Clients:");
	const string symbolsEntry("----- Symbols:");
	const string operationsEntry("----- Operations:");
	const string end("----- End");
	const string sameAsAbove("same as above");
}
#include <vector>
#include <unordered_map>
#include <fstream> //for file read/write
#include <sstream> // for std::stringstream
using namespace std;

#include "Others/Others_EquityOrderManagementSystemTestData.h"

namespace mm {

	//int clientNameLenMin = 10;
	//int clientNameLenMax = 20;
	//int minPrice = 100;
	//int maxPrice = 1000000;
	//double tolerance = 0.00001;
	//int maxPrecision = 17;
	//int finalSortedListLen = 100;

	vector<testDataStats> vecTestDataStats = {
		{200, 200, 100000, 33, 33, 33},
		{200, 200, 100000, 10, 10, 80},
		{200, 200, 100000, 10, 80, 10},
		{200, 200, 100000, 80, 10, 10},
		{500, 500, 500000, 40, 40, 20},
		{1000, 1000, 1000000, 40, 50, 10}
	};
	size_t vecTestDataStatsIndex = 0;

	vector<string> listOfClients(0);
	vector<string> listOfSymbols(0);
	vector<OperationData> operations(0);
	vector<vector<ClientSymbolPair>> finalSortedLists(0);	
	unordered_map<string, size_t> clientNameToIdMap(0);
	unordered_map<string, size_t> symbolNameToIdMap(0);

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
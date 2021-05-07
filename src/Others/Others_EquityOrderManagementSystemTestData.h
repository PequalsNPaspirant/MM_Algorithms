#pragma once

#include <vector>
#include <unordered_map>
#include <fstream> //for file read/write
#include <sstream> // for std::stringstream
using namespace std;

//#define GENERATE_TEST_DATA
#define READ_TEST_DATA_FROM_FILE
//#define MM_DEBUG_OTHERS_EOMS

namespace mm {

	struct testDataStats
	{
		testDataStats(int numClientsIn, int numSymbolsIn, int numOperationsIn, 
			int numPercentageCallsToAddOrderIn, int numPercentageCallsToUpdateMarketPriceIn, int numPercentageCallsToGetTop100In)
			: numClients(numClientsIn),
			numSymbols(numSymbolsIn),
			numOperations(numOperationsIn),
			numOperationsActual(0),
			numPercentageCallsToAddOrder(numPercentageCallsToAddOrderIn),
			numPercentageCallsToUpdateMarketPrice(numPercentageCallsToUpdateMarketPriceIn),
			numPercentageCallsToGetTop100(numPercentageCallsToGetTop100In),
			numCallsToGetTop100Expected(numPercentageCallsToGetTop100 * numOperations / 100),
			numCallsToAddOrderExpected(numPercentageCallsToAddOrder * numOperations / 100),
			numCallsToUpdateMarketPriceExpected(numPercentageCallsToUpdateMarketPrice * numOperations / 100),
			numCallsToGetTop100Actual(0),
			numCallsToAddOrderActual(0),
			numCallsToUpdateMarketPriceActual(0)
		{}

		int numClients;
		int numSymbols;
		int numOperations;
		int numOperationsActual;
		int numPercentageCallsToAddOrder;
		int numPercentageCallsToUpdateMarketPrice;
		int numPercentageCallsToGetTop100;
		int numCallsToGetTop100Expected;
		int numCallsToAddOrderExpected;
		int numCallsToUpdateMarketPriceExpected;
		int numCallsToGetTop100Actual;
		int numCallsToAddOrderActual;
		int numCallsToUpdateMarketPriceActual;
	};
	
	extern vector<testDataStats> vecTestDataStats;
	extern size_t vecTestDataStatsIndex;
	extern constexpr const int clientNameLenMin = 10;
	extern constexpr const int clientNameLenMax = 20;
	extern constexpr const int minPrice = 100;
	extern constexpr const int maxPrice = 1000000;
	extern constexpr const double tolerance = 0.00001;
	extern constexpr const int maxPrecision = 17;
	extern constexpr const int finalSortedListLen = 100;

	enum APIs
	{
		addOrder = 0,
		updateMarketPrice = 1,
		getTop100 = 2
	};

	struct ClientSymbolPair
	{
		ClientSymbolPair()
		{}

#ifndef MM_DEBUG_OTHERS_EOMS
		ClientSymbolPair(size_t clientIdIn, size_t symbolIdIn)
			: clientId(clientIdIn),
			symbolId(symbolIdIn)
		{
		}
#else
		ClientSymbolPair(size_t clientIdIn, size_t symbolIdIn, double orderPriceIn, double marketPriceIn, double offsetIn)
			: clientId(clientIdIn),
			symbolId(symbolIdIn),
			orderPrice(orderPriceIn),
			marketPrice(marketPriceIn),
			offset(offsetIn)
		{
		}
#endif
		bool operator==(const ClientSymbolPair& rhs) const
		{
			return clientId == rhs.clientId
				&& symbolId == rhs.symbolId
//#ifdef MM_DEBUG_OTHERS_EOMS
//				&& fabs(orderPrice - rhs.orderPrice) < tolerance
//				&& fabs(marketPrice - rhs.marketPrice) < tolerance
//				&& fabs(offset - rhs.offset) < tolerance
//#endif
				;
		}

		bool operator!=(const ClientSymbolPair& rhs) const
		{
			return !(*this == rhs);
		}

		friend ostream& operator<<(ostream& o, const ClientSymbolPair& dataIn)
		{
			o << "{ "
				<< dataIn.clientId
				<< ", " << dataIn.symbolId
#ifdef MM_DEBUG_OTHERS_EOMS
				<< ", " << dataIn.orderPrice
				<< ", " << dataIn.marketPrice
				<< ", " << dataIn.offset
#endif
				<< " }";

			return o;
		}

		size_t clientId;
		size_t symbolId;

#ifdef MM_DEBUG_OTHERS_EOMS
		double orderPrice;
		double marketPrice;
		double offset;
#endif

	};

	struct OperationData
	{
		OperationData()
			: fun(APIs::getTop100),
			clientId(0),
			symbolId(0),
			price(0),
			top100(0)
		{
		}

		//friend ostream& operator<<(ostream& o, const OperationData& dataIn);

		friend bool operator==(const OperationData& lhs, const OperationData& rhs)
		{
			return (
				lhs.fun == rhs.fun
				&& lhs.clientId == rhs.clientId
				&& lhs.symbolId == rhs.symbolId
				//&& fabs(lhs.price - rhs.price) < 0.0002 // Not able to maintain this precision during reading double values using stof(). Use stod().
				&& fabs(lhs.price - rhs.price) < 0.000001
				);
		}

		APIs fun;
		int clientId;
		int symbolId;
		double price;
		vector<ClientSymbolPair> top100;
	};

	extern vector<string> listOfClientsGenerated;
	extern vector<string> listOfSymbolsGenerated;
	extern vector<OperationData> operationsGenerated;
	extern unordered_map<string, int> clientNameToIdMapGenerated;
	extern unordered_map<string, int> symbolNameToIdMapGenerated;

	extern vector<string> listOfClients;
	extern vector<string> listOfSymbols;
	extern vector<OperationData> operations;
	extern unordered_map<string, size_t> clientNameToIdMap;
	extern unordered_map<string, size_t> symbolNameToIdMap;

	extern const string filePath;
	extern const string clientsEntry;
	extern const string symbolsEntry;
	extern const string operationsEntry;
	extern const string end;
	extern const string sameAsAbove;
}
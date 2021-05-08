#pragma once

#include <vector>
#include <unordered_map>
#include <fstream> //for file read/write
#include <sstream> // for std::stringstream
using namespace std;

#include "Others/Others_EquityOrderManagementSystemTestData.h"
#include "Others\Others_EquityOrderManagementSystem_v1a.h"
#include "Utils/Utils_MM_Assert.h"

namespace mm {

	void generateClientsOrSymbols(vector<string>& listOfClientsOrSymbols, char start, int numClientsOrSymbols, unordered_map<string, int>& map)
	{
		constexpr const int clientNameLenMin = 10;
		constexpr const int clientNameLenMax = 20;
		listOfClientsOrSymbols.resize(numClientsOrSymbols, string{});
		for (int i = 0; i < numClientsOrSymbols; ++i)
		{
			int clientNameLen = clientNameLenMin + rand() % (clientNameLenMax - clientNameLenMin);
			listOfClientsOrSymbols[i].resize(clientNameLen);
			for (int j = 0; j < clientNameLen; ++j)
			{
				listOfClientsOrSymbols[i][j] = start + rand() % 26;
			}

			map[listOfClientsOrSymbols[i]] = i;
		}
	}

	double getRandomPrice()
	{
		constexpr const int minPrice = 100;
		constexpr const int maxPrice = 1000000;
		return double(minPrice + rand() % (maxPrice - minPrice)) + double(1111 + rand() % 8888) / 10000.0; // to ensure decimal part is between 0.11111 to 0.99999
		//return double(minPrice + rand() % (maxPrice - minPrice));  // Generate integer values
	}

	void generateOperationsAndFinalSortedLists()
	{
		operationsGenerated.resize(vecTestDataStats[vecTestDataStatsIndex].numOperations, OperationData{});
		//First updated market price for all symbols
		int i = 0;
		for (; i < vecTestDataStats[vecTestDataStatsIndex].numSymbols; ++i)
		{
			vecTestDataStats[vecTestDataStatsIndex].numCallsToUpdateMarketPriceActual++;
			operationsGenerated[i].fun = APIs::updateMarketPrice;
			operationsGenerated[i].symbolId = i;
			operationsGenerated[i].price = getRandomPrice();
			Others_OMS_v1a::Others_OMS_updateMarketPrice(listOfSymbolsGenerated[i], operationsGenerated[i].price);
		}
		//Then at least one order before calling getTop100()
		vecTestDataStats[vecTestDataStatsIndex].numCallsToAddOrderActual++;
		operationsGenerated[i].fun = APIs::addOrder;
		operationsGenerated[i].clientId = rand() % vecTestDataStats[vecTestDataStatsIndex].numClients;
		operationsGenerated[i].symbolId = rand() % vecTestDataStats[vecTestDataStatsIndex].numSymbols;
		operationsGenerated[i].price = getRandomPrice();
		Others_OMS_v1a::Others_OMS_addOrder(listOfClientsGenerated[operationsGenerated[i].clientId], listOfSymbolsGenerated[operationsGenerated[i].symbolId], operationsGenerated[i].price);

		int firstPartition = vecTestDataStats[vecTestDataStatsIndex].numPercentageCallsToAddOrder;
		int secondPartition = firstPartition + vecTestDataStats[vecTestDataStatsIndex].numPercentageCallsToUpdateMarketPrice;
		for (++i; i < vecTestDataStats[vecTestDataStatsIndex].numOperations; ++i)
		{
			int randNum = rand() % 100;

			// The last API should be getTop100
			if(i == vecTestDataStats[vecTestDataStatsIndex].numOperations - 1)
				operationsGenerated[i].fun = APIs::getTop100;
			else
			{
				if (randNum < firstPartition
					&& vecTestDataStats[vecTestDataStatsIndex].numCallsToAddOrderActual < vecTestDataStats[vecTestDataStatsIndex].numCallsToAddOrderExpected)
					operationsGenerated[i].fun = APIs::addOrder;
				else if (randNum < secondPartition
					&& vecTestDataStats[vecTestDataStatsIndex].numCallsToUpdateMarketPriceActual < vecTestDataStats[vecTestDataStatsIndex].numCallsToUpdateMarketPriceExpected)
					operationsGenerated[i].fun = APIs::updateMarketPrice;
				else
					operationsGenerated[i].fun = APIs::getTop100;
			}

			switch (operationsGenerated[i].fun)
			{
			case APIs::addOrder:
				vecTestDataStats[vecTestDataStatsIndex].numCallsToAddOrderActual++;
				operationsGenerated[i].clientId = rand() % vecTestDataStats[vecTestDataStatsIndex].numClients;
				operationsGenerated[i].symbolId = rand() % vecTestDataStats[vecTestDataStatsIndex].numSymbols;
				operationsGenerated[i].price = getRandomPrice();
				Others_OMS_v1a::Others_OMS_addOrder(listOfClientsGenerated[operationsGenerated[i].clientId], listOfSymbolsGenerated[operationsGenerated[i].symbolId], operationsGenerated[i].price);
				break;
			case APIs::updateMarketPrice:
				vecTestDataStats[vecTestDataStatsIndex].numCallsToUpdateMarketPriceActual++;
				operationsGenerated[i].symbolId = rand() % vecTestDataStats[vecTestDataStatsIndex].numSymbols;
				operationsGenerated[i].price = getRandomPrice();
				Others_OMS_v1a::Others_OMS_updateMarketPrice(listOfSymbolsGenerated[operationsGenerated[i].symbolId], operationsGenerated[i].price);
				break;
			case APIs::getTop100:
			{
				vecTestDataStats[vecTestDataStatsIndex].numCallsToGetTop100Actual++;
				vector<string> clientSymbolPairs = Others_OMS_v1a::Others_OMS_getTop100v1();
				operationsGenerated[i].top100.resize(clientSymbolPairs.size());
				for (int j = 0; j < clientSymbolPairs.size(); ++j)
				{
					size_t separator = clientSymbolPairs[j].find_first_of('-');
					int clientId = clientNameToIdMapGenerated[clientSymbolPairs[j].substr(0, separator)];
					int symbolId = symbolNameToIdMapGenerated[clientSymbolPairs[j].substr(separator + 1)];
#ifdef MM_DEBUG_OTHERS_EOMS
					double orderPrice = Others_OMS_v1a::clientSymbolPairIndex[&clientSymbolPairs[j]->orderPrice;
					double offset = Others_OMS_v1a::clientSymbolPairIndex[&clientSymbolPairs[j]]->offset;
					string symbol = Others_OMS_v1a::clientSymbolPairIndex[&clientSymbolPairs[j]]->symbol;
					double marketPrice = Others_OMS_v1a::symbolMarketPriceIndex[symbol];
					operationsGenerated[i].top100[j] = ClientSymbolPair(clientId, symbolId, orderPrice, marketPrice, offset);
#else
					operationsGenerated[i].top100[j] = ClientSymbolPair(clientId, symbolId);
#endif
				}
			}
				break;
			default:
				MyAssert::myRunTimeAssert(false, "Unknown API");
				break;
			}
		}
	}

	void generateTestEquityOMSData()
	{
		//Clear all data
		clientNameToIdMapGenerated.clear();
		symbolNameToIdMapGenerated.clear();
		Others_OMS_v1a::clearAllData();

		generateClientsOrSymbols(listOfClientsGenerated, 'A', vecTestDataStats[vecTestDataStatsIndex].numClients, clientNameToIdMapGenerated);
		generateClientsOrSymbols(listOfSymbolsGenerated, 'a', vecTestDataStats[vecTestDataStatsIndex].numSymbols, symbolNameToIdMapGenerated);
		generateOperationsAndFinalSortedLists();
	}

	void printListOfClientsOrSymbols(std::ostream& base, vector<string>& listOfClientsOrSymbols, int count)
	{
		for (int i = 0; i < count;++i)
		{
			if (i % 100 == 0)
				base << "\n";
			else
				base << ",";
			base << listOfClientsOrSymbols[i];
		}
		base << "\n" << end << "\n";
	}

	void printOperations(std::ostream& base)
	{
		base << "\n\n" << operationsEntry << "\n";
		int lastIndexToGetTop100 = -1;
		for (int i = 0; i < vecTestDataStats[vecTestDataStatsIndex].numOperations; ++i)
		{
			base << int(operationsGenerated[i].fun) << ",";
			switch (operationsGenerated[i].fun)
			{
			case APIs::addOrder:
				base << operationsGenerated[i].clientId << "," << operationsGenerated[i].symbolId << "," << operationsGenerated[i].price;
				break;
			case APIs::updateMarketPrice:
				base << operationsGenerated[i].symbolId << "," << operationsGenerated[i].price;
				break;
			case APIs::getTop100:
				if (lastIndexToGetTop100 > 0 && operationsGenerated[i].top100 == operationsGenerated[lastIndexToGetTop100].top100)
				{
					base << sameAsAbove;
					break;
				}
				
				for (int j = 0; j < operationsGenerated[i].top100.size();)
				{
					base << operationsGenerated[i].top100[j].clientId << "-" << operationsGenerated[i].top100[j].symbolId;
					if (++j != operationsGenerated[i].top100.size())
						base << ",";
				}
				lastIndexToGetTop100 = i;
				break;
			default:
				MyAssert::myRunTimeAssert(false, "Unknown API");
				break;
			}
			
			base << "\n";
		}
		base << "\n" << end << "\n";
	}

	void printTestEquityOMSData()
	{
		cout << "\n\n" << clientsEntry << "\n";
		printListOfClientsOrSymbols(cout, listOfClientsGenerated, vecTestDataStats[vecTestDataStatsIndex].numClients);
		cout << "\n\n" << clientsEntry << "\n";
		printListOfClientsOrSymbols(cout, listOfSymbolsGenerated, vecTestDataStats[vecTestDataStatsIndex].numSymbols);
		printOperations(cout);
	}

	void writeTestEquityOMSDataToFile()
	{		
		ofstream testDataFile;

		try
		{
			testDataFile.open(filePath + "_" + to_string(vecTestDataStatsIndex + 1));
		}
		catch (std::ofstream::failure &writeErr)
		{
			cout << "\nERROR: Can not open file: " << filePath << " Error: " << writeErr.what() << endl;
			return;
		}
		catch (...)
		{
			cout << "\nUNKNOWN ERROR while opening file: " << filePath << endl;
			return;
		}

		std::stringstream buffer;
		constexpr const int maxPrecision = 17;
		cout.precision(maxPrecision);
		buffer.precision(maxPrecision);
		string str;

		if (testDataFile.is_open())
		{
			buffer << "\n\n" << clientsEntry << "\n";
			printListOfClientsOrSymbols(buffer, listOfClientsGenerated, vecTestDataStats[vecTestDataStatsIndex].numClients);
			str = buffer.str();
			testDataFile.write(str.c_str(), str.length());
			testDataFile.flush(); //flush() will ensure than the contents are written to file on disk immediately, Otherwise we need to wait until close() is called on it
			buffer.str(std::string());
			buffer.clear();

			buffer << "\n\n" << symbolsEntry << "\n";
			printListOfClientsOrSymbols(buffer, listOfSymbolsGenerated, vecTestDataStats[vecTestDataStatsIndex].numSymbols);
			str = buffer.str();
			testDataFile.write(str.c_str(), str.length());
			testDataFile.flush();
			buffer.str(std::string());
			buffer.clear();

			printOperations(buffer);
			str = buffer.str();
			testDataFile.write(str.c_str(), str.length());
			testDataFile.flush();
			buffer.str(std::string());
			buffer.clear();

			testDataFile.close();
		}
	}

	void generateAndPrintTestEquityOMSData()
	{
		generateTestEquityOMSData();
		//printTestEquityOMSData();
		writeTestEquityOMSDataToFile();
	}
}
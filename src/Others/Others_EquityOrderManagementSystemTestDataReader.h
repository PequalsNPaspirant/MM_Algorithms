#pragma once

#include <vector>
#include <unordered_map>
#include <fstream> //for file read/write
#include <sstream> // for std::stringstream
using namespace std;

#include "Others/Others_EquityOrderManagementSystemTestData.h"
#include "Utils/Utils_MM_Assert.h"

namespace mm {

	void readClientsOrSymbolsFromFile(ifstream& testDataFile, vector<string>& listOfClientsOrSymbols, unordered_map<string, size_t>& clientOrSymbolToIdMap)
	{
		string line;
		int count = -1;
		while (std::getline(testDataFile, line, '\n'))
		{
			if (line == end)
				return;
			else if (line.empty())
				continue;

			size_t start = 0;
			size_t end;
			do
			{
				end = line.find_first_of(',', start);
				string s = line.substr(start, end - start);
				if (!s.empty())
				{
					clientOrSymbolToIdMap[s] = listOfClientsOrSymbols.size();
					listOfClientsOrSymbols.push_back(std::move(s));
				}
				start = end + 1;
			} while (end != string::npos);
		}
	}

	void readOperationsFromFile(ifstream& testDataFile)
	{
		string line;
		int index = -1;
		int lastIndexToGetTop100 = -1;
		while (std::getline(testDataFile, line, '\n'))
		{
			if (line == end)
				return;
			else if (line.empty())
				continue;

			++index;
			OperationData data;
			size_t start = 0;;
			size_t end = line.find_first_of(',', start);
			data.fun = APIs( stoi(line.substr(start, end - start)) );
			start = end + 1;
			switch (data.fun)
			{
			case APIs::addOrder:
				end = line.find_first_of(',', start);
				data.clientId = stoi(line.substr(start, end - start));
				start = end + 1;
				end = line.find_first_of(',', start);
				data.symbolId = stoi(line.substr(start, end - start));
				start = end + 1;
				end = line.find_first_of(',', start);
				data.price = stod(line.substr(start, end - start));
				vecTestDataStats[vecTestDataStatsIndex].numCallsToAddOrderActual++;
				vecTestDataStats[vecTestDataStatsIndex].numOperationsActual++;
				break;
			case APIs::updateMarketPrice:
				end = line.find_first_of(',', start);
				data.symbolId = stoi(line.substr(start, end - start));
				start = end + 1;
				end = line.find_first_of(',', start);
				data.price = stod(line.substr(start, end - start));
				vecTestDataStats[vecTestDataStatsIndex].numCallsToUpdateMarketPriceActual++;
				vecTestDataStats[vecTestDataStatsIndex].numOperationsActual++;
				break;
			case APIs::getTop100:
			{
				vecTestDataStats[vecTestDataStatsIndex].numCallsToGetTop100Actual++;
				vecTestDataStats[vecTestDataStatsIndex].numOperationsActual++;

				if (line.substr(start) == sameAsAbove)
				{
					data.top100 = operations[lastIndexToGetTop100].top100;
					break;
				}

				do
				{
					end = line.find_first_of(',', start);
					string clientSymbolPair = line.substr(start, end - start);
					size_t seperator = clientSymbolPair.find_first_of('-');
					int clientId = stoi(clientSymbolPair.substr(0, seperator));
					int symblId = stoi(clientSymbolPair.substr(seperator + 1));
#ifndef MM_DEBUG_OTHERS_EOMS
					data.top100.push_back(ClientSymbolPair(clientId, symblId));
#else
					data.top100.push_back(ClientSymbolPair(clientId, symblId, 0.0, 0.0, 0.0));
#endif
					start = end + 1;
				} while (end != string::npos);

				lastIndexToGetTop100 = index;
			}
				break;
			default:
				MyAssert::myRunTimeAssert(false, "Unknown API");
				break;
			}

			operations.push_back(move(data));
		}
	}

	void readTestEquityOMSDataFromFile()
	{
		//Clear all data
		listOfClients.clear();
		listOfSymbols.clear();
		operations.clear();
		clientNameToIdMap.clear();
		symbolNameToIdMap.clear();

		vecTestDataStats[vecTestDataStatsIndex].numCallsToAddOrderActual = 0;
		vecTestDataStats[vecTestDataStatsIndex].numCallsToUpdateMarketPriceActual = 0;
		vecTestDataStats[vecTestDataStatsIndex].numCallsToGetTop100Actual = 0;

		ifstream testDataFile;

		try
		{
			testDataFile.open(filePath + "_" + to_string(vecTestDataStatsIndex + 1));
		}
		catch (std::ifstream::failure &readErr)
		{
			cout << "\nERROR: Can not open file: " << filePath << " Error: " << readErr.what() << endl;
			return;
		}
		catch (...)
		{
			cout << "\nUNKNOWN ERROR while opening file: " << filePath << endl;
			return;
		}

		string line;
		while (std::getline(testDataFile, line, '\n'))
		{
			if (line.empty())
				continue;
			else if(line == clientsEntry)
				readClientsOrSymbolsFromFile(testDataFile, listOfClients, clientNameToIdMap);
			else if(line == symbolsEntry)
				readClientsOrSymbolsFromFile(testDataFile, listOfSymbols, symbolNameToIdMap);
			else if(line == operationsEntry)
				readOperationsFromFile(testDataFile);
		}
	}
}
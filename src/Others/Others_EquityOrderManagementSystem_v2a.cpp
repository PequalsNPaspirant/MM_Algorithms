#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>
#include <locale> // For printing number as thousand separated string
#include <cmath>
using namespace std;

#include "Utils/Utils_MM_Assert.h"
#include "Others\Others_EquityOrderManagementSystem_v2a.h"

namespace mm {

/*
This version uses unordered_maps for all data structures. 
It keeps unordered_map of <key = symbol, value = unordered_map<orders> >. It also keeps the set which is ordered as per offset.
*/

	unordered_map<string, Others_OMS_v2a::DataSet_v2> Others_OMS_v2a::symbolHashMap;
	//map<double, string, std::greater<double>> Others_OMS_v2a::returnData;
	set<Others_OMS_v2a::DataSet_v1*, Others_OMS_v2a::DataSet_v2::compare> Others_OMS_v2a::returnData;

	void Others_OMS_v2a::allocateAllMemory()
	{
	}
	void Others_OMS_v2a::deallocateAllMemory()
	{
	}

/*
Complexity:
	search symbolHashMap + search/insert into clientHashMap 
	+ find & delete client-symbol pair from offsetClientSymbolPairMap if already there
	+ insert new client-symbol pair into offsetClientSymbolPairMap with modified offset

	hash(symbol) + O(1) + hash(client) + O(1) + log(orders for this symbol) + log(orders for this symbol)
*/
	//The temporary object creation may be costly, so do not use search/insert, instead first find and then insert object if not already present
	void Others_OMS_v2a::Others_OMS_addOrder(const string& clientName, const string& symbolName, double newOrderPrice)
	{
		auto it = symbolHashMap.find(symbolName);
		auto mapIt = it->second.clientHashMap.find(clientName);
		//string clientSymbolPair;
		if (mapIt != it->second.clientHashMap.end())
		{
			mapIt->second.orderPrice = newOrderPrice;
			auto setIt = it->second.offsetClientSymbolPairMap.find(&mapIt->second);
			MM_Assert::mmRunTimeAssert(setIt != it->second.offsetClientSymbolPairMap.end());
			//clientSymbolPair = std::move(setIt->second);
			it->second.offsetClientSymbolPairMap.erase(setIt);
			mapIt->second.offset = 100 * fabs(newOrderPrice - it->second.marketPrice) / it->second.marketPrice;
		}
		else
		{
			auto result = it->second.clientHashMap.insert(unordered_map<string, DataSet_v1>::value_type(clientName, DataSet_v1(newOrderPrice, it->second.marketPrice, clientName + "-" + symbolName)));
			MM_Assert::mmRunTimeAssert(result.second == true);
			mapIt = result.first;
			//clientSymbolPair = clientName + "-" + symbolName;
		}
		
		//it->second.offsetClientSymbolPairMap[mapIt->second.offset] = std::move(clientSymbolPair);
		//auto result = it->second.offsetClientSymbolPairMap.insert(map<double, string, std::greater<double>>::value_type(mapIt->second.offset, std::move(clientSymbolPair)));
		auto result = it->second.offsetClientSymbolPairMap.insert(&mapIt->second);
		MM_Assert::mmRunTimeAssert(result.second == true);
	}

	/*
	Complexity:
		search in symbolHashMap + iterate through all orders for this symbol + erase and insert new entry into offsetClientSymbolPairMap
		hash(symbol) + O(1) + number of orders of this symbol * (2 * log(number of orders of this symbol))
	*/
	void Others_OMS_v2a::Others_OMS_updateMarketPrice(const string& symbolName, double currentMarketPrice)
	{
		//The temporary object creation may be costly, so do not use search/insert, instead first find and then insert object if not already present
		auto it = symbolHashMap.find(symbolName);
		if (it != symbolHashMap.end())
		{
			it->second.marketPrice = currentMarketPrice;
			for (auto mapIt = it->second.clientHashMap.begin(); mapIt != it->second.clientHashMap.end(); ++mapIt)
			{
				auto setIt = it->second.offsetClientSymbolPairMap.find(&mapIt->second);
				MM_Assert::mmRunTimeAssert(setIt != it->second.offsetClientSymbolPairMap.end());
				//string clientSymbolPair = std::move(setIt->second);
				it->second.offsetClientSymbolPairMap.erase(setIt);
				mapIt->second.offset = 100 * fabs(mapIt->second.orderPrice - currentMarketPrice) / currentMarketPrice;
				//it->second.offsetClientSymbolPairMap[mapIt->second.offset] = std::move(clientSymbolPair);
				//auto result = it->second.offsetClientSymbolPairMap.insert(map<double, string, std::greater<double>>::value_type(mapIt->second.offset, std::move(clientSymbolPair)));
				auto result = it->second.offsetClientSymbolPairMap.insert(&mapIt->second);
				MM_Assert::mmRunTimeAssert(result.second == true);
			}
		}
		else
		{
			symbolHashMap.insert(unordered_map<string, DataSet_v2>::value_type(symbolName, DataSet_v2(currentMarketPrice)));
		}
	}

	/*
	Complexity:
		merge sort top 100 orders from each group (group of orders for each symbol) (Optimized by comparing with min so far)
		i.e. iterate through all (100 * number of symbols) orders and insert them into sorted map
		(100 * number of symbols) * log(100) ----- this is without optimization
	*/
	vector<string> Others_OMS_v2a::Others_OMS_getTop100v1()
	{
		double minOffset = 0.0;
		returnData.clear();
		for (auto it = symbolHashMap.begin(); it != symbolHashMap.end(); ++it)
		{
			int index = 0;
			for (auto setIt = it->second.offsetClientSymbolPairMap.begin(); index < 100 && setIt != it->second.offsetClientSymbolPairMap.end(); ++setIt, ++index)
			{
				if ((*setIt)->offset >= minOffset)
				{
					auto result = returnData.insert(*setIt);
					MM_Assert::mmRunTimeAssert(result.second == true);
					if (returnData.size() > 100)
						returnData.erase(--returnData.end());
					if (returnData.size() == 100)
						minOffset = (*returnData.rbegin())->offset;
				}
			}
		}

		MM_Assert::mmRunTimeAssert(returnData.size() <= 100);

		vector<string> retVal(returnData.size(), "");
		int index = -1;
		for (auto setIt = returnData.begin(); setIt != returnData.end(); ++setIt)
			retVal[++index] = (*setIt)->clientSymbolPair;
		
		//MM_Assert::mmRunTimeAssert(index == 0);

		return retVal;
	}

	void Others_OMS_v2a::Others_OMS_getTop100v2(vector<const string*>& retVal)
	{
		double minOffset = 0.0;
		returnData.clear();
		for (auto it = symbolHashMap.begin(); it != symbolHashMap.end(); ++it)
		{
			int index = 0;
			for (auto setIt = it->second.offsetClientSymbolPairMap.begin(); index < 100 && setIt != it->second.offsetClientSymbolPairMap.end(); ++setIt, ++index)
			{
				if ((*setIt)->offset >= minOffset)
				{
					auto result = returnData.insert(*setIt);
					MM_Assert::mmRunTimeAssert(result.second == true);
					if (returnData.size() > 100)
						returnData.erase(--returnData.end());
					if (returnData.size() == 100)
						minOffset = (*returnData.rbegin())->offset;
				}
			}
		}

		MM_Assert::mmRunTimeAssert(returnData.size() <= 100);

		//vector<const string*> retVal(returnData.size());
		int index = -1;
		for (auto setIt = returnData.begin(); setIt != returnData.end(); ++setIt)
			retVal[++index] = &(*setIt)->clientSymbolPair;
		
		//MM_Assert::mmRunTimeAssert(index == 0);

		//return retVal;
	}
}

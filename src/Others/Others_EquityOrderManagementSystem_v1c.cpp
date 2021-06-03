#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <locale> // For printing number as thousand separated string
#include <cmath>
using namespace std;

#include "Utils/Utils_MM_Assert.h"
#include "Others\Others_EquityOrderManagementSystem_v1c.h"

namespace mm {

/*
This version uses unordered_maps (Hash Tables) for all data structures. But do not keep the sorted list of offsets. 
It calculates top 100 offsets when needed i.e. when Others_OMS_getTop100() is called.
It keeps updated flag so that it can process only updated objects in getTop100().
So, it also keeps sorted list to be reused when getTop100() is called.
*/

	unsigned int Others_OMS_v1c::maxClientId = 0;
	unsigned int Others_OMS_v1c::maxSymbolId = 0;
	int Others_OMS_v1c::data_index = -1;
	vector<Others_OMS_v1c::OrderData> Others_OMS_v1c::orderData_(0);
	unordered_map<unsigned int, Others_OMS_v1c::OrderData*> Others_OMS_v1c::clientSymbolPairIndex(0);
	unordered_multimap <unsigned int, Others_OMS_v1c::OrderData*> Others_OMS_v1c::symbolIdIndex(0);
	unordered_map <string, unsigned int> Others_OMS_v1c::clientNameClientIdMap(0);
	//unordered_map <string, unsigned int> Others_OMS_v1c::symbolNameSymbolIdMap(0);
	//unordered_map<unsigned int, double> Others_OMS_v1c::symbolIdSymbolPriceMap(0);
	unordered_map<string, Others_OMS_v1c::SymbolData> Others_OMS_v1c::symbolNameSymbolDataMap(0);
	set<Others_OMS_v1c::OrderData*, Others_OMS_v1c::Compare> Others_OMS_v1c::sortedByOffsetSet;
	vector<Others_OMS_v1c::SymbolData*> Others_OMS_v1c::updatedSymbolData(0);
	int Others_OMS_v1c::updatedSymbolDataIndex = -1;

	void Others_OMS_v1c::allocateAllMemory()
	{
		orderData_.resize(1000000);
		clientSymbolPairIndex.reserve(1000000);
		symbolIdIndex.reserve(1000000);
		clientNameClientIdMap.reserve(1000);
		//symbolNameSymbolIdMap.reserve(1000);
		//symbolIdSymbolPriceMap.reserve(1000);
		symbolNameSymbolDataMap.reserve(1000);
		updatedSymbolData.resize(1000);
		
	}
	void Others_OMS_v1c::deallocateAllMemory()
	{
		orderData_.clear();
		clientSymbolPairIndex.clear();
		symbolIdIndex.clear();
		clientNameClientIdMap.clear();
		//symbolNameSymbolIdMap.clear();
		//symbolIdSymbolPriceMap.clear();
		symbolNameSymbolDataMap.clear();
		updatedSymbolData.clear();
	}
/*
Complexity:
	hash(clientName) + O(1)					hashtable search to insert/extract clientName
	+ hash(symbolName) + O(1)				hashtable search to get symbol id
	+ hash(unsigned int) + O(1)				hashtable search to get data
*/
	void Others_OMS_v1c::Others_OMS_addOrder(const string& clientName, const string& symbolName, double newOrderPrice)
	{
		unsigned int clientId = maxClientId;
		pair<decltype(clientNameClientIdMap)::iterator, bool> result = clientNameClientIdMap.insert(decltype(clientNameClientIdMap)::value_type(clientName, clientId));
		if (result.second == false) // new key is not interted as old key already exists
		{
			clientId = (result.first)->second;
		}
		else
		{
			++maxClientId;
		}
		
		//unsigned int symbolId = symbolNameSymbolIdMap[symbolName];
		SymbolData* symbolData = &symbolNameSymbolDataMap[symbolName];
		unsigned int clientSymbolId = (clientId << 16) | symbolData->symbolId;

		auto it = clientSymbolPairIndex.find(clientSymbolId);
		if (it != clientSymbolPairIndex.end())
		{
			it->second->orderPrice = newOrderPrice;
			double marketPrice = symbolData->symbolPrice;
			sortedByOffsetSet.erase(it->second);
			it->second->offset = 100 * fabs(newOrderPrice - marketPrice) / marketPrice;
			sortedByOffsetSet.insert(it->second);
			it->second->updated = true;
		}
		else
		{
			orderData_[++data_index] = OrderData(clientSymbolId, clientName + "-" + symbolName, newOrderPrice, symbolData->symbolPrice);
			OrderData* pObj = &orderData_[data_index];
			//update Index
			clientSymbolPairIndex[clientSymbolId] = pObj;
			symbolIdIndex.insert(decltype(symbolIdIndex)::value_type(symbolData->symbolId, pObj));
			std::pair<decltype(sortedByOffsetSet)::iterator, bool> result = sortedByOffsetSet.insert(pObj);
			//MM_Assert::mmRunTimeAssert(result.second == true);
		}
	}

	/*
	Complexity:
		hash(symbolName) + O(1)						hashtable search to insert/extract symbolName
		+ log(orders)								Find lower bound of symbolId
		+ number of orders for this symbol			update offset
	*/
	void Others_OMS_v1c::Others_OMS_updateMarketPrice(const string& symbolName, double currentMarketPrice)
	{
		unsigned int symbolId = maxSymbolId;
		pair<decltype(symbolNameSymbolDataMap)::iterator, bool> result = symbolNameSymbolDataMap.insert(decltype(symbolNameSymbolDataMap)::value_type(symbolName, SymbolData{ symbolId, currentMarketPrice }));
		if (result.second == false) // new key is not interted as old key already exists
		{
			(result.first)->second.symbolPrice = currentMarketPrice;
			if ((result.first)->second.updated == false)
			{
				updatedSymbolData[++updatedSymbolDataIndex] = &(result.first)->second;
				(result.first)->second.updated = true;
			}
		}
		else
		{
			++maxSymbolId;
		}

		//symbolIdSymbolPriceMap[symbolId] = currentMarketPrice;

		//for(auto it = symbolIndex.lower_bound(symbolId); it != symbolIndex.upper_bound(symbolId); ++it)
		//	it->second->offset = 100 * fabs(it->second->orderPrice - currentMarketPrice) / currentMarketPrice;
		//for(auto it = symbolIdIndex.lower_bound(symbolId); it != symbolIdIndex.end() && it->first == symbolId; ++it)
		//{
			//sortedByOffsetSet.erase(it->second);
			//it->second->offset = 100 * fabs(it->second->orderPrice - currentMarketPrice) / currentMarketPrice;
			//sortedByOffsetSet.insert(it->second);
			//it->second->updated = true;
		//}
	}

/*
Others_OMS_getTop100v1() is not updated for new data structure. Take a look at Others_OMS_getTop100v2()
*/
	vector<string> Others_OMS_v1c::Others_OMS_getTop100v1()
	{
		int finalSortedListLen = std::min(100, data_index + 1);
		vector<string> retVal(finalSortedListLen, "");
		auto comp2 = [] (const OrderData* a, const OrderData* b) -> bool
		{
			if (a->offset == b->offset)
			{
				return a->clientSymbolPair < b->clientSymbolPair;
			}
			else
				return a->offset > b->offset;
		};
		priority_queue<Others_OMS_v1c::OrderData*, vector<Others_OMS_v1c::OrderData*>, decltype(comp2)> ret(comp2);
		int i = 0;
		for (; i < data_index + 1 && i < finalSortedListLen; ++i)
		{
			ret.push(&orderData_[i]);
		}
		for (; i < data_index + 1; ++i)
		{
			if (orderData_[i].offset > ret.top()->offset)
			{
				ret.pop();
				ret.push(&orderData_[i]);
			}
		}

		int index = finalSortedListLen;
		MM_Assert::mmRunTimeAssert(ret.size() <= 100);
		while (!ret.empty() && ret.top() != nullptr)
		{
			retVal[++index] = ret.top()->clientSymbolPair;
			ret.pop();
		}

		//MM_Assert::mmRunTimeAssert(index == 0);

		return retVal;
	}

/*
Complexity:
	100										Iterate thrugh list and remove all updated objects
	+ orders * log(100)						Add updated objects in sorted list of size 100 (if offset is greater than min so far)
*/
	void Others_OMS_v1c::Others_OMS_getTop100v2(vector<const string*>& retVal)
	{ 
		//MM_Assert::mmRunTimeAssert(sortedByOffsetSet.size() == data_index + 1);
		//for (auto it = sortedByOffsetSet.begin(); it != sortedByOffsetSet.end();)
		//{
		//	OrderData* pObj = (*it);
		//	if (pObj->updated == true || pObj->symbolData->updated)
		//	{
		//		sortedByOffsetSet.erase(it++);
		//		double currentMarketPrice = pObj->symbolData->symbolPrice;
		//		pObj->offset = 100 * fabs(pObj->orderPrice - currentMarketPrice) / currentMarketPrice;
		//		sortedByOffsetSet.insert(pObj);
		//		pObj->updated = false;
		//	}
		//	else
		//		++it;
		//}

		//for (int i = 0; i < data_index + 1; ++i)
		//{
		//	OrderData* pObj = &orderData_[i];
		//	if (pObj->updated == true || pObj->symbolData->updated)
		//	{
		//		sortedByOffsetSet.erase(pObj);
		//		double currentMarketPrice = pObj->symbolData->symbolPrice;
		//		pObj->offset = 100 * fabs(pObj->orderPrice - currentMarketPrice) / currentMarketPrice;
		//		sortedByOffsetSet.insert(pObj);
		//		pObj->updated = false;
		//	}
		//}

		//for (auto it = symbolNameSymbolDataMap.begin(); it != symbolNameSymbolDataMap.end(); ++it)
		//{
		//	if (it->second.updated == true)
		//	{
		//		for(auto it2 = symbolIdIndex.lower_bound(it->second.symbolId); it2 != symbolIdIndex.end() && it2->first == it->second.symbolId; ++it2)
		//		{
		//			sortedByOffsetSet.erase(it2->second);
		//			it2->second->offset = 100 * fabs(it2->second->orderPrice - it->second.symbolPrice) / it->second.symbolPrice;
		//			sortedByOffsetSet.insert(it2->second);

		//			it->second.updated = false;
		//		}
		//	}
		//}

		for (int i = 0; i <= updatedSymbolDataIndex; ++i)
		{
			SymbolData* pObj = updatedSymbolData[i];
			auto range = symbolIdIndex.equal_range(pObj->symbolId);
			for(auto it = range.first; it != range.second && it->first == pObj->symbolId; ++it)
			{
				sortedByOffsetSet.erase(it->second);
				it->second->offset = 100 * fabs(it->second->orderPrice - pObj->symbolPrice) / pObj->symbolPrice;
				sortedByOffsetSet.insert(it->second);
			}
			pObj->updated = false;
		}
		updatedSymbolDataIndex = -1;
		//MM_Assert::mmRunTimeAssert(sortedByOffsetSet.size() == data_index + 1);

		int finalSortedListLen = std::min(100, data_index + 1);
		//vector<const string*> retVal(finalSortedListLen);
		int index = -1;
		for (auto it = sortedByOffsetSet.begin(); it != sortedByOffsetSet.end() && index < finalSortedListLen - 1; ++it)
			retVal[++index] = &(*it)->clientSymbolPair;

		//MM_Assert::mmRunTimeAssert(index == finalSortedListLen - 1);

		//return retVal;
	}
}

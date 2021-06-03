#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>
#include <locale> // For printing number as thousand separated string
#include <cmath>
using namespace std;

#include "Utils/Utils_MM_Assert.h"
#include "Others\Others_EquityOrderManagementSystem_v1b.h"

namespace mm {

/*
This version uses unordered_maps (Hash Tables) for all data structures. But do not keep the sorted list of offsets. 
It calculates top 100 offsets when needed i.e. when Others_OMS_getTop100() is called.
*/

	unsigned int Others_OMS_v1b::maxClientId = 0;
	unsigned int Others_OMS_v1b::maxSymbolId = 0;
	int Others_OMS_v1b::data_index = -1;
	vector<Others_OMS_v1b::DataSet_v1> Others_OMS_v1b::data_(0);
	unordered_map<unsigned int, Others_OMS_v1b::DataSet_v1*> Others_OMS_v1b::clientSymbolPairIndex(0);
	unordered_multimap <unsigned int, Others_OMS_v1b::DataSet_v1*> Others_OMS_v1b::symbolIndex(0);
	unordered_map <string, unsigned int> Others_OMS_v1b::clientNameClientIdMap(0);
	unordered_map <string, unsigned int> Others_OMS_v1b::symbolNameSymbolIdMap(0);
	//unordered_map<unsigned int, string> Others_OMS_v1b::clientIdClientNameMap(0);
	//unordered_map<unsigned int, string> Others_OMS_v1b::symbolIdSymbolNameMap(0);
	unordered_map<unsigned int, double> Others_OMS_v1b::symbolIdSymbolPriceMap(0);
	//multiset<Others_OMS_v1b::DataSet_v1*, Others_OMS_v1b::comp2> Others_OMS_v1b::offsetIndex(0, Others_OMS_v1b::comp());

	void Others_OMS_v1b::allocateAllMemory()
	{
		data_.resize(1000000);
		clientSymbolPairIndex.reserve(1000000);
		symbolIndex.reserve(1000000);
		clientNameClientIdMap.reserve(1000);
		symbolNameSymbolIdMap.reserve(1000);
		//clientIdClientNameMap.reserve(1000);
		//symbolIdSymbolNameMap.reserve(1000);
		symbolIdSymbolPriceMap.reserve(1000);
	}
	void Others_OMS_v1b::deallocateAllMemory()
	{
		data_.clear();
		clientSymbolPairIndex.clear();
		symbolIndex.clear();
		clientNameClientIdMap.clear();
		symbolNameSymbolIdMap.clear();
		//clientIdClientNameMap.clear();
		//symbolIdSymbolNameMap.clear();
		symbolIdSymbolPriceMap.clear();
	}

/*
Complexity:
	hash(clientName) + O(1)					hashtable search to insert/extract clientName
	+ hash(symbolName) + O(1)				hashtable search to get symbol id
	+ hash(unsigned int) + O(1)				hashtable search to get data
*/
	void Others_OMS_v1b::Others_OMS_addOrder(const string& clientName, const string& symbolName, double newOrderPrice)
	{
		//unsigned int clientId;
		//auto it1 = clientNameClientIdMap.find(clientName);
		//if (it1 == clientNameClientIdMap.end())
		//{
		//	clientNameClientIdMap[clientName] = (clientId = maxClientId++);
		//	clientIdClientNameMap[clientId] = (clientName);
		//}
		//else
		//	clientId = it1->second;
		unsigned int clientId = maxClientId;
		pair<decltype(clientNameClientIdMap)::iterator, bool> result = clientNameClientIdMap.insert(decltype(clientNameClientIdMap)::value_type(clientName, clientId));
		if (result.second == false) // new key is not interted as old key already exists
		{
			clientId = (result.first)->second;
		}
		else
		{
			++maxClientId;
			//clientIdClientNameMap[clientId] = clientName;
		}
		
		unsigned int symbolId = symbolNameSymbolIdMap[symbolName];
		unsigned int clientSymbolId = (clientId << 16) | symbolId;

		auto it = clientSymbolPairIndex.find(clientSymbolId);
		if (it != clientSymbolPairIndex.end())
		{
			it->second->orderPrice = newOrderPrice;
			double marketPrice = symbolIdSymbolPriceMap[symbolId];
			it->second->offset = 100 * fabs(newOrderPrice - marketPrice) / marketPrice;
		}
		else
		{
			data_[++data_index] = DataSet_v1(clientSymbolId, clientName + "-" + symbolName, newOrderPrice, symbolIdSymbolPriceMap[symbolId]);
			//update Index
			clientSymbolPairIndex[clientSymbolId] = &data_[data_index];
			//symbolIndex[&data_[data_index]->symbol] = data_[data_index].get(); // multimap does not have operator[]
			symbolIndex.insert(decltype(symbolIndex)::value_type(symbolId, &data_[data_index]));
		}
	}

	/*
	Complexity:
		hash(symbolName) + O(1)						hashtable search to insert/extract symbolName
		+ log(orders)								Find lower bound of symbolId
		+ number of orders for this symbol			update offset
	*/
	void Others_OMS_v1b::Others_OMS_updateMarketPrice(const string& symbolName, double currentMarketPrice)
	{
		unsigned int symbolId = maxSymbolId;
		pair<decltype(symbolNameSymbolIdMap)::iterator, bool> result = symbolNameSymbolIdMap.insert(decltype(symbolNameSymbolIdMap)::value_type(symbolName, symbolId));
		if (result.second == false) // new key is not interted as old key already exists
		{
			symbolId = (result.first)->second;
		}
		else
		{
			++maxSymbolId;
			//symbolIdSymbolNameMap[symbolId] = symbolName;
		}

		symbolIdSymbolPriceMap[symbolId] = currentMarketPrice;

		//TODO: instead of finding lower and upper bound separately, find equal_range. Need to check if it gives better performance. equal_range 
		//may need 2 * log(n) runtime
		// OR just find lower bound and then keep on incrementing iterator until symbolId is not equal
		//for(auto it = symbolIndex.lower_bound(symbolId); it != symbolIndex.upper_bound(symbolId); ++it)
		//	it->second->offset = 100 * fabs(it->second->orderPrice - currentMarketPrice) / currentMarketPrice;
		using UMapType = unordered_multimap <unsigned int, Others_OMS_v1b::DataSet_v1*>;
		pair<UMapType::iterator, UMapType::iterator> range = symbolIndex.equal_range(symbolId);
		for(auto it = range.first; it != range.second && it->first == symbolId; ++it)
		{
			it->second->offset = 100 * fabs(it->second->orderPrice - currentMarketPrice) / currentMarketPrice;
		}
	}

/*
Complexity:
	orders * log(100)						Add everything in priority queue of size 100 (if offset is greater than min so far)
	+ 100 * log(100)						extract top 100
*/
	vector<string> Others_OMS_v1b::Others_OMS_getTop100v1()
	{
		int finalSortedListLen = std::min(100, data_index + 1);
		vector<string> retVal(finalSortedListLen, "");
		auto comp2 = [] (const DataSet_v1* a, const DataSet_v1* b) -> bool
		{
			if (a->offset == b->offset)
			{
				//const string& lhsClientName = clientIdClientNameMap[a->clientSymbolId >> 16];
				//const string& rhsClientName = clientIdClientNameMap[b->clientSymbolId >> 16];
				//if (lhsClientName == rhsClientName)
				//{
				//	const string& lhsSymbolName = symbolIdSymbolNameMap[(a->clientSymbolId << 16) >> 16];
				//	const string& rhsSymbolName = symbolIdSymbolNameMap[(b->clientSymbolId << 16) >> 16];
				//	return lhsSymbolName < rhsSymbolName;
				//}
				//else
				//	return lhsClientName < rhsClientName;
				return a->clientSymbolPair < b->clientSymbolPair;
			}
			else
				return a->offset > b->offset;
		};
		priority_queue<Others_OMS_v1b::DataSet_v1*, vector<Others_OMS_v1b::DataSet_v1*>, decltype(comp2)> ret(comp2);
		int i = 0;
		for (; i < data_index + 1 && i < finalSortedListLen; ++i)
		{
			ret.push(&data_[i]);
		}
		for (; i < data_index + 1; ++i)
		{
			if (data_[i].offset > ret.top()->offset)
			{
				ret.pop();
				ret.push(&data_[i]);
			}
		}

		int index = finalSortedListLen;
		MM_Assert::mmRunTimeAssert(ret.size() <= 100);
		while (!ret.empty() && ret.top() != nullptr)
		{
			//unsigned int clientSymbolId = ret.top()->clientSymbolId;
			//unsigned int clientId = (clientSymbolId >> 16);
			//unsigned int symbolId = (clientSymbolId & ((1 << 16) - 1));
			//string clientSymbolPair(clientIdClientNameMap[clientId] + "-" + symbolIdSymbolNameMap[symbolId]);
			//retVal.push_back(clientSymbolPair);
			retVal[--index] = ret.top()->clientSymbolPair;
			ret.pop();
		}

		//MM_Assert::mmRunTimeAssert(index == 0);

		return retVal;
	}

	void Others_OMS_v1b::Others_OMS_getTop100v2(vector<const string*>& retVal)
	{
		int finalSortedListLen = std::min(100, data_index + 1);
		//vector<const string*> retVal(finalSortedListLen);
		auto comp2 = [] (const DataSet_v1* a, const DataSet_v1* b) -> bool
		{
			if (a->offset == b->offset)
			{
				//const string& lhsClientName = clientIdClientNameMap[a->clientSymbolId >> 16];
				//const string& rhsClientName = clientIdClientNameMap[b->clientSymbolId >> 16];
				//if (lhsClientName == rhsClientName)
				//{
				//	const string& lhsSymbolName = symbolIdSymbolNameMap[(a->clientSymbolId << 16) >> 16];
				//	const string& rhsSymbolName = symbolIdSymbolNameMap[(b->clientSymbolId << 16) >> 16];
				//	return lhsSymbolName < rhsSymbolName;
				//}
				//else
				//	return lhsClientName < rhsClientName;
				return a->clientSymbolPair < b->clientSymbolPair;
			}
			else
				return a->offset > b->offset;
		};
		priority_queue<Others_OMS_v1b::DataSet_v1*, vector<Others_OMS_v1b::DataSet_v1*>, decltype(comp2)> ret(comp2);
		int i = 0;
		for (; i < data_index + 1 && i < finalSortedListLen; ++i)
		{
			ret.push(&data_[i]);
		}
		for (; i < data_index + 1; ++i)
		{
			if (data_[i].offset > ret.top()->offset)
			{
				ret.pop();
				ret.push(&data_[i]);
			}
		}

		int index = finalSortedListLen;
		MM_Assert::mmRunTimeAssert(ret.size() <= 100);
		while (!ret.empty() && ret.top() != nullptr)
		{
			//unsigned int clientSymbolId = ret.top()->clientSymbolId;
			//unsigned int clientId = (clientSymbolId >> 16);
			//unsigned int symbolId = (clientSymbolId & ((1 << 16) - 1));
			//string clientSymbolPair(clientIdClientNameMap[clientId] + "-" + symbolIdSymbolNameMap[symbolId]);
			//retVal.push_back(clientSymbolPair);
			retVal[--index] = &ret.top()->clientSymbolPair;
			ret.pop();
		}

		//MM_Assert::mmRunTimeAssert(index == 0);

		//return retVal;
	}
}

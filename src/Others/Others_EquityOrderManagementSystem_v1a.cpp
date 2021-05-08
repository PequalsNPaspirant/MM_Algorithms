#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>
#include <locale> // For printing number as thousand separated string
#include <cmath>
using namespace std;

#include "Utils/Utils_MM_Assert.h"
#include "Others\Others_EquityOrderManagementSystem_v1a.h"

namespace mm {

/*
This version uses maps (BSTs) for all data structures. But do not keep the sorted list of offsets.
It calculates top 100 offsets when needed i.e. when Others_OMS_getTop100() is called.
*/

	std::function<bool(const string* a, const string* b)> Others_OMS_v1a::comp = [](const string* a, const string* b) -> bool { return (*a) < (*b); };

	vector<unique_ptr<Others_OMS_v1a::DataSet_v1>> Others_OMS_v1a::data_(0);
	int Others_OMS_v1a::data_index = -1;
	map < string*, Others_OMS_v1a::DataSet_v1*,  decltype(Others_OMS_v1a::comp)> Others_OMS_v1a::clientSymbolPairIndex(Others_OMS_v1a::comp);
	multimap <const string*, Others_OMS_v1a::DataSet_v1*,  decltype(Others_OMS_v1a::comp)> Others_OMS_v1a::symbolIndex(Others_OMS_v1a::comp);
	//multiset<Others_OMS_v1a::DataSet_v1*, Others_OMS_v1a::comp2> Others_OMS_v1a::offsetIndex(0, Others_OMS_v1a::comp());

	map < string, double > Others_OMS_v1a::symbolMarketPriceIndex;

	void Others_OMS_v1a::allocateAllMemory()
	{
		data_.resize(1000000);
	}
	void Others_OMS_v1a::deallocateAllMemory()
	{
		data_.clear();
	}

/*
Compexity: 
if the order is already present:
	log(orders)
if the order is not present:
	search order + insert order into clientSymbolPairIndex + insert entry into symbolIndex
	log(orders) + log(orders) + log(orders)
*/
	void Others_OMS_v1a::Others_OMS_addOrder(const string& client, const string& symbol, double newOrderPrice)
	{
		string clientSymbolPair = client + "-" + symbol;
		double currentMarketPrice = symbolMarketPriceIndex[symbol];
		auto it = clientSymbolPairIndex.find(&clientSymbolPair);
		if (it != clientSymbolPairIndex.end())
		{
			it->second->orderPrice = newOrderPrice;
			it->second->offset = 100 * fabs(newOrderPrice - currentMarketPrice) / currentMarketPrice;
		}
		else
		{
			data_[++data_index] = make_unique<Others_OMS_v1a::DataSet_v1>(client, symbol, std::move(clientSymbolPair), newOrderPrice, currentMarketPrice);
			//update Index
			clientSymbolPairIndex[&(data_[data_index]->clientSymbolPair)] = data_[data_index].get();
			//symbolIndex[&data_[data_index]->symbol] = data_[data_index].get(); // multimap does not have operator[]
			symbolIndex.insert(decltype(symbolIndex)::value_type(&data_[data_index]->symbol, data_[data_index].get()));
		}
	}

	/*
	Compexity: 
		search/update market price + update offset of all relative orders
		log(symbols)               + log(orders) + number of orders of this symbol
	*/
	void Others_OMS_v1a::Others_OMS_updateMarketPrice(const string& symbol, double currentMarketPrice)
	{
		//pair<decltype(symbolMarketPriceIndex)::iterator, bool> it = symbolMarketPriceIndex.insert(decltype(symbolMarketPriceIndex)::value_type(symbol, currentMarketPrice));
		//if (it.second == false) // new key is not interted as old key already exists
		//	(it.first)->second = currentMarketPrice;
		symbolMarketPriceIndex[symbol] = currentMarketPrice;
		auto range = symbolIndex.equal_range(&symbol);
		for(auto it = range.first; it != range.second; ++it)
			it->second->offset = 100 * fabs(it->second->orderPrice - currentMarketPrice) / currentMarketPrice;
	}

	/*
	Complexity:
		Add everything in priority queue of size 100 (if offset is greater than min so far) + extract top 100
		orders * log(100) + 100 * log(100)
	*/
	vector<string> Others_OMS_v1a::Others_OMS_getTop100v1()
	{
		int finalSortedListLen = std::min(100, data_index + 1);
		vector<string> retVal(finalSortedListLen, "");
		auto comp2 = [] (const DataSet_v1* a, const DataSet_v1* b) -> bool
		{
			if (a->offset == b->offset)
				return a->clientSymbolPair < b->clientSymbolPair;
			else
				return a->offset > b->offset;
		};
		priority_queue<Others_OMS_v1a::DataSet_v1*, vector<Others_OMS_v1a::DataSet_v1*>, decltype(comp2)> ret(comp2);
		int i = 0;
		for (; i < data_index + 1 && i < finalSortedListLen; ++i)
		{
			ret.push(data_[i].get());
		}
		for (; i < data_index + 1; ++i)
		{
			if (data_[i]->offset > ret.top()->offset)
			{
				ret.pop();
				ret.push(data_[i].get());
			}
		}

		int index = finalSortedListLen;
		while (!ret.empty())
		{
			retVal[--index] = ret.top()->clientSymbolPair;
			//retVal.push_back(ret.top()->clientSymbolPair);
			ret.pop();
		}

		//MyAssert::myRunTimeAssert(index == 0);

		return retVal;
	}

	void Others_OMS_v1a::Others_OMS_getTop100v2(vector<const string*>& retVal)
	{
		int finalSortedListLen = std::min(100, data_index + 1);
		//vector<const string*> retVal(finalSortedListLen, nullptr);
		//vector<const string*> retVal(finalSortedListLen); // No need to set it to nullptr, let it have garbage value
		auto comp2 = [] (const DataSet_v1* a, const DataSet_v1* b) -> bool
		{
			if (a->offset == b->offset)
				return a->clientSymbolPair < b->clientSymbolPair;
			else
				return a->offset > b->offset;
		};
		priority_queue<Others_OMS_v1a::DataSet_v1*, vector<Others_OMS_v1a::DataSet_v1*>, decltype(comp2)> ret(comp2);
		int i = 0;
		for (; i < data_index + 1 && i < finalSortedListLen; ++i)
		{
			ret.push(data_[i].get());
		}
		for (; i < data_index + 1; ++i)
		{
			if (data_[i]->offset > ret.top()->offset)
			{
				ret.pop();
				ret.push(data_[i].get());
			}
		}

		int index = finalSortedListLen;
		while (!ret.empty())
		{
			retVal[--index] = &ret.top()->clientSymbolPair;
			//retVal.push_back(ret.top()->clientSymbolPair);
			ret.pop();
		}

		//MyAssert::myRunTimeAssert(index == 0);

		//return retVal;
	}

	/*
	void Others_OMS_v1a::Others_OMS_getTop100v3(vector<std::pair<const string*, const string*>>& top100v2, unsigned int& count)
	{
		int finalSortedListLen = std::min(100, data_index + 1);
		//vector<string> retVal(finalSortedListLen, "");
		auto comp2 = [] (const DataSet_v1* a, const DataSet_v1* b) -> bool
		{
			if (a->offset == b->offset)
				return a->clientSymbolPair < b->clientSymbolPair;
			else
				return a->offset > b->offset;
		};
		priority_queue<Others_OMS_v1a::DataSet_v1*, vector<Others_OMS_v1a::DataSet_v1*>, decltype(comp2)> ret(comp2);
		int i = 0;
		for (; i < data_index + 1 && i < finalSortedListLen; ++i)
		{
			ret.push(data_[i].get());
		}
		for (; i < data_index + 1; ++i)
		{
			if (data_[i]->offset > ret.top()->offset)
			{
				ret.pop();
				ret.push(data_[i].get());
			}
		}

		int index = finalSortedListLen;
		count = index;
		while (!ret.empty())
		{
			//retVal[--index] = ret.top()->clientSymbolPair;
			//retVal.push_back(ret.top()->clientSymbolPair);
			top100v2[--index].first = &ret.top()->client;
			top100v2[index].second = &ret.top()->symbol;
			ret.pop();
		}

		//MyAssert::myRunTimeAssert(index == 0);

		//return retVal;
	}
	*/
}

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>
#include <locale> // For printing number as thousand separated string
#include <cmath>
using namespace std;

#include "Utils/Utils_MM_Assert.h"
#include "Others\Others_EquityOrderManagementSystem_v4a.h"

namespace mm {

/*
This version is optimized implementation of boost multiindex. 
It prepares the structure like database and keep indexes on all desired columns except for offsets.
It calculates offsets when required i.e. when Others_OMS_getTop100() is called.
*/

	vector<Others_OMS_v4a::OrderTable> Others_OMS_v4a::OrderTableData(0);
	int Others_OMS_v4a::OrderTableDataIndex = -1;
	unordered_set<Others_OMS_v4a::OrderTable*, Others_OMS_v4a::HasherOrderTablePrimaryKeyClientSymbolId, Others_OMS_v4a::IsEqualOrderTablePrimaryKeyClientSymbolId> 
		Others_OMS_v4a::OrderTablePrimaryKeyClientSymbolId(0);
	unordered_multiset<Others_OMS_v4a::OrderTable*, Others_OMS_v4a::HasherOrderTableIndexSymbolId, Others_OMS_v4a::IsEqualOrderTableIndexSymbolId> 
		Others_OMS_v4a::OrderTableIndexSymbolId(0);

	vector<Others_OMS_v4a::ClientTable> Others_OMS_v4a::ClientTableData(0);
	int Others_OMS_v4a::ClientTableDataIndex = -1;
	unordered_set<Others_OMS_v4a::ClientTable*, Others_OMS_v4a::HasherClientTablePrimaryKeyClientId, Others_OMS_v4a::IsEqualClientTablePrimaryKeyClientId> 
		Others_OMS_v4a::ClientTablePrimaryKeyClientId(0); // This is required in case clientId is not equal to index in ClientTableData
	unordered_set<Others_OMS_v4a::ClientTable*, Others_OMS_v4a::HasherClientTableIndexClientName, Others_OMS_v4a::IsEqualClientTableIndexClientName> 
		Others_OMS_v4a::ClientTableIndexClientName(0);

	vector<Others_OMS_v4a::SymbolTable> Others_OMS_v4a::SymbolTableData(0);
	int Others_OMS_v4a::SymbolTableDataIndex = -1;
	unordered_set<Others_OMS_v4a::SymbolTable*, Others_OMS_v4a::HasherSymbolTablePrimaryKeySymbolId, Others_OMS_v4a::IsEqualSymbolTablePrimaryKeySymbolId> 
		Others_OMS_v4a::SymbolTablePrimaryKeySymbolId(0); // This is required in case symbolId is not equal to index in SymbolTableData
	unordered_set<Others_OMS_v4a::SymbolTable*, Others_OMS_v4a::HasherSymbolTableIndexSymbolName, Others_OMS_v4a::IsEqualSymbolTableIndexSymbolName> 
		Others_OMS_v4a::SymbolTableIndexSymbolName(0);

#ifdef MM_DEBUG_OTHERS_EOMS
	vector<ClientSymbolPair> Others_OMS_v4a::finalSortedList;
#endif

	void Others_OMS_v4a::allocateAllMemory()
	{
		OrderTableData.resize(1000000 + 1);
		OrderTablePrimaryKeyClientSymbolId.reserve(1000000);
		OrderTableIndexSymbolId.reserve(1000000);

		ClientTableData.resize(1000 + 1);
		ClientTablePrimaryKeyClientId.reserve(1000);
		ClientTableIndexClientName.reserve(1000);

		SymbolTableData.resize(1000 + 1);
		SymbolTablePrimaryKeySymbolId.reserve(1000);
		SymbolTableIndexSymbolName.reserve(1000);
	}
	void Others_OMS_v4a::deallocateAllMemory()
	{
		OrderTableData.clear();
		OrderTablePrimaryKeyClientSymbolId.clear();
		OrderTableIndexSymbolId.clear();

		ClientTableData.clear();
		ClientTablePrimaryKeyClientId.clear();
		ClientTableIndexClientName.clear();

		SymbolTableData.clear();
		SymbolTablePrimaryKeySymbolId.clear();
		SymbolTableIndexSymbolName.clear();
	}

/*
Complexity:
	hash(clientName) + O(1)									search/insert clientName in hashtable ClientTableIndexClientName 
	+ hash(clientId i.e. unsinged int) + O(1)				update ClientTablePrimaryKeyClientId if this is new client
	+ hash(symbolName) + O(1)								search symbolName in hashtable SymbolTableIndexSymbolName (symbol is guaranteed to be there)
	+ combined hash for clientId & symbolId + hash(client-symbol ID i.e. unsinged int) + O(1)			search/insert order into OrderTablePrimaryKeyClientSymbolId
	+ if this is first order for this client-symbol i.e. this clint-symbol pair does not exist in order table
		hash(symbolId i.e. unsigned int) + O(1)				insert new entry into OrderTablePositionIndexSymbolId

Changes over v3c: The following part is removed from algo v3c
	else
		log(orders)											erase entry from sorted list OrderTableSortIndexOffset ONLY if the new offset changes the current position in sorted list or is less than min so far
	+ log(orders)											insert new entry into sorted list OrderTableSortIndexOffset ONLY if erased due to above condition or is new element & offset is greater than min so far
*/
	void Others_OMS_v4a::Others_OMS_addOrder(const string& clientName, const string& symbolName, double newOrderPrice)
	{
		unsigned int clientId = ++ClientTableDataIndex;
		ClientTableData[ClientTableDataIndex] = ClientTable(clientId, clientName);
		pair<decltype(ClientTableIndexClientName)::iterator, bool> result1 = ClientTableIndexClientName.insert(&ClientTableData[ClientTableDataIndex]);
		if (result1.second == false) // Key already exists
		{
			--ClientTableDataIndex;
			clientId = (*result1.first)->clientId;
		}
		else
		{
			pair<decltype(ClientTablePrimaryKeyClientId)::iterator, bool> result2 = ClientTablePrimaryKeyClientId.insert(&ClientTableData[clientId]);
			MyAssert::myRunTimeAssert(result2.second == true);
		}
		
		SymbolTable dummySymbol(0, symbolName, 0);
		auto it = SymbolTableIndexSymbolName.find(&dummySymbol);
		unsigned int symbolId = (*it)->symbolId;
		double marketPrice = (*it)->marketPrice;
		double newOffset = 100 * fabs(newOrderPrice - marketPrice) / marketPrice;

		OrderTableData[++OrderTableDataIndex] = OrderTable(clientId, symbolId, clientName + "-" + symbolName, newOrderPrice, newOffset);
		pair<decltype(OrderTablePrimaryKeyClientSymbolId)::iterator, bool> result3 = OrderTablePrimaryKeyClientSymbolId.insert(&OrderTableData[OrderTableDataIndex]);
		if (result3.second == false) // Key already exists
		{
			--OrderTableDataIndex;
			(*result3.first)->orderPrice = newOrderPrice;
			(*result3.first)->offset = newOffset;
		}
		else
		{
			OrderTableIndexSymbolId.insert(&OrderTableData[OrderTableDataIndex]);
		}
	}

/*
Complexity:
	hash(symbolName) + O(1)							search/insert into SymbolTableIndexSymbolName
	+ hash(symbolId i.e. unsigned int) + O(1)		Only for new symbols: insert into SymbolTablePrimaryKeySymbolId
	+ 2 * hash(symbolId i.e. unsigned int) + number of orders for this symbol		update offsets for all orders with this symbol

Changes over v3c: The following part is removed from v3c
	+ number of orders for this symbol * 2 * log(number of orders for this symbol)	erase old and insert new into OrderTableSortIndexOffset ONLY if the new offset changes the current position in sorted list or is less than min so far
*/
	void Others_OMS_v4a::Others_OMS_updateMarketPrice(const string& symbolName, double currentMarketPrice)
	{
		unsigned int symbolId = ++SymbolTableDataIndex;
		SymbolTableData[SymbolTableDataIndex] = SymbolTable(symbolId, symbolName, currentMarketPrice);
		pair<decltype(SymbolTableIndexSymbolName)::iterator, bool> result1 = SymbolTableIndexSymbolName.insert(&SymbolTableData[symbolId]);
		if (result1.second == false) // Key already exists
		{
			--SymbolTableDataIndex; 
			symbolId = (*result1.first)->symbolId;
			(*result1.first)->marketPrice = currentMarketPrice;
		}
		else
		{
			pair<decltype(SymbolTablePrimaryKeySymbolId)::iterator, bool> result2 = SymbolTablePrimaryKeySymbolId.insert(&SymbolTableData[symbolId]);
			MyAssert::myRunTimeAssert(result2.second == true);
		}

		OrderTable dummyOrder(0, symbolId, "", 0, 0);
		auto range = OrderTableIndexSymbolId.equal_range(&dummyOrder);
		for(auto it = range.first; it != range.second; ++it)
			(*it)->offset = 100 * fabs((*it)->orderPrice - currentMarketPrice) / currentMarketPrice;
	}

/*
Complexity:
	number of orders * log(100)			Iterate through all elements and collect top 100 by offset into heap
	+ 100 * log(100)					extract max 100 times from heap	
*/
	vector<string> Others_OMS_v4a::Others_OMS_getTop100v1()
	{
		int finalSortedListLen = std::min(100, OrderTableDataIndex + 1);
#ifdef MM_DEBUG_OTHERS_EOMS
		finalSortedList.resize(finalSortedListLen);
#endif
		vector<string> retVal(finalSortedListLen, "");
		auto comp2 = [] (const OrderTable* a, const OrderTable* b) -> bool
		{
			if (a->offset == b->offset)
			{
				ClientTable lhsDummyClient(a->clientId, "");
				ClientTable rhsDummyClient(b->clientId, "");
				const string& lhsClientName = (*ClientTablePrimaryKeyClientId.find(&lhsDummyClient))->clientName;
				const string& rhsClientName = (*ClientTablePrimaryKeyClientId.find(&rhsDummyClient))->clientName;
				if (lhsClientName == rhsClientName)
				{
					SymbolTable lhsDummySymbol(a->symbolId, "", 0);
					SymbolTable rhsDummySymbol(b->symbolId, "", 0);
					const string& lhsSymbolName = (*SymbolTablePrimaryKeySymbolId.find(&lhsDummySymbol))->symbolName;
					const string& rhsSymbolName = (*SymbolTablePrimaryKeySymbolId.find(&rhsDummySymbol))->symbolName;
					return lhsSymbolName < rhsSymbolName;
				}
				else
					return lhsClientName < rhsClientName;
			}
			else
				return a->offset > b->offset;
		};
		priority_queue<Others_OMS_v4a::OrderTable*, vector<Others_OMS_v4a::OrderTable*>, decltype(comp2)> ret(comp2);
		int i = 0;
		for (; i <= OrderTableDataIndex && i < finalSortedListLen; ++i)
		{
			ret.push(&OrderTableData[i]);
		}
		for (; i <= OrderTableDataIndex; ++i)
		{
			if (OrderTableData[i].offset > ret.top()->offset)
			{
				ret.pop();
				ret.push(&OrderTableData[i]);
			}
		}

		int index = finalSortedListLen;
		while (!ret.empty())
		{
			unsigned int clientId = ret.top()->clientId;
			unsigned int symbolId = ret.top()->symbolId;
			ClientTable dummyClient(clientId, "");
			SymbolTable dummySymbol(symbolId, "", 0);
			string clientSymbolPair((*ClientTablePrimaryKeyClientId.find(&dummyClient))->clientName + "-" + (*SymbolTablePrimaryKeySymbolId.find(&dummySymbol))->symbolName);
			//retVal.push_back(clientSymbolPair);
			retVal[--index] = clientSymbolPair;
#ifdef MM_DEBUG_OTHERS_EOMS
			double marketPrice = (*SymbolTablePrimaryKeySymbolId.find(&dummySymbol))->marketPrice;
			finalSortedList[index] = ClientSymbolPair(clientId, symbolId, ret.top()->orderPrice, marketPrice, ret.top()->offset);
#endif
			ret.pop();
		}

		//MyAssert::myRunTimeAssert(index == 0);

		return retVal;
	}

	void Others_OMS_v4a::Others_OMS_getTop100v2(vector<const string*>& retVal)
	{
		int finalSortedListLen = std::min(100, OrderTableDataIndex + 1);
#ifdef MM_DEBUG_OTHERS_EOMS
		finalSortedList.resize(finalSortedListLen);
#endif
		//vector<const string*> retVal(finalSortedListLen);
		auto comp2 = [] (const OrderTable* a, const OrderTable* b) -> bool
		{
			if (a->offset == b->offset)
			{
				ClientTable lhsDummyClient(a->clientId, "");
				ClientTable rhsDummyClient(b->clientId, "");
				const string& lhsClientName = (*ClientTablePrimaryKeyClientId.find(&lhsDummyClient))->clientName;
				const string& rhsClientName = (*ClientTablePrimaryKeyClientId.find(&rhsDummyClient))->clientName;
				if (lhsClientName == rhsClientName)
				{
					SymbolTable lhsDummySymbol(a->symbolId, "", 0);
					SymbolTable rhsDummySymbol(b->symbolId, "", 0);
					const string& lhsSymbolName = (*SymbolTablePrimaryKeySymbolId.find(&lhsDummySymbol))->symbolName;
					const string& rhsSymbolName = (*SymbolTablePrimaryKeySymbolId.find(&rhsDummySymbol))->symbolName;
					return lhsSymbolName < rhsSymbolName;
				}
				else
					return lhsClientName < rhsClientName;
			}
			else
				return a->offset > b->offset;
		};
		priority_queue<Others_OMS_v4a::OrderTable*, vector<Others_OMS_v4a::OrderTable*>, decltype(comp2)> ret(comp2);
		int i = 0;
		for (; i <= OrderTableDataIndex && i < finalSortedListLen; ++i)
		{
			ret.push(&OrderTableData[i]);
		}
		for (; i <= OrderTableDataIndex; ++i)
		{
			if (OrderTableData[i].offset > ret.top()->offset)
			{
				ret.pop();
				ret.push(&OrderTableData[i]);
			}
		}

		int index = finalSortedListLen;
		while (!ret.empty())
		{
			//unsigned int clientId = ret.top()->clientId;
			//unsigned int symbolId = ret.top()->symbolId;
			//ClientTable dummyClient(clientId, "");
			//SymbolTable dummySymbol(symbolId, "", 0);
			//string clientSymbolPair((*ClientTablePrimaryKeyClientId.find(&dummyClient))->clientName + "-" + (*SymbolTablePrimaryKeySymbolId.find(&dummySymbol))->symbolName);
			//retVal.push_back(clientSymbolPair);
			retVal[--index] = &ret.top()->clientSymbolPair;
#ifdef MM_DEBUG_OTHERS_EOMS
			double marketPrice = (*SymbolTablePrimaryKeySymbolId.find(&dummySymbol))->marketPrice;
			finalSortedList[index] = ClientSymbolPair(clientId, symbolId, ret.top()->orderPrice, marketPrice, ret.top()->offset);
#endif
			ret.pop();
		}

		//MyAssert::myRunTimeAssert(index == 0);

		//return retVal;
	}
}

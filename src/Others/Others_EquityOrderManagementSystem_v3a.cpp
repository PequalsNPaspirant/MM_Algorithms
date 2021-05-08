#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>
#include <locale> // For printing number as thousand separated string
using namespace std;

#include "Utils/Utils_MM_Assert.h"
#include "Others\Others_EquityOrderManagementSystem_v3a.h"

namespace mm {

/*
This version is ideal implementation of boost multiindex. It prepares the structure like database and keeps indexes on all desired columns. 
*/

	vector<Others_OMS_v3a::OrderTable> Others_OMS_v3a::OrderTableData(0);
	int Others_OMS_v3a::OrderTableDataIndex = -1;
	unordered_set<Others_OMS_v3a::OrderTable*, Others_OMS_v3a::HasherOrderTablePrimaryKeyClientSymbolId, Others_OMS_v3a::IsEqualOrderTablePrimaryKeyClientSymbolId> 
		Others_OMS_v3a::OrderTablePrimaryKeyClientSymbolId(0);
	unordered_multiset<Others_OMS_v3a::OrderTable*, Others_OMS_v3a::HasherOrderTableIndexSymbolId, Others_OMS_v3a::IsEqualOrderTableIndexSymbolId> 
		Others_OMS_v3a::OrderTablePositionIndexSymbolId(0);
	//TODO: add index on offset to get the sorted list at last
	// OR keep a running min heap and keep it updating
	set<Others_OMS_v3a::OrderTable*, Others_OMS_v3a::CompareOrderTableIndexOffset> Others_OMS_v3a::OrderTableSortIndexOffset;
	//double Others_OMS_v3a::minOffsetInSortestList = 0.0;

	vector<Others_OMS_v3a::ClientTable> Others_OMS_v3a::ClientTableData(0);
	int Others_OMS_v3a::ClientTableDataIndex = -1;
	unordered_set<Others_OMS_v3a::ClientTable*, Others_OMS_v3a::HasherClientTablePrimaryKeyClientId, Others_OMS_v3a::IsEqualClientTablePrimaryKeyClientId> 
		Others_OMS_v3a::ClientTablePrimaryKeyClientId(0); // This is required in case clientId is not equal to index in ClientTableData
	unordered_set<Others_OMS_v3a::ClientTable*, Others_OMS_v3a::HasherClientTableIndexClientName, Others_OMS_v3a::IsEqualClientTableIndexClientName> 
		Others_OMS_v3a::ClientTableIndexClientName(0);

	vector<Others_OMS_v3a::SymbolTable> Others_OMS_v3a::SymbolTableData(0);
	int Others_OMS_v3a::SymbolTableDataIndex = -1;
	unordered_set<Others_OMS_v3a::SymbolTable*, Others_OMS_v3a::HasherSymbolTablePrimaryKeySymbolId, Others_OMS_v3a::IsEqualSymbolTablePrimaryKeySymbolId> 
		Others_OMS_v3a::SymbolTablePrimaryKeySymbolId(0); // This is required in case symbolId is not equal to index in SymbolTableData
	unordered_set<Others_OMS_v3a::SymbolTable*, Others_OMS_v3a::HasherSymbolTableIndexSymbolName, Others_OMS_v3a::IsEqualSymbolTableIndexSymbolName> 
		Others_OMS_v3a::SymbolTableIndexSymbolName(0);

	void Others_OMS_v3a::allocateAllMemory()
	{
		OrderTableData.resize(1000000 + 1);
		OrderTablePrimaryKeyClientSymbolId.reserve(1000000);
		OrderTablePositionIndexSymbolId.reserve(1000000);
		ClientTableData.resize(1000 + 1);
		ClientTablePrimaryKeyClientId.reserve(1000);
		ClientTableIndexClientName.reserve(1000);
		SymbolTableData.resize(1000 + 1);
		SymbolTablePrimaryKeySymbolId.reserve(1000);
		SymbolTableIndexSymbolName.reserve(1000);
	}
	void Others_OMS_v3a::deallocateAllMemory()
	{
		OrderTableData.clear();
		OrderTablePrimaryKeyClientSymbolId.clear();
		OrderTablePositionIndexSymbolId.clear();
		ClientTableData.clear();
		ClientTablePrimaryKeyClientId.clear();
		ClientTableIndexClientName.clear();
		SymbolTableData.clear();
		SymbolTablePrimaryKeySymbolId.clear();
		SymbolTableIndexSymbolName.clear();
	}

#ifdef MM_DEBUG_OTHERS_EOMS
	vector<ClientSymbolPair> Others_OMS_v3a::finalSortedList;
#endif

/*
Complexity:
	hash(clientName) + O(1)									search/insert clientName in hashtable ClientTableIndexClientName 
	+ hash(clientId i.e. unsinged int) + O(1)				update ClientTablePrimaryKeyClientId if this is new client
	+ hash(symbolName) + O(1)								search symbolName in hashtable SymbolTableIndexSymbolName (symbol is guaranteed to be there)
	+ combined hash for clientId & symbolId + hash(client-symbol ID i.e. unsinged int) + O(1)			search/insert order into OrderTablePrimaryKeyClientSymbolId
	+ if this is first order for this client-symbol i.e. this clint-symbol pair does not exist in order table
		hash(symbolId i.e. unsigned int) + O(1)				insert new entry into OrderTablePositionIndexSymbolId
	  else
		log(orders)											erase entry from sorted list OrderTableSortIndexOffset
	+ log(orders)											insert new entry into sorted list OrderTableSortIndexOffset
*/
	void Others_OMS_v3a::Others_OMS_addOrder(const string& clientName, const string& symbolName, double newOrderPrice)
	{
		unsigned int clientId = ++ClientTableDataIndex;
		ClientTableData[ClientTableDataIndex] = ClientTable(clientId, clientName);
		pair<decltype(ClientTableIndexClientName)::iterator, bool> result1 = ClientTableIndexClientName.insert(&ClientTableData[clientId]);
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
		OrderTable* pNewElement = &OrderTableData[OrderTableDataIndex];
		pair<decltype(OrderTablePrimaryKeyClientSymbolId)::iterator, bool> result3 = OrderTablePrimaryKeyClientSymbolId.insert(&OrderTableData[OrderTableDataIndex]);
		bool updateSet = true;
		if (result3.second == false) // Key already exists
		{
			pNewElement = *result3.first;
			OrderTableSortIndexOffset.erase(pNewElement); // delete before updating offset
			--OrderTableDataIndex;
			pNewElement->orderPrice = newOrderPrice;
			pNewElement->offset = newOffset;
		}
		else
		{
			OrderTablePositionIndexSymbolId.insert(pNewElement);
		}

		OrderTableSortIndexOffset.insert(pNewElement);
	}

/*
Complexity:
	hash(symbolName) + O(1)							search/insert into SymbolTableIndexSymbolName
	+ hash(symbolId i.e. unsigned int) + O(1)		Only for new symbols: insert into SymbolTableIndexSymbolName
	+ 2 * hash(symbolId i.e. unsigned int) + number of orders for this symbol		update offsets for all orders with this symbol
	+ number of orders for this symbol * 2 * log(number of orders for this symbol)	erase old and insert new into OrderTableSortIndexOffset
*/
	void Others_OMS_v3a::Others_OMS_updateMarketPrice(const string& symbolName, double currentMarketPrice)
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
		auto range = OrderTablePositionIndexSymbolId.equal_range(&dummyOrder);
		for (auto it = range.first; it != range.second; ++it)
		{
			double newOffset = 100 * fabs((*it)->orderPrice - currentMarketPrice) / currentMarketPrice;
			OrderTableSortIndexOffset.erase(*it);
			(*it)->offset = newOffset;
			OrderTableSortIndexOffset.insert(*it);
		}
	}

/*
Complexity:
	100						iterate through top 100 elements in already sorted list
*/
	vector<string> Others_OMS_v3a::Others_OMS_getTop100v1()
	{
		int finalSortedListLen = std::min(100, OrderTableDataIndex + 1);

#ifdef MM_DEBUG_OTHERS_EOMS
		finalSortedList.resize(finalSortedListLen);
#endif

		int index = 0;
		vector<string> retVal(finalSortedListLen, "");
		for (auto it = OrderTableSortIndexOffset.begin(); it != OrderTableSortIndexOffset.end() && index < finalSortedListLen; ++it, ++index)
		{
			unsigned int clientId = (*it)->clientId;
			unsigned int symbolId = (*it)->symbolId;
			ClientTable dummyClient(clientId, "");
			SymbolTable dummySymbol(symbolId, "", 0);
			string clientSymbolPair((*ClientTablePrimaryKeyClientId.find(&dummyClient))->clientName + "-" + (*SymbolTablePrimaryKeySymbolId.find(&dummySymbol))->symbolName);
			retVal[index] = clientSymbolPair;

#ifdef MM_DEBUG_OTHERS_EOMS
			double marketPrice = (*SymbolTablePrimaryKeySymbolId.find(&dummySymbol))->marketPrice;
			finalSortedList[index] = ClientSymbolPair(clientId, symbolId, (*it)->orderPrice, marketPrice, (*it)->offset);
#endif
		}

		//MyAssert::myRunTimeAssert(index == finalSortedListLen);

		return retVal;
	}

	vector<const string*> Others_OMS_v3a::Others_OMS_getTop100v2()
	{
		int finalSortedListLen = std::min(100, OrderTableDataIndex + 1);

#ifdef MM_DEBUG_OTHERS_EOMS
		finalSortedList.resize(finalSortedListLen);
#endif

		int index = 0;
		vector<const string*> retVal(finalSortedListLen);
		for (auto it = OrderTableSortIndexOffset.begin(); it != OrderTableSortIndexOffset.end() && index < finalSortedListLen; ++it, ++index)
		{
			//unsigned int clientId = (*it)->clientId;
			//unsigned int symbolId = (*it)->symbolId;
			//ClientTable dummyClient(clientId, "");
			//SymbolTable dummySymbol(symbolId, "", 0);
			//string clientSymbolPair((*ClientTablePrimaryKeyClientId.find(&dummyClient))->clientName + "-" + (*SymbolTablePrimaryKeySymbolId.find(&dummySymbol))->symbolName);
			retVal[index] = &(*it)->clientSymbolPair;

#ifdef MM_DEBUG_OTHERS_EOMS
			double marketPrice = (*SymbolTablePrimaryKeySymbolId.find(&dummySymbol))->marketPrice;
			finalSortedList[index] = ClientSymbolPair(clientId, symbolId, (*it)->orderPrice, marketPrice, (*it)->offset);
#endif
		}

		//MyAssert::myRunTimeAssert(index == finalSortedListLen);

		return retVal;
	}
}

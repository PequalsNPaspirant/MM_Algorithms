#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>
#include <locale> // For printing number as thousand separated string
using namespace std;

#include "Utils/Utils_MM_Assert.h"
#include "Others\Others_EquityOrderManagementSystem_v4b.h"

namespace mm {

/*
This version is optimized implementation of boost multiindex. 
It prepares the structure like database and keep indexes on all desired columns except for offsets.
It keeps the update flag on orders and symbol price.
It calculates offsets when required i.e. when Others_OMS_getTop100() is called.
*/

	vector<Others_OMS_v4b::OrderTable> Others_OMS_v4b::OrderTableData(0);
	int Others_OMS_v4b::OrderTableDataIndex = -1;
	vector<Others_OMS_v4b::OrderTable*> Others_OMS_v4b::UpdatedOrderTableData(0);
	int Others_OMS_v4b::UpdatedOrderTableDataIndex = -1;
	unordered_set<Others_OMS_v4b::OrderTable*, Others_OMS_v4b::HasherOrderTablePrimaryKeyClientSymbolId, Others_OMS_v4b::IsEqualOrderTablePrimaryKeyClientSymbolId> 
		Others_OMS_v4b::OrderTablePrimaryKeyClientSymbolId(0);
	unordered_multiset<Others_OMS_v4b::OrderTable*, Others_OMS_v4b::HasherOrderTableIndexSymbolId, Others_OMS_v4b::IsEqualOrderTableIndexSymbolId> 
		Others_OMS_v4b::OrderTableIndexSymbolId(0);

	vector<Others_OMS_v4b::ClientTable> Others_OMS_v4b::ClientTableData(0);
	int Others_OMS_v4b::ClientTableDataIndex = -1;
	unordered_set<Others_OMS_v4b::ClientTable*, Others_OMS_v4b::HasherClientTablePrimaryKeyClientId, Others_OMS_v4b::IsEqualClientTablePrimaryKeyClientId> 
		Others_OMS_v4b::ClientTablePrimaryKeyClientId(0); // This is required in case clientId is not equal to index in ClientTableData
	unordered_set<Others_OMS_v4b::ClientTable*, Others_OMS_v4b::HasherClientTableIndexClientName, Others_OMS_v4b::IsEqualClientTableIndexClientName> 
		Others_OMS_v4b::ClientTableIndexClientName(0);

	vector<Others_OMS_v4b::SymbolTable> Others_OMS_v4b::SymbolTableData(0);
	int Others_OMS_v4b::SymbolTableDataIndex = -1;
	vector<Others_OMS_v4b::SymbolTable*> Others_OMS_v4b::UpdatedSymbolTableData(0);
	int Others_OMS_v4b::UpdatedSymbolTableDataIndex = -1;
	unordered_set<Others_OMS_v4b::SymbolTable*, Others_OMS_v4b::HasherSymbolTablePrimaryKeySymbolId, Others_OMS_v4b::IsEqualSymbolTablePrimaryKeySymbolId> 
		Others_OMS_v4b::SymbolTablePrimaryKeySymbolId(0); // This is required in case symbolId is not equal to index in SymbolTableData
	unordered_set<Others_OMS_v4b::SymbolTable*, Others_OMS_v4b::HasherSymbolTableIndexSymbolName, Others_OMS_v4b::IsEqualSymbolTableIndexSymbolName> 
		Others_OMS_v4b::SymbolTableIndexSymbolName(0);

#ifdef MM_DEBUG_OTHERS_EOMS
	vector<ClientSymbolPair> Others_OMS_v4b::finalSortedList;
#endif

	void Others_OMS_v4b::allocateAllMemory()
	{
		OrderTableData.resize(1000000 + 1);
		UpdatedOrderTableData.resize(1000000, nullptr);
		OrderTablePrimaryKeyClientSymbolId.reserve(1000000);
		OrderTableIndexSymbolId.reserve(1000000);

		ClientTableData.resize(1000 + 1);
		ClientTablePrimaryKeyClientId.reserve(1000);
		ClientTableIndexClientName.reserve(1000);

		SymbolTableData.resize(1000 + 1);
		UpdatedSymbolTableData.resize(1000, nullptr);
		SymbolTablePrimaryKeySymbolId.reserve(1000);
		SymbolTableIndexSymbolName.reserve(1000);
	}
	void Others_OMS_v4b::deallocateAllMemory()
	{
		OrderTableData.clear();
		UpdatedOrderTableData.clear();
		OrderTablePrimaryKeyClientSymbolId.clear();
		OrderTableIndexSymbolId.clear();

		ClientTableData.clear();
		ClientTablePrimaryKeyClientId.clear();
		ClientTableIndexClientName.clear();

		SymbolTableData.clear();
		UpdatedSymbolTableData.clear();
		SymbolTablePrimaryKeySymbolId.clear();
		SymbolTableIndexSymbolName.clear();
	}

/*
Complexity is same as v4a, but it keeps updated flag on new or updated orders, does not calculate new offset here
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
	void Others_OMS_v4b::Others_OMS_addOrder(const string& clientName, const string& symbolName, double newOrderPrice)
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
		//double newOffset = 100 * fabs(newOrderPrice - marketPrice) / marketPrice;

		OrderTableData[++OrderTableDataIndex] = OrderTable(clientId, symbolId, clientName + "-" + symbolName, newOrderPrice, 0.0);
		pair<decltype(OrderTablePrimaryKeyClientSymbolId)::iterator, bool> result3 = OrderTablePrimaryKeyClientSymbolId.insert(&OrderTableData[OrderTableDataIndex]);
		if (result3.second == false) // Key already exists
		{
			--OrderTableDataIndex;
			(*result3.first)->orderPrice = newOrderPrice;
			//(*result3.first)->offset = newOffset;
			if ((*result3.first)->updated == false)
			{
				(*result3.first)->updated = true;
				UpdatedOrderTableData[++UpdatedOrderTableDataIndex] = *result3.first;
			}
		}
		else
		{
			OrderTableIndexSymbolId.insert(&OrderTableData[OrderTableDataIndex]);
			UpdatedOrderTableData[++UpdatedOrderTableDataIndex] = &OrderTableData[OrderTableDataIndex];
		}
	}

/*
Complexity is same as v4a, but it keeps updated flag on all orders for this symbol
Complexity:
	hash(symbolName) + O(1)							search/insert into SymbolTableIndexSymbolName
	+ hash(symbolId i.e. unsigned int) + O(1)		Only for new symbols: insert into SymbolTablePrimaryKeySymbolId
	+ 2 * hash(symbolId i.e. unsigned int) + number of orders for this symbol		update flag for all orders with this symbol, do not update offset

Changes over v3c: The following part is removed from v3c
	+ number of orders for this symbol * 2 * log(number of orders for this symbol)	erase old and insert new into OrderTableSortIndexOffset ONLY if the new offset changes the current position in sorted list or is less than min so far
*/
	void Others_OMS_v4b::Others_OMS_updateMarketPrice(const string& symbolName, double currentMarketPrice)
	{
		unsigned int symbolId = ++SymbolTableDataIndex;
		SymbolTableData[SymbolTableDataIndex] = SymbolTable(symbolId, symbolName, currentMarketPrice);
		pair<decltype(SymbolTableIndexSymbolName)::iterator, bool> result1 = SymbolTableIndexSymbolName.insert(&SymbolTableData[SymbolTableDataIndex]);
		if (result1.second == false) // Key already exists
		{
			--SymbolTableDataIndex;
			symbolId = (*result1.first)->symbolId;
			(*result1.first)->marketPrice = currentMarketPrice;
			if ((*result1.first)->updated == false)
			{
				(*result1.first)->updated = true;
				UpdatedSymbolTableData[++UpdatedSymbolTableDataIndex] = *result1.first;
				OrderTable dummyOrder(0, symbolId, "", 0, 0);
				for (auto it = OrderTableIndexSymbolId.lower_bound(&dummyOrder); it != OrderTableIndexSymbolId.upper_bound(&dummyOrder); ++it)
					if ((*it)->updated == false)
					{
						(*it)->updated = true;
						UpdatedOrderTableData[++UpdatedOrderTableDataIndex] = *it;
					}
			}
			
		}
		else
		{
			pair<decltype(SymbolTablePrimaryKeySymbolId)::iterator, bool> result2 = SymbolTablePrimaryKeySymbolId.insert(&SymbolTableData[symbolId]);
			MyAssert::myRunTimeAssert(result2.second == true);
		}
	}

/*
Complexity is same as v4a, but calculates new offset for all orders having flag == true
Complexity:
	number of orders * log(100)			Iterate through all elements and collect top 100 by offset into heap
	+ 100 * log(100)					extract max 100 times from heap	
*/
	vector<string> Others_OMS_v4b::Others_OMS_getTop100v1()
	{
		// Update offsets
		//for (int i = 0; i < SymbolTableData.size(); ++i)
		//{
		//	if (!SymbolTableData[i].updated)
		//		continue;

		//	double currentMarketPrice = SymbolTableData[i].marketPrice;
		//	OrderTable dummyOrder(0, SymbolTableData[i].symbolId, 0, 0);
		//	for (auto it = OrderTableIndexSymbolId.lower_bound(&dummyOrder); it != OrderTableIndexSymbolId.upper_bound(&dummyOrder); ++it)
		//	{
		//		(*it)->offset = 100 * fabs((*it)->orderPrice - currentMarketPrice) / currentMarketPrice;
		//		(*it)->updated = false;
		//	}
		//}
		for (int i = 0; i <= UpdatedOrderTableDataIndex; ++i)
		{
			unsigned int symbolId = UpdatedOrderTableData[i]->symbolId;
			SymbolTable dummySymbol(symbolId, "", 0);
			double currentMarketPrice = (*SymbolTablePrimaryKeySymbolId.find(&dummySymbol))->marketPrice;
			UpdatedOrderTableData[i]->offset = 100.0 * fabs(UpdatedOrderTableData[i]->orderPrice - currentMarketPrice) / currentMarketPrice;
			UpdatedOrderTableData[i]->updated = false;
		}
		UpdatedOrderTableDataIndex = -1;
		for (int i = 0; i <= UpdatedSymbolTableDataIndex; ++i)
			UpdatedSymbolTableData[i]->updated = false;
		UpdatedSymbolTableDataIndex = -1;

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
		priority_queue<Others_OMS_v4b::OrderTable*, vector<Others_OMS_v4b::OrderTable*>, decltype(comp2)> ret(comp2);
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

	void Others_OMS_v4b::Others_OMS_getTop100v2(vector<const string*>& retVal)
	{
		// Update offsets
		//for (int i = 0; i < SymbolTableData.size(); ++i)
		//{
		//	if (!SymbolTableData[i].updated)
		//		continue;

		//	double currentMarketPrice = SymbolTableData[i].marketPrice;
		//	OrderTable dummyOrder(0, SymbolTableData[i].symbolId, 0, 0);
		//	for (auto it = OrderTableIndexSymbolId.lower_bound(&dummyOrder); it != OrderTableIndexSymbolId.upper_bound(&dummyOrder); ++it)
		//	{
		//		(*it)->offset = 100 * fabs((*it)->orderPrice - currentMarketPrice) / currentMarketPrice;
		//		(*it)->updated = false;
		//	}
		//}
		for (int i = 0; i <= UpdatedOrderTableDataIndex; ++i)
		{
			unsigned int symbolId = UpdatedOrderTableData[i]->symbolId;
			SymbolTable dummySymbol(symbolId, "", 0);
			double currentMarketPrice = (*SymbolTablePrimaryKeySymbolId.find(&dummySymbol))->marketPrice;
			UpdatedOrderTableData[i]->offset = 100.0 * fabs(UpdatedOrderTableData[i]->orderPrice - currentMarketPrice) / currentMarketPrice;
			UpdatedOrderTableData[i]->updated = false;
		}
		UpdatedOrderTableDataIndex = -1;
		for (int i = 0; i <= UpdatedSymbolTableDataIndex; ++i)
			UpdatedSymbolTableData[i]->updated = false;
		UpdatedSymbolTableDataIndex = -1;

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
		priority_queue<Others_OMS_v4b::OrderTable*, vector<Others_OMS_v4b::OrderTable*>, decltype(comp2)> ret(comp2);
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

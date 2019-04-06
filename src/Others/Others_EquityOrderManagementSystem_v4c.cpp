#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>
#include <locale> // For printing number as thousand separated string
using namespace std;

#include "Utils/Utils_MM_Assert.h"
#include "Others\Others_EquityOrderManagementSystem_v4c.h"

namespace mm {

/*
This version is optimized implementation of boost multiindex. 
It prepares the structure like database and keep indexes on all desired columns except for offsets.
It calculates offsets when required i.e. when Others_OMS_getTop100() is called and keeps it in memory for future use.
For subsequent calls to Others_OMS_getTop100(), it uses the update flags on orders and symbols 
and updates the sorted list for only these updated entries.
*/

	vector<Others_OMS_v4c::OrderTable> Others_OMS_v4c::OrderTableData(0);
	int Others_OMS_v4c::OrderTableDataIndex = -1;
	vector<Others_OMS_v4c::OrderTable*> Others_OMS_v4c::UpdatedOrderTableData(0);
	int Others_OMS_v4c::UpdatedOrderTableDataIndex = -1;
	unordered_set<Others_OMS_v4c::OrderTable*, Others_OMS_v4c::HasherOrderTablePrimaryKeyClientSymbolId, Others_OMS_v4c::IsEqualOrderTablePrimaryKeyClientSymbolId> 
		Others_OMS_v4c::OrderTablePrimaryKeyClientSymbolId(0);
	unordered_multiset<Others_OMS_v4c::OrderTable*, Others_OMS_v4c::HasherOrderTableIndexSymbolId, Others_OMS_v4c::IsEqualOrderTableIndexSymbolId> 
		Others_OMS_v4c::OrderTableIndexSymbolId(0);
	set<Others_OMS_v4c::OrderTable*, Others_OMS_v4c::CompareOrderTableIndexOffset> Others_OMS_v4c::OrderTableSortIndexOffset;
	double Others_OMS_v4c::minOffsetInSortestList = 0.0;

	vector<Others_OMS_v4c::ClientTable> Others_OMS_v4c::ClientTableData(0);
	int Others_OMS_v4c::ClientTableDataIndex = -1;
	unordered_set<Others_OMS_v4c::ClientTable*, Others_OMS_v4c::HasherClientTablePrimaryKeyClientId, Others_OMS_v4c::IsEqualClientTablePrimaryKeyClientId> 
		Others_OMS_v4c::ClientTablePrimaryKeyClientId(0); // This is required in case clientId is not equal to index in ClientTableData
	unordered_set<Others_OMS_v4c::ClientTable*, Others_OMS_v4c::HasherClientTableIndexClientName, Others_OMS_v4c::IsEqualClientTableIndexClientName> 
		Others_OMS_v4c::ClientTableIndexClientName(0);

	vector<Others_OMS_v4c::SymbolTable> Others_OMS_v4c::SymbolTableData(0);
	int Others_OMS_v4c::SymbolTableDataIndex = -1;
	vector<Others_OMS_v4c::SymbolTable*> Others_OMS_v4c::UpdatedSymbolTableData(0);
	int Others_OMS_v4c::UpdatedSymbolTableDataIndex = -1;
	unordered_set<Others_OMS_v4c::SymbolTable*, Others_OMS_v4c::HasherSymbolTablePrimaryKeySymbolId, Others_OMS_v4c::IsEqualSymbolTablePrimaryKeySymbolId> 
		Others_OMS_v4c::SymbolTablePrimaryKeySymbolId(1000); // This is required in case symbolId is not equal to index in SymbolTableData
	unordered_set<Others_OMS_v4c::SymbolTable*, Others_OMS_v4c::HasherSymbolTableIndexSymbolName, Others_OMS_v4c::IsEqualSymbolTableIndexSymbolName> 
		Others_OMS_v4c::SymbolTableIndexSymbolName(1000);

#ifdef MM_DEBUG_OTHERS_EOMS
	vector<ClientSymbolPair> Others_OMS_v4c::finalSortedList;
#endif

	void Others_OMS_v4c::allocateAllMemory()
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
	void Others_OMS_v4c::deallocateAllMemory()
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
v4b v/s v4c: only getTop100() function is changed. v4c keeps the running sorted list of top 100 orders.
			 getTop100() uses updated flags and updated sorted list of top 100 only with updated orders
*/

/*
Algo is exactly same as v4b
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

	void Others_OMS_v4c::Others_OMS_addOrder(const string& clientName, const string& symbolName, double newOrderPrice)
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
Algo is exactly same as v4b
Complexity:
	hash(symbolName) + O(1)							search/insert into SymbolTableIndexSymbolName
	+ hash(symbolId i.e. unsigned int) + O(1)		Only for new symbols: insert into SymbolTablePrimaryKeySymbolId
	+ 2 * hash(symbolId i.e. unsigned int) + number of orders for this symbol		update flag for all orders with this symbol, do not update offset

Changes over v3c: The following part is removed from v3c
	+ number of orders for this symbol * 2 * log(number of orders for this symbol)	erase old and insert new into OrderTableSortIndexOffset ONLY if the new offset changes the current position in sorted list or is less than min so far
*/
	void Others_OMS_v4c::Others_OMS_updateMarketPrice(const string& symbolName, double currentMarketPrice)
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
Change over v4b: v4c keeps the running sorted list of top 100 orders.
				 getTop100() uses updated flags and updated sorted list of top 100 only with updated orders
Complexity:
	number of updated orders					Iterate through all UPDATED orders and calculate new offsets
	+ number of updated orders * 2 * log(100)	For each of UPDATED order, erase entry from sorted list and add it back if new offset is greater than min so far
	+ number of orders * log(100)				ONLY if sorted list does not have 100 elements, iterate through all orders and 
												insert it into sorted list if offset is greater than min so far, also update min_so_far
	+ 100										iterate through all 100 elements in sorted list
*/
	vector<string> Others_OMS_v4c::Others_OMS_getTop100v1()
	{
		for (int i = 0; i <= UpdatedOrderTableDataIndex; ++i)
		{
			if (OrderTableSortIndexOffset.size() > 0)
				OrderTableSortIndexOffset.erase(UpdatedOrderTableData[i]);

			unsigned int symbolId = UpdatedOrderTableData[i]->symbolId;
			SymbolTable dummySymbol(symbolId, "", 0);
			double currentMarketPrice = (*SymbolTablePrimaryKeySymbolId.find(&dummySymbol))->marketPrice;
			UpdatedOrderTableData[i]->offset = 100.0 * fabs(UpdatedOrderTableData[i]->orderPrice - currentMarketPrice) / currentMarketPrice;
			UpdatedOrderTableData[i]->updated = false;
			
			if (/*OrderTableSortIndexOffset.size() < 100 ||*/ UpdatedOrderTableData[i]->offset >= minOffsetInSortestList)
			{
				OrderTableSortIndexOffset.insert(UpdatedOrderTableData[i]);
				if(OrderTableSortIndexOffset.size() > 100)
					OrderTableSortIndexOffset.erase(--OrderTableSortIndexOffset.end());
				if(OrderTableSortIndexOffset.size() == 100)
					minOffsetInSortestList = (*OrderTableSortIndexOffset.rbegin())->offset;
			}
		}
		UpdatedOrderTableDataIndex = -1;
		for (int i = 0; i <= UpdatedSymbolTableDataIndex; ++i)
			UpdatedSymbolTableData[i]->updated = false;
		UpdatedSymbolTableDataIndex = -1;

		int finalSortedListLen = std::min(100, OrderTableDataIndex + 1);
		if(OrderTableSortIndexOffset.size() < finalSortedListLen)
		{
			for (int i = 0; i <= OrderTableDataIndex; ++i)
			{
				if (OrderTableSortIndexOffset.size() < finalSortedListLen)
				{
					OrderTableSortIndexOffset.insert(&OrderTableData[i]);
					minOffsetInSortestList = (*OrderTableSortIndexOffset.rbegin())->offset;
				}
				else if (OrderTableData[i].offset >= minOffsetInSortestList)
				{
					pair<decltype(OrderTableSortIndexOffset)::iterator, bool> result = OrderTableSortIndexOffset.insert(&OrderTableData[i]);
					if (result.second == true) // new key is inserted
					{
						//while (OrderTableSortIndexOffset.size() > finalSortedListLen)
							OrderTableSortIndexOffset.erase(--OrderTableSortIndexOffset.end());
						minOffsetInSortestList = (*OrderTableSortIndexOffset.rbegin())->offset;
					}
				}
			}
		}

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

	void Others_OMS_v4c::Others_OMS_getTop100v2(vector<const string*>& retVal)
	{
		for (int i = 0; i <= UpdatedOrderTableDataIndex; ++i)
		{
			if (OrderTableSortIndexOffset.size() > 0)
				OrderTableSortIndexOffset.erase(UpdatedOrderTableData[i]);

			unsigned int symbolId = UpdatedOrderTableData[i]->symbolId;
			SymbolTable dummySymbol(symbolId, "", 0);
			double currentMarketPrice = (*SymbolTablePrimaryKeySymbolId.find(&dummySymbol))->marketPrice;
			UpdatedOrderTableData[i]->offset = 100.0 * fabs(UpdatedOrderTableData[i]->orderPrice - currentMarketPrice) / currentMarketPrice;
			UpdatedOrderTableData[i]->updated = false;
			
			if (/*OrderTableSortIndexOffset.size() < 100 ||*/ UpdatedOrderTableData[i]->offset >= minOffsetInSortestList)
			{
				OrderTableSortIndexOffset.insert(UpdatedOrderTableData[i]);
				if(OrderTableSortIndexOffset.size() > 100)
					OrderTableSortIndexOffset.erase(--OrderTableSortIndexOffset.end());
				if(OrderTableSortIndexOffset.size() == 100)
					minOffsetInSortestList = (*OrderTableSortIndexOffset.rbegin())->offset;
			}
		}
		UpdatedOrderTableDataIndex = -1;
		for (int i = 0; i <= UpdatedSymbolTableDataIndex; ++i)
			UpdatedSymbolTableData[i]->updated = false;
		UpdatedSymbolTableDataIndex = -1;

		int finalSortedListLen = std::min(100, OrderTableDataIndex + 1);
		if(OrderTableSortIndexOffset.size() < finalSortedListLen)
		{
			for (int i = 0; i <= OrderTableDataIndex; ++i)
			{
				if (OrderTableSortIndexOffset.size() < finalSortedListLen)
				{
					OrderTableSortIndexOffset.insert(&OrderTableData[i]);
					minOffsetInSortestList = (*OrderTableSortIndexOffset.rbegin())->offset;
				}
				else if (OrderTableData[i].offset >= minOffsetInSortestList)
				{
					pair<decltype(OrderTableSortIndexOffset)::iterator, bool> result = OrderTableSortIndexOffset.insert(&OrderTableData[i]);
					if (result.second == true) // new key is inserted
					{
						//while (OrderTableSortIndexOffset.size() > finalSortedListLen)
							OrderTableSortIndexOffset.erase(--OrderTableSortIndexOffset.end());
						minOffsetInSortestList = (*OrderTableSortIndexOffset.rbegin())->offset;
					}
				}
			}
		}

		int index = 0;
		//vector<const string*> retVal(finalSortedListLen);
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
		//return retVal;
	}
}

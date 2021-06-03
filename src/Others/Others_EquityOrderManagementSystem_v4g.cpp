#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>
#include <locale> // For printing number as thousand separated string
#include <cmath>
using namespace std;

#include "Utils/Utils_MM_Assert.h"
#include "Others\Others_EquityOrderManagementSystem_v4g.h"

namespace mm {

/*
This version is optimized implementation of boost multiindex. 
It prepares the structure like database and keep indexes on all desired columns except for offsets.
It calculates offsets when required i.e. when Others_OMS_getTop100() is called and keeps it in memory for future use.
For subsequent calls to Others_OMS_getTop100(), it uses the update flags on orders and symbols 
and updates the sorted list for only these updated entries.
When updating market price of a symbol, it just keeps the update flag on it and do not update all orders for that symbol.
When Others_OMS_getTop100() is called, it uses flags on orders and symbols to take appropriate action.

This version uses min heap, max heap etc along with set to enhance performance
*/
	vector<Others_OMS_v4g::OrderTable> Others_OMS_v4g::OrderTableData(0);
	int Others_OMS_v4g::OrderTableDataIndex = -1;
	vector<Others_OMS_v4g::OrderTable*> Others_OMS_v4g::UpdatedOrderTableData(0);
	int Others_OMS_v4g::UpdatedOrderTableDataIndex = -1;
	unordered_set<Others_OMS_v4g::OrderTable*, Others_OMS_v4g::HasherOrderTablePrimaryKeyClientSymbolId, Others_OMS_v4g::IsEqualOrderTablePrimaryKeyClientSymbolId> 
		Others_OMS_v4g::OrderTablePrimaryKeyClientSymbolId(0);
	unordered_multiset<Others_OMS_v4g::OrderTable*, Others_OMS_v4g::HasherOrderTableIndexSymbolId, Others_OMS_v4g::IsEqualOrderTableIndexSymbolId> 
		Others_OMS_v4g::OrderTableIndexSymbolId(0);
	//set<Others_OMS_v4g::OrderTable*, Others_OMS_v4g::CompareOrderTableIndexOffset> Others_OMS_v4g::OrderTableSortIndexOffset;
	double Others_OMS_v4g::minOffsetInSortestList = 0.0;
	//priority_queue<Others_OMS_v4g::OrderTable*, vector<Others_OMS_v4g::OrderTable*>, Others_OMS_v4g::CompareOrderTableIndexOffsetMaxHeap> Others_OMS_v4g::OrderTableSortIndexOffsetMaxHeap;
	int Others_OMS_v4g::maxHeapSize = 0;
	double Others_OMS_v4g::maxOffsetOfRest = 0.0;
	//priority_queue<Others_OMS_v4g::OrderTable*, vector<Others_OMS_v4g::OrderTable*>, Others_OMS_v4g::CompareOrderTableIndexOffset> Others_OMS_v4g::OrderTableSortIndexOffsetMinHeap;
	Others_OMS_v4g::OrderTable* Others_OMS_v4g::pMaxElementOfRest = nullptr;;
	vector<Others_OMS_v4g::OrderTable*> Others_OMS_v4g::OrderTableMaxHeapVector(0); // Assuming max 10000 order updated during 2 calls to getTop100()
	int Others_OMS_v4g::OrderTableMaxHeapVectorIndex = -1;
	Others_OMS_v4g::MM_Heap<Others_OMS_v4g::OrderTable*, Others_OMS_v4g::CompareOrderTableIndexOffset> Others_OMS_v4g::OrderTableSortIndexOffsetMinHeapTop100(100);
	//Others_OMS_v4g::MM_Heap<Others_OMS_v4g::OrderTable*, Others_OMS_v4g::CompareOrderTableIndexOffset> Others_OMS_v4g::OrderTableSortIndexOffsetMinHeapTop100Copy(100);
	vector<Others_OMS_v4g::OrderTable*> Others_OMS_v4g::sortedTop100Orders(0);
	//Others_OMS_v4g::MM_Heap<Others_OMS_v4g::OrderTable*, Others_OMS_v4g::CompareOrderTableIndexOffsetMaxHeap> Others_OMS_v4g::OrderTableSortIndexOffsetMaxHeapMiddle(10000);
	Others_OMS_v4g::MM_Heap<Others_OMS_v4g::OrderTable*, Others_OMS_v4g::CompareOrderTableIndexOffsetMaxHeap> 
		Others_OMS_v4g::OrderTableSortIndexOffsetMaxHeapRest(1000000 - 100);
	//vector<Others_OMS_v4g::OrderTable*> Others_OMS_v4g::top100(100, nullptr);
	//int Others_OMS_v4g::top100Index = -1;
	int Others_OMS_v4g::numUpdatedFromTop100 = -1;
	//bool Others_OMS_v4g::updateHeapTop100;
	//bool Others_OMS_v4g::updateHeapRest;

	vector<Others_OMS_v4g::ClientTable> Others_OMS_v4g::ClientTableData(0);
	int Others_OMS_v4g::ClientTableDataIndex = -1;
	unordered_set<Others_OMS_v4g::ClientTable*, Others_OMS_v4g::HasherClientTablePrimaryKeyClientId, Others_OMS_v4g::IsEqualClientTablePrimaryKeyClientId> 
		Others_OMS_v4g::ClientTablePrimaryKeyClientId(0); // This is required in case clientId is not equal to index in ClientTableData
	unordered_set<Others_OMS_v4g::ClientTable*, Others_OMS_v4g::HasherClientTableIndexClientName, Others_OMS_v4g::IsEqualClientTableIndexClientName> 
		Others_OMS_v4g::ClientTableIndexClientName(0);

	vector<Others_OMS_v4g::SymbolTable> Others_OMS_v4g::SymbolTableData(0);
	int Others_OMS_v4g::SymbolTableDataIndex = -1;
	vector<Others_OMS_v4g::SymbolTable*> Others_OMS_v4g::UpdatedSymbolTableData(0);
	int Others_OMS_v4g::UpdatedSymbolTableDataIndex = -1;
	unordered_set<Others_OMS_v4g::SymbolTable*, Others_OMS_v4g::HasherSymbolTablePrimaryKeySymbolId, Others_OMS_v4g::IsEqualSymbolTablePrimaryKeySymbolId> 
		Others_OMS_v4g::SymbolTablePrimaryKeySymbolId(1000); // This is required in case symbolId is not equal to index in SymbolTableData
	unordered_set<Others_OMS_v4g::SymbolTable*, Others_OMS_v4g::HasherSymbolTableIndexSymbolName, Others_OMS_v4g::IsEqualSymbolTableIndexSymbolName> 
		Others_OMS_v4g::SymbolTableIndexSymbolName(1000);

	Others_OMS_v4g::OrderTable Others_OMS_v4g::dummyOrder;
	Others_OMS_v4g::ClientTable Others_OMS_v4g::dummyClient;
	Others_OMS_v4g::SymbolTable Others_OMS_v4g::dummySymbol;

	Others_OMS_v4g::CompareOrderTableIndexOffset Others_OMS_v4g::comparator;
	Others_OMS_v4g::CompareOrderTableIndexOffsetMaxHeap Others_OMS_v4g::comparatorMaxHeap;

#ifdef MM_DEBUG_OTHERS_EOMS
	vector<ClientSymbolPair> Others_OMS_v4g::finalSortedList;
#endif

	void Others_OMS_v4g::allocateAllMemory()
	{
		OrderTableData.resize(1000000 + 1);
		UpdatedOrderTableData.resize(1000000, nullptr);
		OrderTablePrimaryKeyClientSymbolId.reserve(1000000);
		OrderTableIndexSymbolId.reserve(1000000);
		OrderTableMaxHeapVector.resize(10000, nullptr);
		sortedTop100Orders.resize(100, nullptr);

		ClientTableData.resize(1000 + 1);
		ClientTablePrimaryKeyClientId.reserve(1000);
		ClientTableIndexClientName.reserve(1000);

		SymbolTableData.resize(1000 + 1);
		UpdatedSymbolTableData.resize(1000, nullptr);
		SymbolTablePrimaryKeySymbolId.reserve(1000);
		SymbolTableIndexSymbolName.reserve(1000);
	}
	void Others_OMS_v4g::deallocateAllMemory()
	{
		OrderTableData.clear();
		UpdatedOrderTableData.clear();
		OrderTablePrimaryKeyClientSymbolId.clear();
		OrderTableIndexSymbolId.clear();
		sortedTop100Orders.clear();

		ClientTableData.clear();
		ClientTablePrimaryKeyClientId.clear();
		ClientTableIndexClientName.clear();

		SymbolTableData.clear();
		UpdatedSymbolTableData.clear();
		SymbolTablePrimaryKeySymbolId.clear();
		SymbolTableIndexSymbolName.clear();
	}

/*
Algo is exactly same as v4b and v4c
Complexity:
	hash(clientName) + O(1)									search/insert clientName in hashtable ClientTableIndexClientName 
	+ hash(clientId i.e. unsinged int) + O(1)				update ClientTablePrimaryKeyClientId if this is new client
	+ hash(symbolName) + O(1)								search symbolName in hashtable SymbolTableIndexSymbolName (symbol is guaranteed to be there)
	+ combined hash for clientId & symbolId + hash(client-symbol ID i.e. unsinged int) + O(1)			search/insert order into OrderTablePrimaryKeyClientSymbolId
	+ if this is first order for this client-symbol i.e. this clint-symbol pair does not exist in order table
		hash(symbolId i.e. unsigned int) + O(1)				insert new entry into OrderTablePositionIndexSymbolId

The following part is removed from algo v3c
	else
		log(orders)											erase entry from sorted list OrderTableSortIndexOffset ONLY if the new offset changes the current position in sorted list or is less than min so far
	+ log(orders)											insert new entry into sorted list OrderTableSortIndexOffset ONLY if erased due to above condition or is new element & offset is greater than min so far
*/

	void Others_OMS_v4g::Others_OMS_addOrder(const string& clientName, const string& symbolName, double newOrderPrice)
	{
		unsigned int clientId = ++ClientTableDataIndex;
		//ClientTableData[ClientTableDataIndex] = ClientTable(clientId, clientName);
		ClientTableData[ClientTableDataIndex].clientId = clientId;
		ClientTableData[ClientTableDataIndex].clientName = clientName;
		pair<decltype(ClientTableIndexClientName)::iterator, bool> result1 = ClientTableIndexClientName.insert(&ClientTableData[ClientTableDataIndex]);
		if (result1.second == false) // Key already exists
		{
			--ClientTableDataIndex;
			clientId = (*result1.first)->clientId;
		}
		else
		{
			pair<decltype(ClientTablePrimaryKeyClientId)::iterator, bool> result2 = ClientTablePrimaryKeyClientId.insert(&ClientTableData[clientId]);
			//MM_Assert::mmRunTimeAssert(result2.second == true);
		}
		
		//SymbolTable dummySymbol(0, symbolName, 0);
		dummySymbol.symbolName = symbolName;
		auto it = SymbolTableIndexSymbolName.find(&dummySymbol);
		unsigned int symbolId = (*it)->symbolId;
		double marketPrice = (*it)->marketPrice;
		//double newOffset = 100 * fabs(newOrderPrice - marketPrice) / marketPrice;

		//OrderTableData[++OrderTableDataIndex] = OrderTable(clientId, symbolId, clientName + "-" + symbolName, newOrderPrice, 0.0);
		OrderTableData[++OrderTableDataIndex].clientId = clientId;
		OrderTableData[OrderTableDataIndex].symbolId = symbolId;
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
			OrderTableData[OrderTableDataIndex].clientSymbolPair = (clientName + "-" + symbolName);
			OrderTableData[OrderTableDataIndex].orderPrice = newOrderPrice;
			OrderTableData[OrderTableDataIndex].offset = 0.0;
			OrderTableData[OrderTableDataIndex].updated = true;
			//OrderTableData[OrderTableDataIndex].numCountInMaxHeap = 0;
			OrderTableData[OrderTableDataIndex].isPresentInTop100 = false;
			OrderTableData[OrderTableDataIndex].isPresentInRest = false;
			OrderTableData[OrderTableDataIndex].indexInMinHeap = -1;
			//OrderTableData[OrderTableDataIndex].sortedArrayIndex = -1;
			OrderTableData[OrderTableDataIndex].symbolPtr = (*it);
			OrderTableIndexSymbolId.insert(&OrderTableData[OrderTableDataIndex]);

			//This is new order, so insert it into sorted list
			//OrderTableSortIndexOffset.insert(&OrderTableData[OrderTableDataIndex]);
			//if (OrderTableSortIndexOffset.size() > 100)
			//{
			//	//Push it to Max Heap
			//}

			UpdatedOrderTableData[++UpdatedOrderTableDataIndex] = &OrderTableData[OrderTableDataIndex];
			//sortedOrderTableData[++sortedOrderTableDataIndex] = &OrderTableData[OrderTableDataIndex];
		}
	}

/*
Change over v4b or v4c: v4d updates flag over only symbol, does not iterate through all orders for this symbol to update flags on orders.
Note: v4b and v4c updateMarketPrice() are same.

Complexity:
	hash(symbolName) + O(1)							search/insert into SymbolTableIndexSymbolName
	+ hash(symbolId i.e. unsigned int) + O(1)		Only for new symbols: insert into SymbolTablePrimaryKeySymbolId

The following part is removed from v3c
	+ number of orders for this symbol * 2 * log(number of orders for this symbol)	erase old and insert new into OrderTableSortIndexOffset ONLY if the new offset changes the current position in sorted list or is less than min so far
*/
	void Others_OMS_v4g::Others_OMS_updateMarketPrice(const string& symbolName, double currentMarketPrice)
	{
		unsigned int symbolId = ++SymbolTableDataIndex;
		//SymbolTableData[SymbolTableDataIndex] = SymbolTable(symbolId, symbolName, currentMarketPrice);
		SymbolTableData[SymbolTableDataIndex].symbolId = symbolId;
		SymbolTableData[SymbolTableDataIndex].symbolName = symbolName;
		SymbolTableData[SymbolTableDataIndex].marketPrice = currentMarketPrice;
		SymbolTableData[SymbolTableDataIndex].updated = false;
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
				//OrderTable dummyOrder(0, symbolId, 0, 0);
				//for (auto it = OrderTableIndexSymbolId.lower_bound(&dummyOrder); it != OrderTableIndexSymbolId.upper_bound(&dummyOrder); ++it)
				//	if ((*it)->updated == false)
				//	{
				//		(*it)->updated = true;
				//		UpdatedOrderTableData[++UpdatedOrderTableDataIndex] = *it;
				//	}
			}
			
		}
		else
		{
			pair<decltype(SymbolTablePrimaryKeySymbolId)::iterator, bool> result2 = SymbolTablePrimaryKeySymbolId.insert(&SymbolTableData[symbolId]);
			//MM_Assert::mmRunTimeAssert(result2.second == true);
		}
	}

/*
Change over v4c: v4d updated the flags on all orders for each of updated symbol

Complexity:
	number of updated symbols * 2 * hash(symbolId i.e. unsigned int) * number of orders for this symbol		
												ONLY for updated symbols, iterate through all orders for this symbol and update offset
	* 2 * log(100)								For all above orders, erase entry from sorted list and add it back if new offset is greater than min so far
	+ number of updated orders					Iterate through all UPDATED orders and calculate new offsets
	+ number of updated orders * 2 * log(100)	For each of UPDATED order, erase entry from sorted list and add it back if new offset is greater than min so far
	+ number of orders * log(100)				ONLY if sorted list does not have 100 elements, iterate through all orders and 
												insert it into sorted list if offset is greater than min so far, also update min_so_far
	+ 100										iterate through all 100 elements in sorted list
*/

	//Dummy function
	vector<string> Others_OMS_v4g::Others_OMS_getTop100v1()
	{
		vector<string> retVal;
		return retVal;
	}

#ifdef UNDEFINED
	vector<string> Others_OMS_v4g::Others_OMS_getTop100v1()
	{
		// Update offsets
		for (int i = 0; i <= UpdatedSymbolTableDataIndex; ++i)
		{
			double currentMarketPrice = UpdatedSymbolTableData[i]->marketPrice;
			OrderTable dummyOrder(0, UpdatedSymbolTableData[i]->symbolId, "", 0, 0);
			for (auto it = OrderTableIndexSymbolId.lower_bound(&dummyOrder); it != OrderTableIndexSymbolId.upper_bound(&dummyOrder); ++it)
			{
				OrderTableSortIndexOffset.erase(*it);

				(*it)->offset = 100 * fabs((*it)->orderPrice - currentMarketPrice) / currentMarketPrice;
				(*it)->updated = false;

				if (/*OrderTableSortIndexOffset.size() < 100 ||*/ (*it)->offset >= minOffsetInSortestList)
				{
					OrderTableSortIndexOffset.insert(*it);
					if(OrderTableSortIndexOffset.size() > 100)
						OrderTableSortIndexOffset.erase(--OrderTableSortIndexOffset.end());
					if(OrderTableSortIndexOffset.size() == 100)
						minOffsetInSortestList = (*OrderTableSortIndexOffset.rbegin())->offset;
				}
			}
			UpdatedSymbolTableData[i]->updated = false;
		}
		UpdatedSymbolTableDataIndex = -1;

		for (int i = 0; i <= UpdatedOrderTableDataIndex; ++i)
		{
			//if (OrderTableSortIndexOffset.size() > 0)
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

		//MM_Assert::mmRunTimeAssert(index == finalSortedListLen);
		return retVal;
	}
#endif

	void Others_OMS_v4g::validate()
	{
		for (int i = 0; i < OrderTableSortIndexOffsetMinHeapTop100.size(); ++i)
		{
			MM_Assert::mmRunTimeAssert(OrderTableSortIndexOffsetMinHeapTop100.get(i)->indexInMinHeap == i);
		}

		/*
		//Test Start
		OrderTableSortIndexOffsetMinHeapTop100Copy.clear();
		vector<OrderTable*> test(0);
		for (int iTest = 0; iTest < OrderTableSortIndexOffsetMinHeapTop100.size(); ++iTest)
		{
			OrderTableSortIndexOffsetMinHeapTop100Copy.addNext(OrderTableSortIndexOffsetMinHeapTop100.get(iTest));
		}

		while (!OrderTableSortIndexOffsetMinHeapTop100Copy.empty())
		{
			test.push_back(OrderTableSortIndexOffsetMinHeapTop100Copy.top());
			OrderTableSortIndexOffsetMinHeapTop100Copy.pop();
		}
		for (int iTest = 1; iTest < test.size(); ++iTest)
			MM_Assert::mmRunTimeAssert(comparator(test[iTest], test[iTest - 1])); // Should be in descending order
		//Test End
		*/
	}

	void Others_OMS_v4g::processUpdatedObject(OrderTable* pObj, int finalSortedListLen, double currentMarketPrice, double previousMinOffset)
	{
		double oldOffset = pObj->offset;
		pObj->offset = 100 * fabs(pObj->orderPrice - currentMarketPrice) / currentMarketPrice;

		if (pObj->isPresentInTop100 == true) // Already in top100
		{
			//updateHeapTop100 = true;
			if (pObj->offset > oldOffset)
			{
				OrderTableSortIndexOffsetMinHeapTop100.siftDown(pObj->indexInMinHeap);
				//validate();
			}
			else
			{
				OrderTableSortIndexOffsetMinHeapTop100.siftUp(pObj->indexInMinHeap);
				//validate();
			}
		}
		else if(pObj->isPresentInRest == true)
		{
			//updateHeapRest = true;
			//if (maxOffsetOfRest < pObj->offset)
			//	maxOffsetOfRest = pObj->offset;
			if (pObj->offset > oldOffset)
				OrderTableSortIndexOffsetMaxHeapRest.siftUp(pObj->indexInMinHeap);
			else
				OrderTableSortIndexOffsetMaxHeapRest.siftDown(pObj->indexInMinHeap);
		}
		else //if (pObj->isPresentInRest == false && pObj->isPresentInTop100 == false) // New element
		{
			if (OrderTableSortIndexOffsetMinHeapTop100.size() < finalSortedListLen)
			{
				pObj->isPresentInTop100 = true;
				pObj->indexInMinHeap = OrderTableSortIndexOffsetMinHeapTop100.size();
				OrderTableSortIndexOffsetMinHeapTop100.push(pObj);
			}
			else
			{
				//updateHeapRest = true;
				pObj->isPresentInRest = true;
				pObj->indexInMinHeap = OrderTableSortIndexOffsetMaxHeapRest.size();
				//OrderTableSortIndexOffsetMaxHeapRest.addNext(pObj);
				OrderTableSortIndexOffsetMaxHeapRest.push(pObj);
			}

			//if (maxOffsetOfRest < pObj->offset)
			//	maxOffsetOfRest = pObj->offset;
		}
	}

	void Others_OMS_v4g::Others_OMS_getTop100v2(vector<const string*>& retVal)
	{
		//updateHeapTop100 = false;
		//updateHeapRest = false;

		OrderTableMaxHeapVectorIndex = -1;
		int finalSortedListLen = std::min(100, OrderTableDataIndex + 1);
		//double previousMinOffset = minOffsetInSortestList;
		double previousMinOffset = OrderTableSortIndexOffsetMaxHeapRest.top()->offset;
		numUpdatedFromTop100 = 0;

		// Update offsets
		for (int i = 0; i <= UpdatedSymbolTableDataIndex; ++i)
		{
			double currentMarketPrice = UpdatedSymbolTableData[i]->marketPrice;
			//OrderTable dummyOrder(0, UpdatedSymbolTableData[i]->symbolId, "", 0, 0);
			dummyOrder.symbolId = UpdatedSymbolTableData[i]->symbolId;
			//for (auto it = OrderTableIndexSymbolId.lower_bound(&dummyOrder); it != OrderTableIndexSymbolId.upper_bound(&dummyOrder); ++it)
			auto range = OrderTableIndexSymbolId.equal_range(&dummyOrder);
			for (auto it = range.first; it != range.second; ++it)
			{
				processUpdatedObject(*it, finalSortedListLen, currentMarketPrice, previousMinOffset);
				(*it)->updated = false;
			}
			UpdatedSymbolTableData[i]->updated = false;
		}
		UpdatedSymbolTableDataIndex = -1;

		for (int i = 0; i <= UpdatedOrderTableDataIndex; ++i)
		{
			OrderTable* pObj = UpdatedOrderTableData[i];
			if (pObj->updated == true)
			{
				double currentMarketPrice = pObj->symbolPtr->marketPrice;
				processUpdatedObject(pObj, finalSortedListLen, currentMarketPrice, previousMinOffset);
				pObj->updated = false;
			}
		}
		UpdatedOrderTableDataIndex = -1;

		//if (updateHeapTop100)
		//	OrderTableSortIndexOffsetMinHeapTop100.heapify();

		//if (OrderTableSortIndexOffsetMinHeapTop100.size() < finalSortedListLen || OrderTableSortIndexOffsetMinHeapTop100.top()->offset <= maxOffsetOfRest)
		if (OrderTableSortIndexOffsetMinHeapTop100.size() < finalSortedListLen 
			//|| OrderTableSortIndexOffsetMinHeapTop100.top()->offset <= OrderTableSortIndexOffsetMaxHeapRest.top()->offset)
			|| (!OrderTableSortIndexOffsetMaxHeapRest.empty() && comparator(OrderTableSortIndexOffsetMaxHeapRest.top(), OrderTableSortIndexOffsetMinHeapTop100.top()))
			)
		{
			//if (updateHeapRest)
			//	OrderTableSortIndexOffsetMaxHeapRest.heapify();
			while (OrderTableSortIndexOffsetMinHeapTop100.size() < finalSortedListLen)
			{
				OrderTableSortIndexOffsetMaxHeapRest.top()->isPresentInTop100 = true;
				OrderTableSortIndexOffsetMaxHeapRest.top()->isPresentInRest = false;
				OrderTableSortIndexOffsetMaxHeapRest.top()->indexInMinHeap = OrderTableSortIndexOffsetMinHeapTop100.size();
				OrderTableSortIndexOffsetMinHeapTop100.push(OrderTableSortIndexOffsetMaxHeapRest.top());
				//validate();
				OrderTableSortIndexOffsetMaxHeapRest.pop();
				//validate();
			}

			while (!OrderTableSortIndexOffsetMaxHeapRest.empty()
				&& comparator(OrderTableSortIndexOffsetMaxHeapRest.top(), OrderTableSortIndexOffsetMinHeapTop100.top()))
			{
				OrderTableSortIndexOffsetMinHeapTop100.top()->isPresentInTop100 = false;
				OrderTableSortIndexOffsetMinHeapTop100.top()->isPresentInRest = true;
				OrderTableSortIndexOffsetMinHeapTop100.top()->indexInMinHeap = OrderTableSortIndexOffsetMaxHeapRest.size();
				OrderTableSortIndexOffsetMaxHeapRest.push(OrderTableSortIndexOffsetMinHeapTop100.top());
				//--validate();
				OrderTableSortIndexOffsetMinHeapTop100.pop();
				//validate();

				OrderTableSortIndexOffsetMaxHeapRest.top()->isPresentInTop100 = true;
				OrderTableSortIndexOffsetMaxHeapRest.top()->isPresentInRest = false;
				OrderTableSortIndexOffsetMaxHeapRest.top()->indexInMinHeap = OrderTableSortIndexOffsetMinHeapTop100.size();
				OrderTableSortIndexOffsetMinHeapTop100.push(OrderTableSortIndexOffsetMaxHeapRest.top());
				//validate();
				OrderTableSortIndexOffsetMaxHeapRest.pop();
				//validate();
			}
			//if (!OrderTableSortIndexOffsetMaxHeapRest.empty() && OrderTableSortIndexOffsetMaxHeapRest.top()->offset > maxOffsetOfRest)
			//	maxOffsetOfRest = OrderTableSortIndexOffsetMaxHeapRest.top()->offset;
		}

		//Create a copy
		//OrderTableSortIndexOffsetMinHeapTop100Copy.clear();
		for (int i = 0; i < OrderTableSortIndexOffsetMinHeapTop100.size(); ++i)
		{
			//OrderTableSortIndexOffsetMinHeapTop100Copy.addNext(OrderTableSortIndexOffsetMinHeapTop100.get(i));
			sortedTop100Orders[i] = OrderTableSortIndexOffsetMinHeapTop100.get(i);
		}

		std::sort_heap(sortedTop100Orders.begin(), sortedTop100Orders.begin() + finalSortedListLen, comparator);

		int index = finalSortedListLen;
		while (index > 0)
		{
			//retVal[--index] = &OrderTableSortIndexOffsetMinHeapTop100Copy.top()->clientSymbolPair;
			//OrderTableSortIndexOffsetMinHeapTop100Copy.pop(false);
			//validate();
			--index;
			retVal[index] = &sortedTop100Orders[index]->clientSymbolPair;
		}

		//MM_Assert::mmRunTimeAssert(index == finalSortedListLen);
	}




#ifdef UNDEFINED
	void Others_OMS_v4g::Others_OMS_getTop100v3(vector<std::pair<const string*, const string*>>& top100v2, unsigned int& count)
	{
		// Update offsets
		for (int i = 0; i <= UpdatedSymbolTableDataIndex; ++i)
		{
			double currentMarketPrice = UpdatedSymbolTableData[i]->marketPrice;
			OrderTable dummyOrder(0, UpdatedSymbolTableData[i]->symbolId, "", 0, 0);
			for (auto it = OrderTableIndexSymbolId.lower_bound(&dummyOrder); it != OrderTableIndexSymbolId.upper_bound(&dummyOrder); ++it)
			{
				OrderTableSortIndexOffset.erase(*it);

				(*it)->offset = 100 * fabs((*it)->orderPrice - currentMarketPrice) / currentMarketPrice;
				(*it)->updated = false;

				if (/*OrderTableSortIndexOffset.size() < 100 ||*/ (*it)->offset >= minOffsetInSortestList)
				{
					OrderTableSortIndexOffset.insert(*it);
					if(OrderTableSortIndexOffset.size() > 100)
						OrderTableSortIndexOffset.erase(--OrderTableSortIndexOffset.end());
					if(OrderTableSortIndexOffset.size() == 100)
						minOffsetInSortestList = (*OrderTableSortIndexOffset.rbegin())->offset;
				}
			}
			UpdatedSymbolTableData[i]->updated = false;
		}
		UpdatedSymbolTableDataIndex = -1;

		for (int i = 0; i <= UpdatedOrderTableDataIndex; ++i)
		{
			//if (OrderTableSortIndexOffset.size() > 0)
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
		//vector<string> retVal(finalSortedListLen, "");
		for (auto it = OrderTableSortIndexOffset.begin(); it != OrderTableSortIndexOffset.end() && index < finalSortedListLen; ++it, ++index)
		{
			unsigned int clientId = (*it)->clientId;
			unsigned int symbolId = (*it)->symbolId;
			ClientTable dummyClient(clientId, "");
			SymbolTable dummySymbol(symbolId, "", 0);
			//string clientSymbolPair((*ClientTablePrimaryKeyClientId.find(&dummyClient))->clientName + "-" + (*SymbolTablePrimaryKeySymbolId.find(&dummySymbol))->symbolName);
			//retVal[index] = clientSymbolPair;
			top100v2[index].first = &(*ClientTablePrimaryKeyClientId.find(&dummyClient))->clientName;
			top100v2[index].second = &(*SymbolTablePrimaryKeySymbolId.find(&dummySymbol))->symbolName;
#ifdef MM_DEBUG_OTHERS_EOMS
			double marketPrice = (*SymbolTablePrimaryKeySymbolId.find(&dummySymbol))->marketPrice;
			finalSortedList[index] = ClientSymbolPair(clientId, symbolId, (*it)->orderPrice, marketPrice, (*it)->offset);
#endif
		}

		count = finalSortedListLen;
		//MM_Assert::mmRunTimeAssert(index == finalSortedListLen);

		//return retVal;
	}
#endif

}

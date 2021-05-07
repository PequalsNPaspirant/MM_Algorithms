#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <chrono>
#include <locale> // For printing number as thousand separated string
using namespace std;

#include "Utils/Utils_MM_Assert.h"
#include "Others\Others_EquityOrderManagementSystem_v4f.h"

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
	vector<Others_OMS_v4f::OrderTable> Others_OMS_v4f::OrderTableData(0);
	size_t Others_OMS_v4f::OrderTableDataIndex = -1;
	vector<Others_OMS_v4f::OrderTable*> Others_OMS_v4f::UpdatedOrderTableData(0);
	int Others_OMS_v4f::UpdatedOrderTableDataIndex = -1;
	unordered_set<Others_OMS_v4f::OrderTable*, Others_OMS_v4f::HasherOrderTablePrimaryKeyClientSymbolId, Others_OMS_v4f::IsEqualOrderTablePrimaryKeyClientSymbolId> 
		Others_OMS_v4f::OrderTablePrimaryKeyClientSymbolId(0);
	unordered_multiset<Others_OMS_v4f::OrderTable*, Others_OMS_v4f::HasherOrderTableIndexSymbolId, Others_OMS_v4f::IsEqualOrderTableIndexSymbolId> 
		Others_OMS_v4f::OrderTableIndexSymbolId(0);
	//set<Others_OMS_v4f::OrderTable*, Others_OMS_v4f::CompareOrderTableIndexOffset> Others_OMS_v4f::OrderTableSortIndexOffset;
	double Others_OMS_v4f::minOffsetInSortestList = 0.0;
	//priority_queue<Others_OMS_v4f::OrderTable*, vector<Others_OMS_v4f::OrderTable*>, Others_OMS_v4f::CompareOrderTableIndexOffsetMaxHeap> Others_OMS_v4f::OrderTableSortIndexOffsetMaxHeap;
	int Others_OMS_v4f::maxHeapSize = 0;
	double Others_OMS_v4f::maxOffsetOfRest = 0.0;
	//priority_queue<Others_OMS_v4f::OrderTable*, vector<Others_OMS_v4f::OrderTable*>, Others_OMS_v4f::CompareOrderTableIndexOffset> Others_OMS_v4f::OrderTableSortIndexOffsetMinHeap;
	Others_OMS_v4f::OrderTable* Others_OMS_v4f::pMaxElementOfRest = nullptr;;
	vector<Others_OMS_v4f::OrderTable*> Others_OMS_v4f::OrderTableMaxHeapVector(0); // Assuming max 10000 order updated during 2 calls to getTop100()
	int Others_OMS_v4f::OrderTableMaxHeapVectorIndex = -1;
	Others_OMS_v4f::MM_Heap<Others_OMS_v4f::OrderTable*, Others_OMS_v4f::CompareOrderTableIndexOffset> 
		Others_OMS_v4f::OrderTableSortIndexOffsetMinHeapTop100(100);
	Others_OMS_v4f::MM_Heap<Others_OMS_v4f::OrderTable*, Others_OMS_v4f::CompareOrderTableIndexOffset> 
		Others_OMS_v4f::OrderTableSortIndexOffsetMinHeapTop100Copy(100);
	Others_OMS_v4f::MM_Heap<Others_OMS_v4f::OrderTable*, Others_OMS_v4f::CompareOrderTableIndexOffsetMaxHeap> 
		Others_OMS_v4f::OrderTableSortIndexOffsetMaxHeapMiddle(10000);
	Others_OMS_v4f::MM_Heap<Others_OMS_v4f::OrderTable*, Others_OMS_v4f::CompareOrderTableIndexOffset> 
		Others_OMS_v4f::OrderTableSortIndexOffsetMinHeapRest(100);
	//vector<Others_OMS_v4f::OrderTable*> Others_OMS_v4f::top100(100, nullptr);
	//int Others_OMS_v4f::top100Index = -1;
	int Others_OMS_v4f::numUpdatedFromTop100 = -1;;

	vector<Others_OMS_v4f::ClientTable> Others_OMS_v4f::ClientTableData(0);
	int Others_OMS_v4f::ClientTableDataIndex = -1;
	unordered_set<Others_OMS_v4f::ClientTable*, Others_OMS_v4f::HasherClientTablePrimaryKeyClientId, Others_OMS_v4f::IsEqualClientTablePrimaryKeyClientId> 
		Others_OMS_v4f::ClientTablePrimaryKeyClientId(0); // This is required in case clientId is not equal to index in ClientTableData
	unordered_set<Others_OMS_v4f::ClientTable*, Others_OMS_v4f::HasherClientTableIndexClientName, Others_OMS_v4f::IsEqualClientTableIndexClientName> 
		Others_OMS_v4f::ClientTableIndexClientName(0);

	vector<Others_OMS_v4f::SymbolTable> Others_OMS_v4f::SymbolTableData(0);
	int Others_OMS_v4f::SymbolTableDataIndex = -1;
	vector<Others_OMS_v4f::SymbolTable*> Others_OMS_v4f::UpdatedSymbolTableData(0);
	int Others_OMS_v4f::UpdatedSymbolTableDataIndex = -1;
	unordered_set<Others_OMS_v4f::SymbolTable*, Others_OMS_v4f::HasherSymbolTablePrimaryKeySymbolId, Others_OMS_v4f::IsEqualSymbolTablePrimaryKeySymbolId> 
		Others_OMS_v4f::SymbolTablePrimaryKeySymbolId(0); // This is required in case symbolId is not equal to index in SymbolTableData
	unordered_set<Others_OMS_v4f::SymbolTable*, Others_OMS_v4f::HasherSymbolTableIndexSymbolName, Others_OMS_v4f::IsEqualSymbolTableIndexSymbolName> 
		Others_OMS_v4f::SymbolTableIndexSymbolName(0);

	Others_OMS_v4f::OrderTable Others_OMS_v4f::dummyOrder;
	Others_OMS_v4f::ClientTable Others_OMS_v4f::dummyClient;
	Others_OMS_v4f::SymbolTable Others_OMS_v4f::dummySymbol;

	Others_OMS_v4f::CompareOrderTableIndexOffset Others_OMS_v4f::comparator;
	Others_OMS_v4f::CompareOrderTableIndexOffsetMaxHeap Others_OMS_v4f::comparatorMaxHeap;

#ifdef MM_DEBUG_OTHERS_EOMS
	vector<ClientSymbolPair> Others_OMS_v4f::finalSortedList;
#endif

	void Others_OMS_v4f::allocateAllMemory()
	{
		OrderTableData.resize(1000000 + 1);
		UpdatedOrderTableData.resize(1000000, nullptr);
		OrderTablePrimaryKeyClientSymbolId.reserve(1000000);
		OrderTableIndexSymbolId.reserve(1000000);
		OrderTableMaxHeapVector.resize(10000, nullptr);

		ClientTableData.resize(1000 + 1);
		ClientTablePrimaryKeyClientId.reserve(1000);
		ClientTableIndexClientName.reserve(1000);

		SymbolTableData.resize(1000 + 1);
		UpdatedSymbolTableData.resize(1000, nullptr);
		SymbolTablePrimaryKeySymbolId.reserve(1000);
		SymbolTableIndexSymbolName.reserve(1000);
	}
	void Others_OMS_v4f::deallocateAllMemory()
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

	void Others_OMS_v4f::Others_OMS_addOrder(const string& clientName, const string& symbolName, double newOrderPrice)
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
			//MyAssert::myRunTimeAssert(result2.second == true);
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
			OrderTableData[OrderTableDataIndex].clientSymbolPair.swap(clientName + "-" + symbolName);
			OrderTableData[OrderTableDataIndex].orderPrice = newOrderPrice;
			OrderTableData[OrderTableDataIndex].offset = 0.0;
			OrderTableData[OrderTableDataIndex].updated = true;
			//OrderTableData[OrderTableDataIndex].numCountInMaxHeap = 0;
			OrderTableData[OrderTableDataIndex].isPresentInSortedList = false;
			//OrderTableData[OrderTableDataIndex].indexInMinHeap = -1;
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
	void Others_OMS_v4f::Others_OMS_updateMarketPrice(const string& symbolName, double currentMarketPrice)
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
			//MyAssert::myRunTimeAssert(result2.second == true);
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
	vector<string> Others_OMS_v4f::Others_OMS_getTop100v1()
	{
		vector<string> retVal;
		return retVal;
	}

#ifdef UNDEFINED
	vector<string> Others_OMS_v4f::Others_OMS_getTop100v1()
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

		//MyAssert::myRunTimeAssert(index == finalSortedListLen);
		return retVal;
	}
#endif

	void Others_OMS_v4f::validate()
	{
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
			MyAssert::myRunTimeAssert(comparator(test[iTest], test[iTest - 1])); // Should be in descending order
		//Test End
	}

	void Others_OMS_v4f::processUpdatedObject(OrderTable* pObj, size_t finalSortedListLen, double currentMarketPrice, double previousMinOffset)
	{
		pObj->offset = 100 * fabs(pObj->orderPrice - currentMarketPrice) / currentMarketPrice;

		if (pObj->isPresentInSortedList == true)
			++numUpdatedFromTop100;
		else if (pObj->offset >= maxOffsetOfRest)
		{
			OrderTableSortIndexOffsetMaxHeapMiddle.addNext(pObj);
		}
	}

	void Others_OMS_v4f::Others_OMS_getTop100v2(vector<const string*>& retVal)
	{
		if (!OrderTableSortIndexOffsetMaxHeapMiddle.empty())
			OrderTableSortIndexOffsetMaxHeapMiddle.clear();
		if (!OrderTableSortIndexOffsetMinHeapRest.empty())
			OrderTableSortIndexOffsetMinHeapRest.clear();

		OrderTableMaxHeapVectorIndex = -1;
		size_t finalSortedListLen = std::min(100ULL, OrderTableDataIndex + 1);
		double previousMinOffset = minOffsetInSortestList;
		numUpdatedFromTop100 = 0;

		// Update offsets
		for (int i = 0; i <= UpdatedSymbolTableDataIndex; ++i)
		{
			double currentMarketPrice = UpdatedSymbolTableData[i]->marketPrice;
			//OrderTable dummyOrder(0, UpdatedSymbolTableData[i]->symbolId, "", 0, 0);
			dummyOrder.symbolId = UpdatedSymbolTableData[i]->symbolId;
			//for (auto it = OrderTableIndexSymbolId.lower_bound(&dummyOrder); it != OrderTableIndexSymbolId.upper_bound(&dummyOrder); ++it)
			for (auto it = OrderTableIndexSymbolId.lower_bound(&dummyOrder); it != OrderTableIndexSymbolId.end() && (*it)->symbolId == dummyOrder.symbolId; ++it)
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
				//unsigned int symbolId = pObj->symbolId;
				//SymbolTable dummySymbol(symbolId, "", 0);
				//dummySymbol.symbolId = symbolId;
				//double currentMarketPrice = (*SymbolTablePrimaryKeySymbolId.find(&dummySymbol))->marketPrice;
				double currentMarketPrice = pObj->symbolPtr->marketPrice;
				processUpdatedObject(pObj, finalSortedListLen, currentMarketPrice, previousMinOffset);
				pObj->updated = false;
			}
		}
		UpdatedOrderTableDataIndex = -1;


		if (numUpdatedFromTop100 > 0)
		{
			OrderTableSortIndexOffsetMinHeapTop100.heapify();
			//validate();

			while (!OrderTableSortIndexOffsetMinHeapTop100.empty()
				&& OrderTableSortIndexOffsetMinHeapTop100.top()->offset <= maxOffsetOfRest)
			{
				OrderTableSortIndexOffsetMinHeapTop100.top()->isPresentInSortedList = false;
				OrderTableSortIndexOffsetMinHeapTop100.pop();
			}
			/*
			for (int i = 0; i < OrderTableSortIndexOffsetMinHeapTop100.size(); ++i)
			{
				OrderTable* pObj = OrderTableSortIndexOffsetMinHeapTop100.get(i);
				if (pObj->updated == true)
				{
					pObj->updated = false;
					double oldOffset = pObj->offset;
					double currentMarketPrice = pObj->symbolPtr->marketPrice;
					pObj->offset = 100 * fabs(pObj->orderPrice - currentMarketPrice) / currentMarketPrice;
					if (pObj->offset > maxOffsetOfRest) // TODO: Compare both offset and client-symbol pair
					{
						if (pObj->offset > oldOffset) // For same object only offset comparison is enough as client-symbol pair is same
						{
							OrderTableSortIndexOffsetMinHeapTop100.siftDown(i);
							//validate();
						}
						else
						{
							OrderTableSortIndexOffsetMinHeapTop100.siftUp(i);
							//validate();
						}
					}
					else
					{
						pObj->isPresentInSortedList = false;
						OrderTableSortIndexOffsetMinHeapTop100.remove(i);
						//validate();
						//OrderTableSortIndexOffsetMaxHeapMiddle.addNext(pObj);
					}

					//validate();
				}
			}
			*/
		}

		if (!OrderTableSortIndexOffsetMaxHeapMiddle.empty())
		{
			size_t numRequired = finalSortedListLen - OrderTableSortIndexOffsetMinHeapTop100.size();
			if (OrderTableSortIndexOffsetMaxHeapMiddle.size() > numRequired) // We have more objects updated object which lie in middle range
			{
				OrderTableSortIndexOffsetMaxHeapMiddle.heapify();
				while (OrderTableSortIndexOffsetMinHeapTop100.size() < finalSortedListLen)
				{
					OrderTableSortIndexOffsetMaxHeapMiddle.top()->isPresentInSortedList = true;
					OrderTableSortIndexOffsetMinHeapTop100.push(OrderTableSortIndexOffsetMaxHeapMiddle.top());
					OrderTableSortIndexOffsetMaxHeapMiddle.pop();
					//validate();
				}

				while (!OrderTableSortIndexOffsetMaxHeapMiddle.empty()
					&& comparator(OrderTableSortIndexOffsetMaxHeapMiddle.top(), OrderTableSortIndexOffsetMinHeapTop100.top()))
				{
					OrderTableSortIndexOffsetMinHeapTop100.top()->isPresentInSortedList = false;
					OrderTableSortIndexOffsetMaxHeapMiddle.push(OrderTableSortIndexOffsetMinHeapTop100.top());
					OrderTableSortIndexOffsetMinHeapTop100.pop();
					//validate();

					OrderTableSortIndexOffsetMaxHeapMiddle.top()->isPresentInSortedList = true;
					OrderTableSortIndexOffsetMinHeapTop100.push(OrderTableSortIndexOffsetMaxHeapMiddle.top());
					OrderTableSortIndexOffsetMaxHeapMiddle.pop();
					//validate();
				}
				if (!OrderTableSortIndexOffsetMaxHeapMiddle.empty() && OrderTableSortIndexOffsetMaxHeapMiddle.top()->offset > maxOffsetOfRest)
					maxOffsetOfRest = OrderTableSortIndexOffsetMaxHeapMiddle.top()->offset;

				//validate();
			}
			else // We need all updated objects
			{
				for (int i = 0; i < OrderTableSortIndexOffsetMaxHeapMiddle.size(); ++i)
				{
					OrderTableSortIndexOffsetMaxHeapMiddle.get(i)->isPresentInSortedList = true;
					OrderTableSortIndexOffsetMinHeapTop100.push(OrderTableSortIndexOffsetMaxHeapMiddle.get(i));
				}
				OrderTableSortIndexOffsetMaxHeapMiddle.clear();
			}
		}
		//Get all remaining from last sorted list
		//for (int i = 0; i <= top100Index; ++i)
		//{
		//	if (top100[i]->isPresentInSortedList == true) // && top100[i]->offset > maxOffsetOfRest)
		//	{
		//		top100[i]->isPresentInSortedList = false;
		//		//OrderTableSortIndexOffsetMaxHeap.push(top100[i]);
		//		OrderTableSortIndexOffsetMaxHeapMiddle.addNext(top100[i]);
		//	}
		//}
		
		//++OrderTableMaxHeapVectorIndex;
		//std::make_heap(&OrderTableMaxHeapVector[0], &OrderTableMaxHeapVector[OrderTableMaxHeapVectorIndex], comparatorMaxHeap);
		
		//int numRequiredObjects = finalSortedListLen - OrderTableSortIndexOffset.size();
		//int sizeOfMaxHeap = OrderTableSortIndexOffsetMaxHeap.size();

		//Fill up the set upto required size if we have elements in Heap
		//top100Index = -1;
		//while (!OrderTableSortIndexOffsetMaxHeapMiddle.empty() && (top100Index + 1) < finalSortedListLen)
		//while (!OrderTableSortIndexOffsetMaxHeap.empty() && OrderTableSortIndexOffset.size() < finalSortedListLen)
		//while (OrderTableMaxHeapVectorIndex > 0 && OrderTableSortIndexOffset.size() < finalSortedListLen)
		//{
			//OrderTableSortIndexOffsetMaxHeapMiddle.top()->isPresentInSortedList = true;
			//OrderTableMaxHeapVector[0]->isPresentInSortedList = true;
			//top100[++top100Index] = OrderTableSortIndexOffsetMaxHeapMiddle.top();
			//std::pair<set<OrderTable*, CompareOrderTableIndexOffset>::iterator, bool> res = OrderTableSortIndexOffset.insert(OrderTableSortIndexOffsetMaxHeap.top());
			//std::pair<set<OrderTable*, CompareOrderTableIndexOffset>::iterator, bool> res = OrderTableSortIndexOffset.insert(OrderTableMaxHeapVector[0]);
			//OrderTableSortIndexOffsetMaxHeap.top()->it = res.first;
			//OrderTableMaxHeapVector[0]->it = res.first;
			//OrderTableSortIndexOffsetMaxHeapMiddle.pop();
			//std::pop_heap(&OrderTableMaxHeapVector[0], &OrderTableMaxHeapVector[OrderTableMaxHeapVectorIndex], comparatorMaxHeap);
			//--OrderTableMaxHeapVectorIndex;
			//minOffsetInSortestList = (*OrderTableSortIndexOffset.rbegin())->offset;
		//}
		/*
		//If we still have elements in Heap, try to have all max in set
		if (!OrderTableSortIndexOffsetMaxHeap.empty())
		//if(OrderTableMaxHeapVectorIndex > 0)
		{
			//MyAssert::myRunTimeAssert(OrderTableSortIndexOffset.size() == finalSortedListLen);
			auto itLast = OrderTableSortIndexOffset.end();
			--itLast;
			while (!OrderTableSortIndexOffsetMaxHeap.empty() && comparator(OrderTableSortIndexOffsetMaxHeap.top(), (*itLast)))
			//while (OrderTableMaxHeapVectorIndex > 0 && comparator(OrderTableMaxHeapVector[0], (*itLast)))
			{
				//MyAssert::myRunTimeAssert(OrderTableSortIndexOffset.size() == finalSortedListLen);
				OrderTableSortIndexOffsetMaxHeap.top()->isPresentInSortedList = true;
				//OrderTableMaxHeapVector[0]->isPresentInSortedList = true;
				std::pair<set<OrderTable*, CompareOrderTableIndexOffset>::iterator, bool> res = OrderTableSortIndexOffset.insert(OrderTableSortIndexOffsetMaxHeap.top());
				//std::pair<set<OrderTable*, CompareOrderTableIndexOffset>::iterator, bool> res = OrderTableSortIndexOffset.insert(OrderTableMaxHeapVector[0]);
				OrderTableSortIndexOffsetMaxHeap.top()->it = res.first;
				//OrderTableMaxHeapVector[0]->it = res.first;
				maxOffsetOfRest = (*itLast)->offset;
				//pMaxElementOfRest = (*itLast);
				(*itLast)->isPresentInSortedList = false;
				OrderTableSortIndexOffset.erase(itLast--);
				OrderTableSortIndexOffsetMaxHeap.pop();
				//std::pop_heap(&OrderTableMaxHeapVector[0], &OrderTableMaxHeapVector[OrderTableMaxHeapVectorIndex], comparatorMaxHeap);
				//--OrderTableMaxHeapVectorIndex;
				//minOffsetInSortestList = (*OrderTableSortIndexOffset.rbegin())->offset;
			}
			
			if (!OrderTableSortIndexOffsetMaxHeap.empty() && OrderTableSortIndexOffsetMaxHeap.top()->offset > maxOffsetOfRest)
				maxOffsetOfRest = OrderTableSortIndexOffsetMaxHeap.top()->offset;
			//if (OrderTableMaxHeapVectorIndex > 0 && OrderTableMaxHeapVector[0]->offset > maxOffsetOfRest)
			//	maxOffsetOfRest = OrderTableMaxHeapVector[0]->offset;
		}
		*/
		//if (!OrderTableSortIndexOffsetMaxHeapMiddle.empty() && OrderTableSortIndexOffsetMaxHeapMiddle.top()->offset > maxOffsetOfRest)
		//		maxOffsetOfRest = OrderTableSortIndexOffsetMaxHeapMiddle.top()->offset;

		//if ((top100Index + 1) < finalSortedListLen)
		//if (OrderTableSortIndexOffset.size() < finalSortedListLen)
		if(OrderTableSortIndexOffsetMinHeapTop100.size() < finalSortedListLen)
		{
			//We still need more in top100. Extract from rest to make top100 list.

			//int requiredMinHeapSize = finalSortedListLen - (top100Index + 1) + 1;
			size_t requiredMinHeapSize = finalSortedListLen - OrderTableSortIndexOffsetMinHeapTop100.size() + 1;
			size_t i = 0;
			for (; i <= OrderTableDataIndex && OrderTableSortIndexOffsetMinHeapRest.size() < requiredMinHeapSize; ++i)
			{
				if (OrderTableData[i].isPresentInSortedList == false)
				{
					OrderTableSortIndexOffsetMinHeapRest.push(&OrderTableData[i]);
				}
			}

			//validate();

			for (; i <= OrderTableDataIndex; ++i)
			{
				if (OrderTableData[i].isPresentInSortedList == false && comparator(&OrderTableData[i], OrderTableSortIndexOffsetMinHeapRest.top()))
				{
					OrderTableSortIndexOffsetMinHeapRest.push(&OrderTableData[i]);
					OrderTableSortIndexOffsetMinHeapRest.pop();
				}
			}
			
			//validate();

			if (OrderTableSortIndexOffsetMinHeapRest.size() == requiredMinHeapSize)
			{
				maxOffsetOfRest = OrderTableSortIndexOffsetMinHeapRest.top()->offset;
				OrderTableSortIndexOffsetMinHeapRest.pop();
			}
			while (!OrderTableSortIndexOffsetMinHeapRest.empty())
			{
				OrderTableSortIndexOffsetMinHeapRest.top()->isPresentInSortedList = true;
				OrderTableSortIndexOffsetMinHeapTop100.push(OrderTableSortIndexOffsetMinHeapRest.top());
				OrderTableSortIndexOffsetMinHeapRest.pop();

				//validate();
			}

			//validate();

			//auto it = OrderTableSortIndexOffset.end();
			//while (!OrderTableSortIndexOffsetMinHeap.empty())
			//{
			//	OrderTableSortIndexOffsetMinHeap.top()->isPresentInSortedList = true;
			//	it = OrderTableSortIndexOffset.insert(it, OrderTableSortIndexOffsetMinHeap.top());
			//	OrderTableSortIndexOffsetMinHeap.top()->it = it;
			//	OrderTableSortIndexOffsetMinHeap.pop();
			//}

			//int index = finalSortedListLen;
			//while (!OrderTableSortIndexOffsetMinHeapRest.empty())
			//{
			//	OrderTableSortIndexOffsetMinHeapRest.top()->isPresentInSortedList = true;
			//	top100[--index] = OrderTableSortIndexOffsetMinHeapRest.top();
			//	OrderTableSortIndexOffsetMinHeapRest.pop();
			//}
			//top100Index = finalSortedListLen - 1;
			/*
			for (int i = 0; i <= OrderTableDataIndex; ++i)
			{
				if (OrderTableData[i].isPresentInSortedList == false && OrderTableSortIndexOffset.size() < finalSortedListLen)
				{
					OrderTableData[i].isPresentInSortedList = true;
					OrderTableSortIndexOffset.insert(&OrderTableData[i]);
					minOffsetInSortestList = (*OrderTableSortIndexOffset.rbegin())->offset;
				}
				else if (OrderTableData[i].isPresentInSortedList == false && OrderTableData[i].offset >= minOffsetInSortestList)
				{
					pair<decltype(OrderTableSortIndexOffset)::iterator, bool> result = OrderTableSortIndexOffset.insert(&OrderTableData[i]);
					if (result.second == true) // new key is inserted
					{
						OrderTableData[i].isPresentInSortedList = true;
						//while (OrderTableSortIndexOffset.size() > finalSortedListLen)
						auto itLast = OrderTableSortIndexOffset.end();
						--itLast;
						maxOffsetOfRest = (*itLast)->offset;
						//pMaxElementOfRest = (*itLast);
						(*itLast)->isPresentInSortedList = false;
						OrderTableSortIndexOffset.erase(*itLast);
						minOffsetInSortestList = (*OrderTableSortIndexOffset.rbegin())->offset;
					}
				}
				else if (OrderTableData[i].isPresentInSortedList == false && OrderTableData[i].offset > maxOffsetOfRest)
						maxOffsetOfRest = OrderTableData[i].offset;
			}
			*/
		}

		//int index = 0;
		//for (auto it = OrderTableSortIndexOffset.begin(); it != OrderTableSortIndexOffset.end() && index < finalSortedListLen; ++it, ++index)
		//{
		//	retVal[index] = &(*it)->clientSymbolPair;
		//}

		//for (int i = 0; i <= top100Index; ++i)
		//{
		//	retVal[i] = &top100[i]->clientSymbolPair;
		//}

		//Create a copy
		OrderTableSortIndexOffsetMinHeapTop100Copy.clear();
		for (int i = 0; i < OrderTableSortIndexOffsetMinHeapTop100.size(); ++i)
		{
			OrderTableSortIndexOffsetMinHeapTop100Copy.addNext(OrderTableSortIndexOffsetMinHeapTop100.get(i));
		}

		size_t index = finalSortedListLen;
		while (index > 0)
		{
			retVal[--index] = &OrderTableSortIndexOffsetMinHeapTop100Copy.top()->clientSymbolPair;
			OrderTableSortIndexOffsetMinHeapTop100Copy.pop();
		}

		/*
		if(OrderTableDataIndex + 1 > 100)
			std::nth_element(sortedOrderTableData.begin(), sortedOrderTableData.begin() + finalSortedListLen, sortedOrderTableData.begin() + OrderTableDataIndex + 1, CompareOrderTableIndexOffset());
		std::sort(sortedOrderTableData.begin(), sortedOrderTableData.begin() + finalSortedListLen, CompareOrderTableIndexOffset());
		auto it = sortedOrderTableData.begin();
		for (int index = 0; index < finalSortedListLen; ++index, ++it)
			retVal[index] = &(*it)->clientSymbolPair;
		*/

		/*
		std::partial_sort(sortedOrderTableData.begin(), sortedOrderTableData.begin() + finalSortedListLen, sortedOrderTableData.begin() + OrderTableDataIndex + 1, CompareOrderTableIndexOffset());
		auto it = sortedOrderTableData.begin();
		for (int index = 0; index < finalSortedListLen; ++index, ++it)
			retVal[index] = &(*it)->clientSymbolPair;
		*/

		//MyAssert::myRunTimeAssert(index == finalSortedListLen);
	}




#ifdef UNDEFINED
	void Others_OMS_v4f::Others_OMS_getTop100v3(vector<std::pair<const string*, const string*>>& top100v2, unsigned int& count)
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
		//MyAssert::myRunTimeAssert(index == finalSortedListLen);

		//return retVal;
	}
#endif

}

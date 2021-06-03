#pragma once

#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <set>
#include <queue>
#include <memory>
#include <functional> // to define lambda
//#include <typeinfo>
#include <cmath>
using namespace std;

#include "Others/Others_EquityOrderManagementSystemTestData.h"

namespace mm {

	class Others_OMS_v4g
	{
	public:
		static void Others_OMS_addOrder(const string& clientName, const string& symbolName, double newOrderPrice);
		static void Others_OMS_updateMarketPrice(const string& symbolName, double currentMarketPrice);
		static vector<string> Others_OMS_getTop100v1();
		static void Others_OMS_getTop100v2(vector<const string*>& retVal);
		//static void Others_OMS_getTop100v3(vector<std::pair<const string*, const string*>>& top100v2, unsigned int& count);
		static void allocateAllMemory();
		static void deallocateAllMemory();
		static void clearAllData()
		{
			//static vector<OrderTable> OrderTableData;
			OrderTableDataIndex = -1;
			UpdatedOrderTableDataIndex = -1;
			OrderTablePrimaryKeyClientSymbolId.clear();
			OrderTableIndexSymbolId.clear();
			//OrderTableSortIndexOffset.clear();
			minOffsetInSortestList = 0.0;
			maxOffsetOfRest = 0.0;
			OrderTableSortIndexOffsetMinHeapTop100.clear();
			OrderTableSortIndexOffsetMaxHeapRest.clear();

			//static vector<ClientTable> ClientTableData;
			ClientTableDataIndex = -1;
			ClientTablePrimaryKeyClientId.clear();
			ClientTableIndexClientName.clear();

			//static vector<SymbolTable> SymbolTableData;
			SymbolTableDataIndex = -1;
			UpdatedSymbolTableDataIndex = -1;
			SymbolTablePrimaryKeySymbolId.clear();
			SymbolTableIndexSymbolName.clear();
		}

#ifdef MM_DEBUG_OTHERS_EOMS
		static vector<ClientSymbolPair> finalSortedList;
#endif

	private:

		template <class T>
		static inline void hash_combine(std::size_t& seed, const T& v)  // Similar to boost::hash_combine<T>
		{
			/*
			The magic number below is the reciprocal of the golden ratio.
			Reference:
			https://stackoverflow.com/questions/4948780/magic-number-in-boosthash-combine
			http://burtleburtle.net/bob/hash/doobs.html
			phi = (1 + sqrt(5)) / 2
			2^32 / phi = 0x9e3779b9
			*/
			std::hash<T> hasher{};
			seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
		}
		struct OrderTable;
		struct CompareOrderTableIndexOffset
		{
			bool operator()(const OrderTable* lhs, const OrderTable* rhs) const noexcept
			{
				if (lhs->offset == rhs->offset)
				{
					return lhs->clientSymbolPair < rhs->clientSymbolPair;
				}
				else
					return lhs->offset > rhs->offset;
			}
		};
		struct SymbolTable;
		struct OrderTable
		{
			OrderTable() : updated(false) {}
			OrderTable(unsigned int clientIdIn, unsigned int symbolIdIn, string&& clientSymbolPairIn, double orderPriceIn, double offsetIn)
				: clientId(clientIdIn),
				symbolId(symbolIdIn),
				clientSymbolPair(std::move(clientSymbolPairIn)),
				orderPrice(orderPriceIn),
				offset(offsetIn),
				updated(true),
				//numCountInMaxHeap(0),
				isPresentInTop100(false),
				isPresentInRest(false),
				indexInMinHeap(-1),
				//sortedArrayIndex(-1),
				symbolPtr(nullptr)
			{}
			unsigned int clientId;
			unsigned int symbolId;
			string clientSymbolPair;
			double orderPrice;
			double offset;
			bool updated;
			//int numCountInMaxHeap;  // 0 = object not in heap, 1 = 1 copy of object present in Heap
			bool isPresentInTop100;
			bool isPresentInRest;
			size_t indexInMinHeap;
			//int sortedArrayIndex;
			//set<OrderTable*, CompareOrderTableIndexOffset>::iterator it;
			SymbolTable* symbolPtr;
		};

		//Helper functions
		static void processUpdatedObject(OrderTable* pObj, int numObjects, double currentMarketPrice, double previousMinOffset);

		struct HasherOrderTablePrimaryKeyClientSymbolId
		{
			size_t operator()(const OrderTable* key) const noexcept
			{
				std::size_t seed = 0;
				hash_combine(seed, key->clientId);
				hash_combine(seed, key->symbolId);
				return seed;
			}
		};
		struct IsEqualOrderTablePrimaryKeyClientSymbolId
		{
			bool operator()(const OrderTable* lhs, const OrderTable* rhs) const noexcept
			{ return lhs->clientId == rhs->clientId && lhs->symbolId == rhs->symbolId; }
		};
		struct HasherOrderTableIndexSymbolId
		{
			size_t operator()(const OrderTable* key) const noexcept
			{
				std::hash<unsigned int> hasher{};
				return hasher(key->symbolId);
			}
		};
		struct IsEqualOrderTableIndexSymbolId
		{
			bool operator()(const OrderTable* lhs, const OrderTable* rhs) const noexcept
			{ return lhs->symbolId == rhs->symbolId; }
		};
		
		static CompareOrderTableIndexOffset comparator;
		static vector<OrderTable> OrderTableData;
		static int OrderTableDataIndex;
		static vector<OrderTable*> UpdatedOrderTableData;
		static int UpdatedOrderTableDataIndex;
		static unordered_set<OrderTable*, HasherOrderTablePrimaryKeyClientSymbolId, IsEqualOrderTablePrimaryKeyClientSymbolId> OrderTablePrimaryKeyClientSymbolId;
		static unordered_multiset<OrderTable*, HasherOrderTableIndexSymbolId, IsEqualOrderTableIndexSymbolId> OrderTableIndexSymbolId;
		//static set<OrderTable*, CompareOrderTableIndexOffset> OrderTableSortIndexOffset;
		static double minOffsetInSortestList;
		static OrderTable* pMaxElementOfRest;

		struct CompareOrderTableIndexOffsetMaxHeap
		{
			bool operator()(const OrderTable* lhs, const OrderTable* rhs) const noexcept
			{
				if (lhs->offset == rhs->offset)
				{
					return lhs->clientSymbolPair > rhs->clientSymbolPair;
				}
				else
					return lhs->offset < rhs->offset;
			}
		};
		static CompareOrderTableIndexOffsetMaxHeap comparatorMaxHeap;

		template<typename T, typename C = std::less<T>>
		class MM_Heap
		{
		public:
			MM_Heap(size_t maxSize, C&& comparatorIn = C()) // by default it is max heap
				: data_(maxSize),
				index_(-1),
				comparator_(std::move(comparatorIn))
			{}
			inline void addNext(const T& obj)
			{
				data_[++index_] = obj;
			}
			inline void addNext(T&& obj)
			{
				data_[++index_] = std::move(obj);
			}
			inline void heapify()
			{
				//std::make_heap(data_.begin(), data_.begin() + index_ + 1, comparator_);
				//Start from the parent of last element
				for(int i = getParent(index_); i >= 0; --i)
					siftDown(i);
			}

			inline void push(const T& obj)
			{
				//MM_Assert::mmRunTimeAssert(index_ < int(data_.size() - 1));
				data_[++index_] = obj;
				//std::push_heap(data_.begin(), data_.begin() + index_ + 1, comparator_);
				siftUp(index_);
			}
			inline const T& top()
			{
				//MM_Assert::mmRunTimeAssert(index_ > -1);
				return data_[0];
			}
			inline void pop(bool updateIndex = true)
			{
				//MM_Assert::mmRunTimeAssert(index_ > -1 && index_ < data_.size());
				//std::pop_heap(data_.begin(), data_.begin() + index_ + 1, comparator_);

				//If only one element
				if (index_ == 0)
				{
					--index_;
					return;
				}

				std::swap(data_[0], data_[index_]);
				//if(updateIndex)
					data_[0]->indexInMinHeap = 0;
				--index_;
				siftDown(0, updateIndex);
			}
			inline size_t size()
			{
				return index_ + 1;
			}
			inline bool empty()
			{
				return index_ == -1;
			}
			inline void clear()
			{
				index_ = -1; //TODO: Do we need to destroy all the elements?
			}
			inline const T& get(int index)
			{
				//MM_Assert::mmRunTimeAssert(index_ > -1 && index <= index_);
				return data_[index];
			}
			inline void remove(int index)
			{
				std::swap(data_[index], data_[index_]);

				--index_;
				if(comparator_(data_[index], data_[index_ + 1]))
					siftDown(index);
				else
					siftUp(index);
			}
			inline void siftDown(size_t subTreeRootIndex, bool updateIndex = true)
			{
				size_t parentOfLastElement = getParent(index_);
				T buffer = data_[subTreeRootIndex];
				while (subTreeRootIndex <= parentOfLastElement)
				{
					size_t largestIndex = getLeftChild(subTreeRootIndex); // Assume left child is largest
					size_t rightChildIndex = largestIndex + 1;
					if(rightChildIndex <= index_ && comparator_(data_[largestIndex], data_[rightChildIndex]))
						largestIndex = rightChildIndex;

					if (comparator_(buffer, data_[largestIndex]))
					{
						data_[subTreeRootIndex] = data_[largestIndex];
						//if(updateIndex)
							data_[subTreeRootIndex]->indexInMinHeap = subTreeRootIndex;
						subTreeRootIndex = largestIndex;
					}
					else
						break;
				}
				data_[subTreeRootIndex] = buffer;
				//if(updateIndex)
					data_[subTreeRootIndex]->indexInMinHeap = subTreeRootIndex;
			}
			inline void siftUp(size_t childIndex)
			{
				T buffer = data_[childIndex];
				while (childIndex > 0)
				{
					size_t parentIndex = getParent(childIndex);
					if (comparator_(data_[parentIndex], buffer))
					{
						data_[childIndex] = data_[parentIndex];
						data_[childIndex]->indexInMinHeap = childIndex;
					}
					else
						break;

					childIndex = parentIndex;
				}

				data_[childIndex] = buffer;
				data_[childIndex]->indexInMinHeap = childIndex;
			}

		private:
			vector<T> data_;
			int index_;
			const C comparator_;

			inline size_t getLeftChild(size_t index) {
				return ((index << 1) + 1);
			}

			inline size_t getRightChild(size_t index) {
				return ((index << 1) + 2);
			}

			inline size_t getParent(size_t index) {
				return ((index - 1) >> 1);
			}
		};

		//static priority_queue<OrderTable*, vector<OrderTable*>, CompareOrderTableIndexOffsetMaxHeap> OrderTableSortIndexOffsetMaxHeap;
		//static priority_queue<OrderTable*, vector<OrderTable*>, CompareOrderTableIndexOffset> OrderTableSortIndexOffsetMinHeap;
		static MM_Heap<OrderTable*, CompareOrderTableIndexOffset> OrderTableSortIndexOffsetMinHeapTop100;
		//static MM_Heap<OrderTable*, CompareOrderTableIndexOffset> OrderTableSortIndexOffsetMinHeapTop100Copy;
		static vector<OrderTable*> sortedTop100Orders;
		//static MM_Heap<OrderTable*, CompareOrderTableIndexOffsetMaxHeap> OrderTableSortIndexOffsetMaxHeapMiddle;
		static MM_Heap<OrderTable*, CompareOrderTableIndexOffsetMaxHeap> OrderTableSortIndexOffsetMaxHeapRest;
		static int maxHeapSize;
		static double maxOffsetOfRest;
		static vector<OrderTable*> OrderTableMaxHeapVector;
		static int OrderTableMaxHeapVectorIndex;
		//static vector<OrderTable*> top100;
		//static int top100Index;
		static int numUpdatedFromTop100;
		//static bool updateHeapTop100;
		//static bool updateHeapRest;

		static void validate();

		struct ClientTable
		{
			ClientTable() {}
			ClientTable(unsigned int clientIdIn, const string& clientNameIn)
				: clientId(clientIdIn),
				clientName(clientNameIn)
			{}
			unsigned int clientId;
			string clientName;
		};

		struct HasherClientTablePrimaryKeyClientId
		{
			size_t operator()(const ClientTable* key) const noexcept
			{
				std::hash<unsigned int> hasher{};
				return hasher(key->clientId);
			}
		};
		struct IsEqualClientTablePrimaryKeyClientId
		{
			bool operator()(const ClientTable* lhs, const ClientTable* rhs) const noexcept
			{ return lhs->clientId == rhs->clientId; }
		};
		struct HasherClientTableIndexClientName
		{
			size_t operator()(const ClientTable* key) const noexcept
			{
				std::hash<string> hasher{};
				return hasher(key->clientName);
			}
		};
		struct IsEqualClientTableIndexClientName
		{
			bool operator()(const ClientTable* lhs, const ClientTable* rhs) const noexcept
			{ return lhs->clientName == rhs->clientName; }
		};

		static vector<ClientTable> ClientTableData;
		static int ClientTableDataIndex;
		static unordered_set<ClientTable*, HasherClientTablePrimaryKeyClientId, IsEqualClientTablePrimaryKeyClientId> ClientTablePrimaryKeyClientId; // This is required in case clientId is not equal to index in ClientTableData
		static unordered_set<ClientTable*, HasherClientTableIndexClientName, IsEqualClientTableIndexClientName> ClientTableIndexClientName;

		struct SymbolTable
		{
			SymbolTable() {}
			SymbolTable(unsigned int symbolIdIn, const string& symbolNameIn, double marketPriceIn)
				: symbolId(symbolIdIn),
				symbolName(symbolNameIn),
				marketPrice(marketPriceIn),
				updated(false)
			{}
			unsigned int symbolId;
			string symbolName;
			double marketPrice;
			bool updated;
		};

		struct HasherSymbolTablePrimaryKeySymbolId
		{
			size_t operator()(const SymbolTable* key) const noexcept
			{
				std::hash<unsigned int> hasher{};
				return hasher(key->symbolId);
			}
		};
		struct IsEqualSymbolTablePrimaryKeySymbolId
		{
			bool operator()(const SymbolTable* lhs, const SymbolTable* rhs) const noexcept
			{ return lhs->symbolId == rhs->symbolId; }
		};
		struct HasherSymbolTableIndexSymbolName
		{
			size_t operator()(const SymbolTable* key) const noexcept
			{
				std::hash<string> hasher{};
				return hasher(key->symbolName);
			}
		};
		struct IsEqualSymbolTableIndexSymbolName
		{
			bool operator()(const SymbolTable* lhs, const SymbolTable* rhs) const noexcept
			{ return lhs->symbolName == rhs->symbolName; }
		};

		static vector<SymbolTable> SymbolTableData;
		static int SymbolTableDataIndex;
		static vector<SymbolTable*> UpdatedSymbolTableData;
		static int UpdatedSymbolTableDataIndex;
		static unordered_set<SymbolTable*, HasherSymbolTablePrimaryKeySymbolId, IsEqualSymbolTablePrimaryKeySymbolId> SymbolTablePrimaryKeySymbolId; // This is required in case symbolId is not equal to index in SymbolTableData
		static unordered_set<SymbolTable*, HasherSymbolTableIndexSymbolName, IsEqualSymbolTableIndexSymbolName> SymbolTableIndexSymbolName;

		static OrderTable dummyOrder;
		static ClientTable dummyClient;
		static SymbolTable dummySymbol;
	};
	

}
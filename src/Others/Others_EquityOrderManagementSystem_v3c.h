#pragma once

#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <set>
//#include <memory>
#include <functional> // to define lambda
//#include <typeinfo>
using namespace std;

#include "Others/Others_EquityOrderManagementSystemTestData.h"

namespace mm {

	class Others_OMS_v3c
	{
	public:
		static void Others_OMS_addOrder(const string& clientName, const string& symbolName, double newOrderPrice);
		static void Others_OMS_updateMarketPrice(const string& symbolName, double currentMarketPrice);
		static vector<string> Others_OMS_getTop100v1();
		static void Others_OMS_getTop100v2(vector<const string*>& retVal);
		static void allocateAllMemory();
		static void deallocateAllMemory();
		static void clearAllData()
		{
			//static vector<OrderTable> OrderTableData;
			OrderTableDataIndex = -1;
			OrderTablePrimaryKeyClientSymbolId.clear();
			OrderTablePositionIndexSymbolId.clear();
			OrderTableSortIndexOffset.clear();
			minOffsetInSortestList = 0.0;

			//static vector<ClientTable> ClientTableData;
			ClientTableDataIndex = -1;
			ClientTablePrimaryKeyClientId.clear();
			ClientTableIndexClientName.clear();

			//static vector<SymbolTable> SymbolTableData;
			SymbolTableDataIndex = -1;
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

		struct OrderTable
		{
			OrderTable() {}
			OrderTable(unsigned int clientIdIn, unsigned int symbolIdIn, string&& clientSymbolPairIn, double orderPriceIn, double offsetIn)
				: clientId(clientIdIn),
				symbolId(symbolIdIn),
				clientSymbolPair(std::move(clientSymbolPairIn)),
				orderPrice(orderPriceIn),
				offset(offsetIn)
			{}
			unsigned int clientId;
			unsigned int symbolId;
			string clientSymbolPair;
			double orderPrice;
			double offset;
		};

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
		struct CompareOrderTableIndexOffset
		{
			bool operator()(const OrderTable* lhs, const OrderTable* rhs) const noexcept
			//{ return lhs->offset > rhs->offset; }
			{
				if (lhs->offset == rhs->offset)
				{
					ClientTable lhsDummyClient(lhs->clientId, "");
					auto lhsItClient = ClientTablePrimaryKeyClientId.find(&lhsDummyClient);
					ClientTable rhsDummyClient(rhs->clientId, "");
					auto rhsItClient = ClientTablePrimaryKeyClientId.find(&rhsDummyClient);
					if ((*lhsItClient)->clientName == (*rhsItClient)->clientName)
					{
						SymbolTable lhsDummySymbol(lhs->symbolId, "", 0);
						auto lhsItSymbol = SymbolTablePrimaryKeySymbolId.find(&lhsDummySymbol);
						SymbolTable rhsDummySymbol(rhs->symbolId, "", 0);
						auto rhsItSymbol = SymbolTablePrimaryKeySymbolId.find(&rhsDummySymbol);
						return ((*lhsItSymbol)->symbolName < (*rhsItSymbol)->symbolName);
					}
					else
						return ((*lhsItClient)->clientName < (*rhsItClient)->clientName);
				}
				else
					return lhs->offset > rhs->offset;
			}
		};

		static vector<OrderTable> OrderTableData;
		static int OrderTableDataIndex;
		static unordered_set<OrderTable*, HasherOrderTablePrimaryKeyClientSymbolId, IsEqualOrderTablePrimaryKeyClientSymbolId> OrderTablePrimaryKeyClientSymbolId;
		static unordered_multiset<OrderTable*, HasherOrderTableIndexSymbolId, IsEqualOrderTableIndexSymbolId> OrderTablePositionIndexSymbolId;
		static set<OrderTable*, CompareOrderTableIndexOffset> OrderTableSortIndexOffset;
		static double minOffsetInSortestList;
		//TODO: keep a running min heap and keep it updating
		// Keeping a running heap is not possible because if we want to update any inner element of heap, its not possible

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
				marketPrice(marketPriceIn)
			{}
			unsigned int symbolId;
			string symbolName;
			double marketPrice;
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
		static unordered_set<SymbolTable*, HasherSymbolTablePrimaryKeySymbolId, IsEqualSymbolTablePrimaryKeySymbolId> SymbolTablePrimaryKeySymbolId; // This is required in case symbolId is not equal to index in SymbolTableData
		static unordered_set<SymbolTable*, HasherSymbolTableIndexSymbolName, IsEqualSymbolTableIndexSymbolName> SymbolTableIndexSymbolName;
	};
	

}
#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <memory>
#include <functional> // to define lambda
#include <queue>
//#include <typeinfo>
using namespace std;

namespace mm {

	class Others_OMS_v1c
	{
	public:
		static void Others_OMS_addOrder(const string& client, const string& symbol, double newOrderPrice);
		static void Others_OMS_updateMarketPrice(const string& symbol, double currentMarketPrice);
		static vector<string> Others_OMS_getTop100v1();
		static void Others_OMS_getTop100v2(vector<const string*>& retVal);
		static void allocateAllMemory();
		static void deallocateAllMemory();
		static void clearAllData()
		{
			data_index = -1;
			clientSymbolPairIndex.clear();
			symbolIdIndex.clear();
			clientNameClientIdMap.clear();
			//symbolNameSymbolIdMap.clear();
			//symbolIdSymbolPriceMap.clear();
			symbolNameSymbolDataMap.clear();
			maxClientId = 0;
			maxSymbolId = 0;
			sortedByOffsetSet.clear();

			updatedSymbolDataIndex = -1;
		}

	private:

		struct SymbolData
		{
			SymbolData() {}
			SymbolData(unsigned int symbolIdIn, double symbolPriceIn)
				: symbolId(symbolIdIn),
				symbolPrice(symbolPriceIn),
				updated(false)
			{}
			unsigned int symbolId;
			//string symbolName;
			double symbolPrice;
			bool updated;
		};

		struct OrderData
		{
			OrderData() {}
			OrderData(unsigned int clientSymbolIdIn, string&& clientSymbolPairIn, double orderPriceIn, double marketPrice)
				: clientSymbolId(clientSymbolIdIn),
				//symbolData(symbolDataIn),
				clientSymbolPair(std::move(clientSymbolPairIn)),
				orderPrice(orderPriceIn),
				offset(100 * fabs(orderPriceIn - marketPrice) / marketPrice),
				updated(false)
			{
			}

			unsigned int clientSymbolId; // client Id and symbol Id will be stored in 16 bits each
			//SymbolData* symbolData;
			string clientSymbolPair;
			double orderPrice;
			double offset;
			bool updated;
		};

		static vector<OrderData> orderData_;
		static int data_index;
		static unordered_map<unsigned int, OrderData*> clientSymbolPairIndex;
		static unordered_multimap <unsigned int, OrderData*> symbolIdIndex;

		static unordered_map<string, unsigned int> clientNameClientIdMap;
		//static unordered_map<string, unsigned int> symbolNameSymbolIdMap;
		//static unordered_map<unsigned int, double> symbolIdSymbolPriceMap;
		static unordered_map<string, SymbolData> symbolNameSymbolDataMap;

		struct Compare
		{
			bool operator()(const OrderData* a, const OrderData* b) const
			{
				if (a->offset == b->offset)
					return a->clientSymbolPair < b->clientSymbolPair;
				else
					return a->offset > b->offset;
			}
		};
		static set<OrderData*, Compare> sortedByOffsetSet;

		static unsigned int maxClientId;
		static unsigned int maxSymbolId;

		static vector<SymbolData*> updatedSymbolData;
		static int updatedSymbolDataIndex;
	};
	

}
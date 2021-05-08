#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <memory>
#include <functional> // to define lambda
//#include <typeinfo>
#include <cmath>
using namespace std;

namespace mm {

	class Others_OMS_v1b
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
			//data_.clear();
			data_index = -1;
			clientSymbolPairIndex.clear();
			symbolIndex.clear();
			clientNameClientIdMap.clear();
			symbolNameSymbolIdMap.clear();
			//clientIdClientNameMap.clear();
			//symbolIdSymbolNameMap.clear();
			symbolIdSymbolPriceMap.clear();
			maxClientId = 0;
			maxSymbolId = 0;
		}

	private:

		struct DataSet_v1
		{
			DataSet_v1() {}
			DataSet_v1(unsigned int clientSymbolIdIn, string&& clientSymbolPairIn, double orderPriceIn, double marketPriceIn)
				: clientSymbolId(clientSymbolIdIn),
				clientSymbolPair(std::move(clientSymbolPairIn)),
				orderPrice(orderPriceIn),
				offset(100 * fabs(orderPriceIn - marketPriceIn) / marketPriceIn)
			{
			}

			unsigned int clientSymbolId; // client Id and symbol Id will be stored in 16 bits each
			string clientSymbolPair;
			double orderPrice;
			double offset;
		};

		static vector<DataSet_v1> data_;
		static int data_index;
		static unordered_map<unsigned int, DataSet_v1*> clientSymbolPairIndex;
		static unordered_multimap <unsigned int, DataSet_v1*> symbolIndex;

		static unordered_map<string, unsigned int> clientNameClientIdMap;
		static unordered_map<string, unsigned int> symbolNameSymbolIdMap;
		//static unordered_map<unsigned int, string> clientIdClientNameMap;
		//static unordered_map<unsigned int, string> symbolIdSymbolNameMap;
		static unordered_map<unsigned int, double> symbolIdSymbolPriceMap;
		
		static unsigned int maxClientId;
		static unsigned int maxSymbolId;
	};
	

}
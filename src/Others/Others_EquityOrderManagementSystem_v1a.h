#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include <memory>
#include <functional> // to define lambda
//#include <typeinfo>
using namespace std;

namespace mm {

	class Others_OMS_v1a
	{
	public:
		static void Others_OMS_addOrder(const string& client, const string& symbol, double newOrderPrice);
		static void Others_OMS_updateMarketPrice(const string& symbol, double currentMarketPrice);
		static vector<string> Others_OMS_getTop100v1();
		static void Others_OMS_getTop100v2(vector<const string*>& retVal);
		//static void Others_OMS_getTop100v3(vector<std::pair<const string*, const string*>>& top100v2, unsigned int& count);
		static void allocateAllMemory();
		static void deallocateAllMemory();

		static void clearAllData()
		{
			//data_.clear();
			data_index = -1;
			clientSymbolPairIndex.clear();
			symbolIndex.clear();
			symbolMarketPriceIndex.clear();
		}

	private:

		struct DataSet_v1
		{
			DataSet_v1(const string& clientIn, const string& symbolIn, string&& clientSymbolPair, double orderPriceIn, double marketPriceIn)
				: client(clientIn),
				symbol(symbolIn),
				clientSymbolPair(std::move(clientSymbolPair)),
				orderPrice(orderPriceIn),
				offset(100 * fabs(orderPriceIn - marketPriceIn) / marketPriceIn)
			{
			}

			string client;
			string symbol;
			string clientSymbolPair;
			double orderPrice;
			double offset;
		};

		static std::function<bool(const string* a, const string* b)> comp;
		static vector<unique_ptr<DataSet_v1>> data_;
		static int data_index;
		static map<string*, DataSet_v1*,  decltype(comp)> clientSymbolPairIndex;
		static multimap<const string*, DataSet_v1*,  decltype(comp)> symbolIndex;
		static map<string, double> symbolMarketPriceIndex;
	};

}
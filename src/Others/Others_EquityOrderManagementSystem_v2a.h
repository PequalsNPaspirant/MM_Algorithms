#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <set>
#include <memory>
#include <functional> // to define lambda
//#include <typeinfo>
using namespace std;

namespace mm {

	class Others_OMS_v2a
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
			symbolHashMap.clear();
			returnData.clear();
		}

	private:

		struct DataSet_v1
		{
			DataSet_v1() {}
			DataSet_v1(double orderPriceIn, double marketPriceIn, string&& clientSymbolPairIn)
				: orderPrice(orderPriceIn),
				offset(100 * fabs(orderPriceIn - marketPriceIn) / marketPriceIn),
				clientSymbolPair(clientSymbolPairIn)
			{
			}

			double orderPrice;
			double offset;
			string clientSymbolPair;
		};

		struct DataSet_v2
		{
			//DataSet_v2() {}
			DataSet_v2(double marketPriceIn)
				: marketPrice(marketPriceIn)
			{
			}

			double marketPrice;
			unordered_map<string, DataSet_v1> clientHashMap;
			struct compare
			{
				bool operator()(const DataSet_v1* lhs, const DataSet_v1* rhs) const
				{
					if (lhs->offset == rhs->offset)
						return lhs->clientSymbolPair < rhs->clientSymbolPair;
					else
						return lhs->offset > rhs->offset;
				}
			};
			//map<double, string, std::greater<double>> offsetClientSymbolPairMap;
			set<DataSet_v1*, compare> offsetClientSymbolPairMap;
		};

		static unordered_map<string, DataSet_v2> symbolHashMap;
		//static map<double, string, std::greater<double>> returnData;
		static set<DataSet_v1*, DataSet_v2::compare> returnData;
		//TODO: try keeping heap instead of set
		//TODO: OR try keeping running sorted list of top 100 only
	};
	

}
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cassert>

#include "Others/Others_FxRatesCache.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	enum currency
	{
		USD = 0,
		EUR,
		CAD,
		AUD,
		GBP,
		NZD,
		JPY,
		SGD,
		CHF,
		HUF,
		MXN,
		HKD,
		SEK,
		KRW,
		NOK,
		INR,
		XCD,
		EGP,
		FJD,
		IDR, //20

		//Some dummy currencies
		CUR1,
		CUR2,
		CUR3,
		CUR4,
		CUR5,
		CUR6,
		CUR7,
		CUR8,
		CUR9,
		CUR10,

		MAX_CURRENCIES //30
	};

	class FxRatesCache_v1
	{
	public:
		FxRatesCache_v1()
			: cache_(MAX_CURRENCIES)
		{

		}

		void addOrUpdate(currency from, currency to, double rate)
		{
			cache_[from][to] = rate;
			cache_[to][from] = 1.0 / rate;
		}

		double get(currency from, currency to)
		{
			double& rate = cache_[from][to]; //Ensure calculation of hash for key "to" is done only once
			if (fabs(rate) < precision)
			{
				//unordered_map<currency, double>& fromMap = (cache_[from].size() < cache_[to].size()) ? cache_[from] : cache_[to];
				//unordered_map<currency, double>& biggerMap = (cache_[from].size() < cache_[to].size()) ? cache_[to] : cache_[from];
				using MapValueType = unordered_map<currency, double>::value_type;
				for (MapValueType& element : cache_[from])
				{
					auto it = cache_[element.first].find(to);
					if (it != cache_[element.first].end())
					{
						rate = element.second * it->second;
						cache_[to][from] = 1.0 / rate;
						break;
					}
				}
			}

			return rate;
		}

	private:
		std::vector< std::unordered_map<currency, double> > cache_;

	public:
		static constexpr const double precision = 0.0001; //10 ^ -4
	};

	MM_DECLARE_FLAG(Others_FxRatesCache);

	MM_UNIT_TEST(Others_FxRatesCache_test_1, Others_FxRatesCache)
	{
		enum funName
		{
			eAdd,
			eGet
		};

		struct testDataStruct
		{
			funName fun;
			currency from;
			currency to;
			double rate;
		};

		std::vector<testDataStruct> testData{
			{ eAdd, USD, GBP, 0.77 },
			{ eAdd, USD, EUR, 0.89 },
			{ eAdd, EUR, INR, 78.65 },
			{ eAdd, INR, AUD, 0.020 },
			{ eAdd, AUD, SGD, 0.96 },
			{ eAdd, MXN, SGD, 0.070 },

			{ eGet, GBP, EUR, 1.1558 },
			{ eGet, USD, MXN, 0.77 },
			{ eGet, USD, SGD, 0.77 },
		};

		FxRatesCache_v1 obj;

		for (int i = 0; i < testData.size(); ++i)
		{
			double actualResult = 0.0;
			switch (testData[i].fun)
			{
			case eAdd:
				obj.addOrUpdate(testData[i].from, testData[i].to, testData[i].rate);
				break;
			case eGet:
				actualResult = obj.get(testData[i].from, testData[i].to);
				MM_EXPECT_TRUE(fabs(actualResult - testData[i].rate) < FxRatesCache_v1::precision, actualResult, testData[i].rate);
				break;
			default:
				assert(false, "unknown API");
			}
		}		
	}
}
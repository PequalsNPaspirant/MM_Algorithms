#include "Others/Others_FixedPriceInstrumentsPricing.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Assert/MyAssert.h"

namespace mm {

	FixedPriceInstrumentsPricing_1::FixedPriceInstrumentsPricing_1(const vector<FixedPriceInstrumentsPricing_1::Data>& data)
	{
		for (const Data& d : data)
		{
			//std::pair< set< unique_ptr<Data> >::iterator, bool> res = dataSet.insert(std::make_unique<Data>(d));
			//multiset< unique_ptr<Data> >::iterator res = dataSet.insert(std::make_unique<Data>(d));
			// If the element is newly inserted, then add it in hash table 
			//if(res.second) multiset always inerts a new element even though the same key is already present
			// instruIdToIteratorMap.insert(std::pair<const hashTableType::key_type, hashTableType::mapped_type>(d.instrument_id, res));

			instruIdToIteratorMap[d.instrument_id] = dataSet.insert(std::make_unique<Data>(d));
		}
	}

	void FixedPriceInstrumentsPricing_1::changePrice(int instrument_id, priceType type, double newPrice)
	{
		hashTableType::iterator it = instruIdToIteratorMap.find(instrument_id);
		if (it != instruIdToIteratorMap.end())
		{
			//std::unique_ptr<Data> temp = std::make_unique<Data>(*it->second);
			setType::iterator it2 = it->second;
			const std::unique_ptr<Data>& ctemp = (*it2);
			std::unique_ptr<Data> temp = std::make_unique<Data>(*ctemp);
			switch (type)
			{
			case priceType::ask_price:
				temp->ask_price = newPrice;
				break;
			case priceType::bid_price:
				temp->bid_price = newPrice;
				break;
			case priceType::mid_price:
				temp->mid_price = newPrice;
				break;
			}

			dataSet.erase(it->second);
			instruIdToIteratorMap[instrument_id] = dataSet.insert(std::move(temp));
		}
		else
			MyAssert::myRunTimeAssert(false, "updating non existing instrument type");
	}

	MM_DECLARE_FLAG(FixedPriceInstrumentsPricingUnitTest);

	MM_UNIT_TEST(FixedPriceInstrumentsPricing_1_UnitTest_sanity, FixedPriceInstrumentsPricingUnitTest)
	{
		vector<FixedPriceInstrumentsPricing_1::Data> testData{
		// {id, ask, bid, mid}
			{ 1, 20, 10, 15 },
			{ 2, 50, 40, 45 },
			{ 3, 60, 50, 55 },
			{ 4, 30, 20, 25 },
			{ 5, 10, 5, 8 },
			{ 6, 90, 80, 85 },
			{ 7, 50, 40, 45 },
			{ 8, 40, 30, 35 }
		};
		FixedPriceInstrumentsPricing_1 testObject(testData);
		testObject.changePrice(5, FixedPriceInstrumentsPricing_1::priceType::ask_price, 7);
		double value = 0.0;
		MM_EXPECT_TRUE((value = 1) == 1, value);

	}






	FixedPriceInstrumentsPricing_2::FixedPriceInstrumentsPricing_2(const vector<FixedPriceInstrumentsPricing_2::Data>& data)
	{
		for (const Data& d : data)
		{
			//std::pair< set< unique_ptr<Data> >::iterator, bool> res = dataSet.insert(std::make_unique<Data>(d));
			//
			
			//if(res.second) multiset always inerts a new element even though the same key is already present
			// instruIdToIteratorMap.insert(std::pair<const hashTableType::key_type, hashTableType::mapped_type>(d.instrument_id, res));

			std::pair<mapType::iterator, bool> res = dataMap.insert(mapType::value_type(d.instrument_id, std::make_unique<Data>(d)));
			// If the element is newly inserted, then add it in hash table
			if (res.second)
			{
				if (d.getOffset() > 0.0)
					instruIdToIteratorMap[d.instrument_id] = dataMultiSet.insert(res.first);
			}
			else
				MyAssert::myRunTimeAssert(false, "duplicate entry for instrument id " + to_string(d.instrument_id));
		}
	}

	void FixedPriceInstrumentsPricing_2::changePrice(int instrument_id, priceType type, double newPrice)
	{
		mapType::iterator it1 = dataMap.find(instrument_id);
		if (it1 != dataMap.end())
		{
			std::unique_ptr<Data>& temp = it1->second;
			switch (type)
			{
			case priceType::ask_price:
				temp->ask_price = newPrice;
				break;
			case priceType::bid_price:
				temp->bid_price = newPrice;
				break;
			case priceType::mid_price:
				temp->mid_price = newPrice;
				break;
			}

			// Erase old entry from dataMultiSet
			hashTableType::iterator it2 = instruIdToIteratorMap.find(instrument_id);
			if (it2 != instruIdToIteratorMap.end())
			{
				multisetType::iterator it3 = it2->second;
				dataMultiSet.erase(it3);
			}

			if (temp->getOffset() > 0.0)
				instruIdToIteratorMap[instrument_id] = dataMultiSet.insert(it1);
		}
		else
			MyAssert::myRunTimeAssert(false, "entry for instrument id " + to_string(instrument_id) + " not found in master table");
	}


	MM_UNIT_TEST(FixedPriceInstrumentsPricing_2_UnitTest_sanity, FixedPriceInstrumentsPricingUnitTest)
	{
		vector<FixedPriceInstrumentsPricing_2::Data> testData{
			// {id, ask, bid, mid}
			{ 1, 20, 10, 15 },
			{ 2, 50, 40, 45 },
			{ 3, 60, 50, 55 },
			{ 4, 30, 20, 25 },
			{ 5, 10, 5, 8 },
			{ 6, 90, 80, 85 },
			{ 7, 50, 40, 45 },
			{ 8, 40, 30, 35 }
		};
		FixedPriceInstrumentsPricing_2 testObject(testData);
		testObject.changePrice(5, FixedPriceInstrumentsPricing_2::priceType::ask_price, 7);
		double value = 0.0;
		MM_EXPECT_TRUE((value = 1) == 1, value);

	}
}


#include "Others/Others_StockPriceHistory_v1.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	/*
	Requirement: This has to be fast...very very fast
	Create a collection that keeps the history of last n stock price changes at the various time in past.
	The goal is to keep only latest n price entries in collection along with their dates.
	If we query the collection for any time, it gives the rate recorded on that time, if its present. 
	If the time is not present in collection, it returns the rate for the latest time available before this time.
	*/

	StockPriceHistory::StockPriceHistory(size_t window)
		: m_maxSize(window),
		m_startTime(1)
	{

	}

	void StockPriceHistory::AddValue(time_t current_time, double value) {
		//Make sure map holds max m_maxSize elements always
		time_t maxTime = m_startTime + m_maxSize - 1;
		if (current_time > maxTime)
		{
			m_startTime = current_time - m_maxSize + 1;
			auto itStart = m_data.begin();
			auto itEnd = m_data.lower_bound(m_startTime);
			m_data.erase(itStart, --itEnd);
		}

		m_data[current_time] = value;
	}

	double StockPriceHistory::GetValue(time_t desired_time) {
		if (desired_time < m_startTime)
			return -1;
		auto itEnd = m_data.upper_bound(desired_time);
		if (itEnd == m_data.begin())
			return 0;

		--itEnd;
		return itEnd->second;
	}

	MM_DECLARE_FLAG(StockPriceHistoryUnitTest);

	MM_UNIT_TEST(StockPriceHistoryUnitTest_sanity, StockPriceHistoryUnitTest)
	{
		StockPriceHistory ts(5000);  // object saves the most recent 5000 seconds
		ts.AddValue(10, 2.0);
		ts.AddValue(12, 3.0);
		ts.AddValue(14, 3.5);
		double value = 0.0;
		MM_EXPECT_TRUE((value = ts.GetValue(13)) == 3.0, value);
		MM_EXPECT_TRUE((value = ts.GetValue(14)) == 3.5, value);
		MM_EXPECT_TRUE((value = ts.GetValue(9)) == 0, value);
		ts.AddValue(5011, 4.0);
		MM_EXPECT_TRUE((value = ts.GetValue(9)) == -1, value);
	}

}
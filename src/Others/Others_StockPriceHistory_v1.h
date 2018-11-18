#pragma once

#include <ctime>
#include <map>

namespace mm {

	class StockPriceHistory {
	public:
		StockPriceHistory(size_t window);
		void AddValue(time_t current_time, double value);
		double GetValue(time_t desired_time);

	private:
		std::map<time_t, double> m_data;
		time_t m_startTime;
		const size_t m_maxSize;
	};

}
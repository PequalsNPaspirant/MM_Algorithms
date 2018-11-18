#pragma once

#include <vector>
#include <set>
#include <unordered_map>
#include <memory>

#include <map>

using namespace std;

namespace mm {

	class employee
	{
		int id;
		string name;
		int phone;
		string city;
	};

	// If sorting is not required, replace map by unordered_map
	// Search or sort by any member of class employee
	using PrimaryKeyType = std::map<int, unique_ptr<employee> >;  // Stores id as key
	PrimaryKeyType primaryKey;
	std::multimap<string, PrimaryKeyType::iterator> index1; // Stores name as key
	std::multimap<int, PrimaryKeyType::iterator> index2; // Stores phone as key
	std::multimap<string, PrimaryKeyType::iterator> index3; // Stores city as key

	//Modify any member of class employee except primary key
	// 1. modify name of some employee with id = x
	/*
	Search id x in primaryKey, remember "iterator" & "old name" and replace by "new name"
	Search in index1 by "old name"
	If multiple elements, then check for id x
	erase that element
	insert new element std::pair<"new name", "iterator"> into index1
	*/



	class FixedPriceInstrumentsPricing_1
	{
	public:
		
		struct Data
		{
			Data(int a, double b, double c, double d)
				: instrument_id(a),
				ask_price(b),
				bid_price(c),
				mid_price(d)
			{ }

			int instrument_id;
			double ask_price; // Market price
			double bid_price; // bid price by clients
			double mid_price; // price offered by traders

			double getOffset() const
			{
				if (bid_price > mid_price)
					return bid_price - mid_price;
				else if (mid_price > ask_price)
					return mid_price - ask_price;
				else
					return 0.0;
			}
		};

		enum class priceType
		{
			ask_price,
			bid_price,
			mid_price
		};

		class comparator
		{
		public:
			bool operator()(const unique_ptr<Data>& d1, const unique_ptr<Data>& d2)
			{
				return d1->getOffset() > d2->getOffset();
			}
		};

		FixedPriceInstrumentsPricing_1(const vector<Data>& data);
		void changePrice(int instrument_id, priceType type, double newPrice);

		using setType = multiset< unique_ptr<Data>, comparator >;
		using hashTableType = unordered_map<int, setType::iterator>;

	private:
		setType dataSet;
		hashTableType instruIdToIteratorMap;
	};



	class FixedPriceInstrumentsPricing_2
	{
	public:

		struct Data
		{
			Data(int a, double b, double c, double d)
				: instrument_id(a),
				ask_price(b),
				bid_price(c),
				mid_price(d)
			{ }

			int instrument_id;
			double ask_price; // Market price
			double bid_price; // bid price by clients
			double mid_price; // price offered by traders

			double getOffset() const
			{
				if (bid_price > mid_price)
					return bid_price - mid_price;
				else if (mid_price > ask_price)
					return mid_price - ask_price;
				else
					return 0.0;
			}
		};

		enum class priceType
		{
			ask_price,
			bid_price,
			mid_price
		};

		

		FixedPriceInstrumentsPricing_2(const vector<Data>& data);
		void changePrice(int instrument_id, priceType type, double newPrice);

		using mapType = unordered_map<int, unique_ptr<Data> >;
		class comparator
		{
		public:
			bool operator()(const mapType::iterator& t1, const mapType::iterator& t2)
			{
				return t1->second->getOffset() > t2->second->getOffset();
			}
		};
		using multisetType = multiset< mapType::iterator, comparator >;
		using hashTableType = unordered_map<int, multisetType::iterator>;

	private:
		mapType dataMap;
		multisetType dataMultiSet;
		hashTableType instruIdToIteratorMap;
	};
}
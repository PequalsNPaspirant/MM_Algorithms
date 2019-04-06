#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include <chrono>
using namespace std;

#include "STL/STL_unordered_map.h"
#include "Timer/Timer_Timer.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {
	
	void unordered_map_operatorSquareBrackets()
	{
		struct myStruct
		{
			myStruct(string time)
				: time_(std::move(time))
			{
				static int id = -1;
				++id;
				id_ = id;
			}

			int id_;
			string time_;
		};

		static unordered_map<string, unique_ptr<myStruct>> cache;
		static int id = 0;

		string key{"myKey"};
		myStruct* pValue;

		// ========= inefficient way to modify/get value from cache
		// number of hash calculations = 4
		// number of calls to default constructor of unique_ptr<myStruct> = 1
		// number of calls to myStruct::myStruct(int id, const string& name) = 1
		// number of calls to move constructor of unique_ptr<myStruct> = 1
		if (cache[key])
			pValue = cache[key].get();
		else
		{
			string time = Timer::getCurrentLocalTimeInNanoSeconds();
			cache[key] = make_unique<myStruct>(time);
			pValue = cache[key].get();
		}

		// ========= little more efficient way to modify/get value from cache
		// number of hash calculations = 2
		// number of calls to default constructor of unique_ptr<myStruct> = 0
		// number of calls to myStruct::myStruct(int id, const string& name) = 1
		// number of calls to constructor of unique_ptr<myStruct> = 1
		// number of calls to move constructor of unique_ptr<myStruct> = 0
		unordered_map<string, unique_ptr<myStruct>>::iterator it = cache.find(key);

		if (it == cache.end())
		{
			string time = Timer::getCurrentLocalTimeInNanoSeconds();
			pair<unordered_map<string, unique_ptr<myStruct>>::iterator, bool> result = 
				cache.insert(unordered_map<string, unique_ptr<myStruct>>::value_type(key, make_unique<myStruct>(time)));
			it = result.first;
		}

		pValue = it->second.get();

		// ========= little more efficient way to modify/get value from cache
		// number of hash calculations = 1
		// number of calls to default constructor of unique_ptr<myStruct> = 1
		// number of calls to myStruct::myStruct(int id, const string& name) = 1
		// number of calls to move constructor of unique_ptr<myStruct> = 1
		string time = Timer::getCurrentLocalTimeInNanoSeconds();
		unique_ptr<myStruct> ptr = make_unique<myStruct>(time);
		std::pair<unordered_map<string, unique_ptr<myStruct>>::iterator, bool> result =
			cache.insert(unordered_map<string, unique_ptr<myStruct>>::value_type{ key, std::move(ptr) });

		pValue = result.first->second.get();

		// ========= efficient way to modify/get value from cache
		// number of hash calculations = 1
		// number of calls to default constructor of unique_ptr<myStruct> = 1
		// number of calls to myStruct::myStruct(int id, const string& name) = 1
		// number of calls to move constructor of unique_ptr<myStruct> = 1
		unique_ptr<myStruct>& pValTemp = cache[key];
		
		if (!pValTemp)
		{
			string time = Timer::getCurrentLocalTimeInNanoSeconds();
			pValTemp = make_unique<myStruct>(time);
		}

		pValue = pValTemp.get();

	}


	void nextTest()
	{

		//MM_EXPECT_TRUE(actualSize == sizeCapacityData[0].size && 
		//	actualCapacity == sizeCapacityData[0].capacity && 
		//	actual_max_size == sizeCapacityData[0].max_size,
		//	actualSize, sizeCapacityData[0].size, actualCapacity, sizeCapacityData[0].capacity, actual_max_size, sizeCapacityData[0].max_size);

	}
	

	MM_DECLARE_FLAG(STL_unordered_map_UnitTest);

	MM_UNIT_TEST(STL_unordered_map_UnitTest_sanity, STL_unordered_map_UnitTest)
	{
		unordered_map_operatorSquareBrackets();
	}

}
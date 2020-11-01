#include <iostream>
#include <chrono>
#include <random>
#include <set>
#include <map>
#include <list>
#include <forward_list>

#include "Memory/Memory_HeapAllocator_v2.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Timer/Timer_Timer.h"

namespace mm {

	class PushFrontTest
	{
	public:
		template <typename Container>
		void operator()(Container& container, size_t iterations, size_t repeat)
		{
			for (int i = 0; i < repeat; ++i)
			{
				int size = 0;
				while (size < iterations)
					container.push_front(size++);

				for (; size > iterations; size--)
					container.pop_front();
				//container.clear();
			}
		}
	};

	class PushBackTest
	{
	public:
		template <typename Container>
		void operator()(Container& container, size_t iterations, size_t repeat)
		{
			for (int i = 0; i < repeat; ++i)
			{
				int size = 0;
				while (size < iterations)
					container.push_back(size++);

				for (; size > iterations; size--)
					container.pop_back();
				//container.clear();
			}
		}
	};

	class MapTest
	{
	public:
		template <typename Container>
		void operator()(Container& container, size_t iterations, size_t repeat)
		{
			for (int i = 0; i < repeat; ++i)
			{
				int size = 0;
				while (size < iterations)
					container.insert(std::pair<char, int>(size++, size));

				while (size > iterations)
					container.erase(--size);
				//container.clear();
			}
		}
	};

	class SetTest
	{
	public:
		template <typename Container>
		void operator()(Container& container, size_t iterations, size_t repeat)
		{
			for (int i = 0; i < repeat; ++i)
			{
				int size = 0;
				while (size < iterations)
					container.insert(size++);

				while (size > iterations)
					container.erase(--size);
				//container.clear();
			}
		}
	};

	template<typename DataType, typename Allocator>
	void Memory_HeapAllocator_v2_unit_test(size_t iterations, size_t repeat)
	{
		auto compareResults = [](const string& msg1, const string& msg2, size_t stdTime, size_t allocatorTime)
		{
			std::cout << "\n" << msg1 << stdTime << " ns";
			std::cout << "\n" << msg2 << allocatorTime << " ns";
			std::cout << "\n" << " % time required : " << 100.0 * double(allocatorTime) / stdTime << " %";
			std::cout << "\n" << " improvement : " << double(stdTime) / allocatorTime << "x";
			std::cout << "\n";
		};

		size_t stdTime = 0;
		size_t allocatorTime = 0;
		string stlMsg{ " - Default STL Allocator : " };
		string allocatorMsg{ " - Memory Pool Allocator : " };
		std::cout << std::fixed;
		std::cout << std::setprecision(2);
		std::cout.imbue(std::locale(""));
		//--------------------
		{
			Timer t;
			std::forward_list<DataType> pushFrontForwardListTestStl;
			PushFrontTest{}(pushFrontForwardListTestStl, iterations, repeat);
			stdTime = t.getDurationTillNowInNanoSeconds();
		}
		{
			Timer t;
			std::forward_list<DataType, Allocator> pushFrontForwardListTestFast;
			PushFrontTest{}(pushFrontForwardListTestFast, iterations, repeat);
			allocatorTime = t.getDurationTillNowInNanoSeconds();
		}
		compareResults("ForwardList PushFront" + stlMsg, "ForwardList PushFront" + allocatorMsg, stdTime, allocatorTime);
		//--------------------
		{
			Timer t;
			std::list<DataType> pushFrontListTestStl;
			PushFrontTest{}(pushFrontListTestStl, iterations, repeat);
			stdTime = t.getDurationTillNowInNanoSeconds();
		}
		{
			Timer t;
			std::list<DataType, Allocator> pushFrontListTestFast;
			PushFrontTest{}(pushFrontListTestFast, iterations, repeat);
			allocatorTime = t.getDurationTillNowInNanoSeconds();
		}
		compareResults("List PushFront" + stlMsg, "List PushFront" + allocatorMsg, stdTime, allocatorTime);
		//--------------------
		{
			Timer t;
			std::list<DataType> pushBackListTestStl;
			PushBackTest{}(pushBackListTestStl, iterations, repeat);
			stdTime = t.getDurationTillNowInNanoSeconds();
		}
		{
			Timer t;
			std::list<DataType, Allocator> pushBackListTestFast;
			PushBackTest{}(pushBackListTestFast, iterations, repeat);
			allocatorTime = t.getDurationTillNowInNanoSeconds();
		}
		compareResults("List PushBack" + stlMsg, "List PushBack" + allocatorMsg, stdTime, allocatorTime);
		//--------------------
		{
			Timer t;
			std::map<DataType, DataType, std::less<DataType>> mapTestStl;
			MapTest{}(mapTestStl, iterations, repeat);
			stdTime = t.getDurationTillNowInNanoSeconds();
		}
		{
			Timer t;
			std::map<DataType, DataType, std::less<DataType>, Allocator> mapTestFast;
			MapTest{}(mapTestFast, iterations, repeat);
			allocatorTime = t.getDurationTillNowInNanoSeconds();
		}
		compareResults("Map" + stlMsg, "Map" + allocatorMsg, stdTime, allocatorTime);
		//--------------------
		{
			Timer t;
			std::set<DataType, std::less<DataType>> setTestStl;
			SetTest{}(setTestStl, iterations, repeat);
			stdTime = t.getDurationTillNowInNanoSeconds();
		}
		{
			Timer t;
			std::set<DataType, std::less<DataType>, Allocator> setTestFast;
			SetTest{}(setTestFast, iterations, repeat);
			allocatorTime = t.getDurationTillNowInNanoSeconds();
		}
		compareResults("Set" + stlMsg, "Set" + allocatorMsg, stdTime, allocatorTime);
		//--------------------
	}

	MM_DECLARE_FLAG(Memory_HeapAllocator_v2);

	MM_UNIT_TEST(Memory_HeapAllocator_v2_unit_test_1, Memory_HeapAllocator_v2)
	{
		/*
		1,000 int = 4 KB
		1,000,000 int = 4 MB
		1,000,000,000 int = 4 GB
		*/

		const int repeat = 2; 
		const int iterations = 10'000'000; //number of integers inserted which requires total 40 MB data

		const std::size_t growSize = 1'000'000; //number of integers = 4 MB
		typedef HeapAllocator_v2<int, growSize> Allocator;
		
		Memory_HeapAllocator_v2_unit_test<int, Allocator>(iterations, repeat);

		/*
		// Note: This allocator does not work for vector

		const std::size_t N = growSize;
		//arena_v1<N> a;
		std::vector<int, HeapAllocator_v2<int, N>> v{};
		v.reserve(100);
		for (int i = 0; i < 100; ++i)
			v.push_back(i);
		std::vector<int, HeapAllocator_v2<int, N>> v2 = std::move(v);
		v = v2;
		*/
	}
}
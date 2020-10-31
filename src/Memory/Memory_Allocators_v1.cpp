#include <iostream>
#include <chrono>
#include <random>
#include <set>
#include <map>
#include <list>
#include <forward_list>

#include "Memory/Memory_Allocators_v1.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	class PushFrontTest
	{
	public:
		template <typename Container>
		void operator()(Container& container, size_t iterations)
		{
			int size = 0;
			while (size < iterations)
				container.push_front(size++);

			for (; size > iterations; size--)
				container.pop_front();
			//container.clear();
		}
	};

	class PushBackTest
	{
	public:
		template <typename Container>
		void operator()(Container& container, size_t iterations)
		{
			int size = 0;
			while (size < iterations)
				container.push_back(size++);

			for (; size > iterations; size--)
				container.pop_back();
			//container.clear();
		}
	};

	class MapTest
	{
	public:
		template <typename Container>
		void operator()(Container& container, size_t iterations)
		{
			int size = 0;
			while (size < iterations)
				container.insert(std::pair<char, int>(size++, size));

			while (size > iterations)
				container.erase(--size);
			//container.clear();
		}
	};

	class SetTest
	{
	public:
		template <typename Container>
		void operator()(Container& container, size_t iterations)
		{
			int size = 0;
			while (size < iterations)
				container.insert(size++);

			while (size > iterations)
				container.erase(--size);
			//container.clear();
		}
	};

	template <typename Container, typename Fun>
	size_t executeAndMeasure(const string& msg, Container &container, Fun f, size_t iterations, size_t repeat)
	{
		auto from = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < repeat; ++i)
			f(container, iterations);
		auto to = std::chrono::high_resolution_clock::now();
		size_t retVal = std::chrono::duration_cast<std::chrono::nanoseconds>(to - from).count();
		std::cout << msg << retVal << " ns" << std::endl;
		return retVal;
	}

	void compareResults_v1(size_t stdTime, size_t allocatorTime)
	{
		std::cout << " % time required : " << 100.0 * double(allocatorTime) / stdTime << " %" << std::endl;
		std::cout << " improvement : " << double(stdTime) / allocatorTime << "x" << std::endl;
		std::cout << std::endl;
	}

	template<typename DataType, typename Allocator>
	void Memory_Allocators_v1_unit_test(size_t iterations, size_t repeat)
	{
		size_t stdTime, allocatorTime;
		string stlMsg{ " - Default STL Allocator : " };
		string allocatorMsg{ " - Memory Pool Allocator : " };
		std::cout << std::fixed;
		std::cout << std::setprecision(2);
		std::cout.imbue(std::locale(""));
		//--------------------
		{
			std::forward_list<int> pushFrontForwardListTestStl;
			stdTime = executeAndMeasure("ForwardList PushFront" + stlMsg, pushFrontForwardListTestStl, PushFrontTest{}, iterations, repeat);
		}
		{
			std::forward_list<int, Allocator> pushFrontForwardListTestFast;
			allocatorTime = executeAndMeasure("ForwardList PushFront" + allocatorMsg, pushFrontForwardListTestFast, PushFrontTest{}, iterations, repeat);
		}
		compareResults_v1(stdTime, allocatorTime);
		//--------------------
		{
			std::list<int> pushFrontListTestStl;
			stdTime = executeAndMeasure("List PushFront" + stlMsg, pushFrontListTestStl, PushFrontTest{}, iterations, repeat);
		}
		{
			std::list<int, Allocator> pushFrontListTestFast;
			allocatorTime = executeAndMeasure("List PushFront" + allocatorMsg, pushFrontListTestFast, PushFrontTest{}, iterations, repeat);
		}
		compareResults_v1(stdTime, allocatorTime);
		//--------------------
		{
			std::list<int> pushBackListTestStl;
			stdTime = executeAndMeasure("List PushBack" + stlMsg, pushBackListTestStl, PushBackTest{}, iterations, repeat);
		}
		{
			std::list<int, Allocator> pushBackListTestFast;
			allocatorTime = executeAndMeasure("List PushBack" + allocatorMsg, pushBackListTestFast, PushBackTest{}, iterations, repeat);
		}
		compareResults_v1(stdTime, allocatorTime);
		//--------------------
		{
			std::map<int, int, std::less<int>> mapTestStl;
			stdTime = executeAndMeasure("Map" + stlMsg, mapTestStl, MapTest{}, iterations, repeat);
		}
		{
			std::map<int, int, std::less<int>, Allocator> mapTestFast;
			allocatorTime = executeAndMeasure("Map" + allocatorMsg, mapTestFast, MapTest{}, iterations, repeat);
		}
		compareResults_v1(stdTime, allocatorTime);
		//--------------------
		{
			std::set<int, std::less<int>> setTestStl;
			stdTime = executeAndMeasure("Set" + stlMsg, setTestStl, SetTest{}, iterations, repeat);
		}
		{
			std::set<int, std::less<int>, Allocator> setTestFast;
			allocatorTime = executeAndMeasure("Set" + allocatorMsg, setTestFast, SetTest{}, iterations, repeat);
		}
		compareResults_v1(stdTime, allocatorTime);
		//--------------------
	}

	MM_DECLARE_FLAG(Memory_Allocators_v1_unit_test);

	MM_UNIT_TEST(Memory_Allocators_v1_unit_test_1, Memory_Allocators_v1_unit_test)
	{
		/*
		1,000 int = 4 KB
		1,000,000 int = 4 MB
		1,000,000,000 int = 4 GB
		*/

		const int repeat = 2; 
		const int iterations = 10'000'000; //number of integers inserted which requires total 40 MB data

		const std::size_t growSize = 1'000'000; //number of intergers = 4 MB
		typedef Allocator_v1<int, growSize> Allocator;
		
		Memory_Allocators_v1_unit_test<int, Allocator>(iterations, repeat);
	}
}
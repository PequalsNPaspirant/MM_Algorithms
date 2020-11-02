#include <iostream>
#include <chrono>
#include <random>
#include <set>
#include <map>
#include <list>
#include <forward_list>
#include <vector>

#include "Memory/Memory_StackAllocator_v1.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Timer/Timer_Timer.h"
#include "Utils/Utils_MM_Assert.h"

namespace mm {

	class PushFrontTest
	{
	public:
		template <typename Container>
		void operator()(Container& container, size_t iterations)
		{
			for (int size = 0; size < iterations; ++size)
				container.push_front(size);

			for (int size = 0; size < iterations; ++size)
				container.pop_front();
			//container.clear();
			MyAssert::myRunTimeAssert(container.empty());
		}
	};

	class PushBackTest
	{
	public:
		template <typename Container>
		void operator()(Container& container, size_t iterations)
		{
			for (int size = 0; size < iterations; ++size)
				container.push_back(size);

			for (int size = 0; size < iterations; ++size)
				container.pop_back();
			//container.clear();
			MyAssert::myRunTimeAssert(container.empty());
		}
	};

	class MapTest
	{
	public:
		template <typename Container>
		void operator()(Container& container, size_t iterations)
		{
			for (int size = 0; size < iterations; ++size)
				container.insert(std::pair<char, int>(size, size));

			for (int size = 0; size < iterations; ++size)
				container.erase(size);
			//container.clear();
			MyAssert::myRunTimeAssert(container.empty());
		}
	};

	class SetTest
	{
	public:
		template <typename Container>
		void operator()(Container& container, size_t iterations)
		{
			for (int size = 0; size < iterations; ++size)
				container.insert(size);

			for (int size = 0; size < iterations; ++size)
				container.erase(size);
			//container.clear();
			MyAssert::myRunTimeAssert(container.empty());
		}
	};

	template <typename Container, typename Fun>
	size_t executeAndMeasure(const string& msg, Container &container, Fun f, size_t iterations, size_t repeat)
	{
		auto from = std::chrono::high_resolution_clock::now();
		for(int i = 0; i < repeat; ++i)
			f(container, iterations);
		auto to = std::chrono::high_resolution_clock::now();
		size_t retVal = std::chrono::duration_cast<std::chrono::nanoseconds>(to - from).count();
		std::cout << msg << retVal << " ns" << std::endl;
		return retVal;
	}


	template<typename DataType, typename Allocator, size_t initialSize>
	void Memory_StackAllocator_v1_unit_test(size_t iterations, size_t repeat)
	{
		auto compareResults = [](size_t stdTime, size_t allocatorTime)
		{
			std::cout << " % time required : " << 100.0 * double(allocatorTime) / stdTime << " %" << std::endl;
			std::cout << " improvement : " << double(stdTime) / allocatorTime << "x" << std::endl;
			std::cout << std::endl;
		};

		arena_v1<initialSize> a;
		size_t stdTime, allocatorTime;
		string stlMsg{ " - Default STL Allocator : " };
		string allocatorMsg{ " - Memory Pool Allocator : " };
		std::cout << std::fixed;
		std::cout << std::setprecision(2);
		std::cout.imbue(std::locale(""));
		//--------------------
		{
			std::forward_list<DataType> pushFrontForwardListTestStl;
			stdTime = executeAndMeasure("ForwardList PushFront" + stlMsg, pushFrontForwardListTestStl, PushFrontTest{}, iterations, repeat);
		}
		{
			std::forward_list<DataType, Allocator> pushFrontForwardListTestFast{ Allocator{a} };
			allocatorTime = executeAndMeasure("ForwardList PushFront" + allocatorMsg, pushFrontForwardListTestFast, PushFrontTest{}, iterations, repeat);
		}
		compareResults(stdTime, allocatorTime);
		//--------------------
		{
			std::list<DataType> pushFrontListTestStl;
			stdTime = executeAndMeasure("List PushFront" + stlMsg, pushFrontListTestStl, PushFrontTest{}, iterations, repeat);
		}
		{
			std::list<DataType, Allocator> pushFrontListTestFast{ Allocator{a} };
			allocatorTime = executeAndMeasure("List PushFront" + allocatorMsg, pushFrontListTestFast, PushFrontTest{}, iterations, repeat);
		}
		compareResults(stdTime, allocatorTime);
		//--------------------
		{
			std::list<DataType> pushBackListTestStl;
			stdTime = executeAndMeasure("List PushBack" + stlMsg, pushBackListTestStl, PushBackTest{}, iterations, repeat);
		}
		{
			std::list<DataType, Allocator> pushBackListTestFast{ Allocator{a} };
			allocatorTime = executeAndMeasure("List PushBack" + allocatorMsg, pushBackListTestFast, PushBackTest{}, iterations, repeat);
		}
		compareResults(stdTime, allocatorTime);
		//--------------------
		{
			std::map<DataType, DataType, std::less<DataType>> mapTestStl;
			stdTime = executeAndMeasure("Map" + stlMsg, mapTestStl, MapTest{}, iterations, repeat);
		}
		{
			std::map<DataType, DataType, std::less<DataType>, Allocator> mapTestFast{ Allocator{a} };
			allocatorTime = executeAndMeasure("Map" + allocatorMsg, mapTestFast, MapTest{}, iterations, repeat);
		}
		compareResults(stdTime, allocatorTime);
		//--------------------
		{
			std::set<DataType, std::less<DataType>> setTestStl;
			stdTime = executeAndMeasure("Set" + stlMsg, setTestStl, SetTest{}, iterations, repeat);
		}
		{
			std::set<DataType, std::less<DataType>, Allocator> setTestFast{ Allocator{a} };
			allocatorTime = executeAndMeasure("Set" + allocatorMsg, setTestFast, SetTest{}, iterations, repeat);
		}
		compareResults(stdTime, allocatorTime);
		//--------------------
	}

	MM_DECLARE_FLAG(Memory_StackAllocator_v1);

	//template <class T, std::size_t N> using A = stack_allocator_v1<T, N>;
	template <class T, std::size_t N> using Vector = std::vector<T, StackAllocator_v1<T, N>>;

	MM_UNIT_TEST(Memory_StackAllocator_v1_unit_test_1, Memory_StackAllocator_v1)
	{
		/*
		1,000 int = 4 KB
		1,000,000 int = 4 MB
		1,000,000,000 int = 4 GB
		*/

		const int repeat = 2;
		const int iterations  = 10'000'000; //number of integers inserted which requires total 40 MB data

		constexpr const int initialSize = 1'000'000'000; //bytes = 1 GB
		typedef StackAllocator_v1<int, initialSize> Allocator;
		
		Memory_StackAllocator_v1_unit_test<int, Allocator, initialSize>(iterations, repeat);

		const std::size_t N = 1024;
		arena_v1<N> a;
		Vector<int, N> v{ StackAllocator_v1<int, N>(a) };
		v.reserve(100);
		for (int i = 0; i < 100; ++i)
			v.push_back(i);
		Vector<int, N> v2 = std::move(v);
		v = v2;
	}


}
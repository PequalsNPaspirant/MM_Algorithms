#include <iostream>
#include <chrono>
#include <random>
#include <set>
#include <map>
#include <list>
#include <forward_list>
#include <unordered_map>
#include <unordered_set>

#include "Memory/Memory_HeapAllocator_v2.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"
#include "Timer/Timer_Timer.h"
#include "Utils/Utils_MM_Assert.h"

namespace mm {

	class PushFrontTest
	{
	public:
		template <typename Container>
		void operator()(Container& container, size_t iterations, size_t repeat)
		{
			for (int i = 0; i < repeat; ++i)
			{
				for (int size = 0; size < iterations; ++size)
					container.push_front(size);

				for (int size = 0; size < iterations; ++size)
					container.pop_front();
				//container.clear();
				MM_Assert::mmRunTimeAssert(container.empty());
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
				for (int size = 0; size < iterations; ++size)
					container.push_back(size);

				for (int size = 0; size < iterations; ++size)
					container.pop_back();
				//container.clear();
				MM_Assert::mmRunTimeAssert(container.empty());
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
				for (int size = 0; size < iterations; ++size)
					container.insert(std::pair<int, int>(size, size));

				for (int size = 0; size < iterations; ++size)
					container.erase(size);
				//container.clear();
				MM_Assert::mmRunTimeAssert(container.empty());
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
				for (int size = 0; size < iterations; ++size)
					container.insert(size);

				for (int size = 0; size < iterations; ++size)
					container.erase(size);
				//container.clear();
				MM_Assert::mmRunTimeAssert(container.empty());
			}
		}
	};

	template<typename DataType, typename Allocator>
	void Memory_HeapAllocator_v2_unit_test(size_t iterations, size_t repeat)
	{
		auto compareResults = [](const string& msg1, const string& msg2, size_t stdTime, size_t allocatorTime)
		{
			std::cout << "\n" << msg1 << std::setw(15) << std::right << stdTime << " ns (100.00 %)";
			std::cout << "\n" << msg2 << std::setw(15) << std::right << allocatorTime << " ns (" << 100.0 * double(allocatorTime) / stdTime << " %)";
			std::cout << "\n" << "Performance improvement factor : " << double(stdTime) / allocatorTime << "x";
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
		//{
		//	Timer t;
		//	std::unordered_map<DataType, DataType, hash<DataType>, equal_to<DataType>> mapTestStl;
		//	MapTest{}(mapTestStl, iterations, repeat);
		//	stdTime = t.getDurationTillNowInNanoSeconds();
		//}
		//{
		//	Timer t;
		//	std::unordered_map<DataType, DataType, hash<DataType>, equal_to<DataType>, Allocator> mapTestFast;
		//	MapTest{}(mapTestFast, iterations, repeat);
		//	allocatorTime = t.getDurationTillNowInNanoSeconds();
		//}
		//compareResults("Unordered Map" + stlMsg, "Unordered Map" + allocatorMsg, stdTime, allocatorTime);
		////--------------------
		//{
		//	Timer t;
		//	std::unordered_set<DataType, hash<DataType>, equal_to<DataType>> setTestStl;
		//	SetTest{}(setTestStl, iterations, repeat);
		//	stdTime = t.getDurationTillNowInNanoSeconds();
		//}
		//{
		//	Timer t;
		//	std::unordered_set<DataType, hash<DataType>, equal_to<DataType>, Allocator> setTestFast;
		//	SetTest{}(setTestFast, iterations, repeat);
		//	allocatorTime = t.getDurationTillNowInNanoSeconds();
		//}
		//compareResults("Unordered Set" + stlMsg, "Unordered Set" + allocatorMsg, stdTime, allocatorTime);
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

		const int repeat = 1'000;
		const int iterations = 10'000; //number of integers inserted which requires total 400 KB data
		//const int iterations = 1;
		//list, map and set requires one extra element/memory allocation than number of elements to be inserted
		const std::size_t bufferSize = 10'001; //number of integers = 40 MB 
		typedef HeapAllocator_v2<int, bufferSize> Allocator;
		
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


/*
Size of node in forward list = sizeof(int) + 12 bytes = 16 bytes =  4 times sizeof(int)
Size of node in         list = sizeof(int) + 20 bytes = 24 bytes =  6 times sizeof(int)
Size of node in          map = sizeof(int) + 36 bytes = 40 bytes = 10 times sizeof(int)
Size of node in          set = sizeof(int) + 28 bytes = 32 bytes =  8 times sizeof(int)

HeapAllocator_v2: Allocate for type: struct std::_Flist_node<int,void * __ptr64> Size: 16 Num Elements: 1
MemoryPool_v2: Allocate for type: struct std::_Flist_node<int,void * __ptr64> Size: 16
HeapAllocator_v2: Deallocate for type: struct std::_Flist_node<int,void * __ptr64> Size: 16 Num Elements: 1
MemoryPool_v2: Deallocate for type: struct std::_Flist_node<int,void * __ptr64> Size: 16
ForwardList PushFront - Default STL Allocator :           1,800 ns (100.00 %)
ForwardList PushFront - Memory Pool Allocator :      51,295,200 ns (2,849,733.33 %)
Improvement in speed : 0.00x

HeapAllocator_v2: Allocate for type: struct std::_List_node<int,void * __ptr64> Size: 24 Num Elements: 1
MemoryPool_v2: Allocate for type: struct std::_List_node<int,void * __ptr64> Size: 24
HeapAllocator_v2: Allocate for type: struct std::_List_node<int,void * __ptr64> Size: 24 Num Elements: 1
MemoryPool_v2: Allocate for type: struct std::_List_node<int,void * __ptr64> Size: 24
HeapAllocator_v2: Copy Ctr
HeapAllocator_v2: Deallocate for type: struct std::_List_node<int,void * __ptr64> Size: 24 Num Elements: 1
MemoryPool_v2: Deallocate for type: struct std::_List_node<int,void * __ptr64> Size: 24
HeapAllocator_v2: Copy Ctr
HeapAllocator_v2: Deallocate for type: struct std::_List_node<int,void * __ptr64> Size: 24 Num Elements: 1
MemoryPool_v2: Deallocate for type: struct std::_List_node<int,void * __ptr64> Size: 24
List PushFront - Default STL Allocator :           1,000 ns (100.00 %)
List PushFront - Memory Pool Allocator :      13,895,300 ns (1,389,530.00 %)
Improvement in speed : 0.00x

HeapAllocator_v2: Allocate for type: struct std::_List_node<int,void * __ptr64> Size: 24 Num Elements: 1
MemoryPool_v2: Allocate for type: struct std::_List_node<int,void * __ptr64> Size: 24
HeapAllocator_v2: Allocate for type: struct std::_List_node<int,void * __ptr64> Size: 24 Num Elements: 1
MemoryPool_v2: Allocate for type: struct std::_List_node<int,void * __ptr64> Size: 24
HeapAllocator_v2: Copy Ctr
HeapAllocator_v2: Deallocate for type: struct std::_List_node<int,void * __ptr64> Size: 24 Num Elements: 1
MemoryPool_v2: Deallocate for type: struct std::_List_node<int,void * __ptr64> Size: 24
HeapAllocator_v2: Copy Ctr
HeapAllocator_v2: Deallocate for type: struct std::_List_node<int,void * __ptr64> Size: 24 Num Elements: 1
MemoryPool_v2: Deallocate for type: struct std::_List_node<int,void * __ptr64> Size: 24
List PushBack - Default STL Allocator :           1,000 ns (100.00 %)
List PushBack - Memory Pool Allocator :      16,086,500 ns (1,608,650.00 %)
Improvement in speed : 0.00x

HeapAllocator_v2: Allocate for type: struct std::_Tree_node<struct std::pair<int const ,int>,void * __ptr64> Size: 40 Num Elements: 1
MemoryPool_v2: Allocate for type: struct std::_Tree_node<struct std::pair<int const ,int>,void * __ptr64> Size: 40
HeapAllocator_v2: Allocate for type: struct std::_Tree_node<struct std::pair<int const ,int>,void * __ptr64> Size: 40 Num Elements: 1
MemoryPool_v2: Allocate for type: struct std::_Tree_node<struct std::pair<int const ,int>,void * __ptr64> Size: 40
HeapAllocator_v2: Copy Ctr
HeapAllocator_v2: Deallocate for type: struct std::_Tree_node<struct std::pair<int const ,int>,void * __ptr64> Size: 40 Num Elements: 1
MemoryPool_v2: Deallocate for type: struct std::_Tree_node<struct std::pair<int const ,int>,void * __ptr64> Size: 40
HeapAllocator_v2: Copy Ctr
HeapAllocator_v2: Deallocate for type: struct std::_Tree_node<struct std::pair<int const ,int>,void * __ptr64> Size: 40 Num Elements: 1
MemoryPool_v2: Deallocate for type: struct std::_Tree_node<struct std::pair<int const ,int>,void * __ptr64> Size: 40
Map - Default STL Allocator :          16,000 ns (100.00 %)
Map - Memory Pool Allocator :      30,073,100 ns (187,956.88 %)
Improvement in speed : 0.00x

HeapAllocator_v2: Allocate for type: struct std::_Tree_node<int,void * __ptr64> Size: 32 Num Elements: 1
MemoryPool_v2: Allocate for type: struct std::_Tree_node<int,void * __ptr64> Size: 32
HeapAllocator_v2: Allocate for type: struct std::_Tree_node<int,void * __ptr64> Size: 32 Num Elements: 1
MemoryPool_v2: Allocate for type: struct std::_Tree_node<int,void * __ptr64> Size: 32
HeapAllocator_v2: Copy Ctr
HeapAllocator_v2: Deallocate for type: struct std::_Tree_node<int,void * __ptr64> Size: 32 Num Elements: 1
MemoryPool_v2: Deallocate for type: struct std::_Tree_node<int,void * __ptr64> Size: 32
HeapAllocator_v2: Copy Ctr
HeapAllocator_v2: Deallocate for type: struct std::_Tree_node<int,void * __ptr64> Size: 32 Num Elements: 1
MemoryPool_v2: Deallocate for type: struct std::_Tree_node<int,void * __ptr64> Size: 32
Set - Default STL Allocator :           1,100 ns (100.00 %)
Set - Memory Pool Allocator :      13,823,000 ns (1,256,636.36 %)
Improvement in speed : 0.00x


Some tests results:

const int repeat = 1'000;
const int iterations = 100'000; //number of integers inserted which requires total 400 KB data
//const int iterations = 1;
const std::size_t bufferSize = 10'000'000;

ForwardList PushFront - Default STL Allocator :   9,744,537,700 ns (100.00 %)
ForwardList PushFront - Memory Pool Allocator :     808,119,800 ns (8.29 %)
Improvement in speed : 12.06x

List PushFront - Default STL Allocator :   7,214,819,300 ns (100.00 %)
List PushFront - Memory Pool Allocator :     847,056,400 ns (11.74 %)
Improvement in speed : 8.52x

List PushBack - Default STL Allocator :   6,276,746,300 ns (100.00 %)
List PushBack - Memory Pool Allocator :     980,330,600 ns (15.62 %)
Improvement in speed : 6.40x

Map - Default STL Allocator :  10,477,372,700 ns (100.00 %)
Map - Memory Pool Allocator :   5,556,992,600 ns (53.04 %)
Improvement in speed : 1.89x

Set - Default STL Allocator :   7,063,788,600 ns (100.00 %)
Set - Memory Pool Allocator :  14,469,793,100 ns (204.84 %)
Improvement in speed : 0.49x



const int repeat = 10'000;
const int iterations = 10'000; //number of integers inserted which requires total 400 KB data
//const int iterations = 1;
const std::size_t bufferSize = 10'000'000;

ForwardList PushFront - Default STL Allocator :   5,863,354,200 ns (100.00 %)
ForwardList PushFront - Memory Pool Allocator :     640,642,800 ns (10.93 %)
Improvement in speed : 9.15x

List PushFront - Default STL Allocator :   5,856,389,200 ns (100.00 %)
List PushFront - Memory Pool Allocator :     862,438,100 ns (14.73 %)
Improvement in speed : 6.79x

List PushBack - Default STL Allocator :   6,006,079,900 ns (100.00 %)
List PushBack - Memory Pool Allocator :     968,286,000 ns (16.12 %)
Improvement in speed : 6.20x

Map - Default STL Allocator :  10,066,745,300 ns (100.00 %)
Map - Memory Pool Allocator :   5,668,981,700 ns (56.31 %)
Improvement in speed : 1.78x

Set - Default STL Allocator :   5,995,393,600 ns (100.00 %)
Set - Memory Pool Allocator :   6,814,775,600 ns (113.67 %)
Improvement in speed : 0.88x




const int repeat = 1'000;
const int iterations = 10'000; //number of integers inserted which requires total 400 KB data
//const int iterations = 1;
//list, map and set requires one extra element/memory allocation than number of elements to be inserted
const std::size_t bufferSize = 10'001; //number of integers = 40 MB

1st test

ForwardList PushFront - Default STL Allocator :   2,252,207,900 ns (100.00 %)
ForwardList PushFront - Memory Pool Allocator :      62,007,800 ns (2.75 %)
Improvement in speed : 36.32x

List PushFront - Default STL Allocator :   1,009,119,400 ns (100.00 %)
List PushFront - Memory Pool Allocator :     219,315,400 ns (21.73 %)
Improvement in speed : 4.60x

List PushBack - Default STL Allocator :     668,016,600 ns (100.00 %)
List PushBack - Memory Pool Allocator :     213,593,800 ns (31.97 %)
Improvement in speed : 3.13x

Map - Default STL Allocator :   1,920,024,800 ns (100.00 %)
Map - Memory Pool Allocator :   1,375,690,300 ns (71.65 %)
Improvement in speed : 1.40x

Set - Default STL Allocator :   1,843,220,700 ns (100.00 %)
Set - Memory Pool Allocator :   1,536,003,300 ns (83.33 %)
Improvement in speed : 1.20x


2nd test

ForwardList PushFront - Default STL Allocator :   2,259,136,400 ns (100.00 %)
ForwardList PushFront - Memory Pool Allocator :      61,604,400 ns (2.73 %)
Improvement in speed : 36.67x

List PushFront - Default STL Allocator :     891,393,300 ns (100.00 %)
List PushFront - Memory Pool Allocator :     225,721,200 ns (25.32 %)
Improvement in speed : 3.95x

List PushBack - Default STL Allocator :     780,009,100 ns (100.00 %)
List PushBack - Memory Pool Allocator :     218,546,200 ns (28.02 %)
Improvement in speed : 3.57x

Map - Default STL Allocator :   1,880,609,100 ns (100.00 %)
Map - Memory Pool Allocator :   1,300,304,900 ns (69.14 %)
Improvement in speed : 1.45x

Set - Default STL Allocator :   1,945,070,200 ns (100.00 %)
Set - Memory Pool Allocator :   1,530,926,700 ns (78.71 %)
Improvement in speed : 1.27x





const int repeat = 1'000;
const int iterations = 100'000; //number of integers inserted which requires total 400 KB data
//const int iterations = 1;
//list, map and set requires one extra element/memory allocation than number of elements to be inserted
const std::size_t bufferSize = 100'001; //number of integers = 40 MB

ForwardList PushFront - Default STL Allocator :  10,894,181,000 ns (100.00 %)
ForwardList PushFront - Memory Pool Allocator :     644,315,000 ns (5.91 %)
Improvement in speed : 16.91x

List PushFront - Default STL Allocator :   7,374,252,800 ns (100.00 %)
List PushFront - Memory Pool Allocator :   2,303,687,100 ns (31.24 %)
Improvement in speed : 3.20x

List PushBack - Default STL Allocator :   7,451,352,100 ns (100.00 %)
List PushBack - Memory Pool Allocator :   2,271,163,700 ns (30.48 %)
Improvement in speed : 3.28x

Map - Default STL Allocator :  23,172,814,800 ns (100.00 %)
Map - Memory Pool Allocator :  20,804,684,500 ns (89.78 %)
Improvement in speed : 1.11x

Set - Default STL Allocator :  23,789,501,700 ns (100.00 %)
Set - Memory Pool Allocator :  25,826,118,700 ns (108.56 %)
Improvement in speed : 0.92x

*/
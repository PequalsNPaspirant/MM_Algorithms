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


	const int numberOfIterations = 2;

	const std::size_t growSize =  1'000'000; //4 MB
	const int randomRange      = 10'000'000; //requires total 10 * 4 MB
	/*
	         1,000 int = 4 KB
	     1,000,000 int = 4 MB
	 1,000,000,000 int = 4 GB
	*/

	template <typename Container>
	class PerformanceTest
	{
		virtual void testIteration(int newSize) = 0;


	protected:

		Container container;

		std::default_random_engine randomNumberGenerator;
		std::uniform_int_distribution<int> randomNumberDistribution;


	public:

		PerformanceTest() :
			randomNumberGenerator(0),
			randomNumberDistribution(0, randomRange)
		{
		}

		size_t run()
		{
			auto from = std::chrono::high_resolution_clock::now();

			for (int i = 0; i < numberOfIterations; i++)
				//testIteration(randomNumberDistribution(randomNumberGenerator));
				testIteration(randomRange);

			auto to = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::nanoseconds>(to - from).count();
		}
	};

	template <typename Container>
	class PushFrontTest : public PerformanceTest<Container>
	{
		virtual void testIteration(int newSize)
		{
			int size = 0;

			while (size < newSize)
				this->container.push_front(size++);

			for (; size > newSize; size--)
				this->container.pop_front();
			//container.clear();
		}
	};

	template <typename Container>
	class PushBackTest : public PerformanceTest<Container>
	{
		virtual void testIteration(int newSize)
		{
			int size = 0;

			while (size < newSize)
				this->container.push_back(size++);

			for (; size > newSize; size--)
				this->container.pop_back();
			//container.clear();
		}
	};

	template <typename Container>
	class MapTest : public PerformanceTest<Container>
	{
		virtual void testIteration(int newSize)
		{
			int size = 0;

			while (size < newSize)
				this->container.insert(std::pair<char, int>(size++, size));

			//while (size > newSize)
			//	this->container.erase(--size);
			container.clear();
		}
	};

	template <typename Container>
	class SetTest : public PerformanceTest<Container>
	{
		virtual void testIteration(int newSize)
		{
			int size = 0;

			while (size < newSize)
				this->container.insert(size++);

			//while (size > newSize)
			//	this->container.erase(--size);
			container.clear();
		}
	};

	template <typename StlContainer, typename FastContainer>
	void printTestStatus(const char *name, StlContainer &stlContainer, FastContainer &fastContainer)
	{
		std::cout << std::fixed;
		std::cout << std::setprecision(2);
		std::cout.imbue(std::locale(""));
		size_t stdTime = stlContainer.run();
		size_t newTime = fastContainer.run();
		std::cout << name << " - Default STL Allocator : " << stdTime << " ns" << std::endl;
		std::cout << name << " - Memory Pool Allocator : " << newTime << " ns" << std::endl;
		std::cout << " % improvement : " << 100.0 * (stdTime - newTime) / double(stdTime) << " %" << std::endl;
		std::cout << "   improvement : " << double(stdTime) / newTime << "x" << std::endl;
		std::cout << std::endl;
	}

	
	void Memory_Allocators_v1_unit_test()
	{
		typedef int DataType;
		typedef Allocator_v1<DataType, growSize> MemoryPoolAllocator;

		std::cout << "Allocator performance measurement example" << std::endl;
		std::cout << "Version: 1.0" << std::endl << std::endl;
		{
			PushFrontTest<std::forward_list<DataType>> pushFrontForwardListTestStl;
			PushFrontTest<std::forward_list<DataType, MemoryPoolAllocator>> pushFrontForwardListTestFast;
			printTestStatus("ForwardList PushFront", pushFrontForwardListTestStl, pushFrontForwardListTestFast);
		}

		{
			PushFrontTest<std::list<DataType>> pushFrontListTestStl;
			PushFrontTest<std::list<DataType, MemoryPoolAllocator>> pushFrontListTestFast;
			printTestStatus("List PushFront", pushFrontListTestStl, pushFrontListTestFast);
		}

		{
			PushBackTest<std::list<DataType>> pushBackListTestStl;
			PushBackTest<std::list<DataType, MemoryPoolAllocator>> pushBackListTestFast;
			printTestStatus("List PushBack", pushBackListTestStl, pushBackListTestFast);
		}

		{
			MapTest<std::map<DataType, DataType, std::less<DataType>>> mapTestStl;
			MapTest<std::map<DataType, DataType, std::less<DataType>, MemoryPoolAllocator>> mapTestFast;
			printTestStatus("Map", mapTestStl, mapTestFast);
		}

		{
			SetTest<std::set<DataType, std::less<DataType>>> setTestStl;
			SetTest<std::set<DataType, std::less<DataType>, MemoryPoolAllocator>> setTestFast;
			printTestStatus("Set", setTestStl, setTestFast);
		}

	}

	MM_DECLARE_FLAG(Memory_Allocators_v1_unit_test);

	MM_UNIT_TEST(Memory_Allocators_v1_unit_test_1, Memory_Allocators_v1_unit_test)
	{
		Memory_Allocators_v1_unit_test();
	}
}
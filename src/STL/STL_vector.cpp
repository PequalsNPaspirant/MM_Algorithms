#include <iostream>
#include <vector>
using namespace std;

#include "STL/STL_vector.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {
	
	void vector_changeInCapacity()
	{
		struct SizeCapacity
		{
			size_t size;
			size_t capacity;
			size_t max_size;
		};

		// The capacity is increased by 50% while doing push_back a new element if size() == capacity()
		vector<SizeCapacity> sizeCapacityData = 
		{
			{ 0, 0, 4611686018427387903 },
			{ 1, 1, 4611686018427387903 },
			{ 2, 2, 4611686018427387903 },
			{ 3, 3, 4611686018427387903 },
			{ 4, 4, 4611686018427387903 },
			{ 5, 6, 4611686018427387903 },
			{ 7, 9, 4611686018427387903 },
			{ 10, 13, 4611686018427387903 },
			{ 14, 19, 4611686018427387903 },
			{ 20, 28, 4611686018427387903 },
			{ 29, 42, 4611686018427387903 },
			{ 43, 63, 4611686018427387903 },
			{ 64, 94, 4611686018427387903 },
			{ 95, 141, 4611686018427387903 },
			{ 142, 211, 4611686018427387903 },
			{ 212, 316, 4611686018427387903 },
			{ 317, 474, 4611686018427387903 },
			{ 475, 711, 4611686018427387903 },
			{ 712, 1066, 4611686018427387903 },
			{ 1067, 1599, 4611686018427387903 }
		};

		vector<int> basicVector;
		auto actualSize = basicVector.size();
		auto actualCapacity = basicVector.capacity();
		auto actual_max_size = basicVector.max_size();
		MM_EXPECT_TRUE(actualSize == sizeCapacityData[0].size && 
			actualCapacity == sizeCapacityData[0].capacity && 
			actual_max_size == sizeCapacityData[0].max_size,
			actualSize, sizeCapacityData[0].size, actualCapacity, sizeCapacityData[0].capacity, actual_max_size, sizeCapacityData[0].max_size);

		int counter = 0;
		for (int n = 1; n < sizeCapacityData.size(); ++n)
		{
			for (int i = actualSize; i <= actualCapacity; ++i)
				basicVector.push_back(i);

			actualSize = basicVector.size();
			actualCapacity = basicVector.capacity();
			actual_max_size = basicVector.max_size();
			MM_EXPECT_TRUE(actualSize == sizeCapacityData[n].size && 
				actualCapacity == sizeCapacityData[n].capacity && 
				actual_max_size == sizeCapacityData[n].max_size,
				actualSize, sizeCapacityData[n].size, actualCapacity, sizeCapacityData[n].capacity, actual_max_size, sizeCapacityData[n].max_size);
		} 

	}

	void vector_callsToDefaultConstructor()
	{
		struct myStruct
		{
			//myStruct() {} //No default constructor available
			myStruct(int n)
				: num_(n)
			{}

			int num_;
		};

		vector<myStruct> vect_1;
		//vect_1.resize(10); //needs default constructor

		//vector<myStruct> vect_2(10); //needs default constructor
	}





	MM_DECLARE_FLAG(STL_vectorUnitTest);

	MM_UNIT_TEST(STL_vectorUnitTest_sanity, STL_vectorUnitTest)
	{
		vector_changeInCapacity();
		vector_callsToDefaultConstructor();
	}

}
//Goal
//Implement binary search

#pragma once

#include <iostream>
#include <vector>
using namespace std;

namespace mm {

	template<typename ObjectType, typename ContainerType = std::vector<ObjectType>>
	//ObjectType should have comparison operators == and < defined for this Object type
	//ContainerType should have operator[] defined to access element at any index ...preferably in O(1)
	// and size() member function to give total number of elements
	int binarySearch_1(
		const ContainerType& container, 
		const ObjectType& target, 
		int rangeStart = 0,
		int rangeEnd = -1)
	{
		int start = rangeStart;
		int end = rangeEnd != -1 ? rangeEnd : container.size() - 1;

		while (start <= end)
		{
			int mid = start + (end - start) / 2; //Do (start + end)/2 to avoid overflow
			if (target == container[mid])
				return mid;
			if (target < container[mid])
				end = mid - 1;
			else
				start = mid + 1;
		}

		return -1; // target is not present in container within given range
	}

}

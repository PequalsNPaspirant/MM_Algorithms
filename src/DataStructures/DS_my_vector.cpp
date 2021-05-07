//Goal
//Implement vector

#include <iostream>
#include <map>
using namespace std;

namespace mm {

	//Store the pointer to next memory chunk in the last element of vector
	//need to use reinterpret cast to store int* as int and convert it back

	//Search element => O(n / capacity)

	class MyVectorGrowingConstantTime
	{
	public:
		MyVectorGrowingConstantTime(int size = 10)
			: m_capacity(size)
		{
			m_data = new size_t[m_capacity];
		}
		void push_back(int n)
		{

		}
		size_t getAt(int index)
		{
			size_t* location = m_data;
			int maxIndex = m_capacity - 1;
			while (index > maxIndex)
				location = reinterpret_cast<size_t*>(location[maxIndex]);

			return location[index % m_capacity];
		}

	private:
		size_t* m_data;
		const int m_capacity;
	};

	//--------------------------------------------

	//Search element => O(n / capacity)

	class List
	{
	private:
		struct Node
		{
			int* m_data;
			Node* next;
		};
	};

	class MyVectorGrowingConstantTime2
	{
	public:

	private:
		List m_locations;
		const int m_capacity;
	};


	//--------------------------------------------

	//Search element => O(log (n / capacity)) - base 2

	class MyVectorGrowingConstantTime3
	{
	public:

	private:
		//Map stores start index and the location where this index starts
		//e.g map.first = 50, map.second = 0x09846352 => 
		//this location has array which stores elements from 50 to 50+capacity 
		map<int, int*> m_locations;
		const int m_capacity;
	};

	//--------------------------------------------

	//Search element => O(1) 
	//but does not grow constant time

	class MyVectorGrowingConstantTime4
	{
	public:

		int getAt(int index)
		{
			return m_locations[index / m_capacity][index % m_capacity];
		}

	private:
		//Create the array to store locations of continuous memory chunks 
		int** m_locations;
		const int m_capacity;
	};

}
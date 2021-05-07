//Goal: 1
//Demonstrate use of various STL containers

//target containers - vector, list, map, set, ...
//how to contruct from available raw array data
//how to get size
//how to loop/iterate through all
//how to insert at any position
//how to delete at specific position
//how to appent at head/tail
//how to find an element
//check if empty
//delete all elements
//sort elements in ascending and descending order
//

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

namespace mm {

/*
--------------------------------
---------- containers ----------
--------------------------------

1. Sequence containers
Sequence containers implement data structures which can be accessed sequentially.

array			- static contiguous array	- implemented as static array on stack
vector			- dynamic contiguous array	- implemented as dymanically growing array on heap
deque			- double-ended queue		- implemented as array of pointers to fixed size arrays on heap
forward_list	- singly-linked list		- implemented as single linked list
list			- doubly-linked list		- implemented as double linked list

2. Associative containers
2.1 Ordered associative containers
Associative containers implement sorted data structures that can be quickly searched (O(log n) complexity).

set				- collection of unique keys, sorted by keys		- implemented as BST or red black tree
map				- collection of key-value pairs, sorted by keys, keys are unique - implemented as BST or red black tree
multiset		- collection of keys, sorted by keys			- implemented as BST or red black tree
multimap		- collection of key-value pairs, sorted by keys	- implemented as BST or red black tree

2.2 Unordered associative containers
Unordered associative containers implement unsorted (hashed) data structures that can be quickly searched 
(O(1) amortized, O(n) worst-case complexity).

unordered_set		- collection of unique keys, hashed by keys		- implemented as hash table
unordered_map		- collection of key-value pairs, hashed by keys, keys are unique - implemented as hash table
unordered_multiset	- collection of keys, hashed by keys			- implemented as hash table
unordered_multimap	- collection of key-value pairs, hashed by keys	- implemented as hash table

3. Container adaptors
Container adaptors provide a different interface for sequential containers.

stack			- adapts a container to provide stack (LIFO data structure)	- implemented as dymanically growing array on heap
queue			- adapts a container to provide queue (FIFO data structure)	- implemented as dymanically growing array on heap
priority_queue	- adapts a container to provide priority queue				- implemented as mean heap

--------------------------------
---------- iterators -----------
--------------------------------

each category of iterator is defined by the operations that can be performed on it.

InputIterator			- read, increment (without multiple passes)			
		- An InputIterator is an Iterator that can read from the pointed-to element
		- examples:

OutputIterator			- write, increment (without multiple passes)
		- An OutputIterator is an Iterator that can write to the pointed-to element. It is also described as mutable.
		- examples:	std::ostream_iterator
		std::ostreambuf_iterator
		std::insert_iterator
		std::back_insert_iterator
		std::front_insert_iterator

ForwardIterator			- All above i.e. Read & Write and increment (with multiple passes)	
						-  
BidirectionalIterator	- All above and decrement							
		- A BidirectionalIterator is a ForwardIterator that can be moved in both directions (i.e. incremented and decremented).
		- provides operations it++, it--
		- examples: vector, list etc

RandomAccessIterator	- All above and random access						
		- provides operations it + n, it - n, it[n] etc

ContiguousIterator		-  All above and contiguous storage	
		- 
*/

// -------------------------- std::vector

void testVector_HowCapacityOfVectorIsIncreased()
{
	cout << "\n------------------ testVector_HowCapacityOfVectorIsIncreased ------------------\n";
	vector<int> intVector;
	size_t lastCapacity = 0;
	for (int i = 0; i < 50; i++)
	{
		intVector.push_back(i);
		size_t currentCapacity = intVector.capacity();
		cout << "\nSize = " << intVector.size()
			<< " Capacity = " << currentCapacity
			<< " Increased by " << (currentCapacity - lastCapacity);
		lastCapacity = currentCapacity;
	}
	cout << endl;

/*
Output:
Testing HowCapacityOfVectorIsIncreased

Size = 1 Capacity = 1 Increased by 1
Size = 2 Capacity = 2 Increased by 1
Size = 3 Capacity = 3 Increased by 1
Size = 4 Capacity = 4 Increased by 1
Size = 5 Capacity = 6 Increased by 2
Size = 6 Capacity = 6 Increased by 0
Size = 7 Capacity = 9 Increased by 3
Size = 8 Capacity = 9 Increased by 0
Size = 9 Capacity = 9 Increased by 0
Size = 10 Capacity = 13 Increased by 4
Size = 11 Capacity = 13 Increased by 0
Size = 12 Capacity = 13 Increased by 0
Size = 13 Capacity = 13 Increased by 0
Size = 14 Capacity = 19 Increased by 6
Size = 15 Capacity = 19 Increased by 0
Size = 16 Capacity = 19 Increased by 0
Size = 17 Capacity = 19 Increased by 0
Size = 18 Capacity = 19 Increased by 0
Size = 19 Capacity = 19 Increased by 0
Size = 20 Capacity = 28 Increased by 9
Size = 21 Capacity = 28 Increased by 0
Size = 22 Capacity = 28 Increased by 0
Size = 23 Capacity = 28 Increased by 0
Size = 24 Capacity = 28 Increased by 0
Size = 25 Capacity = 28 Increased by 0
Size = 26 Capacity = 28 Increased by 0
Size = 27 Capacity = 28 Increased by 0
Size = 28 Capacity = 28 Increased by 0
Size = 29 Capacity = 42 Increased by 14
Size = 30 Capacity = 42 Increased by 0
Size = 31 Capacity = 42 Increased by 0
Size = 32 Capacity = 42 Increased by 0
Size = 33 Capacity = 42 Increased by 0
Size = 34 Capacity = 42 Increased by 0
Size = 35 Capacity = 42 Increased by 0
Size = 36 Capacity = 42 Increased by 0
Size = 37 Capacity = 42 Increased by 0
Size = 38 Capacity = 42 Increased by 0
Size = 39 Capacity = 42 Increased by 0
Size = 40 Capacity = 42 Increased by 0
Size = 41 Capacity = 42 Increased by 0
Size = 42 Capacity = 42 Increased by 0
Size = 43 Capacity = 63 Increased by 21
Size = 44 Capacity = 63 Increased by 0
Size = 45 Capacity = 63 Increased by 0
Size = 46 Capacity = 63 Increased by 0
Size = 47 Capacity = 63 Increased by 0
Size = 48 Capacity = 63 Increased by 0
Size = 49 Capacity = 63 Increased by 0
Size = 50 Capacity = 63 Increased by 0
*/
}

void print_vec(vector<int>& intVector)
{
	cout << "\nVector Elements: ";
	vector<int>::iterator it;
	for (it = intVector.begin(); it != intVector.end(); ++it)
		cout << *it << " ";
}

void testVector_basic()
{
	cout << "\n------------------ testVector_basic ------------------\n";
	/*
	Constructors:

	1) Default constructor. Constructs an empty container.

	explicit vector( const Allocator& alloc = Allocator() ); //(until C++14)

	vector() : vector( Allocator() ) {} //(since C++14)
	explicit vector( const Allocator& alloc ); //(since C++14)

	2) Constructs the container with count copies of elements with value value.
	vector( size_type count, const T& value, const Allocator& alloc = Allocator());

	3) Constructs the container with count default-inserted instances of T. No copies are made.
	explicit vector( size_type count, const Allocator& alloc = Allocator() );

	4) Constructs the container with the contents of the range [first, last).
	template< class InputIt >
	vector( InputIt first, InputIt last, const Allocator& alloc = Allocator() );

	5) Copy constructor. Constructs the container with the copy of the contents of other.
	If alloc is not provided, allocator is obtained as if by calling
	std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.get_allocator())
	vector( const vector& other );
	vector( const vector& other, const Allocator& alloc );

	6) Move constructor. Constructs the container with the contents of other using move semantics.
	If alloc is not provided, allocator is obtained by move-construction from the allocator belonging to other.
	vector( vector&& other )
	vector( vector&& other, const Allocator& alloc );

	7) Constructs the container with the contents of the initializer list init.
	vector( std::initializer_list<T> init, const Allocator& alloc = Allocator() );

	*/
	typedef vector<int> IntVector;

	IntVector intVectorOfDefaultSize;
	cout << "\nCreated object of type vector<int>\n";
	cout << "\nSize of vector = " << intVectorOfDefaultSize.size() << endl;
	cout << "\nIs vector empty? : " << intVectorOfDefaultSize.empty() << "\n";

	cout << "\nInserting 3 integers at back of vector<int>\n";
	intVectorOfDefaultSize.push_back(10); // Inserts a new element at the end.
	intVectorOfDefaultSize.push_back(20);
	intVectorOfDefaultSize.push_back(30);
	cout << "\nSize of vector = " << intVectorOfDefaultSize.size() << endl;
	cout << "\nIs vector empty? : " << intVectorOfDefaultSize.empty() << "\n";

	cout << "\nInserting 4 more integers at back of vector<int>\n";
	intVectorOfDefaultSize.push_back(40);
	intVectorOfDefaultSize.push_back(50);
	intVectorOfDefaultSize.push_back(60);
	intVectorOfDefaultSize.push_back(70);
	cout << "\nSize of vector = " << intVectorOfDefaultSize.size() << endl;
	cout << "\nIs vector empty? : " << intVectorOfDefaultSize.empty() << "\n";

}

void testVector_loop()
{
	cout << "\n------------------ testVector_loop ------------------\n";
	typedef vector<int> IntVector;
	typedef vector<int>::iterator IntIterator;
	typedef vector<int>::const_iterator ConstIntIterator;
	typedef vector<int>::reverse_iterator ReverseIntIterator;
	typedef vector<int>::const_reverse_iterator ConstReverseIntIterator;

	IntVector intVectorOfDefaultSize;
	for (int i : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10})
		intVectorOfDefaultSize.push_back(i);

	cout << "\nIterate through int vector using normal iterator\nElements: ";
	IntIterator it;
	for (it = intVectorOfDefaultSize.begin(); it != intVectorOfDefaultSize.end(); ++it)
		cout << *it << " ";
	cout << endl;

	cout << "\nIncrement every element of int vector by 2 using normal iterator\n";
	for (it = intVectorOfDefaultSize.begin(); it != intVectorOfDefaultSize.end(); ++it)
		*it = *it + 2;

	cout << "\nIterate through int vector using const iterator\nElements: ";
	ConstIntIterator cit;
	for (cit = intVectorOfDefaultSize.begin(); cit != intVectorOfDefaultSize.end(); ++cit)
		cout << *cit << " ";
	cout << endl;
	//*cit = 50 //Can not do this with const interator

	cout << "\nIterate through int vector using reverse iterator\nElements: ";
	ReverseIntIterator rit;
	// You can't use begin() and end() with reverse_iterator.
	for (rit = intVectorOfDefaultSize.rbegin(); rit != intVectorOfDefaultSize.rend(); ++rit)
		cout << *rit << " ";
	cout << endl;

	cout << "\nIterate through int vector using const reverse iterator\nElements: ";
	ConstReverseIntIterator crit;
	for (crit = intVectorOfDefaultSize.rbegin(); crit != intVectorOfDefaultSize.rend(); ++crit)
		cout << *crit << " ";
	cout << endl;

	cout << "\nIterate through int vector using subscript operator []\nElements: ";
	for (unsigned int i = 0; i < intVectorOfDefaultSize.size(); ++i)
		cout << intVectorOfDefaultSize[i] << " ";
	cout << endl;

	cout << "\nIterate through int vector using new style for loop\nElements: ";
	for (int element : intVectorOfDefaultSize)
		cout << element << " ";
	cout << endl;

	intVectorOfDefaultSize[4] = 1000; //Can modify vector with subscript operator.

	IntVector intVectorOfCustomSize(5); //Creates vector with 5 elements, all elements are initialized with zero.
	cout << "\nSize of intVectorOfCustomSize = " << intVectorOfCustomSize.size();
	cout << "\nCapacity of intVectorOfCustomSize = " << intVectorOfCustomSize.capacity();
	print_vec(intVectorOfCustomSize);

	IntVector intVectorOfCustomSizeAndInitialValue(5, 200); //Creates vector with 5 elements, all elements are initialized with 200.
	cout << "\nSize of intVectorOfCustomSizeAndInitialValue = " << intVectorOfCustomSizeAndInitialValue.size();
	cout << "\nCapacity of intVectorOfCustomSizeAndInitialValue = " << intVectorOfCustomSizeAndInitialValue.capacity();
	print_vec(intVectorOfCustomSizeAndInitialValue);

	IntVector copyOfVector(intVectorOfDefaultSize); //Creates a copy of vector

	cout << "\nBefore assignment...";
	cout << "\nSize of intVectorOfCustomSize = " << intVectorOfCustomSize.size();
	cout << "\nCapacity of intVectorOfCustomSize = " << intVectorOfCustomSize.capacity();
	print_vec(intVectorOfCustomSize);
	cout << "\nSize of intVectorOfDefaultSize = " << intVectorOfDefaultSize.size();
	cout << "\nCapacity of intVectorOfDefaultSize = " << intVectorOfDefaultSize.capacity();
	print_vec(intVectorOfDefaultSize);
	intVectorOfCustomSize = intVectorOfDefaultSize; //Assignment operator. 
													//The size, elements are same after assignment. Capacity may be different and is at least equal to size
	cout << "\nAfter assignment...";
	cout << "\nSize of intVectorOfCustomSize = " << intVectorOfCustomSize.size();
	cout << "\nCapacity of intVectorOfCustomSize = " << intVectorOfCustomSize.capacity();
	print_vec(intVectorOfCustomSize);
	cout << "\nSize of intVectorOfDefaultSize = " << intVectorOfDefaultSize.size();
	cout << "\nCapacity of intVectorOfDefaultSize = " << intVectorOfDefaultSize.capacity();
	print_vec(intVectorOfDefaultSize);

	cout << "\nFirst Element of intVectorOfDefaultSize : " << intVectorOfDefaultSize.front();
	cout << "\nLast Element of intVectorOfDefaultSize : " << intVectorOfDefaultSize.back();
	//Can change first or last element using front() and back()
	intVectorOfDefaultSize.front() = 1000;
	intVectorOfDefaultSize.back() = 5000;

	//Insert element at end
	intVectorOfDefaultSize.push_back(500); //Return type is void
										   //Remove element at end
										   //int num = intVectorOfDefaultSize.pop_back(); // Error: return type is void
	intVectorOfDefaultSize.pop_back();

	//Swap two vectors
	intVectorOfDefaultSize.swap(intVectorOfCustomSize);

	//Vector iterators are bidirectional
	vector<int>::iterator iter = intVectorOfCustomSizeAndInitialValue.end();
	cout << "\n2nd last element of vector: " << *(--(--iter));
}

void testVector_insert()
{
	cout << "\n------------------ testVector_insert ------------------\n";
	//Insert

	std::vector<int> vec(3, 100);
	print_vec(vec);

	//iterator insert( const_iterator pos, const T& value ); 
	// Insert values before position in a vector<T>
	auto it1 = vec.begin();
	it1 = vec.insert(it1, 200);
	print_vec(vec);

	//iterator insert( const_iterator pos, T&& value ); 
	// Insert values before position in a vector<T>
	//TODO: write an example

	//iterator insert( const_iterator pos, size_type count, const T& value ); 
	// inserts count copies of the value before pos in a vector<T>
	vec.insert(it1, 2, 300);
	print_vec(vec);

	// "it" no longer valid, get a new one:
	it1 = vec.begin();

	//template< class InputIterator >
	//iterator insert( const_iterator pos, InputIterator first, InputIterator last ); 
	//inserts elements from range [first, last) before pos in a vector<T> from iterator InputIterator 
	//to any type of container containing T (e.g. list<T>::iterator)
	std::vector<int> vec2(2, 400);
	vec.insert(it1 + 2, vec2.begin(), vec2.end());
	print_vec(vec);

	int arr[] = { 501,502,503 };
	vec.insert(vec.begin(), arr, arr + 3);
	print_vec(vec);

	//iterator insert( const_iterator pos, std::initializer_list<T> ilist ); 
	//inserts elements from initializer list ilist before pos.
	//TODO: write an example
	/*
	Output:
	100 100 100
	200 100 100 100
	300 300 200 100 100 100
	300 300 400 400 200 100 100 100
	501 502 503 300 300 400 400 200 100 100 100
	*/
}

void testVector_erase()
{
	cout << "\n------------------ testVector_erase ------------------\n";
	//Erase
	vector<int> intVectorOfDefaultSize;
	for (int i : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10})
		intVectorOfDefaultSize.push_back(i);

	//iterator erase(const_iterator _Where);
	//iterator erase(const_iterator _First, const_iterator _Last);
	//erase third element
	intVectorOfDefaultSize.erase(intVectorOfDefaultSize.begin() + 2);
	//erase last two elements
	intVectorOfDefaultSize.erase(intVectorOfDefaultSize.end() - 2, intVectorOfDefaultSize.end());
	//erase first two elements
	intVectorOfDefaultSize.erase(intVectorOfDefaultSize.begin(), intVectorOfDefaultSize.begin() + 2);

	intVectorOfDefaultSize.clear(); // Erases all the elements.
}

void testVector_resize()
{
	cout << "\n------------------ testVector_resize ------------------\n";
	vector<int> intVectorOfDefaultSize;
	for (int i : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10})
		intVectorOfDefaultSize.push_back(i);
	cout << "\nBefore resizing";
	print_vec(intVectorOfDefaultSize);
	cout << "\nSize: " << intVectorOfDefaultSize.size();
	cout << "\nCapacity: " << intVectorOfDefaultSize.capacity();
	intVectorOfDefaultSize.resize(5); //Inserts or erases elements at end so that size becomes 5.
	cout << "\nAfter resizing: intVectorOfDefaultSize.resize(5)";
	print_vec(intVectorOfDefaultSize);
	cout << "\nSize: " << intVectorOfDefaultSize.size();
	cout << "\nCapacity: " << intVectorOfDefaultSize.capacity();
}

void testVector_reserve()
{
	cout << "\n------------------ testVector_reserve ------------------\n";
	vector<int> intVectorOfDefaultSize;
	for (int i : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10})
		intVectorOfDefaultSize.push_back(i);
	cout << "\nBefore reserving";
	print_vec(intVectorOfDefaultSize);
	cout << "\nSize: " << intVectorOfDefaultSize.size();
	cout << "\nCapacity: " << intVectorOfDefaultSize.capacity();
	intVectorOfDefaultSize.reserve(50); 
	//increases copacity to 50 ONLY if its less than 50. Otherwise capacity is unchanged, does not shrink capacity, 
		//use shrink_to_fit to shrink capacity
	//Size is unchanged. 
	//TODO: Need to check if old elements are copied or not => VERIFIED - OLD ELEMENTS ARE COPIED
	cout << "\nAfter reserving: intVectorOfDefaultSize.reserve(50)";
	print_vec(intVectorOfDefaultSize);
	cout << "\nSize: " << intVectorOfDefaultSize.size();
	cout << "\nCapacity: " << intVectorOfDefaultSize.capacity();
}

void testVector_shrink_to_fit()
{
	cout << "\n------------------ testVector_shrink_to_fit ------------------\n";
	/*
	Requests the removal of unused capacity.
	It is a non-binding request to reduce capacity() to size(). It depends on the implementation if the request is fulfilled.
	All iterators, including the past the end iterator, are potentially invalidated.
	*/
	
	{		
		vector<int> intVectorOfDefaultSize;
		for (int i : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10})
			intVectorOfDefaultSize.push_back(i);
		cout << "\nBefore shrink_to_fit";
		print_vec(intVectorOfDefaultSize);
		cout << "\nSize: " << intVectorOfDefaultSize.size();
		cout << "\nCapacity: " << intVectorOfDefaultSize.capacity();
		intVectorOfDefaultSize.shrink_to_fit(); //increases copacity to 50 if its less than 50. Size is unchanged. 
											//TODO: Need to check if old elements are copied or not => VERIFIED - OLD ELEMENTS ARE COPIED
		cout << "\nAfter shrink_to_fit: intVectorOfDefaultSize.shrink_to_fit()";
		print_vec(intVectorOfDefaultSize);
		cout << "\nSize: " << intVectorOfDefaultSize.size();
		cout << "\nCapacity: " << intVectorOfDefaultSize.capacity();
	}

	//It is NOT equivalent to reserve(vector<T>::size()). reserve does not change capacity if target capacity is less than size()
	{
		vector<int> intVectorOfDefaultSize;
		for (int i : {1, 2, 3, 4, 5, 6, 7, 8, 9, 10})
			intVectorOfDefaultSize.push_back(i);
		cout << "\nBefore reserve(vector<T>::size())";
		print_vec(intVectorOfDefaultSize);
		cout << "\nSize: " << intVectorOfDefaultSize.size();
		cout << "\nCapacity: " << intVectorOfDefaultSize.capacity();
		intVectorOfDefaultSize.reserve(intVectorOfDefaultSize.size()); //increases copacity to 50 if its less than 50. Size is unchanged. 
												//TODO: Need to check if old elements are copied or not => VERIFIED - OLD ELEMENTS ARE COPIED
		cout << "\nAfter reserve(vector<T>::size())";
		print_vec(intVectorOfDefaultSize);
		cout << "\nSize: " << intVectorOfDefaultSize.size();
		cout << "\nCapacity: " << intVectorOfDefaultSize.capacity();
	}
}

void testVector_assign()
{
	cout << "\n------------------ testVector_assign ------------------\n";
	/*
	vector::assign
	Erases a vector and copies the specified elements to the empty vector.

	(1) Replaces the contents with count copies of value value
	void assign( size_type count, const T& value );

	(2) Replaces the contents with copies of those in the range [first, last).
	This overload has the same effect as overload (1) if InputIt is an integral type.	(until C++11)
	This overload only participates in overload resolution if InputIt satisfies InputIterator.	(since C++11)
	template< class InputIt >
	void assign( InputIt first, InputIt last );

	(3)	(since C++11) Replaces the contents with the elements from the initializer list ilist.
	void assign( std::initializer_list<T> ilist );

	Parameters
	count	-	the new size of the container
	value	-	the value to initialize elements of the container with
	first, last	-	the range to copy the elements from
	ilist	-	initializer list to copy the values from
	Complexity
	1) Linear in count
	2) Linear in distance between first and last
	3) Linear in ilist.size()
	*/

	vector<int> v1, v2, v3;

	v1.push_back(10);
	v1.push_back(20);
	v1.push_back(30);
	v1.push_back(40);
	v1.push_back(50);

	cout << "v1 = ";
	for (auto& v : v1) {
		cout << v << " ";
	}
	cout << endl;

	v2.assign(v1.begin(), v1.end());
	cout << "v2 = ";
	for (auto& v : v2) {
		cout << v << " ";
	}
	cout << endl;

	v3.assign(7, 4);
	cout << "v3 = ";
	for (auto& v : v3) {
		cout << v << " ";
	}
	cout << endl;

	v3.assign({ 5, 6, 7 });
	for (auto& v : v3) {
		cout << v << " ";
	}
	cout << endl;

	/*
	Output:
	v1 = 10 20 30 40 50
	v2 = 10 20 30 40 50
	v3 = 4 4 4 4 4 4 4
	5 6 7
	*/
}

void testVector_get_allocator()
{
	cout << "\n------------------ testVector_get_allocator ------------------\n";
	/*
	vector::allocator_type
	typedef Allocator allocator_type;
	*/
	/*
	allocator_type vector<T>::get_allocator() const;
	Returns the allocator associated with the container.
	*/
	{
		using namespace std;
		// The following lines declare objects that use the default allocator.
		vector<int> v1;
		vector<int, allocator<int> > v2 = vector<int, allocator<int> >(allocator<int>());

		// v3 will use the same allocator class as v1
		vector <int> v3(v1.get_allocator());

		vector<int>::allocator_type xvec = v3.get_allocator();
		// You can now call functions on the allocator class used by vec
	}

	{
		std::vector<int> myvector;
		int * p;
		unsigned int i;

		// allocate an array with space for 5 elements using vector's allocator:
		p = myvector.get_allocator().allocate(5);

		// construct values in-place on the array:
		for (i = 0; i<5; i++) myvector.get_allocator().construct(&p[i], i);

		std::cout << "The allocated array contains:";
		for (i = 0; i<5; i++) std::cout << ' ' << p[i];
		std::cout << '\n';

		// destroy and deallocate:
		for (i = 0; i<5; i++) myvector.get_allocator().destroy(&p[i]);
		myvector.get_allocator().deallocate(p, 5);
	}
	
}

void testVector_at()
{
	cout << "\n------------------ testVector_at ------------------\n";
	/*
	std::vector::at
	reference       at(size_type pos);
	const_reference at(size_type pos) const;
	Returns a reference to the element at specified location pos, with bounds checking.
	If pos is not within the range of the container, an exception of type std::out_of_range is thrown.
	Parameters
	pos - position of the element to return
	Return value
	Reference to the requested element.
	Exceptions
	std::out_of_range if !(pos < size()).
	Complexity
	Constant.

	If the return value of at is assigned to a const_reference, the vector object cannot be modified.
	If the return value of at is assigned to a reference, the vector object can be modified.

	*/
	{
		using namespace std;
		vector <int> v1;

		v1.push_back(10);
		v1.push_back(20);

		const int &i = v1.at(0);
		int &j = v1.at(1);
		cout << "The first element is " << i << endl;
		cout << "The second element is " << j << endl;
	}
}

void testVector_data()
{
	cout << "\n------------------ testVector_data ------------------\n";
	/*
	std::vector::data
	T* data() noexcept;						(since C++11)
	const T* data() const noexcept;				(since C++11)

	Returns pointer to the underlying array serving as element storage.The pointer is such that range[data(); data() + size()) is always a valid range, even if the container is empty(data() is not dereferenceable in that case).
	Parameters
	(none)
	Return value
	Pointer to the underlying element storage.For non - empty containers, returns &front()
	Complexity
	Constant.
	Exceptions
	noexcept specification:  noexcept

	*/
}

void testVector_max_size()
{
	cout << "\n------------------ testVector_max_size ------------------\n";
	/*
	std::vector::max_size
	size_type max_size() const noexcept;
	Returns the maximum number of elements the container is able to hold due to system or library implementation limitations, i.e. std::distance(begin(), end()) for the largest container.
	Parameters
	(none)
	Return value
	Maximum number of elements.
	Exceptions
	(none)	(until C++11)
	noexcept specification:  noexcept  	(since C++11)
	Complexity
	Constant.
	Notes
	This value typically reflects the theoretical limit on the size of the container. At runtime, the size of the container may be limited to a value smaller than max_size() by the amount of RAM available.
	*/

	{
		std::vector<int> myvector;

		// set some content in the vector:
		for (int i = 0; i<100; i++) myvector.push_back(i);

		std::cout << "size: " << myvector.size() << "\n";
		std::cout << "capacity: " << myvector.capacity() << "\n";
		std::cout << "max_size: " << myvector.max_size() << "\n";
	}

	/*
	size: 100
	capacity: 128
	max_size: 1073741823
	*/
}

void testVector_emplace()
{
	cout << "\n------------------ testVector_emplace ------------------\n";

	//Not same as Insert
	//Emplace takes the arguments necessary to construct an object in place, whereas insert takes(a reference to) an object.

	//template <class... Args>
	//iterator emplace(const_iterator position, Args&&... args);
	//Inserts a new element into the container directly before pos. 
	//The element is constructed through std::allocator_traits::construct, which typically uses 
	//placement-new to construct the element in-place at a location provided by the container. 
	//The arguments args... are forwarded to the constructor as std::forward<Args>(args)....

	struct Foo
	{
		Foo(int nIn, double xIn)
		{
			n = nIn;
			x = xIn;
		}

		int n;
		double x;
	};

	std::vector<Foo> v = { Foo(1, 1.111), Foo(2, 2.222), Foo(3, 3.333) };
	auto it = v.emplace(v.begin(), 4, 4.444);
	it = v.insert(v.begin(), Foo(5, 5.555));

	it = v.emplace(v.begin() + 1, 6, 6.666);
	v.emplace(it, 7, 7.777);
	v.emplace(v.end(), 8, 8.888);

	std::cout << "myvector contains:";
	for (auto& x : v)
		std::cout << ' ' << x.n << " " << x.x;
	std::cout << '\n';	

/*
Output:
------------------ testVector_emplace ------------------
myvector contains: 5 5.555 7 7.777 6 6.666 4 4.444 1 1.111 2 2.222 3 3.333 8 8.888
*/

	//template <class... Args>
	//void emplace_back(Args&&... args);
	//Appends a new element to the end of the container.
	//The element is constructed through std::allocator_traits::construct, which typically uses 
	//placement - new to construct the element in - place at the location provided by the container.
	//The arguments args... are forwarded to the constructor as std::forward<Args>(args)....
} 

void funWith_vector()
{
	cout << "\n------------------ funWithVectors ------------------\n";

	testVector_HowCapacityOfVectorIsIncreased();
	testVector_basic();
	testVector_loop();
	testVector_insert();
	testVector_erase();
	testVector_resize();
	testVector_reserve();
	testVector_shrink_to_fit();
	testVector_assign();
	testVector_get_allocator();
	testVector_at();
	testVector_data();
	testVector_max_size();
	testVector_emplace();

/*
Vectors:
insertion at begining	- O(n)
insertion in middle		- O(n)
insertion at end		- best case - O(1), worst case - O(n)
removal at begining		- O(n)
removal in middle		- O(n)
**removal at end		- O(1)

Notes:
A vector's iterators are invalidated when its memory is reallocated.
Additionally, inserting or deleting an element in the middle of a vector
invalidates all iterators that point to elements following the
insertion or deletion point.
It follows that you can prevent a vector's iterators from being
invalidated if you use reserve() to preallocate as
much memory as the vector will ever use, and if all insertions and
deletions are at the vector's end.
*/

}

//-------------------------------- std::vector<bool>

void funWith_vector_bool()
{
	//The vector<bool> class is a partial specialization of vector for elements of type bool.
	//It has an allocator for the underlying type that's used by the specialization, 
	//which provides space optimization by storing one bool value per bit.

	 std::vector<bool> V(5);
	 V[0] = true;
	 V[1] = false;
	 V[2] = false;
	 V[3] = true;
	 V[4] = false;
	 
	 for (std::vector<bool>::iterator i = V.begin(); i < V.end(); ++i)
		 cout << (*i ? "This is true" : "This is false") << endl;
}


//-------------------------------- std::array 

void test_array_basic()
{
	//array<int, 5> arr;
}

void funWith_array()
{
	cout << "\n------------------ funWith_array ------------------\n";

	test_array_basic();
}


//-------------------------------- std::deque

void funWith_deque()
{
	cout << "\n------------------ funWith_deque ------------------\n";
	//double-ended queue
	/*
	Both vectors and deques provide a very similar interface and can be used for similar purposes, 
	but internally both work in quite different ways: While vectors use a single array that needs to be occasionally 
	reallocated for growth, the elements of a deque can be scattered in different chunks of storage, 
	with the container keeping the necessary incformation internally to provide direct access to any of its 
	elements in constant time and with a uniform sequential interface (through iterators). 
	--Deque is implemented as a vector of vectors

	Following functions are not in vector, but are available in deque
	push_front() - Adds elements to the front of a deque.
	pop_front() - Removes elements from the front of a deque.
	Following functions are in vector, but not in deque
	capacity() - Returns the current capacity of a vector.
	reserve() - Allocates room for a specified number of elements in a vector.
	*/
}

//-------------------------------- funWith_set

void funWith_set()
{
	cout << "\n------------------ funWith_set ------------------\n";

/*
std::set is an associative container that contains a sorted set of unique objects of type Key. 
Sorting is done using the key comparison function Compare. 
Search, removal, and insertion operations have logarithmic complexity. 
Sets are usually implemented as red-black trees. OR Sets are typically implemented as binary search trees.

*/

}

template<typename _Kty, typename _Ty>
std::ostream& operator<<(std::ostream& o, std::pair<const _Kty, _Ty> element)
{
	cout << "{" << element.first << "," << element.second << "}";
	return o;
}

void print_deque(deque<int>& intDeque)
{
	cout << "\nDeque Elements: ";
	deque<int>::iterator it;
	for (it = intDeque.begin(); it != intDeque.end(); ++it)
		cout << *it << " ";
}

template<typename T>
void print(T& container, const string &msg)
{
	cout << endl;
	typename T::iterator it;
	for (it = container.begin(); it != container.end(); ++it)
		cout << *it << " ";
	cout << "\t\t" << msg;
}


void funWith_map()
{
	std::pair<int, char> pairArray[5] = { {1, 'a'}, {2, 'b'}, {3, 'c'}, {4, 'd'}, {5, 'e'} };
	//std::map<int, int> m1(&intArray[2], &intArray[4]);
}

void EffectiveSTL_Item9()
{
	{
		//Erase first item having value x

		//Vector, Deque and string
		cout << endl;
		vector<int> v({ 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 });
		print(v, ": Original vector<int>");
		//Find is not available for sequence containers because it has to do sequencial search
		//It finds the first element satisfying specific criteria 
		vector<int>::iterator itv = std::find(v.begin(), v.end(), 3);
		if(itv != v.end())
			v.erase(itv);
		print(v, ": After erasing first element of value 3");
		itv = std::find(v.begin(), v.end(), 100);
		if (itv != v.end())
			v.erase(itv);
		print(v, ": After erasing first element of value 100");

		//List
		cout << endl;
		list<int> l({ 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 });
		print(l, ": Original list<int>");
		list<int>::iterator itl = std::find(l.begin(), l.end(), 3);
		if (itl != l.end())
			l.erase(itl);
		print(l, ": After erasing first element of value 3");
		itl = std::find(l.begin(), l.end(), 100);
		if (itl != l.end())
			l.erase(itl);
		print(l, ": After erasing first element of value 100");

		//Set
		cout << endl;
		set<int> s({ 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 });
		print(s, ": nOriginal set<int>");
		s.erase(3); //Set has every key only once, so no need to find first key
		print(s, ": After set::erase(Key) FIRST AND ONLY element of value 3");
		s.erase(100); //Set has every key only once, so no need to find first key
		print(s, ": After erasing FIRST AND ONLY element of value 100 if present");
		set<int>::iterator its = s.find(5);
		if(its != s.end())
			s.erase(its); //Set has every key only once, so no need to find first key
		print(s, ": After set::erase(set::find(Value) != end()) FIRST AND ONLY element of value 5");
		its = s.find(100);
		if (its != s.end())
			s.erase(its); //Set has every key only once, so no need to find first key
		print(s, ": After set::erase(set::find(Value) != end()) FIRST AND ONLY element of value 100 if present");

		//Multi-Set
		cout << endl;
		multiset<int> ms({ 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 });
		print(ms, ": Original multiset<int>");
		multiset<int>::iterator itms = ms.find(3);
		if (itms != ms.end())
			ms.erase(itms); //Set has every key only once, so no need to find first key
		print(ms, ": After multiset::erase(multiset::find(Value) != end()) first element of value 3");
		itms = ms.find(100);
		if (itms != ms.end())
			ms.erase(itms); //Set has every key only once, so no need to find first key
		print(ms, ": After multiset::erase(multiset::find(Value) != end()) first element of value 100 if present");
			
		//unordered_map
		cout << endl;
		unordered_map<int, char> um({ { 1, 'A' }, { 2, 'B' }, { 3, 'C' }, { 4, 'D' }, { 5, 'E' },  
									  { 1, 'A' }, { 2, 'B' }, { 3, 'C' }, { 4, 'D' }, { 5, 'E' } });
		print(um, ": nOriginal unordered_map<int>");
		um.erase(3); //Set has every key only once, so no need to find first key
		print(um, ": After unordered_map::erase(Key) FIRST AND ONLY element with key 3");
		um.erase(100); //unordered_map has every key only once, so no need to find first key
		print(um, ": After erasing FIRST AND ONLY element wiht key 100 if present");
		unordered_map<int, char>::iterator itum = um.find(5);
		if (itum != um.end())
			um.erase(itum); //unordered_map has every key only once, so no need to find first key
		print(um, ": After unordered_map::erase(unordered_map::find(Value) != end()) FIRST AND ONLY element with key 5");
		itum = um.find(100);
		if (itum != um.end())
			um.erase(itum); //unordered_map has every key only once, so no need to find first key
		print(um, ": After unordered_map::erase(unordered_map::find(Value) != end()) FIRST AND ONLY element wiht key 100 if present");

		//unordered_multimap
		cout << endl;
		unordered_multimap<int, char> umm({ { 1, 'A' },{ 2, 'B' },{ 3, 'C' },{ 4, 'D' },{ 5, 'E' },
		{ 1, 'A' },{ 2, 'B' },{ 3, 'C' },{ 4, 'D' },{ 5, 'E' } });
		print(umm, ": nOriginal unordered_multimap<int>");
		unordered_multimap<int, char>::iterator itumm = umm.find(3);
		if (itumm != umm.end())
			umm.erase(itumm); //unordered_map has every key only once, so no need to find first key
		print(umm, ": After unordered_multimap::erase(unordered_multimap::find(Value) != end()) FIRST element with key 3");
		itumm = umm.find(100);
		if (itumm != umm.end())
			umm.erase(itumm); //unordered_map has every key only once, so no need to find first key
		print(umm, ": After unordered_multimap::erase(unordered_multimap::find(Value) != end()) FIRST element wiht key 100 if present");

	}

	{
		//TODO: 
		//Erase first item with UnaryPredicate (returns true on specific condition)

		//Define predicate
		auto UnaryPredicate = [](int n) {return n % 2 == 0; };

		//Vector, Deque and string
		cout << endl;
		vector<int> v({ 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 });
		print(v, ": Original vector<int>");
		//Find is not available for sequence containers because it has to do sequencial search
		//It finds the first element satisfying specific criteria 
		vector<int>::iterator itv = std::find(v.begin(), v.end(), 3);
		if (itv != v.end())
			v.erase(itv);
		print(v, ": After erasing first element of value 3");
		itv = std::find(v.begin(), v.end(), 100);
		if (itv != v.end())
			v.erase(itv);
		print(v, ": After erasing first element of value 100");

		//List
		cout << endl;
		list<int> l({ 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 });
		print(l, ": Original list<int>");
		list<int>::iterator itl = std::find(l.begin(), l.end(), 3);
		if (itl != l.end())
			l.erase(itl);
		print(l, ": After erasing first element of value 3");
		itl = std::find(l.begin(), l.end(), 100);
		if (itl != l.end())
			l.erase(itl);
		print(l, ": After erasing first element of value 100");

		//Set
		cout << endl;
		set<int> s({ 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 });
		print(s, ": nOriginal set<int>");
		s.erase(3); //Set has every key only once, so no need to find first key
		print(s, ": After set::erase(Key) FIRST AND ONLY element of value 3");
		s.erase(100); //Set has every key only once, so no need to find first key
		print(s, ": After erasing FIRST AND ONLY element of value 100 if present");
		set<int>::iterator its = s.find(5);
		if (its != s.end())
			s.erase(its); //Set has every key only once, so no need to find first key
		print(s, ": After set::erase(set::find(Value) != end()) FIRST AND ONLY element of value 5");
		its = s.find(100);
		if (its != s.end())
			s.erase(its); //Set has every key only once, so no need to find first key
		print(s, ": After set::erase(set::find(Value) != end()) FIRST AND ONLY element of value 100 if present");

		//Multi-Set
		cout << endl;
		multiset<int> ms({ 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 });
		print(ms, ": Original multiset<int>");
		multiset<int>::iterator itms = ms.find(3);
		if (itms != ms.end())
			ms.erase(itms); //Set has every key only once, so no need to find first key
		print(ms, ": After multiset::erase(multiset::find(Value) != end()) first element of value 3");
		itms = ms.find(100);
		if (itms != ms.end())
			ms.erase(itms); //Set has every key only once, so no need to find first key
		print(ms, ": After multiset::erase(multiset::find(Value) != end()) first element of value 100 if present");

		//unordered_map
		cout << endl;
		unordered_map<int, char> um({ { 1, 'A' },{ 2, 'B' },{ 3, 'C' },{ 4, 'D' },{ 5, 'E' },
		{ 1, 'A' },{ 2, 'B' },{ 3, 'C' },{ 4, 'D' },{ 5, 'E' } });
		print(um, ": nOriginal unordered_map<int>");
		um.erase(3); //Set has every key only once, so no need to find first key
		print(um, ": After unordered_map::erase(Key) FIRST AND ONLY element with key 3");
		um.erase(100); //unordered_map has every key only once, so no need to find first key
		print(um, ": After erasing FIRST AND ONLY element wiht key 100 if present");
		unordered_map<int, char>::iterator itum = um.find(5);
		if (itum != um.end())
			um.erase(itum); //unordered_map has every key only once, so no need to find first key
		print(um, ": After unordered_map::erase(unordered_map::find(Value) != end()) FIRST AND ONLY element with key 5");
		itum = um.find(100);
		if (itum != um.end())
			um.erase(itum); //unordered_map has every key only once, so no need to find first key
		print(um, ": After unordered_map::erase(unordered_map::find(Value) != end()) FIRST AND ONLY element wiht key 100 if present");

		//unordered_multimap
		cout << endl;
		unordered_multimap<int, char> umm({ { 1, 'A' },{ 2, 'B' },{ 3, 'C' },{ 4, 'D' },{ 5, 'E' },
		{ 1, 'A' },{ 2, 'B' },{ 3, 'C' },{ 4, 'D' },{ 5, 'E' } });
		print(umm, ": nOriginal unordered_multimap<int>");
		unordered_multimap<int, char>::iterator itumm = umm.find(3);
		if (itumm != umm.end())
			umm.erase(itumm); //unordered_map has every key only once, so no need to find first key
		print(umm, ": After unordered_multimap::erase(unordered_multimap::find(Value) != end()) FIRST element with key 3");
		itumm = umm.find(100);
		if (itumm != umm.end())
			umm.erase(itumm); //unordered_map has every key only once, so no need to find first key
		print(umm, ": After unordered_multimap::erase(unordered_multimap::find(Value) != end()) FIRST element wiht key 100 if present");

	}

	{
		//Erase all items having value x

		//Vector, Deque and string
		cout << endl;
		vector<int> v({ 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 });
		print(v, ": original vector<int>");
		//Removing is done by shifting(by means of move assignment) the elements in the range in such a way that 
		//the elements that are not to be removed appear in the beginning of the range.Relative order of the 
		//elements that remain is preserved and the physical size of the container is unchanged.Iterators 
		//pointing to an element between the new logical end and the physical end of the range are still 
		//dereferenceable, but the elements themselves have unspecified values(as per MoveAssignable post - condition).
		//A call to remove is typically followed by a call to a container's erase method, which erases the 
		//unspecified values and reduces the physical size of the container to match its new logical size.
		v.erase(std::remove(v.begin(), v.end(), 3), v.end()); // the erase-remove idiom
		print(v, ": After erasing all elements of value 3");
		v.erase(std::remove(v.begin(), v.end(), 100), v.end()); // the erase-remove idiom
		print(v, ": After erasing all elements of value 100");

		//List
		cout << endl;
		list<int> l({ 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 });
		print(l, ": original list<int>");
		l.remove(3);
		print(l, ": After erasing all elements of value 3");
		l.remove(100);
		print(l, ": After erasing all elements of value 100");

		//Set
		cout << "\n\nThere are unique keys in set. So the case of \"Removing all elements from set which has value x\" doesnt apply to set";

		//Multi-Set
		cout << endl;
		multiset<int> ms({ 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 });
		print(ms, ": Original multiset<int>");
		ms.erase(3);
		print(ms, ": After erasing all elements of value 3");
		cout << "\nTotal " << ms.count(3) << "element were erased having value 3";
		ms.erase(100);
		print(ms, ": After erasing all elements of value 100");
		cout << "\nTotal " << ms.count(100) << "element were erased having value 100";

		//unordered_map
		cout << "\n\nThere are unique keys in unordered_map. So the case of \"Removing all elements from unordered_map which has key x\" doesnt apply to unordered_map";
		
		//unordered_multimap
		cout << endl;
		unordered_multimap<int, char> umm({ { 1, 'A' },{ 2, 'B' },{ 3, 'C' },{ 4, 'D' },{ 5, 'E' },
		{ 1, 'A' },{ 2, 'B' },{ 3, 'C' },{ 4, 'D' },{ 5, 'E' } });
		print(umm, ": nOriginal unordered_multimap<int>");
		umm.erase(3); //Set has every key only once, so no need to find first key
		print(umm, ": After unordered_multimap::erase(Key) all elements with key 3");
		umm.erase(100); //unordered_map has every key only once, so no need to find first key
		print(umm, ": After erasing all elements wiht key 100 if present");
		
	}
}

void STLContainers()
{
	cout << "\n------------------ STLContainers ------------------\n";

	//Sequence containers
	////  funWith_array();
	////  funWith_vector();
	//funWith_vector_bool();
	//funWith_deque();
	//funWith_forward_list();
	//funWith_list();

	//Associative containers (Sorted)
	//funWith_set
	//funWith_map
	//funWith_multiset
	//funWith_multimap

	//Unordered associative containers
	//funWith_unordered_set
	//funWith_unordered_map
	//funWith_unordered_multiset
	//funWith_unordered_multimap

	//Container adaptors
	//funWith_stack
	//funWith_queue
	//funWith_priority_queue

	//EffectiveSTL_Item9(); //Item 9. Choose carefully among erasing options.
}

}
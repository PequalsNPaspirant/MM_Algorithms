//-----------------------------------------------------------------
//         TO BE ARRANGED
//-----------------------------------------------------------------

/*

Gabriel's list:
- Print a binary tree level by level. Do it without recursion.
- Given a root node of a binary tree check if the tree is a binary search tree.
- Reverse a string word by word. (Hello World -> World Hello)
- Write a simple shared pointer
- Write a template class that can calculate the powers of 2 compile time
- You have stock prices for N days. You can buy once and then sell once. How do you pick the days to buy and sell to make the most profit with O(N) complexity.
- Given an array of integers find the pairs that sum to a certain value with O(N) complexity

My list:
- How to balance a BST?

2) Boost:
- shared_ptr
- weak_ptr
- unique_ptr
- intrusive_ptr

3) STL:

*/

/*

Question:

struct X {
int member_;

virtual void fn1();
virtual void fn2();
virtual void fn3();
};

what is sizeof(X)? 32 bit system



Question:

struct Base {
Base() { print(); }
virtual void print() { cout << "I am Base" << endl; }
};

struct Derived : public Base {
Derived() { print(); }
virtual void print() { cout << "I am Derived" << endl; }
};

void main() {
Derived derived;
}


Question:

---------- singletonA.h ---------
struct SingletonA {
static SingletonA& Instance() {
static SingletonA Instance_;
return Instance_;
}
private:
SingletonA() { cout << "A constructor" << endl; }
}

---------- singletonB.h ---------
struct SingletonB {
static SingletonB& Instance() { return Instance_; }

private:
SingletonB() { cout << "B constructor" << endl; }
static SingletonB Instance_;
}

What would happen to this in case of multithreading environment?


---------- main.cpp ----------
int main()
{
cout << "Singleton test" << endl;
SingletonA& a = SingletonA::Instance();
SingletonB& b = SingletonB::Instance();
}



Question:

std::set<int> mySet;

mySet.insert(1);
mySet.insert(2);

// TODO: Print the first element
std::cout << *mySet.begin();

map
unordered_map

shared_ptr {
T* ptr_;
int count_;
};

weak_ptr?



Question:

void main()
{
// crete a vector with size 10000
std::vector<int> randomNumbers(10000);

// fill it all with random integers
std::generate(randomNumbers.begin(), randomNumbers.end(), std::rand);

// TODO: How do you select the highest 100 numbers. The highest 100 can
//       be in any order.
}

N = 10000
K = 100

bubble sort for first 100 elements
K * N

BST to store K elements
log(K)*N

//Quick sort
Save sorting on 100 elements.....so save KlogK
nlogn - klogk

Further More efficient -
10000

Do not recursively call quick sort on the group having lower values

5000 < PIVOT < 5000

All-10000,
Max-5000 (Discart min 5000),
Max-2500 (Discart min 2500),
Max-1200...
...

N + N/2 + N/4... -> 2 N -> O(N)

std::nth_element

---Another method

use min heap of size 100
traverse through array
if element is greater than min element in min heap...discard that and add this new element



//Question: Print latest 3 events
//Ans: Use

class History {
public:
enum Event { EventA, EventB, EventC, EventD, EventE }

void insert(Event event) {
if(store_.size() > 3)
{
store_.pop_front();
store_.push_back(event);
}
/* TODO: Implement the function
}

void printLast3Events() {
/* TODO: Implement the function
}

private:
std::list<Event> store_;
/* TODO - Define the storage mebers for the data
};

void main() {
History h;

// Insert events potentially 10000
h.insert(EventA);
h.insert(EventA);
h.insert(EventC);
.....
h.insert(EventD);
h.insert(EventD);

// At some point we want to see the last 3
h.printLast3Events();

// Insert more events
h.insert(EventE);
h.insert(EventE);
h.insert(EventD);
h.insert(EventA);

// Again we want to see the last 3
h.printLast3Events();
}
}




You can use template metaprogramming. Let me show the code.

template <int A, int B>
struct get_power
{
static const int value = A * get_power<A, B - 1>::value;
};
template <int A>
struct get_power<A, 0>
{
static const int value = 1;
};
Usage:

std::cout << get_power<3, 3>::value << std::endl;





*/

/*

//Goal:
//Sample Interview questions

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <map>
#include <cmath>
using namespace std;

namespace mm {

	//---------------------------------- Question 1 ----------------------------------

	int getDecimalDigit(char ch)
	{
		if (ch >= '0' && ch <= '9')
			return (ch - '0');
		else if (ch >= 'A' && ch <= 'F')
			return (ch - 'A' + 10);
		//switch (ch)
		//{
		//case 'A': return 10;
		//case 'B': return 11;
		//case 'C': return 12;
		//case 'D': return 13;
		//case 'E': return 14;
		//case 'F': return 15;
		//}
	}

	char getHexDigit(int n)
	{
		if (n >= 0 && n <= 9)
			return ('0' + n);
		else if (n >= 10 && n <= 15)
			return ('A' + n - 10);
	}

	// 100F = 4111
	unsigned int hexToDecimal(const char* hex)
	{
		if (hex == nullptr)
			return 0;

		int retVal = 0;
		int len = strlen(hex);
		for (int i = len - 1; i >= 0; i--)
		{
			retVal += getDecimalDigit(hex[i]) * pow(16, len - i - 1);
		}

		return retVal;
	}

	void reverseString(char* str)
	{
		if (str == NULL) return;

		int len = strlen(str);
		for (int i = 0; i < len / 2; i++)
			std::swap(str[i], str[len - i - 1]);
	}

	char* decimalToHex(unsigned int num)
	{
		int numDigits = 0;
		char* ch = new char[25];
		int base = 16;
		int i = 0;
		for (; num > 0; num /= base, i++)
			ch[i] = getHexDigit(num % base);

		ch[i] = '\0';

		reverseString(ch);
		return ch;
	}

	void testHexToDecomal()
	{
		cout << "\n------------------ testHexToDecomal ------------------";
		int testNum[] = { 4111, 0, 1, 234, 7483, 2839563 };
		for (int i = 0; i < 6; i++)
		{
			char* ch = decimalToHex(testNum[i]);
			int n = hexToDecimal(ch);
			cout << "\nInt = " << testNum[i] << " Hex = " << ch << " Decimal = " << n;
		}
	}

	//---------------------------------- Question 2 ----------------------------------

	struct Node
	{
		int value;
		Node* next;
	};

	Node* createList(int array[], int size)
	{
		Node* head = NULL;
		Node* prev = NULL;
		for (int i = 0; i < size; i++)
		{
			Node* newNode = new Node;
			newNode->value = array[i];
			newNode->next = NULL;
			if (i == 0)
				head = newNode;
			else
				prev->next = newNode;
			prev = newNode;
		}
		return head;
	}

	Node* removeDuplicate(Node* list)
	{
		Node* returnHead = NULL;

		for (Node* start = list; start != NULL; start = start->next)
		{
			Node* temp = returnHead;
			while (true)
			{
				if (temp == NULL)
				{
					//Return list is not yet created
					Node* newNode = new Node;
					newNode->value = start->value;
					newNode->next = NULL;
					returnHead = newNode;
					break;
				}
				else if (start->value == temp->value)
				{
					break;
				}
				else if (temp->next == NULL)
				{
					//We did not find the value, so add it
					Node* newNode = new Node;
					newNode->value = start->value;
					newNode->next = NULL;
					//Append it to return list
					temp->next = newNode;
					break;
				}
				if (temp != NULL)
					temp = temp->next;
			}
		}

		return returnHead;
	}


	void removeDuplicate2(Node* list)
	{
		Node* last = NULL;
		for (Node* current = list; current != NULL; current = last->next)
		{
			bool deleted = false;
			for (Node* temp = list; temp != current; temp = temp->next)
			{
				if (current->value == temp->value)
				{
					//remove current from list
					last->next = last->next->next;
					deleted = true;
					break;
				}
			}
			if (!deleted)
				last = current;
		}
	}

	class UniqueBST
	{
	public:
		UniqueBST()
			: root(NULL)
		{
		}
		~UniqueBST()
		{
			//Do postorder traversal to delete all nodes
			deleteTree(root);
		}

		bool addUniqueElement(int num)
		{
			return addUniqueElement(&root, num);
		}

	private:
		struct BSTNode
		{
			int value;
			BSTNode *left, *right;
		};
		void deleteTree(BSTNode* node)
		{
			if (node == NULL) return;
			if (node->left)
				deleteTree(node->left);
			if (node->right)
				deleteTree(node->right);
			delete node;
		}
		//find is not required since finding element and adding it if not found can be done in one traversal. See addUniqueElement 
		//bool find(BSTNode* root, int n)
		//{
		//	if (root == NULL) return false;
		//	if (root->value == n) return true;
		//	if (n < root->value) return find(root->left, n);
		//	if (n > root->value) return find(root->right, n);
		//}
		bool addUniqueElement(BSTNode** ppCurrent, int num) //Returns true if the number is added
		{
			if (ppCurrent == NULL) return false;

			if (*ppCurrent == NULL)
			{
				BSTNode* newNode = new BSTNode;
				newNode->value = num;
				newNode->left = NULL;
				newNode->right = NULL;
				*ppCurrent = newNode;

				return true;
			}
			else if ((*ppCurrent)->value == num)
				return false;
			else if (num < (*ppCurrent)->value)
				addUniqueElement(&((*ppCurrent)->left), num);
			else if (num > (*ppCurrent)->value)
				addUniqueElement(&((*ppCurrent)->right), num);
		}

		BSTNode* root;
	};

	void removeDuplicateBestApproach(Node* list)
	{
		if (list == NULL) return;

		UniqueBST bst;
		Node* prev = NULL;
		for (Node* current = list; current != NULL; current = prev->next)
			if (!bst.addUniqueElement(current->value))
			{
				prev->next = current->next;
				delete current;
			}
			else
				prev = current;
	}

	void printList(Node* head)
	{
		cout << "\nElements of list: ";
		for (; head != NULL; head = head->next)
			cout << head->value << ", ";
	}

	void testRemoveDeplicate()
	{
		cout << "\n------------------ testRemoveDeplicate ------------------";
		const int testcases = 3;
		const int size = 10;
		int array[testcases][size] = {
			{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
			{ 5, 2, 9, 2, 4, 4, 4, 5, 5, 2 },
			{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }
		};
		for (int i = 0; i < testcases; i++)
		{
			Node* head = createList(array[i], 10);
			printList(head);
			cout << "\nFirst approach: ";
			Node* newList = removeDuplicate(head);
			printList(newList);
			cout << "\nSecond approach: ";
			removeDuplicateBestApproach(head);
			printList(head);
		}
	}

	//---------------------------------- Question 3 ----------------------------------

	class Complex
	{
	public:
		Complex(double realpart, double ImagPart)
			:m_realPart(realpart), m_imagPart(ImagPart)
		{
			cout << "\n Calling constructor";
		}

		~Complex()
		{
			cout << "\n Calling destructor";
		}

		Complex(const Complex& obj)
			:m_realPart(obj.m_realPart), m_imagPart(obj.m_imagPart)
		{
			cout << "\n Calling copy constructor";
		}

		friend const Complex operator+(const Complex& lhs, const Complex& rhs)
		{
			cout << "\n Calling operator+";
			return Complex(lhs.m_realPart + rhs.m_realPart, lhs.m_imagPart + rhs.m_imagPart);
		}

		void print()
		{
			cout << "\n Real = " << m_realPart << " imag = " << m_imagPart;
		}

	private:
		double m_realPart;
		double m_imagPart;

	};

	void testComplex()
	{
		cout << "\n------------------ testComplex ------------------";
		Complex a(3, 4), b(6, 7);
		Complex c = a + b;
		c.print();
	}

}

	/*

	//------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------

	// This is the text editor interface.
	// Anything you type or change here will be seen by the other person in real time.

	// 100F = 4111
	int getValue(char ch)
	{
	if (ch >= '0' && ch <= '9')
	return (ch - '0');
	switch (ch)
	{
	case 'A': return 10;
	case 'B': return 11;
	case 'C': return 12;
	case 'D': return 13;
	case 'E': return 14;
	case 'F': return 15;
	}
	}

	unsigned int hex2int(const char * hex)
	{
	if (hex == nullptr)
	return 0;

	int retval = 0;
	int len = strlen(hex);
	for (int i = len - 1; i >= 0; i--)
	{
	retVal += getValue(hex[i]) * pow(16, len - i - 1);
	}

	return retval;
	}

	int main()
	{
	int val = hex2int("100F");
	cout << "\nInteger value = " << val;
	}





	// This is the text editor interface.
	// Anything you type or change here will be seen by the other person in real time.

	write a function which removes all the duplicate values in the single linked list.
	Hint: you can use any additional data structure to store some temporary values.

	struct Node
	{
	int value;
	Node* next;
	}

	Node* removeDuplicate(Node* list)
	{
	Node* returnHead = NULL;

	for (Node* start = list; start != NULL; start = start->next)
	{
	Node* temp = returnHead;
	while (true)
	{
	if (temp = NULL)
	{
	//Return list is not yet created
	Node* newNode = new Node;
	newNode->value = start->value;
	newNode->next = NULL;
	returnHead = newNode;
	break;
	}
	else if (start->value == temp->value)
	{
	found = true;
	break;
	}
	else if (temp->next == NULL)
	{
	//We did not find the value, so add it
	Node* newNode = new Node;
	newNode->value = start->value;
	newNode->next = NULL;
	//Append it to return list
	temp->next = newNode;
	break;
	}
	if (temp != NULL)
	temp = temp->next;
	}
	}

	return returnHead;
	}

	1->2->3->3->3->2 - 5

	void removeDuplicate(Node* list)
	{
	node* last = NULL;
	for (Node* current = list; current != NULL; current = current->next)
	{
	bool deleted = false;
	for (Node* temp = list; temp != current; temp = temp->next)
	{
	if (currrent->value == temp->value)
	{
	//remove current from list
	last->next = last->next->next;
	deleted = true;
	break;
	}
	}
	if (!deleted)
	last = current;
	}
	}





	// This is the text editor interface.
	// Anything you type or change here will be seen by the other person in real time.

	Define the class Complex

	class Complex
	{
	public:
	Complex(double realpart, double ImagPart)
	:m_realPart(realpart), m_imagPart(ImagPart)
	{

	}

	~Complex()
	{}

	Complex(const Complex& obj)
	{
	m_realPart = obj.m_realPart;
	m_imagPart = obj.m_imagPart;
	}

	friend const Complex operator+(const Complex& lhs, const Complex& rhs)
	{
	return Complex(lhs.m_realPart + rhs.m_realPart, lhs.m_imagPart + rhs.m_imagPart);
	}

	private:
	double m_realPart;
	double m_imagPart;

	};

	Complex a(1, 10);
	Complex b(2, 3);
	Complex c = a + b;

	(a + b) = c

	*/


	/*

	//Using recursion
	double power(double base, int exponent)
	{
	if(base == 0) return 0; //if(base == 0 && exponent == 0) return 0; is also covered in this condition
	if(exponent == 0) return 1;
	if(exponent == 1) return base; //Break condition for recursion

	if(exponent < 0)
	return 1 / power(base, -exponent);
	else if(exponent % 2 != 0)
	return base * power(base * base, exponent/2);
	else
	return power(base * base, exponent/2);
	}

	//Using loop
	double power(double base, int exponent)
	{
	if(base == 0) return 0; //if(base == 0 && exponent == 0) return 0; is also covered in this condition
	if(exponent == 0) return 1;
	if(exponent == 1) return base; //Break condition for recursion
	bool isExponentNegative = false;
	if(exponent < 0)
	{
	isExponentNegative = false;
	exponent = -exponent;
	}
	bol isExponentOdd = false;
	if(exponent % 2 != 0)
	{
	isExponentOdd = true;
	exponent = exponent - 1;
	}
	double result = 1;
	for(; exponent > 1; exponent /= 2)
	{
	result *= base * base;
	}
	if(isExponentOdd == true)
	result = base * result;
	if(isExponentNegative == false)
	result = 1/result;

	return result;
	}

	*/


	/*
	//------- Question 1 --------

	Find the number of homes to rob so that a robber can get maximum amount in total from the houses numbered 1 to n
	having some fixed amount in the every home which is known to robber. The condition is that the robber can not
	rob any two adjucent homes.

	Hint: Use dynamic programming

	*/

	/*
	//------- Question 2 --------

	http://www.geeksforgeeks.org/find-the-k-most-frequent-words-from-a-file/
	Find the k most frequent words from a file
	Given a book of words. Assume you have enough main memory to accommodate all words. design a data structure to find
	top K maximum occurring words. The data structure should be dynamic so that new words can be added.

	A simple solution is to use Hashing. Hash all words one by one in a hash table. If a word is already present,
	then increment its count. Finally, traverse through the hash table and return the k words with maximum counts.

	We can use Trie and Min Heap to get the k most frequent words efficiently. The idea is to use Trie for searching
	existing words adding new words efficiently. Trie also stores count of occurrences of words. A Min Heap of size k is
	used to keep track of k most frequent words at any point of time(Use of Min Heap is same as we used it to
	find k largest elements in this post).
	Trie and Min Heap are linked with each other by storing an additional field in Trie ‘indexMinHeap’ and
	a pointer ‘trNode’ in Min Heap. The value of ‘indexMinHeap’ is maintained as -1 for the words which are
	currently not in Min Heap (or currently not among the top k frequent words). For the words which are present
	in Min Heap, ‘indexMinHeap’ contains, index of the word in Min Heap. The pointer ‘trNode’ in Min Heap points
	to the leaf node corresponding to the word in Trie.

	Following is the complete process to print k most frequent words from a file.

	Read all words one by one. For every word, insert it into Trie. Increase the counter of the word, if already exists.
	Now, we need to insert this word in min heap also. For insertion in min heap, 3 cases arise:

	1. The word is already present. We just increase the corresponding frequency value in min heap and call minHeapify()
	for the index obtained by “indexMinHeap” field in Trie. When the min heap nodes are being swapped, we change the
	corresponding minHeapIndex in the Trie. Remember each node of the min heap is also having pointer to Trie leaf node.

	2. The minHeap is not full. we will insert the new word into min heap & update the root node in the min heap node &
	min heap index in Trie leaf node. Now, call buildMinHeap().

	3. The min heap is full. Two sub-cases arise.
	….3.1 The frequency of the new word inserted is less than the frequency of the word stored in the head of min heap. Do nothing.

	….3.2 The frequency of the new word inserted is greater than the frequency of the word stored in the head of min heap.
	Replace & update the fields. Make sure to update the corresponding min heap index of the “word to be replaced” in Trie
	with -1 as the word is no longer in min heap.

	4. Finally, Min Heap will have the k most frequent words of all words present in given file. So we just need to print
	all words present in Min Heap.

	// A program to find k most frequent words in a file
	#include <stdio.h>
	#include <string.h>
	#include <ctype.h>

	# define MAX_CHARS 26
	# define MAX_WORD_SIZE 30

	// A Trie node
	struct TrieNode
	{
	bool isEnd; // indicates end of word
	unsigned frequency;  // the number of occurrences of a word
	int indexMinHeap; // the index of the word in minHeap
	TrieNode* child[MAX_CHARS]; // represents 26 slots each for 'a' to 'z'.
	};

	// A Min Heap node
	struct MinHeapNode
	{
	TrieNode* root; // indicates the leaf node of TRIE
	unsigned frequency; //  number of occurrences
	char* word; // the actual word stored
	};

	// A Min Heap
	struct MinHeap
	{
	unsigned capacity; // the total size a min heap
	int count; // indicates the number of slots filled.
	MinHeapNode* array; //  represents the collection of minHeapNodes
	};

	// A utility function to create a new Trie node
	TrieNode* newTrieNode()
	{
	// Allocate memory for Trie Node
	TrieNode* trieNode = new TrieNode;

	// Initialize values for new node
	trieNode->isEnd = 0;
	trieNode->frequency = 0;
	trieNode->indexMinHeap = -1;
	for( int i = 0; i < MAX_CHARS; ++i )
	trieNode->child[i] = NULL;

	return trieNode;
	}

	// A utility function to create a Min Heap of given capacity
	MinHeap* createMinHeap( int capacity )
	{
	MinHeap* minHeap = new MinHeap;

	minHeap->capacity = capacity;
	minHeap->count  = 0;

	// Allocate memory for array of min heap nodes
	minHeap->array = new MinHeapNode [ minHeap->capacity ];

	return minHeap;
	}

	// A utility function to swap two min heap nodes. This function
	// is needed in minHeapify
	void swapMinHeapNodes ( MinHeapNode* a, MinHeapNode* b )
	{
	MinHeapNode temp = *a;
	*a = *b;
	*b = temp;
	}

	// This is the standard minHeapify function. It does one thing extra.
	// It updates the minHapIndex in Trie when two nodes are swapped in
	// in min heap
	void minHeapify( MinHeap* minHeap, int idx )
	{
	int left, right, smallest;

	left = 2 * idx + 1;
	right = 2 * idx + 2;
	smallest = idx;
	if ( left < minHeap->count &&
	minHeap->array[ left ]. frequency <
	minHeap->array[ smallest ]. frequency
	)
	smallest = left;

	if ( right < minHeap->count &&
	minHeap->array[ right ]. frequency <
	minHeap->array[ smallest ]. frequency
	)
	smallest = right;

	if( smallest != idx )
	{
	// Update the corresponding index in Trie node.
	minHeap->array[ smallest ]. root->indexMinHeap = idx;
	minHeap->array[ idx ]. root->indexMinHeap = smallest;

	// Swap nodes in min heap
	swapMinHeapNodes (&minHeap->array[ smallest ], &minHeap->array[ idx ]);

	minHeapify( minHeap, smallest );
	}
	}

	// A standard function to build a heap
	void buildMinHeap( MinHeap* minHeap )
	{
	int n, i;
	n = minHeap->count - 1;

	for( i = ( n - 1 ) / 2; i >= 0; --i )
	minHeapify( minHeap, i );
	}

	// Inserts a word to heap, the function handles the 3 cases explained above
	void insertInMinHeap( MinHeap* minHeap, TrieNode** root, const char* word )
	{
	// Case 1: the word is already present in minHeap
	if( (*root)->indexMinHeap != -1 )
	{
	++( minHeap->array[ (*root)->indexMinHeap ]. frequency );

	// percolate down
	minHeapify( minHeap, (*root)->indexMinHeap );
	}

	// Case 2: Word is not present and heap is not full
	else if( minHeap->count < minHeap->capacity )
	{
	int count = minHeap->count;
	minHeap->array[ count ]. frequency = (*root)->frequency;
	minHeap->array[ count ]. word = new char [strlen( word ) + 1];
	strcpy( minHeap->array[ count ]. word, word );

	minHeap->array[ count ]. root = *root;
	(*root)->indexMinHeap = minHeap->count;

	++( minHeap->count );
	buildMinHeap( minHeap );
	}

	// Case 3: Word is not present and heap is full. And frequency of word
	// is more than root. The root is the least frequent word in heap,
	// replace root with new word
	else if ( (*root)->frequency > minHeap->array[0]. frequency )
	{

	minHeap->array[ 0 ]. root->indexMinHeap = -1;
	minHeap->array[ 0 ]. root = *root;
	minHeap->array[ 0 ]. root->indexMinHeap = 0;
	minHeap->array[ 0 ]. frequency = (*root)->frequency;

	// delete previously allocated memoory and
	delete [] minHeap->array[ 0 ]. word;
	minHeap->array[ 0 ]. word = new char [strlen( word ) + 1];
	strcpy( minHeap->array[ 0 ]. word, word );

	minHeapify ( minHeap, 0 );
	}
	}

	// Inserts a new word to both Trie and Heap
	void insertUtil ( TrieNode** root, MinHeap* minHeap,
	const char* word, const char* dupWord )
	{
	// Base Case
	if ( *root == NULL )
	*root = newTrieNode();

	//  There are still more characters in word
	if ( *word != '\0' )
	insertUtil ( &((*root)->child[ tolower( *word ) - 97 ]),
	minHeap, word + 1, dupWord );
	else // The complete word is processed
	{
	// word is already present, increase the frequency
	if ( (*root)->isEnd )
	++( (*root)->frequency );
	else
	{
	(*root)->isEnd = 1;
	(*root)->frequency = 1;
	}

	// Insert in min heap also
	insertInMinHeap( minHeap, root, dupWord );
	}
	}


	// add a word to Trie & min heap.  A wrapper over the insertUtil
	void insertTrieAndHeap(const char *word, TrieNode** root, MinHeap* minHeap)
	{
	insertUtil( root, minHeap, word, word );
	}

	// A utility function to show results, The min heap
	// contains k most frequent words so far, at any time
	void displayMinHeap( MinHeap* minHeap )
	{
	int i;

	// print top K word with frequency
	for( i = 0; i < minHeap->count; ++i )
	{
	printf( "%s : %d\n", minHeap->array[i].word,
	minHeap->array[i].frequency );
	}
	}

	// The main funtion that takes a file as input, add words to heap
	// and Trie, finally shows result from heap
	void printKMostFreq( FILE* fp, int k )
	{
	// Create a Min Heap of Size k
	MinHeap* minHeap = createMinHeap( k );

	// Create an empty Trie
	TrieNode* root = NULL;

	// A buffer to store one word at a time
	char buffer[MAX_WORD_SIZE];

	// Read words one by one from file.  Insert the word in Trie and Min Heap
	while( fscanf( fp, "%s", buffer ) != EOF )
	insertTrieAndHeap(buffer, &root, minHeap);

	// The Min Heap will have the k most frequent words, so print Min Heap nodes
	displayMinHeap( minHeap );
	}

	// Driver program to test above functions
	int main()
	{
	int k = 5;
	FILE *fp = fopen ("file.txt", "r");
	if (fp == NULL)
	printf ("File doesn't exist ");
	else
	printKMostFreq (fp, k);
	return 0;
	}
	Run on IDE
	Output:

	your : 3
	well : 3
	and : 4
	to : 4
	Geeks : 6
	The above output is for a file with following content.


	----------- second method ---------------

	--- first data structure ---

	use unordered_map WordFrequency (not unordered_multimap as we need unique entry of each word) to store word-frequency mapping
	i.e. key-value pair will be (String*, int)
	Read each NewWord from file, search it in WordFrequency, get OldFrequency. (OldFrequency = 0 if NewWord is not in WordFrequency)
	Add this NewWord into unordered_map WordFrequency.

	NewWord = new String(Read next word from file stream);
	OldFrequency = 0;
	if((Iterator it = WordFrequency.find(NewWord)) != WordFrequency.end())
	{
	OldFrequency = it.second;
	it.second = it.second + 1;
	}
	else
	WordFrequency[NewWord] = 1;

	--- second data structure ---

	Store frequency-word mapping (int, String*) in multimap TopKWords so that it is sorted. For top k words by frequency,
	store the number of elements in map TopKWords, say NumberOfTopWords, and the lowest frequency of words available
	in TopKWords, say MinFrequency.
	Initially NumberOfTopWords = 0, MinFrequency = 0.

	if(OldFrequency >= MinFrequency)
	{
	find all the elements from multimap with key OldFrequency
	pair<iterator,iterator> iteratorRange = TopKWords.equal_range (const key_type& k)
	Traverse through above range to find if the word already exists
	if word already exist, remove that entry from map TopKWords
	Add new entry key-value pair (OldFrequency + 1, NewWord) into map TopKWords
	NumberOfTopWords = TopKWords.size();
	if(NumberOfTopWords >= k)
	{
	remove first entry from TopKWords
	MinFrequency = key of first entry in map TopKWords (assuming keys are arranged in assending order)
	}
	}

	----------- third method ---------------

	Above problem is equivalent to having a table in database with two rows- Words and Frequency
	If we have indexing on both, we can get the top k most frequent words. It is as good as using second method.
	But in case of database, it may be further optimized as we dont dumplicate the data
	Second method is optimized to index all data by words but to index only top k words by frequency

	If column1 and column2 are indexed, then it will have two multimaps
	Data* pData = new Data(Column1, Column2, Column3, Column4, ...);
	multimap<column1, Data*> DS1;
	multimap<column2, Column1> DS2;

	When any column Column3 onwards is updated, it is simply updated in Data and there is no change in DS1 and DS3
	When Column1 is updated, it is updated in Data, and then old value of Column1 is removed from DS1 and
	new value of Column1 is inserted into DS1. Also all the maps like DS2 where column1 is referred are modified.
	Similarly, When Column2 is updated, it is updated in Data, and then old value of Column2 is removed from DS2 and
	new value of Column2 is inserted into DS2

	Above is simpler representation of the Data Structure behind table in Database.
	It actually used B+ tree:
	https://en.wikipedia.org/wiki/B%2B_tree

	database is a set of tables, and table is a set of indexes. And each index is, essentially, a B+ tree storing its data.


	----------- Forth method ---------------

	We can not use min heap as shown in first method, because min heap has to be destroyed to get top k words.
	If we want to get top k words anytime while reading file is not completed, we should use different data structure.

	unordered_map<String*, Frequency*>
	struct Frequency
	{

	}

	*/

/*
	void FindTopKWordsInFile(const string& iFile, const int ReqTopWords = -1)
	{
		cout << "\nReading file: " << iFile;
		ifstream inputFile(iFile);

		std::unordered_map<string, int> WordFrequency;
		std::multimap<int, const string*> TopKWords;
		int NumberOfTopWords = 0, MinFrequency = 0;

		if (inputFile.is_open())
		{
			int keyLen = 256;
			char* inputString = new char[keyLen];
			string line;

			//while (!inputFile.eof())
			//{
			//	//ifstream temp = inputFile.getline(&inputString[0], keyLen);
			//	//inputFile.getline(line, keyLen);
			//	//if (!inputFile.getline(&inputString[0], keyLen).fail())
			//	streamoff currPos = inputFile.tellg();
			//	if (inputFile.getline(&inputString[0], keyLen))
			//		cout << "\nLine: " << inputString;
			//	else
			//	{
			//		cout << "\nLine: " << inputString;
			//		inputFile.clear();
			//		inputFile.seekg(keyLen, currPos);
			//	}
			//}

			while (std::getline(inputFile, line).good())
			{
				//cout << "\n" << line;

				std::stringstream strstr(line);

				// use stream iterators to copy the stream to the vector as whitespace separated strings
				std::istream_iterator<std::string> it(strstr);
				std::istream_iterator<std::string> end;
				std::vector<std::string> results(it, end);
				//for each (string word in results)
				for (string word : results)
				{
					//cout << "\nWords: " << word;

					int OldFrequency = 0;
					std::unordered_map<string, int>::iterator it = WordFrequency.find(word);
					if (it != WordFrequency.end())
					{
						OldFrequency = it->second;
						it->second = it->second + 1;
					}
					else
					{
						// In this case, there is no old key, so insert a new key-value pair
						//WordFrequency[word] = OldFrequency + 1;
						pair<std::unordered_map<string, int>::iterator, bool> retPair = WordFrequency.insert(pair<string, int>(word, OldFrequency + 1));
						if (retPair.second == true) //No old/already existing key found
							it = retPair.first;
						else
							cout << "\nERROR: TODO: Handle this case. (Ideally, we shoudl not fall here.)";
					}

					const string* pWord = &(it->first);
					if (OldFrequency >= MinFrequency)
					{
						//find all the elements from multimap with key OldFrequency
						pair<std::multimap<int, const string*>::iterator, std::multimap<int, const string*>::iterator> iteratorRange = TopKWords.equal_range(OldFrequency);
						//Traverse through above range to find if the word already exists
						//if word already exist, remove that entry from map TopKWords
						//Add new entry key - value pair(OldFrequency + 1, NewWord) into map TopKWords
						for (std::multimap<int, const string*>::iterator itStart = iteratorRange.first; itStart != iteratorRange.second; ++itStart)
						{
							if (itStart->second == pWord)
							{
								TopKWords.erase(itStart);
								//Ideally there should be only one entry, so break here
								break;
							}
						}

						TopKWords.insert(std::pair<int, const string*>(OldFrequency + 1, pWord));

						if (ReqTopWords != -1)
						{
							NumberOfTopWords = TopKWords.size();
							if (NumberOfTopWords >= ReqTopWords)
							{
								//remove first entry from TopKWords
								TopKWords.erase(TopKWords.begin());
								//MinFrequency = key of first entry in map TopKWords(assuming keys are arranged in assending order)
								MinFrequency = TopKWords.begin()->first;
							}
						}
					}
				}

			}

			cout << "\n\nThe top words:\n";
			for (std::multimap<int, const string*>::reverse_iterator it = TopKWords.rbegin(); it != TopKWords.rend(); it++)
				cout << "\n" << it->first << " " << *(it->second);

			inputFile.close();
		}
		else
			cout << "\nCould not open input file " << iFile;
	}

	void testFindTopKWordsInFile()
	{
		//FindTopKWordsInFile("E:\\@_Programming\\Resumes\\Cover Letter.txt", 10);
		FindTopKWordsInFile("E:\\@_Programming\\Resumes\\My Resume\\8 Pages Removed HMT and M&M\\Maruti_13_Yrs_C++_Experience.txt");
	}

	//------------------ testing ----------------------------------

	void test2()
	{
		//testHexToDecomal();
		//testRemoveDeplicate();
		//testComplex();

		testFindTopKWordsInFile();
	}
	
}

*/


/*

RBC:


select group_id, player_id
from
(
	select group_id, P.player_id,
	row_number() over(partition by P.group_id order by score_board.total_score desc) as row_num
	from
	(
		select player_id, sum(score) as total_score
		from
		(
			select first_player as player_id, first_score as score
			from matches
			union all
			select second_player as player_id, second_score as score
			from matches
		) as A
		group by player_id
	) as score_board
	full join players P
	on score_board.player_id = P.player_id
) as final_table
where row_num = 1
order by group_id asc;




select group_id, player_id
from
(
	select group_id, P.player_id,
	row_number() over(partition by P.group_id order by S.total_score desc) as row_num
	from
	(
		select player_id, sum(score) as total_score
		from
		(
			select first_player as player_id, first_score as score
			from matches
			union all
			select second_player as player_id, second_score as score
			from matches
		) as A
		group by A.player_id
	) as S
	right join players P
	on S.player_id = P.player_id
) as FINAL
where row_num = 1
order by group_id asc;





select FINAL.group_id, FINAL.player_id
from
(
	select group_id, P.player_id,
	row_number() over(partition by P.group_id order by S.total_score desc) as row_num
	from
	(
		select player_id, sum(score) as total_score
		from
		(
			select first_player as player_id, first_score as score
			from matches
			union all
			select second_player as player_id, second_score as score
			from matches
		) as A
		group by A.player_id
	) as S
	right join players P
	on S.player_id = P.player_id
) as FINAL
where FINAL.row_num = 1
order by FINAL.group_id asc;


*/

/*

Puzzles:

1. n windows, initially all closed, persons 1 to n goes and toggle windows with index multiple of 1 to n. Which windows will be open/closed at end?
2. n persons in a circular queue, first kills 2nd and passes sword to 3rd....and so on....who is saved at end?
3. a tetrahedron formed by selecting 4 random points on the surface of sphere, what is probability that the center of sphere lies inside tetrahedron = 1/8

*/





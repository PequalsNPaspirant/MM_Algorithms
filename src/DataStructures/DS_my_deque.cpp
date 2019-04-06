//Goal
//Implement deque (D-ouble E-nded QUE-ue = D-E-QUE)

#include <iostream>
using namespace std;

// --------------------- using array


/*
// Deque
0:       ++
1: ++++++++
2: ++++++++
3: ++++++++
4: +++++

// Insert O into Deque
0:      +++
1: ++++++++
2: +O++++++
3: ++++++++
4: +++++

In the worst case, inserting a single element into a deque takes time linear in the minimum of the distance 
from the insertion point to the beginning of the deque and the distance from the insertion point to the end of the deque.

bool size() const { return first.size + (buckets.size() - 2) * SIZE + last.size; }

T& operator[](size_t i) {
	if (i < first.size) { return first[SIZE - i]; }

	size_t const correctedIndex = i - first.size;
	return buckets[correctedIndex / SIZE][correctedIndex % SIZE];
}
*/

// ---------------------- using linked list

namespace mm {

	template <class T>
	class Node {

	public:
		Node();
		Node(T item) :next(NULL), prev(NULL), data(item) { }
		~Node() { prev = next = NULL; }

		T data;
		Node<T> *next;
		Node<T> *prev;
	};

	class DequeEmptyException {
	public:
		DequeEmptyException() {
			cout << "Deque is empty" << endl;
		}

		~DequeEmptyException() { }
	};

	template <class T>
	class Deque {
	public:
		Deque() :front(NULL), rear(NULL), count(0) {}

		int AddFront(const T element);
		T RemoveFront();
		int AddBack(const T element);
		T RemoveBack();

		int Size() const;

		T Front() const;
		T Back() const;

		int IsEmpty() const;

	private:
		Node<T> *front;
		Node<T> *rear;
		int count;

	};

	template <class T>
	int Deque<T>::IsEmpty() const {
		return count == 0 ? 1 : 0;
	}

	template <class T>
	int Deque<T>::Size() const {
		return count;
	}

	template <class T>
	T Deque<T>::Front() const {
		if (IsEmpty())
			throw new DequeEmptyException();

		return front->data;
	}

	template <class T>
	T Deque<T>::Back() const {
		if (IsEmpty())
			throw new DequeEmptyException();

		return rear->data;
	}

	template <class T>
	int Deque<T>::AddFront(const T element) {

		Node<T> *newNode = new Node<T>(element);

		cout << "item: " << newNode->data << endl;

		if (IsEmpty()) {
			front = rear = newNode;
			++count;
			return 0;
		}
		newNode->next = front;
		front->prev = newNode;
		front = newNode;
		++count;

		return 0;
	}

	template <class T>
	T Deque<T>::RemoveFront() {

		if (IsEmpty()) {
			throw new DequeEmptyException();
		}

		T retVal = front->data;

		Node<T> *tmp = front;
		if (front->next != NULL) {
			front = front->next;
			front->prev = NULL;
		}
		else {
			front = NULL;
		}
		count--;
		delete tmp;
		tmp = NULL;

		return retVal;
	}

	template <class T>
	int Deque<T>::AddBack(const T element) {

		Node<T> *newNode = new Node<T>(element);

		if (IsEmpty()) {
			front = rear = newNode;
			++count;
			return 0;
		}
		// append to the list
		rear->next = newNode;
		newNode->prev = rear;
		rear = newNode;
		count++;
	}

	template <class T>
	T Deque<T>::RemoveBack() {

		if (IsEmpty()) {
			throw new DequeEmptyException();
		}

		T retVal = rear->data;

		// delete the back node
		Node<T> *tmp = rear;

		if (rear->prev != NULL) {
			rear = rear->prev;
			rear->next = NULL;
		}
		else {
			rear = NULL;
		}
		count--;
		delete tmp;
		tmp = NULL;

		return retVal;
	}

}
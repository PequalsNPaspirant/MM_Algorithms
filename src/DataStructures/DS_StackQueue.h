//Goal
//All about stack and queue

#include <iostream>
#include <vector>
#include <string>
using namespace std;

namespace mm {

	class StackusingArray
	{
	public:
		StackusingArray(int size = 10)
			:m_capacity(size), increment(size), m_top(-1)
		{
			m_elements = new int[m_capacity];
		}

		~StackusingArray()
		{
			delete[] m_elements;
		}

		void push(int numIn)
		{
			if (++m_top == m_capacity)
			{
				cout << "\nStack overflow. Increasing the capacity.";
				m_capacity = m_capacity + increment;
				int* temp = new int[m_capacity];
				for (int i = 0; i <= m_top; i++)
					temp[i] = m_elements[i];
				delete[] m_elements;
				m_elements = temp;
			}

			m_elements[m_top] = numIn;

			cout << "\n\nNew stack elements after pushing " << numIn << " = ";
			for (int i = 0; i <= m_top; i++)
				cout << m_elements[i] << " ";
			cout << " Stack Size = " << getSize();
		}

		int pop()
		{
			cout << "\n\nNew stack elements after popping = ";
			int retvalue = -1;
			if (m_top >= 0)
			{
				retvalue = m_elements[m_top];
				--m_top;
				cout << "(popped " << retvalue << ") ";
				for (int i = 0; i <= m_top; i++)
					cout << m_elements[i] << " ";
			}
			else
				cout << "ERROR: Stack is empty (Stack underflow)";

			cout << " Stack Size = " << getSize();
			return retvalue;
		}

		int getSize()
		{
			return m_top + 1;
		}

	private:
		int* m_elements;
		int m_top;
		int m_capacity;
		const int increment;
	};

	void testPushPopForStackUsingArray()
	{
		cout << "\n\n--------------------------------testPushPopForStackUsingArray--------------------------------------";
		StackusingArray stack;
		stack.pop();
		stack.push(1);
		stack.push(2);
		stack.pop();
		stack.pop();
		stack.pop();
		stack.pop();
		stack.push(3);
		stack.push(5);
		stack.push(7);
		stack.push(9);
		stack.push(11);
		stack.pop();
		stack.pop();
	}

	//----------------------------------------------------------------------

	class StackUsingLinkList
	{
	public:
		StackUsingLinkList(int size = 10)
			:m_head(nullptr), m_size(0)
		{
		}

		~StackUsingLinkList()
		{
			for (Node* node = m_head; node != nullptr;)
			{
				Node* toDelete = node;
				node = node->next;
				delete toDelete;
			}
			m_size = 0;
			m_head = nullptr;
		}

		//Push the new element at head's position so that pop is easy
		void push(int numIn)
		{
			++m_size;
			Node* newNode = new Node;
			newNode->value = numIn;
			newNode->next = m_head;
			m_head = newNode;

			cout << "\n\nNew stack elements after pushing " << numIn << " = ";
			for (Node* node = m_head; node != nullptr; node = node->next)
				cout << node->value << " ";
			cout << " Stack Size = " << getSize();
		}

		int pop()
		{
			cout << "\n\nNew stack elements after popping = ";
			if (m_head = nullptr)
			{
				cout << "ERROR: Stack is empty";
				return -1;
			}

			--m_size;
			int retvalue = m_head->value;
			Node* toDelete = m_head;
			m_head = m_head->next;
			delete toDelete;

			cout << "(popped " << retvalue << ") ";
			for (Node* node = m_head; node != nullptr; node = node->next)
				cout << node->value << " ";

			cout << " Stack Size = " << getSize();
			return retvalue;
		}

		int getSize()
		{
			return m_size;
		}

	private:
		struct Node
		{
			int value;
			Node* next;
		};

		Node* m_head;
		int m_size;
	};

	void testPushPopForStackUsingLinkList()
	{
		cout << "\n\n--------------------------------testPushPopForStackUsingLinkList--------------------------------------";
		StackusingArray stack;
		stack.pop();
		stack.push(1);
		stack.push(2);
		stack.pop();
		stack.pop();
		stack.pop();
		stack.pop();
		stack.push(3);
		stack.push(5);
		stack.push(7);
		stack.push(9);
		stack.push(11);
		stack.pop();
		stack.pop();
	}

	//----------------------------------------------------------------------

	class QueueUsingArrayRotating
	{
	public:
		QueueUsingArrayRotating(int size = 10)
			:m_front(-1), m_rear(-1), m_capacity(size), increment(size), m_size(0)
		{
			m_elements = new int[m_capacity];
		}

		~QueueUsingArrayRotating()
		{
			delete[] m_elements;
		}

		void enque(int numIn)
		{
			m_rear = rotationalIncrement(m_rear);
			if (m_front == m_rear)
			{
				cout << "\nQueue overflow. Increasing the capacity.";
				int* temp = new int[m_capacity + increment];
				int j = 0;
				for (int i = m_front; j < m_size; i = rotationalIncrement(i), j++)
					temp[j] = m_elements[i];
				m_capacity = m_capacity + increment;
				m_front = 0;
				m_rear = j;
				delete[] m_elements;
				m_elements = temp;
			}

			++m_size;
			m_elements[m_rear] = numIn;
			if (m_front == -1) m_front = 0;

			cout << "\n\nNew Queue elements after enque() " << numIn << " = ";
			for (int i = m_front, j = 0; j < m_size; i = rotationalIncrement(i), j++)
				cout << m_elements[i] << " ";

			cout << " Queue Size = " << getSize();
		}

		int deque()
		{
			cout << "\n\nNew Queue elements after deque() = ";
			int retvalue = -1;
			if (m_front < 0)
			{
				cout << "ERROR: Queue is empty";
				return retvalue;
			}

			retvalue = m_elements[m_front];
			cout << "(popped " << retvalue << ") ";
			--m_size;
			if (m_front == m_rear)
				m_front = m_rear = -1;
			else
			{
				m_front = rotationalIncrement(m_front);
				for (int i = m_front, j = 0; j < m_size; i = rotationalIncrement(i), j++)
					cout << m_elements[i] << " ";
			}

			cout << " Queue Size = " << getSize();
			return retvalue;
		}

		int rotationalIncrement(int index)
		{
			if (++index >= m_capacity)
				return index - m_capacity;
			else
				return index;
		}

		//int rotationalDecrement(int index)
		//{
		//	if (--index < 0)
		//		return m_capacity - index;
		//	else
		//		return index;
		//}

		int getSize()
		{
			//return m_size;
			if (m_front == -1)
				return 0;
			else
				return (m_rear >= m_front ? m_rear - m_front + 1 : m_capacity - (m_front - m_rear) + 1);
		}

	private:
		int* m_elements;
		int m_front;
		int m_rear;

		int m_size;
		int m_capacity;
		const int increment;

	};

	void testQueueDequeForQueueUsingArrayRotating()
	{
		cout << "\n\n--------------------------------testQueueDequeForQueueUsingArrayRotating--------------------------------------";
		QueueUsingArrayRotating queue;
		queue.deque();
		queue.enque(1);
		queue.enque(2);
		queue.deque();
		queue.deque();
		queue.deque();
		queue.deque();
		queue.enque(3);
		queue.enque(5);
		queue.enque(7);
		queue.enque(9);
		queue.enque(11);
		queue.deque();
		queue.deque();
		queue.enque(12);
		queue.enque(13);
		queue.enque(14);
		queue.enque(15);
		queue.enque(16);
		queue.enque(17);
		queue.enque(18);
		queue.enque(19);
		queue.enque(20);
		queue.enque(21);
		queue.enque(22);
		queue.enque(23);
		queue.deque();
		queue.deque();
	}

	//----------------------------------------------------------------------

	class QueueUsingArray
	{
	public:
		QueueUsingArray(int size = 10)
			:m_front(-1), m_rear(-1), m_capacity(size), increment(size), m_size(0)
		{
			m_elements = new int[m_capacity];
		}

		~QueueUsingArray()
		{
			delete[] m_elements;
		}

		void enque(int numIn)
		{
			++m_rear;
			if (m_rear >= m_capacity)
			{
				cout << "\nQueue overflow. Increasing the capacity.";
				m_capacity = m_capacity + increment;
				int* temp = new int[m_capacity];
				int j = 0;
				for (int i = m_front; j <= m_rear; i++, j++)
					temp[j] = m_elements[i];

				m_front = 0;
				m_rear = j;
				delete[] m_elements;
				m_elements = temp;
			}

			++m_size;
			m_elements[m_rear] = numIn;
			if (m_front == -1) m_front = 0;

			cout << "\n\nNew Queue elements after enque() " << numIn << " = ";
			for (int i = m_front; i <= m_rear; i++)
				cout << m_elements[i] << " ";

			cout << " Queue Size = " << getSize();
		}

		int deque()
		{
			cout << "\n\nNew Queue elements after deque() = ";
			int retvalue = -1;
			if (m_front < 0)
			{
				cout << "ERROR: Queue is empty";
				return retvalue;
			}

			retvalue = m_elements[m_front];
			cout << "(popped " << retvalue << ") ";
			--m_size;
			if (m_front == m_rear)
				m_front = m_rear = -1;
			else
			{
				++m_front;
				for (int i = m_front; i < m_rear; i++)
					cout << m_elements[i] << " ";
			}

			cout << " Queue Size = " << getSize();
			return retvalue;
		}

		int getSize()
		{
			//return m_size;
			return m_rear - m_front + 1;
		}

	private:
		int* m_elements;
		int m_front;
		int m_rear;

		int m_size;
		int m_capacity;
		const int increment;

	};

	void testQueueDequeForQueueUsingArray()
	{
		cout << "\n\n--------------------------------testQueueDequeForQueueUsingArray--------------------------------------";
		QueueUsingArrayRotating queue;
		queue.deque();
		queue.enque(1);
		queue.enque(2);
		queue.deque();
		queue.deque();
		queue.deque();
		queue.deque();
		queue.enque(3);
		queue.enque(5);
		queue.enque(7);
		queue.enque(9);
		queue.enque(11);
		queue.deque();
		queue.deque();
		queue.enque(12);
		queue.enque(13);
		queue.enque(14);
		queue.enque(15);
		queue.enque(16);
		queue.enque(17);
		queue.enque(18);
		queue.enque(19);
		queue.enque(20);
		queue.enque(21);
		queue.enque(22);
		queue.enque(23);
		queue.deque();
		queue.deque();
	}

	//----------------------------------------------------------------------

	void StackQueue()
	{
		//testPushPopForStackUsingArray();
		//testPushPopForStackUsingLinkList();
		//testQueueDequeForQueueUsingArrayRotating();
		testQueueDequeForQueueUsingArray();
	}

}
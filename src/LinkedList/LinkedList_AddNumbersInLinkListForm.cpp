#include <algorithm>
#include <list>
#include <forward_list>
#include <initializer_list>

#include "LinkedList/LinkedList_AddNumbersInLinkListForm.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	/*
	 Given two decimal numbers in the form of linked list - Every digit represented by a node in link list,
	 create a link list that represents the sum of these numbers.
	*/

	// Using std::list<int>
	std::list<int> AddNumbersInLinkListForm_v1(const std::list<int> &lhs, const std::list<int> &rhs)
	{
		std::list<int> result;
		const std::list<int> &biggerList = lhs.size() >= rhs.size() ? lhs : rhs;
		const std::list<int> &smallerList = lhs.size() >= rhs.size() ? rhs : lhs;
		
		int carry = 0;
		for (auto bigIt = biggerList.rbegin(), smallIt = smallerList.rbegin();
			bigIt != biggerList.rend();
			++bigIt)
		{
			int digitOfSmallList = smallIt != smallerList.rend() ? *smallIt++ : 0;
			int currentResult = (*bigIt + digitOfSmallList + carry);
			result.push_front(currentResult % 10);
			carry = currentResult / 10;
		}

		if (carry != 0)
			result.push_front(carry);

		//Remove all zeros at front
		auto it = result.begin();
		for (; it != result.end() && *it == 0; ++it);
		result.erase(result.begin(), it);
		return result;
	}
	
	// Using std::forward_list<int>
	std::forward_list<int> AddNumbersInLinkListForm_v2(const std::forward_list<int> &lhs, const std::forward_list<int> &rhs)
	{
		std::forward_list<int> result;
		int lhsSize = 0, rhsSize = 0;
		for (auto it = lhs.begin(); it != lhs.end(); ++it, ++lhsSize);
		for (auto it = rhs.begin(); it != rhs.end(); ++it, ++rhsSize);
		const std::forward_list<int> &biggerList = lhsSize >= rhsSize ? lhs : rhs;
		const std::forward_list<int> &smallerList = lhsSize >= rhsSize ? rhs : lhs;
		auto bigIt = biggerList.begin();
		auto smallIt = smallerList.begin();

		for (int i = abs(lhsSize - rhsSize); i > 0; --i, ++bigIt)
			result.push_front(*bigIt);

		int carry = 0;
		for (; bigIt != biggerList.end(); ++bigIt, ++smallIt)
		{
			int currentResult = (*bigIt + *smallIt + carry);
			result.push_front(currentResult % 10);
			carry = currentResult / 10;
		}

		if (carry != 0)
			result.push_front(carry);

		result.reverse();
		//Remove all zeros at front
		for (auto it = result.begin(); it != result.end() && *it == 0; ++it, result.pop_front()); //increment the iterator before doing pop_front(), otherwise ++it will be undefined

		return result;
	}

	// Using my own implementation of list
	struct Node
	{
		int data;
		Node* next;
	};

	class LinkList
	{
	public:
		LinkList()
			:head(nullptr)
		{

		}

		LinkList(std::initializer_list<int> list)
			:head(nullptr)
		{
			//for each (int num in list) // It works only in Visual Studio
			for(int num : list)
			{
				add(num);
			}
		}

		LinkList(Node* node)
			:head(node)
		{

		}

		LinkList(const LinkList& rhs)
			: head(nullptr)
		{
			for (Node* start = rhs.head; start != nullptr; start = start->next)
				add(start->data);
		}

		LinkList(LinkList&& rhs)
		{
			head = rhs.head;
			rhs.head = nullptr;
		}

		LinkList& operator=(const LinkList& rhs) = delete;
		LinkList& operator=(LinkList&& rhs)
		{
			head = rhs.head;
			rhs.head = nullptr;
			return *this;
		}

		~LinkList()
		{
			for (Node* start = head; start != nullptr; )
			{
				Node* temp = start;
				start = start->next;
				delete temp;
			}

		}

		void add(int n)
		{
			Node* temp = new Node;
			temp->data = n;
			temp->next = nullptr;
			if (head == nullptr)
			{
				head = temp;
				return;
			}
			Node* start = head;
			for (; start->next != nullptr; start = start->next);
			start->next = temp;
		}

		void reverse()
		{
			if (head == nullptr || head->next == nullptr) return;
			Node* first = head;
			Node* second = first->next;
			Node* temp = nullptr;
			for (; second != nullptr; )
			{
				temp = second->next;
				//reverse link
				second->next = first;

				first = second;
				second = temp;
			}

			//sealing the end of list
			head->next = nullptr;
			//assigning the new head
			head = first;
		}

		Node*& getHead()
		{
			return head;
		}

		void print() const
		{
			cout << "{ ";
			for (Node* start = head; start != nullptr; start = start->next)
			{
				cout << start->data;
				if (start->next != nullptr)
					cout << ", ";
			}
			cout << " }";
		}

		bool operator==(const LinkList& rhs)
		{
			Node* start = head, *startRhs = rhs.head;
			for (; start != nullptr && startRhs != nullptr; start = start->next, startRhs = startRhs->next)
				if (start->data != startRhs->data)
					return false;

			return start == nullptr && startRhs == nullptr;
		}

		void removeLeadingZeros()
		{
			for (Node* start = head; start != nullptr && start->data == 0;)
			{
				Node* toDelete = start;
				start = start->next;
				head = start;
				delete toDelete;
			}
		}

	private:
		Node* head;
	};

	LinkList AddNumbersInLinkListForm_v3(LinkList& first, LinkList& second)
	{
		LinkList retList;
		first.reverse();
		second.reverse();

		Node* firstHead = first.getHead();
		Node* secondHead = second.getHead();
		Node* current = nullptr;
		int carry = 0;

		for (; firstHead != nullptr || secondHead != nullptr;)
		{
			int firstData = firstHead == nullptr ? 0 : firstHead->data;
			int secondData = secondHead == nullptr ? 0 : secondHead->data;

			Node* temp = new Node;
			temp->data = (firstData + secondData + carry) % 10;
			temp->next = nullptr;
			if (current == nullptr)
			{
				retList.getHead() = temp;
				current = temp;
			}
			else
			{
				current->next = temp;
				current = current->next;
			}

			carry = (firstData + secondData) / 10;

			if (firstHead != nullptr)
				firstHead = firstHead->next;
			if (secondHead != nullptr)
				secondHead = secondHead->next;
		}

		current->next = nullptr;
		retList.reverse();

		//Remove all leading zeros
		retList.removeLeadingZeros();

		//Do not change input lists
		first.reverse();
		second.reverse();

		return retList;
	}

	//----------------------------------------------------- Unit Tests -------------------------------------------------------------

	MM_DECLARE_FLAG(AddNumbersInLinkListFormUnitTest);

	template<typename T>
	class ListPrintHelper
	{
	public:
		ListPrintHelper(const T& result)
			: data(result)
		{
		}

		template<typename Type>
		friend ostream& operator<<(ostream& o, const ListPrintHelper<Type>& obj);

	//private:
		T data;
	};

	template<typename Type>
	ostream& operator<<(ostream& o, const ListPrintHelper<Type>& obj)
	{
		o << "{ ";
		auto it = obj.data.begin();
		while (true)
		{
			o << *it;
			if (++it != obj.data.end())
				o << ", ";
			else
				break;
		}
		o << " }";

		return o;
	}

	// Template specialization
	template<>
	ostream& operator<<(ostream& o, const ListPrintHelper<LinkList>& obj)
	{
		obj.data.print();		
		return o;
	}

	template<typename T>
	struct testSet
	{
		testSet(T&& a, T&& b, T&& c)
			: lhs(std::move(a)), rhs(std::move(b)), result(std::move(c))
		{
		}

		T lhs;
		T rhs;
		T result;
	};

	template<typename T>
	vector<testSet<T>>& getTestData()
	{
		static vector<testSet<T>> testData({
			testSet<T>({ 1, 2, 3 }, { 4,5 }, { 1,6,8 }),
			testSet<T>({ 0,0,1,2,3 },{ 4,5 },{ 1,6,8 }),
			testSet<T>({ { 1,2,3 },{ 0,0 },{ 1,2,3 } }),
			testSet<T>({ { 1,2,3 },{ 0,0,0,4,5 },{ 1,6,8 } }),
			testSet<T>({ { 0,0,0,1,2,3 },{ 0,0,0,0,0,0,0,0,4,5 },{ 1,6,8 } })
		});

		return testData;
	}

	MM_UNIT_TEST(AddNumbersInLinkListForm_v1_UnitTest_sanity, AddNumbersInLinkListFormUnitTest)
	{
		std::list<int> result1;
		std::forward_list<int> result2;
		LinkList result3;
		for (size_t i = 0; i < getTestData<std::list<int>>().size(); ++i)
		{
			MM_EXPECT_TRUE((result1 = AddNumbersInLinkListForm_v1(getTestData<std::list<int>>()[i].lhs, getTestData<std::list<int>>()[i].rhs)) == getTestData<std::list<int>>()[i].result,
				ListPrintHelper<std::list<int>>(getTestData<std::list<int>>()[i].lhs),
				ListPrintHelper<std::list<int>>(getTestData<std::list<int>>()[i].rhs),
					ListPrintHelper<std::list<int>>(result1));
			MM_EXPECT_TRUE((result2 = AddNumbersInLinkListForm_v2(getTestData<std::forward_list<int>>()[i].lhs, getTestData<std::forward_list<int>>()[i].rhs)) == getTestData<std::forward_list<int>>()[i].result,
				ListPrintHelper<std::forward_list<int>>(getTestData<std::forward_list<int>>()[i].lhs),
				ListPrintHelper<std::forward_list<int>>(getTestData<std::forward_list<int>>()[i].rhs),
				ListPrintHelper<std::forward_list<int>>(result2));
			MM_EXPECT_TRUE((result3 = AddNumbersInLinkListForm_v3(getTestData<LinkList>()[i].lhs, getTestData<LinkList>()[i].rhs)) == getTestData<LinkList>()[i].result,
				ListPrintHelper<LinkList>(getTestData<LinkList>()[i].lhs),
				ListPrintHelper<LinkList>(getTestData<LinkList>()[i].rhs),
				ListPrintHelper<LinkList>(result3));
		}
	}
}



#include "LinkedList/LinkedList_ReverseSingleLinkedList.h"
#include "Others/PrintUtils.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	struct Node
	{
		int data;
		Node* next;
	};

	Node* createLinkedListFromVector(const vector<int>& vec)
	{
		if (vec.empty())
			return nullptr;

		Node* head = nullptr;
		Node* current = nullptr;
		for (int i = 0; i < vec.size(); ++i)
		{
			Node* newNode = new Node;
			newNode->data = vec[i];
			newNode->next = nullptr; //seal the end

			if (head == nullptr)
				head = newNode;
			else
				current->next = newNode;
			
			current = newNode;
		}

		return head;
	}

	vector<int> getVectorFromList(Node* head)
	{
		vector<int> result;

		while (head != nullptr)
		{
			result.push_back(head->data);
			head = head->next;
		}

		return result;
	}

	Node* LinkedList_ReverseSingleLinkedList(Node* head)
	{
		Node* prev = nullptr;
		Node* current = head;		
		while (current != nullptr)
		{
			Node* temp = current->next;
			current->next = prev; // reverse the link

			prev = current;
			current = temp;
		}

		Node* headReversedList = prev;
		return headReversedList;
	}

	MM_DECLARE_FLAG(LinkedList_ReverseSingleLinkedList);

	MM_UNIT_TEST(LinkedList_ReverseSingleLinkedList_test_1, LinkedList_ReverseSingleLinkedList)
	{
		struct testDataStruct
		{
			vector<int> list;
			vector<int> reversedList;
		};

		std::vector<testDataStruct> testData{
			{ {}, {} },
			{ {1}, {1} },
			{ {1, 2, 3, 4, 5}, {5, 4, 3, 2, 1} }
		};

		for (int i = 0; i < testData.size(); ++i)
		{
			vector<int> actualResult;
			MM_EXPECT_TRUE((actualResult = getVectorFromList(LinkedList_ReverseSingleLinkedList(createLinkedListFromVector(testData[i].list)))) == testData[i].reversedList, testData[i].list, testData[i].reversedList, actualResult);
		}		
	}
}
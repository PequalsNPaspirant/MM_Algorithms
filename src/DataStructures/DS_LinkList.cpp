//Goal:
//Try various Link list operations

#include <iostream>
using namespace std;

namespace mm {

	template <typename Type>
	class singleLinkList
	{
	private:
		struct singleLink
		{
			Type data;
			singleLink* next;
		};
		singleLink* head;

	public:
		singleLinkList()
			: head(nullptr)
		{

		}
		~singleLinkList()
		{
			singleLink* node = head;
			while (node != nullptr)
			{
				singleLink* temp = node;
				node = node->next;
				delete temp;
			}
		}
		void add(Type elements[], int size)
		{
			for (int i = 0; i < size; i++)
				add(elements[i]);
		}
		void add(Type element)
		{
			if (head == nullptr)
			{
				head = new singleLink;
				head->data = element;
				head->next = nullptr;
			}
			else
			{
				singleLink* last = getLast();
				last->next = new singleLink;
				last->next->data = element;
				last->next->next = nullptr;
			}
		}
		singleLink* getLast()
		{
			if (head == nullptr) return nullptr;
			singleLink* last = head;
			while (last->next != nullptr) last = last->next;
			return last;
		}
		void reverse1()
		{
			if (head == nullptr) return;

			singleLink* first = head;
			singleLink* second = first->next;
			singleLink* temp = nullptr;
			for (; second != nullptr; first = second, second = temp)
			{
				temp = second->next;
				second->next = first;
			}
			//Seal the front end
			head->next = nullptr;
			//point head to last
			head = first;
		}
		void reverse2()
		{
			if (head == nullptr) return;
			if (head->next == nullptr) return;
			singleLink* first = head;
			getNext(first)->next = first;
			first->next = nullptr;
		}
		singleLink* getNext(singleLink* node)
		{
			if (node->next == nullptr)
				head = node;
			else
			{
				node = node->next;
				getNext(node)->next = node;
			}

			return node;
		}
		void print()
		{
			if (head == nullptr) return;
			cout << endl;
			for (singleLink* nextNode = head; nextNode != nullptr; nextNode = nextNode->next)
				cout << nextNode->data << " ";
		}

	};

	void reverseSingleLinkList()
	{
		singleLinkList<int> myList;
		int array[] = { 1, 2, 3, 4, 5, 6 };
		//myList.add(1);
		//myList.add(2);
		//myList.add(3);
		//myList.add(4);
		//myList.add(5);
		//myList.add(6);
		myList.add(array, 6);
		myList.print();
		myList.reverse1();
		myList.print();
	}

	void deleteNode()
	{
		/*
		Method 1- using single pointer

			struct list *lp, *prevlp;
			for (lp = list; lp != NULL; lp = lp->next)
			{
				if (lp->item == i)
				{
					if (lp == list)
						list = lp->next;
					else	prevlp->next = lp->next;
					break;
				}
				prevlp = lp;
			}
		}

		Method 2: using pointer to pointer

		struct list **lpp;
		for (lpp = &list; *lpp != NULL; lpp = &(*lpp)->next)
		{
			if ((*lpp)->item == i)
			{
				*lpp = (*lpp)->next;
				break;
			}
		}
			}

		*/
	}

	/*

	struct list **lpp;
	for (lpp = &list; *lpp != NULL; lpp = &(*lpp)->next)
	{
		struct list *lp = *lpp;
		if (newlp->item < lp->item)
		{
			newlp->next = lp;
			*lpp = newlp;
			break;
		}
	}
		}
	*/


	template <typename type>
	struct doubleLink
	{
		type data;
		doubleLink* next;
		doubleLink* prev;
	};

	void LinkList()
	{
		reverseSingleLinkList();
	}

}
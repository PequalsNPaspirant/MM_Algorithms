//Goal
//BST - All in one

#include <iostream>
#include <vector>
#include <string>
using namespace std;

namespace mm {

	class BST
	{
	private:
		struct Node
		{
			int value;
			Node* left;
			Node* right;
		};

		enum childSpec
		{
			leftChild,
			rightChild,
			unspecified
		};

		Node* m_root;
		int m_level;

	public:
		BST()
			:m_root(nullptr), m_level(0)
		{

		}
		BST(int arrayInt[], int size)
			:m_root(nullptr), m_level(0)
		{
			for (int i = 0; i < size; i++)
				this->insert(arrayInt[i]);
		}

		BST& insert(int num)
		{
			add(&m_root, num);
			return *this;
		}

		void add(Node** root, int num)
		{
			if (*root == nullptr)
			{
				Node* newNode = new Node;
				newNode->value = num;
				newNode->left = nullptr;
				newNode->right = nullptr;
				*root = newNode;
			}
			else if (num > (*root)->value)
				add(&((*root)->right), num);
			else
				add(&((*root)->left), num);
		}

		int getLevels()
		{
			int currentLevel = 1;
			getLevels(m_root, currentLevel);
			return m_level;
		}

		void getLevels(Node* node, int currentLevel)
		{
			if (node == NULL)
				return;
			if (m_level < currentLevel)
				m_level = currentLevel;
			getLevels(node->left, currentLevel + 1);
			getLevels(node->right, currentLevel + 1);
		}

		//		****1		-0 L 0
		//		***/*\		-1 
		//		**2***3		-2 L 1
		//		*/*****\	-3 
		//		4*******5	-4 L 2
		//##1\n#/#\\\n

#define SPACES_BET_ELEMENTS 1
#define SIZE_OF_ELEMENT 1

		void print()
		{
			//Do level order traversal for printing
			//OR
			getLevels();
			int pos = getRootPosition();
			vector<string*> str;
			printTree(str, m_root, 1, pos, unspecified);
			//prepareToPrint(str);
			cout << "\n\nTree:\n";
			int size = str.size();
			for (int i = 0; i < size; i++)
			{
				cout << *(str[i]) << endl;
			}
		}

		int getNumElements(int level)
		{
			return pow(2, level - 1);
		}

		int getRootPosition()
		{
			return (getNumElements(m_level + 1) * (SPACES_BET_ELEMENTS + SIZE_OF_ELEMENT)) / 2;
		}

		void printTree(vector<string*>& str, Node* node, int currentLevel, int parentPos, childSpec child)
		{
			if (node == nullptr) return;

			int size = str.size();
			int expectedSize = currentLevel;
			if (size < expectedSize)
			{
				int reqNewStrs = expectedSize - size;
				for (int i = 0; i < reqNewStrs; i++)
				{
					string* temp = new string;
					str.push_back(temp);
				}
			}

			int currentElementPos = 0;
			string *str1 = str[currentLevel - 1];

			if (child == unspecified)
			{
				*str1 += (getSpaces(parentPos - 1) + to_string(node->value));
				currentElementPos = parentPos;
			}
			else if (child == leftChild)
			{
				currentElementPos = parentPos - getSpaceBetElements(currentLevel);
				*str1 += (getSpaces(currentElementPos - str1->length()) + to_string(node->value));
			}
			else if (child == rightChild)
			{
				currentElementPos = parentPos + getSpaceBetElements(currentLevel);
				*str1 += (getSpaces(currentElementPos - str1->length()) + to_string(node->value));
			}
			printTree(str, node->left, currentLevel + 1, currentElementPos, leftChild);
			printTree(str, node->right, currentLevel + 1, currentElementPos, rightChild);
		}

		void prepareToPrint(vector<string*>& str)
		{
			vector<string*> temp = str;
			str.clear();
			int size = temp.size();
			for (int i = 0; i < size; i++)
			{
				str.push_back(temp[i]);
				if (i < size - 1)
				{
					int totalElementsAtCurrentLevel = getNumElements(i + 1);
					string* newStr = new string();
					for (int j = 0; j < totalElementsAtCurrentLevel; j++)
					{
						int elePosCurrentLevel = getElementPos(i + 1, j + 1);
						int leftElePosNextLevel = getElementPos(i + 2, j + 1);
						int rightElePosNextLevel = getElementPos(i + 2, j + 2);
						int leftBranchSignPos = (elePosCurrentLevel + leftElePosNextLevel) / 2;
						int rightBranchSignPos = (rightElePosNextLevel + elePosCurrentLevel) / 2;
						*newStr += getSpaces(leftBranchSignPos) + "/"
							+ getSpaces(rightBranchSignPos - leftBranchSignPos) + "\\";
					}
					str.push_back(newStr);
				}
			}
		}

		int getSpaceBetElements(int currentLevel)
		{
			//int lastLevelLength = (getNumElements(m_level + 1) * (SPACES_BET_ELEMENTS + SIZE_OF_ELEMENT));
			//int currentLevelElements = getNumElements(currentLevel + 1);
			//return lastLevelLength / currentLevelElements;
			return pow(2, (m_level - currentLevel)) * (SPACES_BET_ELEMENTS + SIZE_OF_ELEMENT);
		}

		int getElementPos(int currentLevel, int pos)
		{
			if (pos == 1)
				return getSpaceBetElements(currentLevel);
			else
				return getSpaceBetElements(currentLevel) * 2;
		}

		string getSpaces(int n)
		{
			string buffer;
			for (int i = 0; i < n; i++)
				buffer += " ";
			return buffer;
		}

		int getHeight()
		{
			return getLevels();
		}

		void levelOrderTraversal()
		{

		}

		bool isBST()
		{
			return checkIfBST(m_root);
		}

		bool checkIfBST(Node* node)
		{
			if (!node) return true;

			if (node->left && node->value < node->left->value)
				return false;

			if (node->right && node->value > node->right->value)
				return false;

			return checkIfBST(node->left) && checkIfBST(node->right);
		}

		void changeElement(int position, int newValue)
		{
			int currentPos = 0;
			doChangeElement(m_root, currentPos, position, newValue);

		}

		void doChangeElement(Node* node, int& currentPos, int position, int newValue)
		{
			if (node == NULL) return;
			if (++currentPos > position) return;
			if (currentPos == position) node->value = newValue;
			doChangeElement(node->left, currentPos, position, newValue);
			doChangeElement(node->right, currentPos, position, newValue);
		}
	};

	void testCreateBST()
	{
		BST bst;
		bst.insert(1);
		bst.insert(2);
		bst.insert(3);
		bst.insert(4);
		bst.insert(5);
		bst.print();

		int arrayInt2[] = { 5, 4, 3, 2, 1 };
		BST bst2(arrayInt2, 5);
		bst2.print();

		int arrayInt3[] = { 8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 11, 13, 15 };
		BST bst3(arrayInt3, 15);
		bst3.print();
	}

	void testIsBST()
	{
		int arrayInt1[] = { 5, 4, 3, 2, 1 };
		BST bst1(arrayInt1, 5);
		bst1.print();
		cout << "\nis BST = " << bst1.isBST();
		bst1.changeElement(3, 6);
		bst1.print();
		cout << "\nis BST = " << bst1.isBST();

		int arrayInt2[] = { 5, 4, 3, 2, 1 };
		BST bst2(arrayInt2, 5);
		bst2.print();
		cout << "\nis BST = " << bst2.isBST();
		bst2.changeElement(4, 10);
		bst2.print();
		cout << "\nis BST = " << bst2.isBST();

		int arrayInt3[] = { 8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 11, 13, 15 };
		BST bst3(arrayInt3, 15);
		bst3.print();
		cout << "\nis BST = " << bst3.isBST();
		bst3.changeElement(7, 6);
		bst3.print();
		cout << "\nis BST = " << bst3.isBST();
	}

	void BST()
	{
		//testCreateBST();
		testIsBST();
	}

}
#pragma once

#include <queue>
#include <string>
#include <iostream>
#include <io.h>
#include <fcntl.h>
using namespace std;

#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	struct Node
	{
		static Node* create(int d, Node* l = nullptr, Node* r = nullptr)
		{
			return new Node{ d, l, r };
		}

		int data;
		Node* left;
		Node* right;
	};

	class BinaryTree
	{
	public:
		static const int Empty = std::numeric_limits<int>::min();

		BinaryTree()
			: root_{ nullptr }, levels_{ 0 }
		{}

		BinaryTree(const BinaryTree&) = delete;
		BinaryTree& operator=(const BinaryTree&) = delete;
		BinaryTree(BinaryTree&& rhs)
		{
			root_ = rhs.root_;
			rhs.root_ = nullptr;
			levels_ = rhs.levels_;
			rhs.levels_ = 0;
		}
		BinaryTree& operator=(BinaryTree&& rhs)
		{
			root_ = rhs.root_;
			rhs.root_ = nullptr;
			levels_ = rhs.levels_;
			rhs.levels_ = 0;
			return *this;
		}

		void addLevel(Node*& root, int currentLevel, int targetLevel, int& index, const vector<int>& numbers)
		{
			if (currentLevel == targetLevel)
			{
				MM_EXPECT_TRUE(root == nullptr, root);

				if (numbers[++index] != Empty)
					root = Node::create(numbers[index]);

				return;
			}

			if (!root)
			{
				index += static_cast<int>(pow(2, targetLevel - currentLevel));
				return;
			}

			addLevel(root->left, currentLevel + 1, targetLevel, index, numbers);
			addLevel(root->right, currentLevel + 1, targetLevel, index, numbers);
		}

		void addLevel2(int targetLevel, const vector<int>& numbers)
		{
			if (root_ == nullptr)
			{
				if (numbers[0] != Empty)
					root_ = Node::create(numbers[0]);
				return;
			}

			int currentLevel = 0;
			std::queue<Node*> q;
			q.push(root_);
			++currentLevel;
			while (++currentLevel < targetLevel)
			{
				int count = q.size();
				while (count > 0)
				{
					Node* currNode = q.front();
					q.pop();
					--count;
					q.push(currNode ? currNode->left : nullptr);
					q.push(currNode ? currNode->right : nullptr);
				}
			}

			for (int i = 0; i < numbers.size(); i += 2)
			{
				Node* currNode = q.front();
				q.pop();
				if (currNode != nullptr)
				{
					if (numbers[i] != Empty)
						currNode->left = Node::create(numbers[i]);
					if (numbers[i + 1] != Empty)
						currNode->right = Node::create(numbers[i + 1]);
				}
			}
		}

		BinaryTree& addNextLevel(const vector<int>& numbers)
		{
			int index = -1;
			//addLevel(root_, 1, ++levels_, index, numbers);
			addLevel2(++levels_, numbers);

			return *this;
		}

		Node* getRoot()
		{
			return root_;
		}

		std::wstring convertToString()
		{
			/*
			Output:
			
//                                           9
//                           ┌───────────────┴───────────────┐
//                           5                               13
//                   ┌───────┴───────┐               ┌───────┴───────┐
//                   3               7               11              15
//               ┌───┴───┐       ┌───┴───┐       ┌───┴───┐       ┌───┴───┐
//               2       4       6       8       10      12      14      16


			Another way to try:
//          			      8             8
//          			     / \           / \
//          			    /   \         /   \
//          			   /     \       /     \
//          			  5      10     5      10
//          			 / \     / \   / \     / \
//          			2   6   9  11 2   6   9  11

			*/

			// Note: (charWidth + clearance) should be divisible by 2
			const int charWidth = 4;
			const int clearance = 2;
			
			vector<int> currBarHalfWidths(levels_, 0);
			int barHalfWidth = (charWidth + clearance) / 2;
			currBarHalfWidths[levels_ - 1] = barHalfWidth;
			//Initialize the widths at each level
			for (int i = levels_ - 2; i >= 0; --i)
			{
				currBarHalfWidths[i] = 2 * currBarHalfWidths[i + 1];
			}

			int currentLevel = 0;
			int elementsAtCurrentLevel = 1;

			wstring retVal{ L"\n\n" };

			std::queue<Node*> q;
			q.push(root_);
			while (!q.empty())
			{
				//Collect all nodes at this level
				vector<Node*> nodesAtCurrentLevel;
				for (int i = 0; i < elementsAtCurrentLevel; ++i)
				{
					Node* currNode = q.front();
					nodesAtCurrentLevel.push_back(currNode);
					q.pop();
					if (currentLevel < levels_ - 1)
					{
						q.push(currNode ? currNode->left : nullptr);
						q.push(currNode ? currNode->right : nullptr);
					}
				}

				//Print bars at this level
				if (currentLevel != 0)
				{
					retVal += L"\n"; //newline for bar
					retVal += wstring(currBarHalfWidths[currentLevel], L' '); //space before starting printing bars
					for (int i = 0; i < elementsAtCurrentLevel / 2; ++i)
					{
						if (i > 0)
							retVal += wstring(2 * currBarHalfWidths[currentLevel] - 1, L' ');

						if (nodesAtCurrentLevel[2 * i] != nullptr || nodesAtCurrentLevel[2 * i + 1] != nullptr)
						{
							//Print bar
							retVal += L"┌";
							retVal += wstring(currBarHalfWidths[currentLevel] - 1, L'─');
							retVal += L"┴";
							retVal += wstring(currBarHalfWidths[currentLevel] - 1, L'─');
							retVal += L"┐";
						}
						else
							retVal += wstring(2 * currBarHalfWidths[currentLevel] + 1, L' ');
					}
					retVal += L"\n"; //new line for printing numbers below bar
				}
				
				//Print data at each node in this level
				for (int i = 0; i < elementsAtCurrentLevel; ++i)
				{
					//space before starting printing numbers
					if (i == 0)
						retVal += wstring(currBarHalfWidths[currentLevel], L' ');
					else
						retVal += wstring(2 * currBarHalfWidths[currentLevel] - charWidth, L' ');

					wstring s;
					Node* currNode = nodesAtCurrentLevel[i];
					if (currNode)
						s = to_wstring(currNode->data);
					else
						s = L"";
					retVal += s;
					retVal += wstring(charWidth - s.length(), L' ');
				}

				elementsAtCurrentLevel *= 2;
				++currentLevel;
			}

			retVal += L"\n\n";

			return retVal;
		}

		static void enableWideCharPrinting()
		{
			//This is required to print wchar on console output
			_setmode(_fileno(stdout), _O_U16TEXT);
		}

		static void disableWideCharPrinting()
		{
			_setmode(_fileno(stdout), _O_TEXT);
		}

	private:
		Node* root_;
		int levels_;
	};

}
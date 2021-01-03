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

				while (numbers[++index] == Empty)
				{
				}
				root = Node::create(numbers[index]);
				return;
			}

			if (!root)
				return;

			addLevel(root->left, currentLevel + 1, targetLevel, index, numbers);
			addLevel(root->right, currentLevel + 1, targetLevel, index, numbers);
		}

		BinaryTree& addNextLevel(const vector<int>& numbers)
		{
			int index = -1;
			addLevel(root_, 1, ++levels_, index, numbers);

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

			auto getStartSpaces = [](int totalLevels, int charWidth, int clearance,	vector<int>& currBarHalfWidth)
			{
				currBarHalfWidth.resize(totalLevels, 0);
				int i = totalLevels - 1;
				int barHalfWidth = (charWidth + clearance) / 2;
				currBarHalfWidth[i] = barHalfWidth;

				for (--i; i >= 0; --i)
				{
					currBarHalfWidth[i] = 2 * currBarHalfWidth[i + 1];
				}
			};

			//(charWidth + clearance) should be divisible by 2
			const int charWidth = 4;
			const int clearance = 4;
			
			vector<int> currBarHalfWidths;
			getStartSpaces(levels_, charWidth, clearance, currBarHalfWidths);

			int currentLevel = 0;
			int elementsAtCurrentLevel = 1;
			int elements = 0;
			bool newLevel = true;

			wstring retVal;
			retVal += L"\n";

			std::queue<Node*> q;
			q.push(root_);
			while (!q.empty() && currentLevel < levels_)
			{
				Node* currNode = q.front();
				q.pop();
				q.push(currNode ? currNode->left : nullptr);
				q.push(currNode ? currNode->right : nullptr);

				if (newLevel)
				{
					newLevel = false;
					retVal += L"\n"; //newline for bar
					
					if (currentLevel != 0)
					{
						retVal += wstring(currBarHalfWidths[currentLevel], L' '); //space before starting printing bars
						for (int i = 0; i < elementsAtCurrentLevel / 2; ++i)
						{
							if (i > 0)
							{
								retVal += wstring(2 * currBarHalfWidths[currentLevel] - 1, L' ');
							}

							//Print bar
							retVal += L"┌";
							retVal += wstring(currBarHalfWidths[currentLevel] - 1, L'─');
							retVal += L"┴";
							retVal += wstring(currBarHalfWidths[currentLevel] - 1, L'─');
							retVal += L"┐";
						}
						retVal += L"\n"; //new line for printing numbers below bar
					}

					retVal += wstring(currBarHalfWidths[currentLevel], L' '); //space before starting printing numbers
				}
				
				if (elements > 0)
				{
					retVal += wstring(2 * currBarHalfWidths[currentLevel] - charWidth, L' ');
				}

				if (currNode)
				{ 
					wstring s = to_wstring(currNode->data);
					retVal += s;
					retVal += wstring(charWidth - s.length(), L' ');
				}
				else
					retVal += L"*";

				if (++elements == elementsAtCurrentLevel)
				{
					elements = 0;
					elementsAtCurrentLevel *= 2;
					newLevel = true;
					++currentLevel;
				}
			}

			retVal += L"\n";
			retVal += L"\n";

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
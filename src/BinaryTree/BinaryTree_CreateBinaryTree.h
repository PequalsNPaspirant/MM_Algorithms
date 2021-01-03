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
			------5------
			----/---\----
			--3-------7--
			-/-\-----/-\-
			2---4---6---8
			*/

			/*
			      8             8
			     / \           / \
			    /   \         /   \
			   /     \       /     \
			  5      10     5      10
			 / \     / \   / \     / \
			2   6   9  11 2   6   9  11

			          8
			__________|_______
			|                 |

			*/


			/*
			┴


			                      1
			┌────────────┬────────┴────────────────────┐
			11           12                            13
			┌────┼────┐    ┌──┴─┐                 ┌─────────┴────┬────┐
			111  112  113  121  122               131            132  133
			┌─────┼─────┐     ┌─────┼─────┐     ┌──┴──┐
			1221  1222  1223  1311  1312  1313  1321  1322


			                            1
			               ┌────────────┴────────────────────┐
			               11                                     13
			      ┌────────┴────────┐                     ┌─────────┴────┬────┐
			      111               113                 131            132  133
			┌─────┴─────┐     ┌─────┴─────┐     ┌──┴──┐
		    1221        1223  1311        1313  1321  1322
			
			



			2x + charWidth + clearance
			2x

			*/

			_setmode(_fileno(stdout), _O_U16TEXT);
			//wstring test(L"\n\n                      1\n         ┌────────────┴────────────────────┐\n         11                                     13\n┌────────┴────────┐                     ┌─────────┴────┬────┐\n\n");
			//wcout << test;

			auto getStartSpaces = [](int totalLevels, int charWidth, int clearance,	vector<int>& currBarHalfWidth)
			{
				int barHalfWidth = (charWidth + clearance) / 2;
				currBarHalfWidth.resize(totalLevels, 0);
				int i = totalLevels - 1;
				currBarHalfWidth[i] = barHalfWidth;

				for (--i; i >= 0; --i)
				{
					currBarHalfWidth[i] = 2 * currBarHalfWidth[i + 1];
				}
			};

			//const int barHalfWidth = 10; //Half width of bar at lowest level
			//charWidth + clearance should be divisible by 2
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
						retVal += wstring(currBarHalfWidths[currentLevel], ' '); //space before starting printing bars
						for (int i = 0; i < elementsAtCurrentLevel / 2; ++i)
						{
							if (i > 0)
							{
								//space between two bars
								//if(currentLevel + 1 < currBarHalfWidths.size())
								//	retVal += wstring(2 * currBarHalfWidths[currentLevel + 1] + charWidth + clearance, ' ');
								//else
								//	retVal += wstring(charWidth + clearance, ' ');
								retVal += wstring(2 * currBarHalfWidths[currentLevel] - 1, ' ');
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

					retVal += wstring(currBarHalfWidths[currentLevel], ' '); //space before starting printing numbers
				}
				
				if (elements > 0)
				{
					//if (elements % 2 == 0)
					//{
					//	if (currentLevel + 1 < currBarHalfWidths.size())
					//		retVal += wstring(2 * currBarHalfWidths[currentLevel + 1] + clearance, ' ');
					//	else
					//		retVal += wstring(clearance, ' ');
					//}
					//else
						retVal += wstring(2 * currBarHalfWidths[currentLevel] - charWidth, ' ');
				}

				if (currNode)
				{ 
					wstring s = to_wstring(currNode->data);
					retVal += s;
					retVal += wstring(charWidth - s.length(), ' ');
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

	private:
		Node* root_;
		int levels_;
	};
}
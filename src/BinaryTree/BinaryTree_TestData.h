#pragma once

#include <vector>
#include <string>
using namespace std;

#include "BinaryTree/BinaryTree_BuildFromArrayRepresentation.h"

namespace mm {

	struct Node;

	/*
	struct BinaryTreeNode
	{
		BinaryTreeNode()
		{
		}

		BinaryTreeNode(int n, BinaryTreeNode* l, BinaryTreeNode* r)
			: data(n),
			left(l),
			right(r)
		{
		}

		int data;
		BinaryTreeNode* left;
		BinaryTreeNode* right;		
	};
	*/
	
	namespace BinaryTreeNodeNamespace
	{
		vector<int> getArrayRepresentation(const string& str)
		{
			vector<int> result;
			for (int i = 0; i < str.length(); ++i)
			{
				if (str[i] == '.')
					result.push_back(0);
				else if ('0' <= str[i] && str[i] <= '9')
					result.push_back(str[i] - '0');
			}

			return result;
		}

		//BinaryTreeNode* rootNode(int n, BinaryTreeNode* l, BinaryTreeNode* r)
		//{
		//	return new BinaryTreeNode(n, l, r);
		//}

		//BinaryTreeNode* leftNode(int n, BinaryTreeNode* l, BinaryTreeNode* r)
		//{
		//	return new BinaryTreeNode(n, l, r);
		//}

		//BinaryTreeNode* rightNode(int n, BinaryTreeNode* l, BinaryTreeNode* r)
		//{
		//	return new BinaryTreeNode(n, l, r);
		//}

		//BinaryTreeNode* left_null = nullptr;
		//BinaryTreeNode* right_null = nullptr;
	}

	vector< vector<int> > getBinaryTreesArrayRepresentation_TestData()
	{
		using namespace BinaryTreeNodeNamespace;

		static const vector<string> testRawData
		{
			// Empty binary tree
			"",
			// Binary tree with 1 node
			"1",
			// Binary tree with 2 nodes
			"        1            "\
			"      2   .          ",

			"        1            "\
			"      .   2          ",

			// Binary tree with 3 nodes

			"           1              "\
			"       2       .          "\
			"     3   .   .   .        ",

			"           1              "\
			"       2       .          "\
			"     .   3   .   .        ",

			"           1              "\
			"       .       2          "\
			"     .   .   3   .        ",

			"           1              "\
			"       .       2          "\
			"     .   .   .   3        ",

			"           1              "\
			"       2       3          "\
			"     .   .   .   .        ",

			// Binary tree with 4 nodes

			"                     1                     "\
			"             2               .             "\
			"         3       .       .       .         "\
			"       4   .   .   .   .   .   .   .       ",

			"                     1                     "\
			"             2               .             "\
			"         3       .       .       .         "\
			"       .   4   .   .   .   .   .   .       ",

			"                     1                     "\
			"             2               .             "\
			"         .       3       .       .         "\
			"       .   .   4   .   .   .   .   .       ",

			"                     1                     "\
			"             2               .             "\
			"         .       3       .       .         "\
			"       .   .   .   4   .   .   .   .       ",

			"                     1                     "\
			"             .               2             "\
			"         .       .       3       .         "\
			"       .   .   .   .   4   .   .   .       ",

			"                     1                     "\
			"             .               2             "\
			"         .       .       3       .         "\
			"       .   .   .   .   .   4   .   .       ",

			"                     1                     "\
			"             .               2             "\
			"         .       .       .       3         "\
			"       .   .   .   .   .   .   4   .       ",

			"                     1                     "\
			"             .               2             "\
			"         .       .       .       3         "\
			"       .   .   .   .   .   .   .   4       ",

			"                     1                     "\
			"             2               .             "\
			"         3       4       .       .         "\
			"       .   .   .   .   .   .   .   .       ",

			"                     1                     "\
			"             .               2             "\
			"         .       .       3       4         "\
			"       .   .   .   .   .   .   .   .       ",

			"                     1                     "\
			"             2               3             "\
			"         4       .       .       .         "\
			"       .   .   .   .   .   .   .   .       ",

			"                     1                     "\
			"             2               3             "\
			"         .       4       .       .         "\
			"       .   .   .   .   .   .   .   .       ",

			"                     1                     "\
			"             2               3             "\
			"         .       .       4       .         "\
			"       .   .   .   .   .   .   .   .       ",

			"                     1                     "\
			"             2               3             "\
			"         .       .       .       4         "\
			"       .   .   .   .   .   .   .   .       "

		};

		vector< vector<int> > result;
		for (int i = 0; i < testRawData.size(); ++i)
			result.push_back(getArrayRepresentation(testRawData[i]));

		return result;
	};

	vector<Node*> getBinaryTrees_TestData()
	{
		using namespace BinaryTreeNodeNamespace;

		vector< vector<int> > BTArray = getBinaryTreesArrayRepresentation_TestData();

		vector<Node*> result;
		for(int i = 0; i < BTArray.size(); ++i)
			result.push_back(BinaryTree_BuildFromArrayRepresentation(BTArray[i]));

		return result;
		/*
		vector<BinaryTreeNode*> result{
			// Insert empty binary tree
			nullptr,

			// Insert binary tree with 1 node
			rootNode(1, nullptr, nullptr),

			// Insert binary tree with 2 nodes
			rootNode(1,
				leftNode(2, nullptr, nullptr),
				right_null),

			rootNode(1,
				left_null,
				rightNode(2, nullptr, nullptr)),

				// Insert binary tree with 3 nodes
				rootNode(1,
					leftNode(2, nullptr, nullptr),
					rightNode(3, nullptr, nullptr)),

				rootNode(1,
					leftNode(2,
						leftNode(3, nullptr, nullptr),
						right_null),
					right_null),

				rootNode(1,
					left_null,
					rightNode(2,
						leftNode(3, nullptr, nullptr),
						right_null)),

						// Insert binary tree with 4 nodes
						rootNode(1,
							leftNode(2,
								leftNode(3,
									leftNode(4, nullptr, nullptr),
									right_null),
								right_null),
							right_null),

							rootNode(1,
							leftNode(3,
							leftNode(3,
							leftNode(3, nullptr, nullptr),
							right_null),
							right_null),
							right_null)
		};
		*/
	}
}
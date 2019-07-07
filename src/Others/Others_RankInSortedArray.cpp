#include <iostream>
#include <cassert>
using namespace std;

#include "Others/Others_RankInSortedArray.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	void RankInSortedArray_v1::update(const string& stock, double price)
	{
		std::pair<double, Node*>& value = data_[stock];
		value.first = price;
		if (value.second == nullptr)
		{
			++totalCount_;
			value.second = insert(root_, stock, price);
		}
		else
		{
			//value.second->update(price);
			deleteNode(value.second);
			value.second = insert(root_, stock, price);
		}
	}

	int RankInSortedArray_v1::getRank(const string& stock)
	{
		std::pair<double, Node*>& value = data_[stock];
		Node* current = value.second;
		assert(current);
		int rankFromLeft = (current->left_ ? current->left_->numNodesInSubtree_ : 0) + 1;
		while (current)
		{
			if(current->parent_ && current->parent_->right_ == current)
				rankFromLeft += ((current->left_ ? current->left_->numNodesInSubtree_ : 0) + 1);
			current = current->parent_;
		}

		return rankFromLeft;
	}

	string RankInSortedArray_v1::getStockWithRank(int rank)
	{
		Node* current = root_;
		if (!current)
			return "";

		int rankFromLeft = ((current->left_ ? current->left_->numNodesInSubtree_ : 0) + 1);
		while (rankFromLeft != rank)
		{
			if (rankFromLeft < rank)
			{
				current = current->right_;
				rankFromLeft += ((current && current->left_ ? current->left_->numNodesInSubtree_ : 0) + 1);
			}
			else
			{
				current = current->left_;
				rankFromLeft -= ((current && current->right_ ? current->right_->numNodesInSubtree_ : 0) + 1);
			}
		}

		return current->stock_;
	}

	RankInSortedArray_v1::~RankInSortedArray_v1()
	{
		deleteTree(root_);
	}

	RankInSortedArray_v1::Node* RankInSortedArray_v1::insert(Node*& current, const string& stock, double price)
	{
		if (current == nullptr)
		{
			current = new Node;
			current->price_ = price;
			current->stock_ = stock;
			current->left_ = nullptr;
			current->right_ = nullptr;
			current->parent_ = nullptr;
			current->numNodesInSubtree_ = 1;
			return current;
		}

		Node* retVal = nullptr;
		if (current->price_ < price)
		{
			retVal = insert(current->right_, stock, price);
			current->right_->parent_ = current;
		}
		else
		{
			retVal = insert(current->left_, stock, price);
			current->left_->parent_ = current;
		}

		current->numNodesInSubtree_ = 1
			+ (current->left_ ? current->left_->numNodesInSubtree_ : 0)
			+ (current->right_ ? current->right_->numNodesInSubtree_ : 0);
		return retVal;
	}

	void RankInSortedArray_v1::deleteNode(Node* current)
	{
		if (!current) return;
		Node* parent = nullptr;
		if (!current->left_)
		{
			if (current->parent_->left_ == current)
				current->parent_->left_ = current->right_;
			else
				current->parent_->right_ = current->right_;
			parent = current->parent_;
			delete current;
		}
		else if(!current->right_)
		{
			if (current->parent_->left_ == current)
				current->parent_->left_ = current->left_;
			else
				current->parent_->right_ = current->left_;
			parent = current->parent_;
			delete current;
		}
		else
		{
			//Get inorder successor (smallest in the right subtree)
			Node* inorderSuccessor = current->right_;
			assert(inorderSuccessor);
			while (!inorderSuccessor->left_)
				inorderSuccessor = inorderSuccessor->left_;
			*current = *inorderSuccessor; //copy the contents of the inorderSuccessor to current node instead of rearranging pointers
			parent = inorderSuccessor->parent_;
			delete inorderSuccessor;
		}

		while (!parent)
		{
			parent->numNodesInSubtree_ = 1
				+ (parent->left_ ? current->left_->numNodesInSubtree_ : 0)
				+ (parent->right_ ? current->right_->numNodesInSubtree_ : 0);
			parent = parent->parent_;
		}
	}

	void RankInSortedArray_v1::deleteTree(Node* current)
	{
		if (!current)
			return;

		deleteTree(current->left_);
		deleteTree(current->right_);
		delete current;
	}

	//void RankInSortedArray_v1::Node::update(double price)
	//{
	//}

	MM_DECLARE_FLAG(Others_RankInSortedArray);

	enum class API
	{
		update,
		getRank,
		getStockWithRank
	};

	struct Data
	{
		API api;
		string stock;
		double priceOrRank;
	};

	template<typename T>
	void test(const vector<Data>& data)
	{
		T obj;
		for (int k = 0; k < data.size(); ++k)
		{
			int actualRank = -1;
			switch (data[k].api)
			{
			case API::update:
				obj.update(data[k].stock, data[k].priceOrRank);
				break;
			case API::getRank:
				actualRank = obj.getRank(data[k].stock);
				MM_EXPECT_TRUE(actualRank == static_cast<int>(data[k].priceOrRank), actualRank, static_cast<int>(data[k].priceOrRank));
				break;
			case API::getStockWithRank:
				string actualStock = obj.getStockWithRank(static_cast<int>(data[k].priceOrRank));
				MM_EXPECT_TRUE(actualStock == data[k].stock, actualStock, data[k].stock);
				break;
			}
		}
	}

	MM_UNIT_TEST(Others_RankInSortedArray_test_1, Others_RankInSortedArray)
	{
		std::vector<vector<Data>> testData
		{
			{
				vector<Data>{
					{ API::update, "AAPL", 116.25 },
					{ API::update, "IBM", 225.36 },
					{ API::update, "GOOGL", 180.56 },
					{ API::update, "FB", 80.29 },
					{ API::getRank, "AAPL", 2 },
					{ API::getRank, "IBM", 4 },
					{ API::getRank, "GOOGL", 3 },
					{ API::getRank, "FB", 1 },
					{ API::getStockWithRank, "AAPL", 2 },
					{ API::getStockWithRank, "IBM", 4 },
					{ API::getStockWithRank, "GOOGL", 3 },
					{ API::getStockWithRank, "FB", 1 }
				}
			}

			//{
			//	vector<Data>{
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 },
			//		{ API::update, "AAPL", 116.25 }
			//	}
			//}
		};

		for (int i = 0; i < testData.size(); ++i)
		{
			test<RankInSortedArray_v1>(testData[i]);
		}
	}
}
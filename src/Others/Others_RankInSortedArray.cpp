#include <iostream>
#include <cassert>
#include <time.h>       /* time */
using namespace std;

#include "Others/Others_RankInSortedArray.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	// +++++++++++++++++ RankInSortedArray_v0 +++++++++++++++++

	void RankInSortedArray_v0::update(const string& stock, double price)
	{
		unordered_map<string, double>::mapped_type& value = stockToPrice_[stock];
		if (fabs(value) < 0.000001)
		{
			value = price;
			priceToStock_[price] = stock;
		}
		else
		{
			priceToStock_.erase(value);
			value = price;
			priceToStock_[price] = stock;
		}
	}

	int RankInSortedArray_v0::getRank(const string& stock)
	{
		int rank = 1;
		for (auto it = priceToStock_.begin(); it != priceToStock_.end() && it->second != stock; ++it, ++rank);

		return rank;
	}

	string RankInSortedArray_v0::getStockWithRank(int rank)
	{
		auto it = priceToStock_.begin();
		int i = 1;
		for (; it != priceToStock_.end() && i < rank; ++it, ++i);

		return it->second;
	}


	// +++++++++++++++++ RankInSortedArray_v1 +++++++++++++++++

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
			Node* replacement = removeNodeFromTree(value.second);
			if (value.second == root_)
				root_ = replacement;
			delete value.second;
			value.second = insert(root_, stock, price);
		}

		assert(validateBST(root_, std::numeric_limits<double>::min(), std::numeric_limits<double>::max()));
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
				rankFromLeft += ((current->parent_->left_ ? current->parent_->left_->numNodesInSubtree_ : 0) + 1);
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

	RankInSortedArray_v1::Node* RankInSortedArray_v1::removeNodeFromTree(Node* current)
	{
		if (!current) return nullptr;
		Node* parent = nullptr;
		Node* retVal = nullptr;

		if (!current->left_)
		{
			if (current->parent_)
			{
				if (current->parent_->left_ == current)
					current->parent_->left_ = current->right_;
				else
					current->parent_->right_ = current->right_;
			}

			if (current->right_)
				current->right_->parent_ = current->parent_;
			parent = current->parent_;
			//delete current;
			retVal = current->right_;
		}
		else if (!current->right_)
		{
			if (current->parent_)
			{
				if (current->parent_->left_ == current)
					current->parent_->left_ = current->left_;
				else
					current->parent_->right_ = current->left_;
			}

			if (current->left_)
				current->left_->parent_ = current->parent_;
			parent = current->parent_;
			//delete current;
			retVal = current->left_;
		}
		else if(current->left_ && current->right_)
		{
			//Get inorder successor (smallest in the right subtree)
			Node* inorderSuccessor = current->right_;
			assert(inorderSuccessor);
			while (inorderSuccessor->left_)
				inorderSuccessor = inorderSuccessor->left_;

			removeNodeFromTree(inorderSuccessor);
			inorderSuccessor->parent_ = current->parent_;
			inorderSuccessor->left_ = current->left_;
			inorderSuccessor->right_ = current->right_;
			inorderSuccessor->numNodesInSubtree_ = current->numNodesInSubtree_;
			if (current->parent_)
			{
				if (current->parent_->left_ == current)
					current->parent_->left_ = inorderSuccessor;
				else
					current->parent_->right_ = inorderSuccessor;
			}
			if (current->left_)
				current->left_->parent_ = inorderSuccessor;
			if (current->right_)
				current->right_->parent_ = inorderSuccessor;
			parent = nullptr;
			retVal = inorderSuccessor;
		}

		while (parent)
		{
			parent->numNodesInSubtree_ = 1
				+ (parent->left_ ? parent->left_->numNodesInSubtree_ : 0)
				+ (parent->right_ ? parent->right_->numNodesInSubtree_ : 0);
			parent = parent->parent_;
		}

		return retVal;
	}

	void RankInSortedArray_v1::deleteTree(Node* current)
	{
		if (!current)
			return;

		deleteTree(current->left_);
		deleteTree(current->right_);
		delete current;
	}

	bool RankInSortedArray_v1::validateBST(Node* current, double min, double max)
	{
		bool result = (
			!current 
			|| (fabs(min - current->price_) < 1e-7 || min < current->price_)
			&& (fabs(max - current->price_) < 1e-7 || current->price_ < max)
			&& validateBST(current->left_, min, current->price_)
			&& validateBST(current->right_, current->price_, max)
		);

		return result;
	}

	//void RankInSortedArray_v1::Node::update(double price)
	//{
	//}

	MM_DECLARE_FLAG(Others_RankInSortedArray);

	template<typename T>
	void test(const vector<Data>& data)
	{
		T obj;
		for (int k = 0; k < data.size(); ++k)
		{
			int actualRank = -1;
			int expectedRank = -1;
			switch (data[k].api)
			{
			case API::update:
				obj.update(data[k].stock, data[k].priceOrRank);
				break;
			case API::getRank:
				actualRank = obj.getRank(data[k].stock);
				expectedRank = static_cast<int>(data[k].priceOrRank);
				MM_EXPECT_TRUE(actualRank == expectedRank, actualRank, expectedRank);
				string actualStock = obj.getStockWithRank(expectedRank);
				MM_EXPECT_TRUE(actualStock == data[k].stock, actualStock, data[k].stock);
				break;
			}
		}
	}

	void generateRandomTestCases();
	std::vector<vector<Data>> getTestData();
	
	MM_UNIT_TEST(Others_RankInSortedArray_test_1, Others_RankInSortedArray)
	{
		//return generateRandomTestCases();

		std::vector<vector<Data>> testData = getTestData();

		for (int i = 0; i < testData.size(); ++i)
		{
			test<RankInSortedArray_v0>(testData[i]);
			test<RankInSortedArray_v1>(testData[i]);
		}
	}
}
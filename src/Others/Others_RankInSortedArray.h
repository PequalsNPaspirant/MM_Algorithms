#pragma once

#include <string>
#include <unordered_map>
using namespace std;

namespace mm {

	class RankInSortedArray_v1
	{
	public:
		void update(const string& stock, double price);
		int getRank(const string& stock);
		string getStockWithRank(int rank);

		~RankInSortedArray_v1();

	private:
		struct Node
		{
			double price_;
			string stock_;
			int numNodesInSubtree_;
			Node* parent_;
			Node* left_;
			Node* right_;

			//void update(double price);
		};

		Node* insert(Node*& root, const string& stock, double price);
		void deleteNode(Node* current);
		void deleteTree(Node* current);

		unordered_map<string, std::pair<double, Node*>> data_;
		Node* root_{ nullptr };
		int totalCount_{ 0 };
	};
}
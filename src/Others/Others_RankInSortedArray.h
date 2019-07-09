#pragma once

#include <string>
#include <unordered_map>
#include <map>
using namespace std;

namespace mm {

	enum class API
	{
		update,
		getRank
	};

	struct Data
	{
		API api;
		string stock;
		double priceOrRank;
	};

	class RankInSortedArray_v0
	{
	public:
		void update(const string& stock, double price);
		int getRank(const string& stock);
		string getStockWithRank(int rank);

	private:
		unordered_map<string, double> stockToPrice_;
		struct compare
		{
			compare(double epsilon = 1e-7)
				: epsilon_(epsilon)
			{}
			bool operator()(double left, double right) const
			{
				return (abs(left - right) > epsilon_) && (left < right);
			}
		private:
			double epsilon_;
		};
		map<double, string, compare> priceToStock_;
	};

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
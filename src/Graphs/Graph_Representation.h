#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

namespace mm {

	struct Vertex
	{
		Vertex(int v)
			: val{ v }
		{
		}

		int val;

		bool operator==(const Vertex& rhs) const
		{
			return val == rhs.val;
		}

		struct Hasher
		{
			std::size_t operator()(const Vertex& v) const
			{
				std::hash<int> hasher;
				return hasher(v.val);
			}
		};
	};

	// data structure to store graph edges
	struct Edge {
		Edge(const Vertex& s, const Vertex& d, int w = 0)
			: src{ s }, dest{ d }, weight{ 0 }
		{
		}

		Vertex src, dest;
		int weight;
	};

	// class to represent a graph object
	class Graph
	{
	public:

		// Graph Constructor
		Graph(const vector<Edge>& edges, int N)
		{
			// add edges to the directed graph
			for (auto& edge : edges)
			{
				// insert at the end
				adjList[edge.src].push_back(edge.dest);

				// If its undirected graph, push reverse edge as well
				// adjList[edge.dest].push_back(edge.src);
			}
		}

		// print adjacency list representation of graph
		void printGraph() const
		{
			for (auto& vv : adjList)
			{
				// print current vertex number
				cout << vv.first.val << " --> ";

				// print all neighboring vertices of vertex i
				for (const Vertex& v : vv.second)
					cout << v.val << " ";
				cout << endl;
			}
		}

	private:
		unordered_map<Vertex, vector<Vertex>, Vertex::Hasher> adjList;
	};

	

}
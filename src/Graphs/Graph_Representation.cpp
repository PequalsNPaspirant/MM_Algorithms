#include <iostream>
#include <string>
using namespace std;

#include "Graphs/Graph_Representation.h"
#include "MM_UnitTestFramework/MM_UnitTestFramework.h"

namespace mm {

	MM_DECLARE_FLAG(Graphs_RepresentationUnitTest);

	MM_UNIT_TEST(Graphs_RepresentationUnitTest_sanity, Graphs_RepresentationUnitTest)
	{
		vector<Edge> edges =
		{
			{ 0, 1 },{ 1, 2 },{ 2, 0 },{ 2, 1 },
		{ 3, 2 },{ 4, 5 },{ 5, 4 }
		};

		// Number of nodes in the graph
		int N = 6;

		// construct graph
		Graph graph(edges, N);

		// print adjacency list representation of graph
		graph.printGraph();

		//MM_EXPECT_TRUE(atualResult == testDataComparison[i].isLessThan, testDataComparison[i].lhs, testDataComparison[i].rhs, testDataComparison[i].isLessThan, atualResult);
	}

}
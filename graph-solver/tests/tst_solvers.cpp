#include <gtest/gtest.h>
#include "graph.h"
#include "vertex.h"
#include "edge.h"
#include "shortestpathsolver.h"
#include "connectivitysolver.h"
#include "coloringsolver.h"
#include "matchingsolver.h"
#include "maxflowsolver.h"
#include "euleriansolver.h"
#include "isomorphismsolver.h"
#include "mincostflowsolver.h"

class GraphTest : public ::testing::Test {
protected:
    Graph* graph;
    Vertex* v1;
    Vertex* v2;
    Vertex* v3;
    Vertex* v4;

    void SetUp() override {
        graph = new Graph();
        v1 = new Vertex(0, 0, "1");
        v2 = new Vertex(10, 10, "2");
        v3 = new Vertex(20, 20, "3");
        v4 = new Vertex(30, 30, "4");

        graph->AddVertex(v1);
        graph->AddVertex(v2);
        graph->AddVertex(v3);
        // v4 is not added to graph by default in all tests, add where needed
    }

    void TearDown() override {
        delete graph; // Graph destructor cleans up vertices and edges
    }
};

// --- Graph Logic Tests ---

TEST_F(GraphTest, AddVertexCorrectly) {
    EXPECT_EQ(graph->_vertices.size(), 3);
    EXPECT_TRUE(graph->_vertices.contains(v1));
}

TEST_F(GraphTest, AddEdgeCorrectly) {
    Edge* e = new Edge(v1, v2);
    graph->AddEdge(e);

    EXPECT_EQ(graph->_edges.size(), 1);
    EXPECT_EQ(graph->_out_edges[v1].size(), 1);
    EXPECT_EQ(graph->_in_edges[v2].size(), 1);
}

TEST_F(GraphTest, RemoveVertexDisconnectsEdges) {
    Edge* e = new Edge(v1, v2);
    graph->AddEdge(e);

    graph->RemoveVertex(v1);

    EXPECT_EQ(graph->_vertices.size(), 2);
    EXPECT_EQ(graph->_edges.size(), 0); // Edge should be removed
}

// --- Connectivity Solver Tests ---

TEST_F(GraphTest, WeaklyConnectedPositive) {
    Edge* e1 = new Edge(v1, v2);
    Edge* e2 = new Edge(v2, v3);
    graph->AddEdge(e1);
    graph->AddEdge(e2);

    EXPECT_TRUE(ConnectivitySolver::IsWeaklyConnected(*graph));
}

TEST_F(GraphTest, WeaklyConnectedNegative) {
    // v3 is isolated
    Edge* e1 = new Edge(v1, v2);
    graph->AddEdge(e1);

    EXPECT_FALSE(ConnectivitySolver::IsWeaklyConnected(*graph));
}

TEST_F(GraphTest, StrongComponentsCycle) {
    // Cycle v1 -> v2 -> v3 -> v1
    graph->AddEdge(new Edge(v1, v2));
    graph->AddEdge(new Edge(v2, v3));
    graph->AddEdge(new Edge(v3, v1));

    EXPECT_TRUE(ConnectivitySolver::IsStronglyConnected(*graph));
    EXPECT_EQ(ConnectivitySolver::StrongComponentsAmmount(*graph), 1);
}

TEST_F(GraphTest, StrongComponentsDisconnected) {
    // v1 -> v2, v3 isolated
    graph->AddEdge(new Edge(v1, v2));

    EXPECT_FALSE(ConnectivitySolver::IsStronglyConnected(*graph));
    EXPECT_EQ(ConnectivitySolver::StrongComponentsAmmount(*graph), 3); // {v1}, {v2}, {v3}
}

// --- Shortest Path Solver Tests ---

TEST_F(GraphTest, DijkstraSimplePath) {
    Edge* e1 = new Edge(v1, v2, Qt::black, 10);
    Edge* e2 = new Edge(v2, v3, Qt::black, 5);
    graph->AddEdge(e1);
    graph->AddEdge(e2);

    QList<Edge*> path = ShortestPathSolver::Dijkstra(*graph, v1, v3);

    EXPECT_EQ(path.size(), 2);
    EXPECT_EQ(path[0], e1);
    EXPECT_EQ(path[1], e2);
}

TEST_F(GraphTest, DijkstraNoPath) {
    // No edges
    QList<Edge*> path = ShortestPathSolver::Dijkstra(*graph, v1, v3);
    EXPECT_TRUE(path.isEmpty());
}

// --- Coloring Solver Tests ---

TEST_F(GraphTest, ColoringTriangle) {
    // Complete graph K3 requires 3 colors
    graph->AddEdge(new Edge(v1, v2));
    graph->AddEdge(new Edge(v2, v1));
    graph->AddEdge(new Edge(v2, v3));
    graph->AddEdge(new Edge(v3, v2));
    graph->AddEdge(new Edge(v3, v1));
    graph->AddEdge(new Edge(v1, v3));

    auto coloring = ColoringSolver::DSatur(*graph);
    int chromaticNum = ColoringSolver::ChromaticNumber(*graph, coloring);

    EXPECT_EQ(chromaticNum, 3);
}

TEST_F(GraphTest, ColoringDisconnected) {
    // No edges -> 1 color
    EXPECT_EQ(graph->_edges.size(), 0) << "Graph should have no edges initially";
    
    auto coloring = ColoringSolver::DSatur(*graph);
    int chromaticNum = ColoringSolver::ChromaticNumber(*graph, coloring);
    EXPECT_EQ(chromaticNum, 1);
}

// --- Matching Solver Tests ---

TEST_F(GraphTest, BipartiteMatching) {
    graph->AddVertex(v4);
    // Bipartite sets: {v1, v2} and {v3, v4}
    // Edges: v1-v3, v2-v4
    graph->AddEdge(new Edge(v1, v3));
    graph->AddEdge(new Edge(v2, v4));

    auto matching = MatchingSolver::HopcroftKarp(*graph);
    EXPECT_EQ(matching.size(), 2);
}

TEST_F(GraphTest, NotBipartite) {
    // Triangle is not bipartite
    graph->AddEdge(new Edge(v1, v2));
    graph->AddEdge(new Edge(v2, v3));
    graph->AddEdge(new Edge(v3, v1));

    QSet<Vertex*> left, right;
    EXPECT_FALSE(MatchingSolver::IsBipartite(*graph, left, right));
    
    // HopcroftKarp should return empty on non-bipartite
    auto matching = MatchingSolver::HopcroftKarp(*graph);
    EXPECT_TRUE(matching.isEmpty());
}

// --- Eulerian Solver Tests ---

TEST_F(GraphTest, IsEulerianCycle) {
    // Cycle v1-v2-v3-v1
    graph->AddEdge(new Edge(v1, v2));
    graph->AddEdge(new Edge(v2, v3));
    graph->AddEdge(new Edge(v3, v1));

    EXPECT_TRUE(EulerianSolver::IsEulerian(*graph));
}

TEST_F(GraphTest, IsEulerianPath) {
    // Path v1-v2-v3
    graph->AddEdge(new Edge(v1, v2));
    graph->AddEdge(new Edge(v2, v3));

    EXPECT_FALSE(EulerianSolver::IsEulerian(*graph)); // Not a cycle
    EXPECT_TRUE(EulerianSolver::IsHalfEulerian(*graph)); // Is a path
}

// --- Max Flow Solver Tests ---

TEST_F(GraphTest, DinicMaxFlowSimple) {
    // v1 (Source) -> v2 (Cap 10) -> v3 (Sink)
    Edge* e1 = new Edge(v1, v2, Qt::black, 10);
    Edge* e2 = new Edge(v2, v3, Qt::black, 10);
    graph->AddEdge(e1);
    graph->AddEdge(e2);

    int maxFlow = MaxFlowSolver::DinicMaxFlow(*graph, v1, v3);
    EXPECT_EQ(maxFlow, 10);
}

TEST_F(GraphTest, DinicMaxFlowSplit) {
    graph->AddVertex(v4);
    // v1 -> v2 (10)
    // v1 -> v4 (10)
    // v2 -> v3 (10)
    // v4 -> v3 (10)
    // Total max flow to v3 should be 20
    graph->AddEdge(new Edge(v1, v2, Qt::black, 10));
    graph->AddEdge(new Edge(v1, v4, Qt::black, 10));
    graph->AddEdge(new Edge(v2, v3, Qt::black, 10));
    graph->AddEdge(new Edge(v4, v3, Qt::black, 10));

    int maxFlow = MaxFlowSolver::DinicMaxFlow(*graph, v1, v3);
    EXPECT_EQ(maxFlow, 20);
}

// --- Isomorphism Solver Tests ---

TEST_F(GraphTest, IsomorphismIdentical) {
    // Create identical graph 2
    Graph* graph2 = new Graph();
    Vertex* g2v1 = new Vertex(0, 0, "1");
    Vertex* g2v2 = new Vertex(10, 10, "2");
    Vertex* g2v3 = new Vertex(20, 20, "3");
    graph2->AddVertex(g2v1);
    graph2->AddVertex(g2v2);
    graph2->AddVertex(g2v3);

    // v1->v2 in both
    graph->AddEdge(new Edge(v1, v2));
    graph2->AddEdge(new Edge(g2v1, g2v2));

    auto result = IsomorphismSolver::VF2Algorithm(*graph, *graph2);
    EXPECT_FALSE(result.isEmpty());
    EXPECT_EQ(result.size(), 3); // 3 vertices mapped

    delete graph2;
}

TEST_F(GraphTest, IsomorphismDifferentStructure) {
    Graph* graph2 = new Graph();
    Vertex* g2v1 = new Vertex(0, 0, "1");
    Vertex* g2v2 = new Vertex(10, 10, "2");
    Vertex* g2v3 = new Vertex(20, 20, "3");
    graph2->AddVertex(g2v1);
    graph2->AddVertex(g2v2);
    graph2->AddVertex(g2v3);

    // Graph1: v1->v2
    graph->AddEdge(new Edge(v1, v2));
    // Graph2: v1->v2 AND v2->v3
    graph2->AddEdge(new Edge(g2v1, g2v2));
    graph2->AddEdge(new Edge(g2v2, g2v3));

    auto result = IsomorphismSolver::VF2Algorithm(*graph, *graph2);
    EXPECT_TRUE(result.isEmpty());

    delete graph2;
}

// --- Min Cost Flow Solver Tests ---

TEST_F(GraphTest, MinCostFlowSimple) {
    // v1 -> v2: Cap 10, Cost 5
    // Need flow 5. Total cost should be 5 * 5 = 25.
    Edge* e = new Edge(v1, v2, Qt::black, 10, 5);
    graph->AddEdge(e);

    QPair<int, int> result = MinCostFlowSolver::MinCostFlow(*graph, v1, v2, 5);
    EXPECT_EQ(result.first, 5); // Total Flow
    EXPECT_EQ(result.second, 25); // Total Cost
}

TEST_F(GraphTest, MinCostFlowInsufficientCapacity) {
    // v1 -> v2: Cap 5, Cost 10
    // Need flow 10. Can only push 5.
    Edge* e = new Edge(v1, v2, Qt::black, 5, 10);
    graph->AddEdge(e);

    QPair<int, int> result = MinCostFlowSolver::MinCostFlow(*graph, v1, v2, 10);
    EXPECT_EQ(result.first, 5); // Flow capped at 5
    EXPECT_EQ(result.second, 50); // 5 * 10
}
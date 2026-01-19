#include <gtest/gtest.h>
#include <QGraphicsScene>
#include <QUndoStack>
#include "graph.h"
#include "commands.h"

class CommandsTest : public ::testing::Test {
protected:
    QGraphicsScene* scene;
    Graph* graph;
    QUndoStack* stack;

    void SetUp() override {
        scene = new QGraphicsScene();
        graph = new Graph();
        stack = new QUndoStack();
    }

    void TearDown() override {
        delete stack; // Deletes commands
        delete graph;
        delete scene;
    }
};

TEST_F(CommandsTest, AddVertexUndoRedo) {
    QPointF pos(10, 20);
    // Fixed: Added name parameter "1"
    AddVertexCommand* cmd = new AddVertexCommand(scene, graph, pos, "1");
    stack->push(cmd);

    EXPECT_EQ(graph->_vertices.size(), 1);
    EXPECT_EQ(scene->items().size(), 1);

    stack->undo();
    EXPECT_EQ(graph->_vertices.size(), 0);
    EXPECT_EQ(scene->items().size(), 0);

    stack->redo();
    EXPECT_EQ(graph->_vertices.size(), 1);
    EXPECT_EQ(scene->items().size(), 1);
}

TEST_F(CommandsTest, AddEdgeUndoRedo) {
    // Setup vertices
    Vertex* v1 = new Vertex(0, 0);
    Vertex* v2 = new Vertex(10, 10);
    graph->AddVertex(v1);
    graph->AddVertex(v2);
    scene->addItem(v1);
    scene->addItem(v2);

    AddEdgeCommand* cmd = new AddEdgeCommand(scene, graph, v1, v2, 5, 0);
    stack->push(cmd);

    EXPECT_EQ(graph->_edges.size(), 1);
    EXPECT_EQ(scene->items().size(), 3); // 2 vertices + 1 edge

    stack->undo();
    EXPECT_EQ(graph->_edges.size(), 0);

    stack->redo();
    EXPECT_EQ(graph->_edges.size(), 1);
}

TEST_F(CommandsTest, RemoveVertexUndoRedo) {
    // Setup
    Vertex* v1 = new Vertex(0, 0);
    graph->AddVertex(v1);
    scene->addItem(v1);

    RemoveItemCommand* cmd = new RemoveItemCommand(scene, graph, v1);
    stack->push(cmd);

    // After push (Redo implicit), vertex should be removed
    EXPECT_EQ(graph->_vertices.size(), 0);
    EXPECT_EQ(scene->items().size(), 0);

    stack->undo();
    EXPECT_EQ(graph->_vertices.size(), 1);
    EXPECT_EQ(scene->items().size(), 1);
}

TEST_F(CommandsTest, RemoveVertexWithEdgesUndoRedo) {
    Vertex* v1 = new Vertex(0, 0);
    Vertex* v2 = new Vertex(10, 10);
    Edge* e1 = new Edge(v1, v2);
    
    graph->AddVertex(v1);
    graph->AddVertex(v2);
    graph->AddEdge(e1);
    scene->addItem(v1);
    scene->addItem(v2);
    scene->addItem(e1);

    // Remove v1, should remove e1 too
    RemoveItemCommand* cmd = new RemoveItemCommand(scene, graph, v1);
    stack->push(cmd);

    EXPECT_EQ(graph->_vertices.size(), 1); // v2 remains
    EXPECT_EQ(graph->_edges.size(), 0);    // e1 gone

    stack->undo();
    EXPECT_EQ(graph->_vertices.size(), 2);
    EXPECT_EQ(graph->_edges.size(), 1);
}
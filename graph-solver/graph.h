#ifndef GRAPH_H
#define GRAPH_H
#pragma once

#include <QHash>
#include <QVector>
#include <QList>
#include <QSet>
#include <QPair>
#include <QMap>
#include <climits>
#include <set>
#include <QFile>
#include <QDataStream>
#include <fstream>
#include <iostream>
#include <vector>
#include "vertex.h"
#include "edge.h"

class QUndoStack;     // Forward declaration
class QGraphicsScene; // Forward declaration

const int int_max = 2147483647;

/**
 * @class Graph
 * @brief Represents a graph structure consisting of vertices and edges.
 * 
 * Manages the collection of vertices and edges, including adjacency lists
 * and ownership of the objects. Supports serialization to binary format.
 */
class Graph
{
public:
    QSet<Vertex*> _vertices; ///< Set of all vertices in the graph.
    QSet<Edge*> _edges;      ///< Set of all edges in the graph.

    QList<Vertex*> _sources; ///< List of source vertices (logic specific).
    QList<Vertex*> _targets; ///< List of target vertices (logic specific).

    QHash<Vertex*, QList<Edge*>> _out_edges; ///< Adjacency list for outgoing edges.
    QHash<Vertex*, QList<Edge*>> _in_edges;  ///< Adjacency list for incoming edges.

    // Robust ID management
    std::set<int> _free_indices; ///< Set of recycled indices.
    int _next_index = 0;         ///< Next available new index.

public:
    /**
     * @brief Default constructor.
     */
    Graph();

    /**
     * @brief Destructor. Cleans up all vertices and edges.
     */
    ~Graph();

    /**
     * @brief Adds a vertex to the graph.
     * @param v The vertex to add.
     */
    void AddVertex(Vertex* v);

    /**
     * @brief Adds an edge to the graph.
     * @param e The edge to add.
     */
    void AddEdge(Edge* e);

    /**
     * @brief Checks if a directed edge exists between source and target.
     * @param source Source vertex.
     * @param target Target vertex.
     * @return true if edge exists, false otherwise.
     */
    bool HasEdge(Vertex* source, Vertex* target) const;

    /**
     * @brief Removes a vertex and its connected edges from the graph.
     * @param v The vertex to remove.
     */
    void RemoveVertex(Vertex* v);

    /**
     * @brief Removes an edge from the graph.
     * @param e The edge to remove.
     */
    void RemoveEdge(Edge *e);

    // --- Undo/Redo Support Methods (Do NOT delete memory) ---

    /**
     * @brief Detaches a vertex from the graph structures but DOES NOT delete the pointer.
     */
    void DetachVertex(Vertex* v);

    /**
     * @brief Reattaches a previously detached vertex.
     */
    void ReattachVertex(Vertex* v);

    /**
     * @brief Detaches an edge from the graph structures but DOES NOT delete the pointer.
     */
    void DetachEdge(Edge* e);

    /**
     * @brief Reattaches a previously detached edge.
     */
    void ReattachEdge(Edge* e);

    // -----------------------------------------------------

    /**
     * @brief Saves the graph AND its Undo history to a binary file.
     * @param file_name Path to file.
     * @param stack Pointer to the view's UndoStack (required for history).
     */
    int SaveToBinary(const QString& file_name, const QUndoStack* stack);

    /**
     * @brief Loads the graph by replaying the Undo history from the binary file.
     * @param file_name Path to file.
     * @param stack Pointer to the view's UndoStack (to restore history).
     * @param scene Pointer to the view's Scene (needed to recreate items).
     */
    int LoadFromBinary(const QString& file_name, QUndoStack* stack, QGraphicsScene* scene);

    /**
     * @brief Saves the graph to a text file using std::fstream. (Requirement 3.1)
     */
    int SaveToText(const std::string& file_name);

    /**
     * @brief Loads the graph from a text file using std::fstream. (Requirement 3.1)
     */
    int LoadFromText(const std::string& file_name);
};

#endif // GRAPH_H
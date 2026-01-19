#ifndef CONNECTIVITYSOLVER_H
#define CONNECTIVITYSOLVER_H
#pragma once

#include "graph.h"
#include <QQueue>
#include <QStack>
#include "TemplateStack.h" // Include custom data structure

/**
 * @namespace ConnectivitySolver
 * @brief Algorithms related to graph connectivity (Weak and Strong components).
 */
namespace ConnectivitySolver
{
    /**
     * @brief Performs BFS to find a weakly connected component.
     * @param graph The graph.
     * @param start The starting vertex.
     * @return Set of vertices in the component.
     */
    QSet<Vertex*> BFSWeak(const Graph& graph, Vertex* start);

    /**
     * @brief Identifies all weakly connected components.
     * @param graph The graph.
     * @return Map of Vertex to Component ID.
     */
    QHash<Vertex*, int> WeakComponents(const Graph& graph);

    /**
     * @brief Counts the number of weakly connected components.
     * @param graph The graph.
     * @return Component count.
     */
    int WeakComponentsAmmount(const Graph& graph);

    /**
     * @brief Checks if the graph is weakly connected.
     * @param graph The graph.
     * @return True if connected, false otherwise.
     */
    bool IsWeaklyConnected(const Graph& graph);

    /** 
     * @brief First pass DFS for Kosaraju's algorithm (fill order stack).
     * @note Uses Custom TemplateStack instead of QStack.
     */
    void DFSFirstForKosaraju(const Graph& graph, Vertex* start,
                             QSet<Vertex*>& visited, TemplateStack<Vertex*>& order);
                             
    /** @brief Second pass DFS for Kosaraju's algorithm (identify components). */
    void DFSSecondForKosaraju(const Graph& graph, Vertex* start, QSet<Vertex*>& visited,
                              int count, QHash<Vertex*, int>& result);
    
    /**
     * @brief Finds strongly connected components using Kosaraju's algorithm.
     * @param graph The graph.
     * @return Map of Vertex to Component ID.
     */
    QHash<Vertex*, int> Kosaraju(const Graph& graph);

    /**
     * @brief Counts strongly connected components.
     */
    int StrongComponentsAmmount(const Graph& graph);

    /**
     * @brief Checks if the graph is strongly connected.
     */
    bool IsStronglyConnected(const Graph& graph);
}

#endif // CONNECTIVITYSOLVER_H
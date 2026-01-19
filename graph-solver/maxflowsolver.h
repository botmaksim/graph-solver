#ifndef MAXFLOWSOLVER_H
#define MAXFLOWSOLVER_H
#pragma once

#include "graph.h"
#include <QQueue>

/**
 * @namespace MaxFlowSolver
 * @brief Algorithms for calculating Maximum Flow and Minimum Cut.
 */
namespace MaxFlowSolver
{
    /**
     * @brief Calculates Max Flow with multiple sources and sinks.
     * @return Pair containing the max flow value and the list of edges in the Min Cut.
     */
    QPair<int, QList<Edge*>> MultyMaxFlow(Graph& graph, const QList<Vertex*>& sources, const QList<Vertex*>& sinks);

    /**
     * @brief Calculates Max Flow using Dinic's Algorithm.
     */
    int DinicMaxFlow(const Graph& graph, Vertex* source, Vertex* sink);
    
    /** @brief Builds the Level Graph for Dinic's algorithm using BFS. */
    bool BFSLayeredNetwork(const Graph& graph, QHash<Vertex*, int>& level,
                           Vertex* source, Vertex* sink);
                           
    /** @brief Pushes flow along blocking paths in the Level Graph using DFS. */
    int DFSBlockingFlow(const Graph& graph, Vertex* u, Vertex* sink,
                        int flow, QHash<Vertex*, int>& level,
                        QHash<Vertex*, int>& ptr);
    
    /**
     * @brief Identifies the edges part of the Minimum Cut.
     */
    QList<Edge*> FindMinCut(const Graph& graph, Vertex* source, Vertex* sink);
    
    /** @brief Finds reachable vertices in residual graph (helper for MinCut). */
    QSet<Vertex*> BFSResidual(const Graph& graph, Vertex* source, Vertex* sink);
}

#endif // MAXFLOWSOLVER_H
#ifndef MATCHINGSOLVER_H
#define MATCHINGSOLVER_H
#pragma once

#include <QQueue>
#include "graph.h"

/**
 * @namespace MatchingSolver
 * @brief Algorithms for finding matching in bipartite graphs.
 */
namespace MatchingSolver
{
    /**
     * @brief Checks if graph is bipartite and splits vertices.
     */
    bool IsBipartite(const Graph& graph, QSet<Vertex*>& left, QSet<Vertex*>& right);
    
    /** @brief BFS Helper to color graph 2-color for bipartite check. */
    bool BFSColoring(const Graph& graph, Vertex* start,
                 QHash<Vertex*, int>& color,
                 QSet<Vertex*>& left, QSet<Vertex*>& right);

    /**
     * @brief Finds maximal matching using Hopcroft-Karp algorithm.
     * @return List of edges in the matching.
     */
    QList<Edge*> HopcroftKarp(const Graph& graph);
    
    /** @brief BFS Phase of Hopcroft-Karp to find augmenting paths. */
    bool BFSHopcroft(const Graph& graph, const QSet<Vertex*>& left,
                     QHash<Vertex*, Vertex*>& pair_u,
                     QHash<Vertex*, Vertex*>& pair_v,
                     QHash<Vertex*, int>& dist);
                     
    /** @brief DFS Phase of Hopcroft-Karp to apply augmenting paths. */
    bool DFSHopcroft(const Graph& graph, Vertex* u,
                     QHash<Vertex*, Vertex*>& pair_u,
                     QHash<Vertex*, Vertex*>& pair_v,
                     QHash<Vertex*, int>& dist);
}

#endif // MATCHINGSOLVER_H
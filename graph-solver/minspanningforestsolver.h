#ifndef MINSPANNINGFORESTSOLVER_H
#define MINSPANNINGFORESTSOLVER_H
#pragma once

#include "graph.h"
#include <algorithm>

/**
 * @namespace MinSpanningForestSolver
 * @brief Algorithms for finding Minimal Spanning Trees/Forests.
 */
namespace MinSpanningForestSolver
{
    /**
     * @brief Finds the Minimum Spanning Forest using Kruskal's algorithm.
     * @param graph The graph.
     * @return List of edges in the spanning forest.
     */
    QList<Edge*> Kruskal(const Graph& graph);

    /**
     * @brief Helper for Union-Find set representative lookup.
     */
    Vertex* FindParent(Vertex* v, QHash<Vertex*, Vertex*>& parent);
}

#endif // MINSPANNINGFORESTSOLVER_H
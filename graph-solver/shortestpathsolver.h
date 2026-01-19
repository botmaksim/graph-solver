#ifndef SHORTESTPATHSOLVER_H
#define SHORTESTPATHSOLVER_H
#pragma once

#include <queue>

#include "graph.h"

/**
 * @namespace ShortestPathSolver
 * @brief Contains algorithms for finding the shortest path in a graph.
 */
namespace ShortestPathSolver
{
    /**
     * @brief Finds the shortest path between a start and end vertex using Dijkstra's algorithm.
     *
     * @param graph The graph to search.
     * @param start Pointer to the starting Vertex.
     * @param end Pointer to the target Vertex.
     * @return QList<Edge*> A list of edges representing the shortest path, or an empty list if no path exists.
     */
    QList<Edge*> Dijkstra(const Graph& graph, Vertex* start, Vertex* end);
}

#endif // SHORTESTPATHSOLVER_H
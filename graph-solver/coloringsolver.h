#ifndef COLORINGSOLVER_H
#define COLORINGSOLVER_H

#include <queue>
#include <tuple>
#include "graph.h"

/**
 * @namespace ColoringSolver
 * @brief Algorithms for Graph Coloring.
 */
namespace ColoringSolver
{
    /**
     * @brief Colors the graph using the DSatur (Degree of Saturation) algorithm.
     * @param graph The graph to color.
     * @return Map of Vertex to Color ID (int).
     */
    QHash<Vertex*, int> DSatur(const Graph& graph);

    /**
     * @brief Calculates the chromatic number based on a coloring.
     * @param graph The graph.
     * @param colors The coloring map.
     * @return The number of colors used.
     */
    int ChromaticNumber(const Graph& graph, const QHash<Vertex*, int>& colors);

    /**
     * @brief Gets neighbors of a vertex.
     */
    QSet<Vertex*> GetNeighbors(const Graph& graph, Vertex* v);
}

#endif // COLORINGSOLVER_H
#ifndef MINCOSTFLOWSOLVER_H
#define MINCOSTFLOWSOLVER_H

#include "graph.h"

/**
 * @namespace MinCostFlowSolver
 * @brief Algorithms for calculating Minimum Cost Flow.
 */
namespace MinCostFlowSolver
{
    /**
     * @brief Calculates Minimum Cost Flow for a required flow amount.
     * @param graph The graph.
     * @param source Source vertex.
     * @param sink Sink vertex.
     * @param required_flow The amount of flow to push.
     * @return Pair of {total flow pushed, total cost}.
     */
    QPair<int, int> MinCostFlow(const Graph& graph, Vertex* source, Vertex* sink, int required_flow);

    /**
     * @brief Helper using Bellman-Ford (or SPFA) to find shortest path in residual graph with potentials.
     */
    QPair<QList<Edge*>, QHash<Vertex*, int>> BellmanFord(const Graph& graph, Vertex* source,
                                                       Vertex* sink, QHash<Vertex*, int>& pot);
}

#endif // MINCOSTFLOWSOLVER_H
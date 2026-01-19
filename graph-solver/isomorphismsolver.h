#ifndef ISOMORPHISMSOLVER_H
#define ISOMORPHISMSOLVER_H
#pragma once

#include <QMap>
#include "graph.h"

/**
 * @namespace IsomorphismSolver
 * @brief Algorithms to check if two graphs are isomorphic.
 */
namespace IsomorphismSolver
{
    /**
     * @brief Checks isomorphism using the VF2 algorithm.
     * @param graph1 First graph.
     * @param graph2 Second graph.
     * @return List of matched vertex pairs if isomorphic, empty otherwise.
     */
    QList<QPair<Vertex*, Vertex*>> VF2Algorithm(const Graph& graph1, const Graph& graph2);

    /** @brief Checks basic invariants (vertex/edge count, degree sequence). */
    bool CheckBasicInvariants(const Graph& graph1, const Graph& graph2);
    
    /** @brief Recursive backtracking for VF2. */
    bool IsomorphismBacktracking(const Graph& graph1, const Graph& graph2,
                                 QMap<Vertex*, Vertex*>& mapping, QSet<Vertex*> used,
                                 QList<Vertex*> unassigned);
                                 
    /** @brief Finds valid candidate vertices for mapping. */
    QList<Vertex*> FindCandidates(const Graph& graph1, const Graph& graph2,
                                   const QMap<Vertex*, Vertex*>& mapping, const QSet<Vertex*> used,
                                   Vertex* v);
                                   
    /** @brief Verifies if the complete mapping preserves adjacency. */
    bool VerifyIsomorphism(const Graph& graph1, const Graph& graph2,
                           const QMap<Vertex*, Vertex*>& mapping);
}

#endif // ISOMORPHISMSOLVER_H
#ifndef EULERIANSOLVER_H
#define EULERIANSOLVER_H
#pragma once

#include "connectivitysolver.h"
#include "TemplateStack.h" // Use Custom Data Structure

/**
 * @namespace EulerianSolver
 * @brief Algorithms for finding Eulerian paths and cycles.
 */
namespace EulerianSolver
{
    /**
     * @brief Checks if the graph has an Eulerian Cycle.
     */
    bool IsEulerian(const Graph& graph);

    /**
     * @brief Checks if the graph has an Eulerian Path (Half-Eulerian).
     */
    bool IsHalfEulerian(const Graph& graph);

    /**
     * @brief Finds an Eulerian Path or Cycle if one exists.
     * @param graph The graph.
     * @return List of edges in order of traversal.
     */
    QList<Edge*> FindEulirianPath(const Graph& graph);
};

#endif // EULERIANSOLVER_H
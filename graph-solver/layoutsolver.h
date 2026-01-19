#ifndef LAYOUTSOLVER_H
#define LAYOUTSOLVER_H

#include "graph.h"
#include <QRectF>

/**
 * @class LayoutSolver
 * @brief Implements a Force-Directed Graph Layout algorithm.
 * 
 * @details This solver uses a physics simulation inspired by the Fruchterman-Reingold algorithm.
 * Nodes repel each other like charged particles (Coulomb's Law), while Edges act as springs 
 * attracting connected nodes (Hooke's Law).
 * 
 * The system is subjected to a "Simulated Annealing" process where the 'temperature'
 * limits the maximum displacement per step, allowing the graph to settle into a 
 * minimal energy state over time.
 */
class LayoutSolver
{
public:
    /**
     * @brief Performs one iteration of the physics simulation to update vertex positions.
     * 
     * @param graph The graph object containing vertices and edges to layout.
     * @param bounds The boundary rectangle (Scene Rect) to keep nodes contained within. 
     *               Soft repulsion walls are applied near edges.
     * @param temperature Controls the maximum displacement (cooling factor). 
     *                    High temperature allows large movements (exploring global minima),
     *                    Low temperature allows fine-tuning (settling).
     */
    static void CalculateStep(Graph* graph, const QRectF& bounds, float temperature);
};

#endif // LAYOUTSOLVER_H
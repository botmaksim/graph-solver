#include "layoutsolver.h"
#include <cmath>
#include <QVector2D>
#include <QMap>
#include <random>
#include <algorithm>
#include <QGraphicsScene>

void LayoutSolver::CalculateStep(Graph* graph, const QRectF& bounds, float temperature)
{
    if (graph->_vertices.isEmpty()) return;

    // --- Tuning Constants ---
    const float k = 120.0f;              // Optimal distance between nodes
    const float repulsionConst = 2.0f;   // Multiplier for repulsion
    const float springStrength = 0.08f;  // Hooke's law coefficient
    const float damping = 0.85f;         // Velocity damping (0.0 - 1.0)
    const float maxVelocity = 40.0f;     // Cap absolute speed to prevent explosions
    const float wallRepulsion = 100.0f;  // How strongly edges push back
    
    // Vertex visual radius (approximate) used for collision with walls
    const float vRadius = 20.0f; 

    QMap<Vertex*, QVector2D> forces;

    // Initialize forces
    for (Vertex* v : graph->_vertices) {
        forces[v] = QVector2D(0, 0);
    }

    // 1. Repulsive Forces (Coulomb's Law: F = k^2 / d)
    // Applied between EVERY pair of vertices
    for (Vertex* v : graph->_vertices) {
        for (Vertex* u : graph->_vertices) {
            if (v == u) continue;

            QVector2D delta(v->x() - u->x(), v->y() - u->y());
            float dist = delta.length();
            
            // Avoid division by zero and extreme forces
            if (dist < 1.0f) dist = 1.0f; 

            // Calculate force magnitude
            float force = (k * k) / dist * repulsionConst;
            
            forces[v] += delta.normalized() * force;
        }
    }

    // 2. Attractive Forces (Hooke's Law: F = d^2 / k)
    // Applied only along Edges
    for (Edge* e : graph->_edges) {
        Vertex* v = e->_source;
        Vertex* u = e->_target;

        QVector2D delta(u->x() - v->x(), u->y() - v->y());
        float dist = delta.length();
        if (dist < 1.0f) dist = 1.0f;

        // Attractive force
        float force = (dist * dist) / k * springStrength;
        
        QVector2D vec = delta.normalized() * force;

        forces[v] += vec;
        forces[u] -= vec;
    }

    // 3. Center Gravity & Wall Repulsion
    QVector2D center(0, 0); // Attract to (0,0) which is center of scene usually
    
    for (Vertex* v : graph->_vertices) {
        // Weak gravity to center
        QVector2D toCenter = center - QVector2D(v->x(), v->y());
        forces[v] += toCenter * 0.05f; 

        // Wall Repulsion (Soft Walls)
        // Pushes nodes away if they get too close to the scene borders
        // We use vRadius to start pushing before the vertex visually hits the wall
        float distLeft = v->x() - bounds.left();
        float distRight = bounds.right() - v->x();
        float distTop = v->y() - bounds.top();
        float distBottom = bounds.bottom() - v->y();

        float threshold = 100.0f; // Distance from wall to start feeling force

        if (distLeft < threshold)  forces[v].setX(forces[v].x() + wallRepulsion * (threshold - distLeft)/threshold);
        if (distRight < threshold) forces[v].setX(forces[v].x() - wallRepulsion * (threshold - distRight)/threshold);
        if (distTop < threshold)   forces[v].setY(forces[v].y() + wallRepulsion * (threshold - distTop)/threshold);
        if (distBottom < threshold) forces[v].setY(forces[v].y() - wallRepulsion * (threshold - distBottom)/threshold);
    }

    // 4. Apply Physics (Velocity Integration)
    // v = v * damping + force * dt
    // x = x + v
    
    // Explicit padding to keep the circle entirely within view
    // Radius ~15-20, so 25 ensures it doesn't clip
    const qreal padding = 25.0; 

    for (Vertex* v : graph->_vertices) {
        // RESTRICTION: Do not move if the user is dragging the item
        if (v->scene() && v->scene()->mouseGrabberItem() == v) {
            v->_vx = 0;
            v->_vy = 0;
            continue;
        }

        QVector2D force = forces[v];
        
        // Update Velocity
        v->_vx = (v->_vx * damping) + (force.x() * 0.05f); // 0.05 is an arbitrary time step
        v->_vy = (v->_vy * damping) + (force.y() * 0.05f);

        // Cap velocity by temperature to allow cooling down
        float speed = std::sqrt(v->_vx * v->_vx + v->_vy * v->_vy);
        
        // The temperature acts as a speed limit that decreases over time
        float effectiveLimit = std::min(maxVelocity, temperature);
        
        if (speed > effectiveLimit) {
            float scale = effectiveLimit / speed;
            v->_vx *= scale;
            v->_vy *= scale;
        }

        // Apply to position
        if (std::abs(v->_vx) > 0.1f || std::abs(v->_vy) > 0.1f) {
            qreal newX = v->x() + v->_vx;
            qreal newY = v->y() + v->_vy;

            // RESTRICTION: Hard constraint to bounds
            // Ensure vertices never leave the visible area
            // We clamp the CENTER of the vertex so that (Center - Padding) >= Left
            newX = std::max(bounds.left() + padding, std::min(newX, bounds.right() - padding));
            newY = std::max(bounds.top() + padding, std::min(newY, bounds.bottom() - padding));

            v->setPos(newX, newY);
        }
    }
}
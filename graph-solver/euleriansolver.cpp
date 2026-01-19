#include "euleriansolver.h"
#include <spdlog/spdlog.h>

bool EulerianSolver::IsEulerian(const Graph &graph)
{
    if (!ConnectivitySolver::IsWeaklyConnected(graph)) {
        spdlog::debug("Graph is not weakly connected, cannot be Eulerian.");
        return false;
    }

    for (Vertex* v : graph._vertices)
    {
        int difference = graph._out_edges.value(v).size() - graph._in_edges.value(v).size();
        if (difference)
            return false;
    }

    return true;
}

bool EulerianSolver::IsHalfEulerian(const Graph &graph)
{
    if (!ConnectivitySolver::IsWeaklyConnected(graph)) {
        spdlog::debug("Graph is not weakly connected, cannot be Half-Eulerian.");
        return false;
    }

    int start_count = 0;
    int end_count = 0;

    for (Vertex* v : graph._vertices)
    {
        int difference = graph._out_edges.value(v).size() - graph._in_edges.value(v).size();
        if (difference > 1 || difference < -1) return false;
        if (difference == 1) ++start_count;
        if (difference == -1) ++end_count;
    }

    return (start_count == 0 && end_count == 0) || (start_count == 1 && end_count == 1);
}

QList<Edge *> EulerianSolver::FindEulirianPath(const Graph &graph)
{
    spdlog::debug("Searching for Eulerian Path...");
    QList<Edge*> path;

    if (!IsHalfEulerian(graph)) {
        // Changed from WARN to INFO, finding no path is a valid algorithm result
        spdlog::info("Graph is not Eulerian or Semi-Eulerian.");
        return path;
    }

    QHash<Vertex*, QList<Edge*>> out_edges;
    // Copy for modification during algorithm
    for (auto it = graph._out_edges.begin(); it != graph._out_edges.end(); ++it)
        out_edges[it.key()] = it.value();

    Vertex* start = *graph._vertices.begin();
    for (Vertex* v : graph._vertices)
        if (graph._out_edges.value(v).size() - graph._in_edges.value(v).size() == 1)
        {
            start = v;
            break;
        }

    // Use Custom TemplateStack
    TemplateStack<Vertex*> vertices_stack;
    vertices_stack.push(start);

    while (!vertices_stack.isEmpty())
    {
        Vertex* current = vertices_stack.top();

        if (!out_edges[current].isEmpty())
        {
            Edge* edge = out_edges[current].takeFirst();
            vertices_stack.push(edge->_target);
        }
        else
        {
            // TemplateStack::pop() is void, unlike QStack
            Vertex* target = vertices_stack.top();
            vertices_stack.pop();
            
            if (vertices_stack.isEmpty())
                break;

            Vertex* source = vertices_stack.top();
            for (Edge* e : graph._out_edges.value(source))
                if (e->_target == target)
                {
                    path.prepend(e);
                    break;
                }
        }
    }

    spdlog::info("Eulerian Path found. Length: {}", path.size());
    return path;
}
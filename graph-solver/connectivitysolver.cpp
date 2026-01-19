#include "connectivitysolver.h"
#include <spdlog/spdlog.h>

QSet<Vertex *> ConnectivitySolver::BFSWeak(const Graph &graph, Vertex *start)
{
    QSet<Vertex*> visited;
    QQueue<Vertex*> queue;

    queue.enqueue(start);
    visited.insert(start);

    while (!queue.isEmpty())
    {
        Vertex* u = queue.dequeue();

        for (Edge* e : graph._out_edges.value(u))
            if (!visited.contains(e->_target))
            {
                visited.insert(e->_target);
                queue.enqueue(e->_target);
            }

        for (Edge* e : graph._in_edges.value(u))
            if (!visited.contains(e->_source))
            {
                visited.insert(e->_source);
                queue.enqueue(e->_source);
            }
    }

    return visited;
}

QHash<Vertex *, int> ConnectivitySolver::WeakComponents(const Graph &graph)
{
    spdlog::debug("Searching for Weak Components...");
    int count = 0;
    QHash<Vertex*, int> coloring;
    QSet<Vertex*> unvisited = graph._vertices;

    while (!unvisited.isEmpty())
    {
        QSet<Vertex*> component = BFSWeak(graph, *unvisited.begin());
        for (Vertex* v : component)
            coloring[v] = count;
        unvisited.subtract(component);
        ++count;
    }

    spdlog::info("Found {} weak components.", count);
    return coloring;
}

int ConnectivitySolver::WeakComponentsAmmount(const Graph &graph)
{
    int count = 0;
    QSet<Vertex*> unvisited = graph._vertices;

    while (!unvisited.isEmpty())
    {
        QSet<Vertex*> component = BFSWeak(graph, *unvisited.begin());
        unvisited.subtract(component);
        ++count;
    }

    return count;
}

bool ConnectivitySolver::IsWeaklyConnected(const Graph &graph)
{
    return WeakComponentsAmmount(graph) <= 1;
}

void ConnectivitySolver::DFSFirstForKosaraju(const Graph &graph, Vertex *start, QSet<Vertex *> &visited, TemplateStack<Vertex *> &order)
{
    visited.insert(start);

    for (Edge* e : graph._out_edges.value(start))
        if (!visited.contains(e->_target))
            DFSFirstForKosaraju(graph, e->_target, visited, order);

    order.push(start);
}

void ConnectivitySolver::DFSSecondForKosaraju(const Graph &graph, Vertex *start, QSet<Vertex *> &visited,
                                              int count, QHash<Vertex*, int>& result)
{
    visited.insert(start);
    result[start] = count;

    for (Edge* e : graph._in_edges.value(start))
        if (!visited.contains(e->_source))
            DFSSecondForKosaraju(graph, e->_source, visited, count, result);
}

int ConnectivitySolver::StrongComponentsAmmount(const Graph &graph)
{
    QHash<Vertex *, int> strong_components = Kosaraju(graph);
    int max_color = 0;
    for (Vertex* v : graph._vertices)
        if (strong_components[v] > max_color)
            max_color = strong_components[v];

    return max_color + 1;
}

bool ConnectivitySolver::IsStronglyConnected(const Graph &graph)
{
    return StrongComponentsAmmount(graph) <= 1;
}

QHash<Vertex *, int> ConnectivitySolver::Kosaraju(const Graph &graph)
{
    spdlog::debug("Running Kosaraju's Algorithm for Strong Components...");
    QSet<Vertex*> visited;
    
    // Using custom TemplateStack instead of QStack
    TemplateStack<Vertex*> order;

    for (Vertex* v : graph._vertices)
        if (!visited.contains(v))
            DFSFirstForKosaraju(graph, v, visited, order);

    visited.clear();
    int count = 0;
    QHash<Vertex*, int> result;

    while (!order.isEmpty())
    {
        // Adaptation for TemplateStack (pop returns void)
        Vertex* v = order.top();
        order.pop();
        
        if (!visited.contains(v))
        {
            DFSSecondForKosaraju(graph, v, visited, count, result);
            ++count;
        }
    }

    spdlog::info("Found {} strong components.", count);
    return result;
}
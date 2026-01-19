#include "matchingsolver.h"
#include <spdlog/spdlog.h>
#include <climits>

bool MatchingSolver::IsBipartite(const Graph &graph, QSet<Vertex *> &left, QSet<Vertex *> &right)
{
    spdlog::debug("Checking if graph is bipartite...");
    QHash<Vertex*, int> color;

    for (Vertex* v : graph._vertices)
        if (!color.contains(v))
            if (!BFSColoring(graph, v, color, left, right)) {
                spdlog::debug("Graph is NOT bipartite.");
                return false;
            }

    spdlog::debug("Graph IS bipartite. Left set size: {}, Right set size: {}", left.size(), right.size());
    return true;
}

bool MatchingSolver::BFSColoring(const Graph &graph, Vertex *start, QHash<Vertex *, int> &color, QSet<Vertex *> &left, QSet<Vertex *> &right)
{
    QQueue<Vertex*> queue;
    queue.enqueue(start);
    color[start] = 0;
    left.insert(start);

    while (!queue.isEmpty())
    {
        Vertex* u = queue.dequeue();
        int cur_color = color[u];

        for (Edge* e : graph._out_edges.value(u))
        {
            Vertex* v = e->_target;
            if (!color.contains(v))
            {
                color[v] = 1 - cur_color;
                queue.enqueue(v);

                if (color[v]) right.insert(v);
                else left.insert(v);
            }
            else if (color[v] == cur_color)
                return false;
        }
        for (Edge* e : graph._in_edges.value(u))
        {
            Vertex* v = e->_source;
            if (!color.contains(v))
            {
                color[v] = 1 - cur_color;
                queue.enqueue(v);

                if (color[v]) right.insert(v);
                else left.insert(v);
            }
            else if (color[v] == cur_color)
                return false;
        }
    }

    return true;
}

QList<Edge *> MatchingSolver::HopcroftKarp(const Graph &graph)
{
    spdlog::info("Running Hopcroft-Karp algorithm...");
    QSet<Vertex*> left, right;
    if (!IsBipartite(graph, left, right)) {
        // Changed from WARN to INFO, finding not bipartite is a valid algorithm result
        spdlog::info("Hopcroft-Karp failed: Graph is not bipartite.");
        return {};
    }

    QHash<Vertex*, Vertex*> pair_u, pair_v;
    QHash<Vertex*, int> dist;

    for (Vertex* u : left)
        pair_u[u] = nullptr;
    for (Vertex* v : right)
        pair_v[v] = nullptr;

    int matching = 0;
    while (BFSHopcroft(graph, left, pair_u, pair_v, dist))
        for (Vertex* u : left)
            if (pair_u[u] == nullptr && DFSHopcroft(graph, u, pair_u, pair_v, dist))
                ++matching;

    QList<Edge*> result;
    for (auto it = pair_u.begin(); it != pair_u.end(); ++it)
        if (it.value() != nullptr) {
            bool found = false;
            // Try out edges (Left -> Right)
            for (Edge* e : graph._out_edges.value(it.key()))
                if (e->_target == it.value())
                {
                    result.append(e);
                    found = true;
                    break;
                }
            if (!found) {
                // Try in edges (Right -> Left)
                for (Edge* e : graph._in_edges.value(it.key()))
                    if (e->_source == it.value())
                    {
                         result.append(e);
                         found = true;
                         break;
                    }
            }
        }
    
    spdlog::info("Maximal matching found. Size: {}", result.size());
    return result;
}

bool MatchingSolver::DFSHopcroft(const Graph &graph, Vertex *u, QHash<Vertex *, Vertex *> &pair_u, QHash<Vertex *, Vertex *> &pair_v, QHash<Vertex *, int> &dist)
{
    if (!u)
        return true;

    // Check outgoing edges
    for (Edge* e : graph._out_edges.value(u))
    {
        Vertex* v = e->_target;
        if (dist.value(pair_v[v]) == dist.value(u) + 1)
            if (DFSHopcroft(graph, pair_v[v], pair_u, pair_v, dist))
            {
                pair_v[v] = u;
                pair_u[u] = v;
                return true;
            }
    }

    // Check incoming edges (treating edges as undirected for matching)
    for (Edge* e : graph._in_edges.value(u))
    {
        Vertex* v = e->_source;
        if (dist.value(pair_v[v]) == dist.value(u) + 1)
            if (DFSHopcroft(graph, pair_v[v], pair_u, pair_v, dist))
            {
                pair_v[v] = u;
                pair_u[u] = v;
                return true;
            }
    }

    dist[u] = INT_MAX;
    return false;
}

bool MatchingSolver::BFSHopcroft(const Graph &graph, const QSet<Vertex *> &left, QHash<Vertex *, Vertex *> &pair_u, QHash<Vertex *, Vertex *> &pair_v, QHash<Vertex *, int> &dist)
{
    dist.clear();
    QQueue<Vertex*> q;

    for (Vertex* u : left)
    {
        if (pair_u[u] == nullptr)
        {
            dist[u] = 0;
            q.enqueue(u);
        }
        else
            dist[u] = INT_MAX;
    }

    dist[nullptr] = INT_MAX;

    while (!q.empty())
    {
        Vertex* u = q.dequeue();

        if (dist.value(u) < dist.value(nullptr)) {
            // Check out edges
            for (Edge* e : graph._out_edges.value(u))
            {
                Vertex* v = e->_target;
                if (dist.value(pair_v[v]) == INT_MAX)
                {
                    dist[pair_v[v]] = dist[u] + 1;
                    q.enqueue(pair_v[v]);
                }
            }
            // Check in edges
            for (Edge* e : graph._in_edges.value(u))
            {
                Vertex* v = e->_source;
                if (dist.value(pair_v[v]) == INT_MAX)
                {
                    dist[pair_v[v]] = dist[u] + 1;
                    q.enqueue(pair_v[v]);
                }
            }
        }
    }

    return dist.value(nullptr) != INT_MAX;
}
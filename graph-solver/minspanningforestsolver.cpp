#include "minspanningforestsolver.h"
#include <spdlog/spdlog.h>

QList<Edge*> MinSpanningForestSolver::Kruskal(const Graph &graph)
{
    spdlog::debug("Kruskal's Algorithm started. Vertices: {}, Edges: {}", graph._vertices.size(), graph._edges.size());

    QList<Edge*> min_span_tree;
    QList<Edge*> edges = graph._edges.values();

    std::sort(edges.begin(), edges.end(),
              [](Edge* a, Edge* b) { return a->_weight < b->_weight; });

    QHash<Vertex*, Vertex*> parent;
    QHash<Vertex*, int> rank;

    for (Vertex* v : graph._vertices) {
        parent[v] = v;
        rank[v] = 0;
    }

    int total_weight = 0;

    for (Edge* e : edges)
    {
        Vertex* root1 = FindParent(e->_source, parent);
        Vertex* root2 = FindParent(e->_target, parent);

        if (root1 != root2)
        {
            min_span_tree.append(e);
            total_weight += e->_weight;

            if (rank[root1] < rank[root2])
                parent[root1] = root2;
            else if (rank[root1] > rank[root2])
                parent[root2] = root1;
            else
            {
                parent[root2] = root1;
                rank[root1]++;
            }
        }
    }

    spdlog::info("Kruskal finished. Forest size: {} edges. Total weight: {}", min_span_tree.size(), total_weight);
    return min_span_tree;
}

Vertex *MinSpanningForestSolver::FindParent(Vertex *v, QHash<Vertex *, Vertex *> &parent)
{
    if (parent[v] != v)
        parent[v] = FindParent(parent[v], parent);

    return parent[v];
}
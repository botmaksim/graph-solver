#include "coloringsolver.h"
#include <spdlog/spdlog.h>

QHash<Vertex *, int> ColoringSolver::DSatur(const Graph &graph)
{
    spdlog::debug("DSatur algorithm started. Vertices: {}", graph._vertices.size());
    QHash<Vertex*, int> color;
    QHash<Vertex*, int> saturation;
    QHash<Vertex*, QSet<int>> adjacent_colors;

    using QTuple = std::tuple<int, int, Vertex*>;
    auto cmp = [](const QTuple& a, const QTuple& b)
    {
        if (std::get<0>(a) != std::get<0>(b))
            return std::get<0>(a) < std::get<0>(b);
        return std::get<1>(a) < std::get<1>(b);
    };

    std::priority_queue<QTuple, std::vector<QTuple>, decltype(cmp)> pr_queue(cmp);

    for (Vertex* v : graph._vertices)
    {
        // Use value() to avoid inserting empty lists into the hash if they don't exist
        int degree = graph._out_edges.value(v).size() + graph._in_edges.value(v).size();
        pr_queue.push(std::make_tuple(0, degree, v));
        saturation[v] = 0;
    }

    while (!pr_queue.empty())
    {
        auto [max_sat, max_deg, max_v] = pr_queue.top();
        pr_queue.pop();

        if (color.contains(max_v))
            continue;

        int min_color = 0;
        // Use contains() instead of iterators for QSet
        while (adjacent_colors[max_v].contains(min_color))
            ++min_color;

        color[max_v] = min_color;

        for (Vertex* neighbor : GetNeighbors(graph, max_v))
            if (!color.contains(neighbor))
                if (!adjacent_colors[neighbor].contains(min_color))
                {
                    adjacent_colors[neighbor].insert(min_color);
                    ++saturation[neighbor];
                    pr_queue.push(std::make_tuple(saturation[neighbor], GetNeighbors(graph, neighbor).size(), neighbor));
                }
    }

    spdlog::info("DSatur finished. Vertices colored: {}", color.size());
    return color;
}

int ColoringSolver::ChromaticNumber(const Graph &graph, const QHash<Vertex *, int> &colors)
{
    if (graph._vertices.isEmpty())
        return 0;

    int max_color = 0;
    for (Vertex* v : graph._vertices)
    {
        if (colors.contains(v)) {
            int c = colors.value(v);
            if (c > max_color)
                max_color = c;
        }
    }

    return max_color + 1;
}

QSet<Vertex *> ColoringSolver::GetNeighbors(const Graph &graph, Vertex *v)
{
    QSet<Vertex*> neighbors;
    // Use value() for const correctness
    for (Edge* e : graph._out_edges.value(v))
        neighbors.insert(e->_target);
    for (Edge* e : graph._in_edges.value(v))
        neighbors.insert(e->_source);

    return neighbors;
}
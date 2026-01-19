#include "maxflowsolver.h"
#include <spdlog/spdlog.h>

QPair<int, QList<Edge *> > MaxFlowSolver::MultyMaxFlow(Graph &graph, const QList<Vertex *> &sources, const QList<Vertex *> &sinks)
{
    spdlog::info("Calculating Multi-Source/Multi-Sink Max Flow...");
    Vertex* super_source = new Vertex(0, 0 , "S");
    Vertex* super_sink = new Vertex(0, 0 , "T");
    graph.AddVertex(super_source);
    graph.AddVertex(super_sink);

    QSet<Edge*> artificial_edges;

    for (Vertex* v : sources)
    {
        Edge* e = new Edge(super_source, v, Qt::black, INT_MAX);
        graph.AddEdge(e);
        artificial_edges.insert(e);
    }

    for (Vertex* v : sinks)
    {
        Edge* e = new Edge(v, super_sink, Qt::black, INT_MAX);
        graph.AddEdge(e);
        artificial_edges.insert(e);
    }

    int max_flow = MaxFlowSolver::DinicMaxFlow(graph, super_source, super_sink);
    QList<Edge*> result = MaxFlowSolver::FindMinCut(graph, super_source, super_sink);

    QList<Edge*> filtered_result;
    for (Edge* e : result)
        if (!artificial_edges.contains(e))
            filtered_result.append(e);

    graph.RemoveVertex(super_source);
    graph.RemoveVertex(super_sink);

    artificial_edges.clear();
    
    spdlog::info("Max Flow: {}, Min Cut Edges: {}", max_flow, filtered_result.size());
    return {max_flow, filtered_result};
}

int MaxFlowSolver::DinicMaxFlow(const Graph &graph, Vertex *source, Vertex *sink)
{
    spdlog::debug("Dinic's Algorithm started.");
    for (Edge* e : graph._edges)
    {
        e->_flow = 0;

        if (!e->reverse)
        {
            e->reverse = new Edge(e->_target, e->_source, Qt::black, 0);
            e->reverse->reverse = e;
        }

        e->reverse->_flow = 0;
    }

    int max_flow = 0;

    QHash<Vertex*, int> level;

    while (BFSLayeredNetwork(graph, level, source, sink))
    {
        QHash<Vertex*, int> ptr;
        for (Vertex* v : graph._vertices)
            ptr[v] = 0;

        while (int path_flow = DFSBlockingFlow(graph, source, sink, INT_MAX, level, ptr))
            max_flow += path_flow;
    }

    return max_flow;
}

bool MaxFlowSolver::BFSLayeredNetwork(const Graph &graph, QHash<Vertex *, int> &level, Vertex *source, Vertex *sink)
{
    level.clear();
    for (Vertex* v : graph._vertices)
        level[v] = -1;

    QQueue<Vertex*> queue;
    queue.enqueue(source);
    level[source] = 0;

    while (!queue.isEmpty()) {
        Vertex* u = queue.dequeue();

        for (Edge* e : graph._out_edges.value(u))
            if (level[e->_target] == -1 && e->_flow < e->_weight)
            {
                level[e->_target] = level[u] + 1;
                queue.enqueue(e->_target);
            }
        for (Edge* e : graph._in_edges.value(u))
            if (level[e->reverse->_target] == -1 && e->reverse->_flow < e->reverse->_weight)
            {
                level[e->reverse->_target] = level[u] + 1;
                queue.enqueue(e->reverse->_target);
            }
    }

    return level[sink] != -1;
}

int MaxFlowSolver::DFSBlockingFlow(const Graph &graph, Vertex *u, Vertex *sink, int flow, QHash<Vertex *, int> &level, QHash<Vertex *, int> &ptr)
{
    if (u == sink || flow == 0)
        return flow;

    const QList<Edge*>& out = graph._out_edges.value(u);
    const QList<Edge*>& in = graph._in_edges.value(u);
    int outSize = out.size();
    int totalSize = outSize + in.size();

    for (int i = ptr[u]; i < totalSize; ++i)
    {
        Edge* e;
        if (i >= outSize)
            e = in[i - outSize]->reverse;
        else
            e = out[i];

        Vertex* v = e->_target;

        if (level[v] == level[u] + 1)
        {
            int pushed = DFSBlockingFlow(graph, v, sink, qMin(flow, e->_weight - e->_flow), level, ptr);

            if (pushed > 0)
            {
                e->_flow += pushed;
                e->reverse->_flow -= pushed;
                return pushed;
            }
        }

        ++ptr[u];
    }

    return 0;
}

QList<Edge *> MaxFlowSolver::FindMinCut(const Graph &graph, Vertex *source, Vertex *sink)
{
    QSet<Vertex*> reachable = BFSResidual(graph, source, sink);
    QList<Edge*> cut_edges;

    for (Edge* e : graph._edges)
    {
        if (e->_weight == 0)
            continue;

        if (reachable.contains(e->_source) && !reachable.contains(e->_target))
            cut_edges.append(e);
    }

    return cut_edges;
}

QSet<Vertex *> MaxFlowSolver::BFSResidual(const Graph &graph, Vertex *source, Vertex *sink)
{
    QSet<Vertex*> reachable;
    QQueue<Vertex*> queue;
    queue.enqueue(source);
    reachable.insert(source);

    while (!queue.isEmpty()) {
        Vertex* u = queue.dequeue();

        for (Edge* e : graph._out_edges.value(u))
            if (e->_flow < e->_weight && !reachable.contains(e->_target))
            {
                reachable.insert(e->_target);
                queue.enqueue(e->_target);
            }
    }

    return reachable;
}

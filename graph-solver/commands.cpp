#include "commands.h"
#include <spdlog/spdlog.h>

// Helper to find vertex by ID (needed for loading)
Vertex* findVertexById(Graph* graph, int id) {
    for (Vertex* v : graph->_vertices) {
        if (v->GetIndex() == id) return v;
    }
    return nullptr;
}

// ================= AddVertexCommand =================

AddVertexCommand::AddVertexCommand(QGraphicsScene* scene, Graph* graph, const QPointF& pos, const QString& name, int forceID, QUndoCommand* parent)
    : QUndoCommand(parent), _scene(scene), _graph(graph), _pos(pos), _name(name), _forceID(forceID), _ownsItem(false)
{
    _vertex = new Vertex(pos.x(), pos.y(), name);
    // If loading from file, we must restore the exact ID
    if (_forceID != -1) {
        _vertex->SetIndex(_forceID);
    }
    setText("Add Vertex");
}

AddVertexCommand::~AddVertexCommand()
{
    if (_ownsItem && _vertex) {
        delete _vertex;
    }
}

void AddVertexCommand::undo()
{
    _scene->removeItem(_vertex);
    _graph->DetachVertex(_vertex);
    _ownsItem = true;
    spdlog::debug("Undo: Vertex removed");
}

void AddVertexCommand::redo()
{
    // Important: When redoing, we must ensure the ID is respected if we forced it
    // Graph::AddVertex logic handles _next_index updates automatically
    _graph->AddVertex(_vertex);
    _scene->addItem(_vertex);
    _ownsItem = false;
    spdlog::debug("Redo: Vertex added");
}

void AddVertexCommand::save(QDataStream& out) const {
    out << _pos << _name << _vertex->GetIndex();
}

AddVertexCommand* AddVertexCommand::create(QDataStream& in, QGraphicsScene* scene, Graph* graph) {
    QPointF pos;
    QString name;
    int id;
    in >> pos >> name >> id;
    return new AddVertexCommand(scene, graph, pos, name, id);
}

// ================= AddEdgeCommand =================

AddEdgeCommand::AddEdgeCommand(QGraphicsScene* scene, Graph* graph, Vertex* source, Vertex* target, int weight, int price, QUndoCommand* parent)
    : QUndoCommand(parent), _scene(scene), _graph(graph), _source(source), _target(target), _ownsItem(false)
{
    _edge = new Edge(source, target, Qt::black, weight, price);
    setText("Add Edge");
}

AddEdgeCommand::~AddEdgeCommand()
{
    if (_ownsItem && _edge) {
        delete _edge;
    }
}

void AddEdgeCommand::undo()
{
    _scene->removeItem(_edge);
    _graph->DetachEdge(_edge);
    _ownsItem = true;
    spdlog::debug("Undo: Edge removed");
}

void AddEdgeCommand::redo()
{
    _graph->AddEdge(_edge);
    _scene->addItem(_edge);
    _ownsItem = false;
    spdlog::debug("Redo: Edge added");
}

void AddEdgeCommand::save(QDataStream& out) const {
    // Save IDs of source and target, plus attributes
    out << _source->GetIndex() << _target->GetIndex() << _edge->_weight << _edge->_price;
}

AddEdgeCommand* AddEdgeCommand::create(QDataStream& in, QGraphicsScene* scene, Graph* graph) {
    int srcId, tgtId, w, p;
    in >> srcId >> tgtId >> w >> p;
    Vertex* s = findVertexById(graph, srcId);
    Vertex* t = findVertexById(graph, tgtId);
    if (!s || !t) return nullptr;
    return new AddEdgeCommand(scene, graph, s, t, w, p);
}

// ================= RemoveItemCommand =================

RemoveItemCommand::RemoveItemCommand(QGraphicsScene* scene, Graph* graph, QGraphicsItem* item, QUndoCommand* parent)
    : QUndoCommand(parent), _scene(scene), _graph(graph), _item(item), _ownsItem(false)
{
    setText("Remove Item");

    _vertexRef = dynamic_cast<Vertex*>(item);
    _edgeRef = dynamic_cast<Edge*>(item);

    if (_vertexRef) {
        _isVertex = true;
        _itemId = _vertexRef->GetIndex();
        // Save connected edges so we can restore them on undo
        for(Edge* e : _graph->_out_edges[_vertexRef]) _connectedEdges.append(e);
        for(Edge* e : _graph->_in_edges[_vertexRef]) _connectedEdges.append(e);
    } else if (_edgeRef) {
        _isVertex = false;
        _edgeSrcId = _edgeRef->_source->GetIndex();
        _edgeTgtId = _edgeRef->_target->GetIndex();
        // Capture properties for robust matching during load
        _edgeWeight = _edgeRef->GetWeight();
        _edgePrice = _edgeRef->GetPrice();
    }
}

RemoveItemCommand::~RemoveItemCommand()
{
    if (_ownsItem) {
        if (_vertexRef) {
            delete _vertexRef;
            for(Edge* e : _connectedEdges) delete e;
        } else if (_edgeRef) {
            delete _edgeRef;
        }
    }
}

void RemoveItemCommand::undo()
{
    if (_vertexRef) {
        _graph->ReattachVertex(_vertexRef);
        _scene->addItem(_vertexRef);
        
        for(Edge* e : _connectedEdges) {
            _graph->ReattachEdge(e);
            _scene->addItem(e);
        }
    } else if (_edgeRef) {
        _graph->ReattachEdge(_edgeRef);
        _scene->addItem(_edgeRef);
    }
    
    _ownsItem = false;
    spdlog::debug("Undo: Item restored");
}

void RemoveItemCommand::redo()
{
    if (_vertexRef) {
        for(Edge* e : _connectedEdges) {
            _graph->DetachEdge(e);
            _scene->removeItem(e);
        }
        _graph->DetachVertex(_vertexRef);
        _scene->removeItem(_vertexRef);
    } else if (_edgeRef) {
        _graph->DetachEdge(_edgeRef);
        _scene->removeItem(_edgeRef);
    }
    
    _ownsItem = true;
    spdlog::debug("Redo: Item removed");
}

void RemoveItemCommand::save(QDataStream& out) const {
    out << _isVertex;
    if (_isVertex) {
        out << _itemId;
    } else {
        out << _edgeSrcId << _edgeTgtId;
        // Updated in Version 3: Save extra data to disambiguate multi-edges
        out << _edgeWeight << _edgePrice;
    }
}

RemoveItemCommand* RemoveItemCommand::create(QDataStream& in, QGraphicsScene* scene, Graph* graph, int version) {
    bool isVert;
    in >> isVert;
    
    QGraphicsItem* targetItem = nullptr;
    
    if (isVert) {
        int id;
        in >> id;
        targetItem = findVertexById(graph, id);
    } else {
        int srcId, tgtId;
        in >> srcId >> tgtId;
        
        int w = -1, p = -1;
        if (version >= 3) {
            in >> w >> p;
        }

        // Find edge by endpoints AND properties (if V3)
        Vertex* s = findVertexById(graph, srcId);
        Vertex* t = findVertexById(graph, tgtId);
        if (s && t) {
            for(Edge* e : graph->_out_edges[s]) {
                if (e->_target == t) {
                    // If V3, check weight/price match
                    if (version >= 3) {
                        if (e->_weight == w && e->_price == p) {
                            targetItem = e;
                            break;
                        }
                    } else {
                        // V2 fallback: Pick first match
                        targetItem = e;
                        break;
                    }
                }
            }
        }
    }
    
    if (!targetItem) return nullptr;
    return new RemoveItemCommand(scene, graph, targetItem);
}
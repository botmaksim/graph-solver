#include "graph.h"
#include <spdlog/spdlog.h>
#include <iomanip>
#include <QUndoStack>
#include <QUndoCommand>
#include "commands.h"

Graph::Graph() {
    spdlog::debug("Graph created.");
}

Graph::~Graph()
{
    spdlog::debug("Graph destroying. Vertices: {}, Edges: {}", _vertices.size(), _edges.size());
    // Create a copy of values to safely iterate while deleting
    QList<Vertex*> vertices = _vertices.values();
    for (Vertex* v : vertices) {
        RemoveVertex(v);
    }

    _vertices.clear();
    _edges.clear();
    _sources.clear();
    _targets.clear();
    _out_edges.clear();
    _in_edges.clear();
}

void Graph::AddVertex(Vertex* v)
{
    if (!v) return;
    
    // Assign index from free list or next available
    // IMPORTANT: If we are loading from file/undo, v->_index might already be set.
    // We only assign new index if it is not conflicting.
    
    bool needsId = true;
    
    // Check if ID is already valid and not in use
    bool idInUse = false;
    for(Vertex* existing : _vertices) {
        if(existing->_index == v->_index) {
            idInUse = true;
            break;
        }
    }
    
    if (!idInUse && v->_index >= 0) {
        // ID is pre-set (e.g. from file load)
        needsId = false;
        if (v->_index >= _next_index) _next_index = v->_index + 1;
        // If it was in free_indices, remove it
        if (_free_indices.count(v->_index)) _free_indices.erase(v->_index);
    }

    if (needsId) {
        if (!_free_indices.empty()) {
            v->_index = *_free_indices.begin();
            _free_indices.erase(_free_indices.begin());
        } else {
            v->_index = _next_index++;
        }
    }

    // FIX: Ensure vertex has a name so it appears on screen
    if (v->GetName().isEmpty()) {
        v->SetName(QString::number(v->_index));
    }

    _vertices.insert(v);
    spdlog::trace("Vertex added: {} (ID: {})", v->GetName().toStdString(), v->_index);
}

void Graph::AddEdge(Edge* e)
{
    if (!e || !e->_source || !e->_target) {
        spdlog::warn("Attempted to add invalid edge");
        return;
    }

    _edges.insert(e);
    _out_edges[e->_source].append(e);
    _in_edges[e->_target].append(e);
    spdlog::trace("Edge added: {} -> {} (Weight: {})", 
        e->_source->GetName().toStdString(), 
        e->_target->GetName().toStdString(), 
        e->_weight);
}

bool Graph::HasEdge(Vertex* source, Vertex* target) const
{
    if (!source || !target) return false;
    
    // Use value() to get the list safely without inserting default
    const QList<Edge*>& edges = _out_edges.value(source);
    for (Edge* e : edges) {
        if (e->_target == target) return true;
    }
    return false;
}

void Graph::RemoveVertex(Vertex* v)
{
    if (!_vertices.contains(v))
        return;

    _free_indices.insert(v->_index);

    // Make copies of the lists to avoid iterator invalidation during removal
    QList<Edge*> outEdgesToRemove = _out_edges.value(v);
    for (Edge* e : outEdgesToRemove)
        RemoveEdge(e);

    QList<Edge*> inEdgesToRemove = _in_edges.value(v);
    for (Edge* e : inEdgesToRemove)
        RemoveEdge(e);

    _vertices.remove(v);
    _out_edges.remove(v);
    _in_edges.remove(v);

    spdlog::trace("Vertex removed: {}", v->GetName().toStdString());
    delete v;
}

void Graph::RemoveEdge(Edge *e)
{
    if (!_edges.contains(e))
        return;

    Vertex* source = e->_source;
    Vertex* target = e->_target;

    if(_out_edges.contains(source)) 
        _out_edges[source].removeAll(e);
    
    if(_in_edges.contains(target))
        _in_edges[target].removeAll(e);

    _edges.remove(e);
    spdlog::trace("Edge removed: {} -> {}", 
        source ? source->GetName().toStdString() : "?", 
        target ? target->GetName().toStdString() : "?");

    delete e;
}

void Graph::DetachVertex(Vertex* v) {
    if (!_vertices.contains(v)) return;
    
    _free_indices.insert(v->_index);
    _vertices.remove(v);
    _out_edges.remove(v);
    _in_edges.remove(v);
    spdlog::trace("Vertex detached: {}", v->GetName().toStdString());
}

void Graph::ReattachVertex(Vertex* v) {
    // Reinsert vertex
    _vertices.insert(v);
    
    // Mark index as used
    if (_free_indices.count(v->_index)) {
        _free_indices.erase(v->_index);
    } else {
        // If it wasn't free, we ensure _next_index is ahead of it to prevent future collisions
        if (v->_index >= _next_index) {
            _next_index = v->_index + 1;
        }
    }
    
    spdlog::trace("Vertex reattached: {}", v->GetName().toStdString());
}

void Graph::DetachEdge(Edge* e) {
    if (!_edges.contains(e)) return;

    Vertex* source = e->_source;
    Vertex* target = e->_target;

    if(_out_edges.contains(source)) _out_edges[source].removeAll(e);
    if(_in_edges.contains(target)) _in_edges[target].removeAll(e);
    _edges.remove(e);
    spdlog::trace("Edge detached");
}

void Graph::ReattachEdge(Edge* e) {
    _edges.insert(e);
    _out_edges[e->_source].append(e);
    _in_edges[e->_target].append(e);
    spdlog::trace("Edge reattached");
}

int Graph::SaveToBinary(const QString& file_name, const QUndoStack* stack)
{
    if (!stack) return 1;

    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly))
        return 1;

    QDataStream out(&file);
    // Magic: GRPH, Version: 3 (Updated RemoveItem serialization)
    out.writeRawData("GRPH", 4);
    out << static_cast<qint16>(3);

    int count = stack->count();
    int index = stack->index();
    
    out << count << index;
    spdlog::info("Saving {} commands. Current index: {}", count, index);

    // QUndoStack doesn't let us iterate easily.
    // We rely on the fact that we can access commands via index.
    for (int i = 0; i < count; ++i) {
        const QUndoCommand* cmd = stack->command(i);
        // We use id() to determine type
        int cmdId = cmd->id(); 
        out << cmdId;
        
        // Polymorphic save
        if (cmdId == CMD_ADD_VERTEX) {
            static_cast<const AddVertexCommand*>(cmd)->save(out);
        } else if (cmdId == CMD_ADD_EDGE) {
            static_cast<const AddEdgeCommand*>(cmd)->save(out);
        } else if (cmdId == CMD_REMOVE_ITEM) {
            static_cast<const RemoveItemCommand*>(cmd)->save(out);
        } else {
            spdlog::error("Unknown command ID: {}", cmdId);
        }
    }

    return 0;
}

int Graph::LoadFromBinary(const QString& file_name, QUndoStack* stack, QGraphicsScene* scene)
{
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly))
        return 1;

    QDataStream in(&file);
    char magic[5]{};
    in.readRawData(magic, 4);
    if (strcmp(magic, "GRPH") != 0)
        return 2;

    quint16 version;
    in >> version;
    if (version != 3 && version != 2) {
        spdlog::error("File version {} not supported (requires 2 or 3).", version);
        return 3;
    }

    // --- Graph Reset ---
    // Critical: Properly clean up existing graph elements to prevent leaks and ID collisions.
    // We cannot just delete vertices because edges hold pointers to them.
    // RemoveVertex safely removes connected edges first.
    
    // Copy list because we modify container while iterating
    QList<Vertex*> verts = _vertices.values();
    for (Vertex* v : verts) {
        RemoveVertex(v);
    }
    
    _vertices.clear();
    _edges.clear();
    _out_edges.clear();
    _in_edges.clear();
    _free_indices.clear();
    _next_index = 0;
    
    // --- Stack Reset ---
    // Clear undo stack after cleaning graph.
    // This destroys existing commands. 
    // Since Graph is empty, commands in Undo/Redo state won't cause double-frees
    // (Checked in command destructors via _ownsItem checks vs Graph state).
    stack->clear();

    int count, targetIndex;
    in >> count >> targetIndex;
    spdlog::info("Loading {} commands. Target index: {}", count, targetIndex);

    for (int i = 0; i < count; ++i) {
        int cmdId;
        in >> cmdId;
        QUndoCommand* cmd = nullptr;

        if (cmdId == CMD_ADD_VERTEX) {
            cmd = AddVertexCommand::create(in, scene, this);
        } else if (cmdId == CMD_ADD_EDGE) {
            cmd = AddEdgeCommand::create(in, scene, this);
        } else if (cmdId == CMD_REMOVE_ITEM) {
            cmd = RemoveItemCommand::create(in, scene, this, version);
        }

        if (cmd) {
            // push() executes redo(), rebuilding the graph linearly from history
            stack->push(cmd);
        } else {
            spdlog::warn("Failed to create command from file at index {}. History truncated.", i);
            // If we fail to create a command (e.g. edge connects to missing vertex), 
            // we must stop to prevent cascading crashes.
            break; 
        }
    }

    // Set the index to where it was saved (handles Undo if Saved with Undo steps active)
    if (targetIndex >= 0 && targetIndex <= stack->count()) {
        stack->setIndex(targetIndex);
    } else {
        spdlog::warn("Target index {} out of bounds (Count: {}). Resetting to end.", targetIndex, stack->count());
        stack->setIndex(stack->count());
    }

    return 0;
}

int Graph::SaveToText(const std::string& file_name)
{
    // Text save remains legacy (State only, no history)
    std::ofstream out(file_name);
    if (!out.is_open()) {
        spdlog::error("Failed to open file for writing: {}", file_name);
        return 1;
    }

    out << "VERTICES " << _vertices.size() << "\n";
    std::map<Vertex*, int> v_indices;
    
    std::vector<Vertex*> sorted_vertices(_vertices.begin(), _vertices.end());
    
    for(Vertex* v : sorted_vertices) {
        v_indices[v] = v->_index;
        out << v->_index << " " << v->x() << " " << v->y() << " " << v->GetName().toStdString() << "\n";
    }

    out << "EDGES " << _edges.size() << "\n";
    for(Edge* e : _edges) {
        out << e->_source->_index << " " 
            << e->_target->_index << " " 
            << e->_weight << " " 
            << e->_price << "\n";
    }

    out.close();
    spdlog::info("Graph saved to text file: {}", file_name);
    return 0;
}

int Graph::LoadFromText(const std::string& file_name)
{
    std::ifstream in(file_name);
    if (!in.is_open()) {
        spdlog::error("Failed to open file for reading: {}", file_name);
        return 1;
    }

    std::string header;
    int v_count, e_count;

    in >> header >> v_count;
    if (header != "VERTICES") return 2;

    // Reset Graph State
    QList<Vertex*> verts = _vertices.values();
    for (Vertex* v : verts) RemoveVertex(v);
    
    _vertices.clear();
    _edges.clear();
    _out_edges.clear();
    _in_edges.clear();
    _free_indices.clear();
    _next_index = 0;

    std::map<int, Vertex*> id_map;

    for(int i = 0; i < v_count; ++i) {
        int id;
        float x, y;
        std::string name;
        in >> id >> x >> y >> name;
        
        Vertex* v = new Vertex(x, y, QString::fromStdString(name));
        v->_index = id;
        
        // Insert directly
        _vertices.insert(v);
        if (id >= _next_index) _next_index = id + 1;
        
        id_map[id] = v;
    }

    in >> header >> e_count;
    if (header != "EDGES") return 3;

    for(int i = 0; i < e_count; ++i) {
        int src_id, tgt_id, w, p;
        in >> src_id >> tgt_id >> w >> p;
        
        if (id_map.find(src_id) != id_map.end() && id_map.find(tgt_id) != id_map.end()) {
            Edge* e = new Edge(id_map[src_id], id_map[tgt_id], Qt::black, w, p);
            AddEdge(e);
        }
    }

    in.close();
    spdlog::info("Graph loaded from text file: {}", file_name);
    return 0;
}
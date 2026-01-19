#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QGraphicsScene>
#include <QDataStream>
#include "graph.h"
#include "vertex.h"
#include "edge.h"

// Command IDs for serialization
const int CMD_ADD_VERTEX = 1;
const int CMD_ADD_EDGE = 2;
const int CMD_REMOVE_ITEM = 3;

/**
 * @class AddVertexCommand
 * @brief Command to add a vertex to the scene and graph. Support Undo/Redo.
 */
class AddVertexCommand : public QUndoCommand
{
public:
    AddVertexCommand(QGraphicsScene* scene, Graph* graph, const QPointF& pos, const QString& name, int forceID = -1, QUndoCommand* parent = nullptr);
    ~AddVertexCommand();

    void undo() override;
    void redo() override;
    
    // Serialization
    int id() const override { return CMD_ADD_VERTEX; }
    void save(QDataStream& out) const;
    static AddVertexCommand* create(QDataStream& in, QGraphicsScene* scene, Graph* graph);

private:
    QGraphicsScene* _scene;
    Graph* _graph;
    Vertex* _vertex;
    QPointF _pos;
    QString _name;
    int _forceID; // Used when loading from file to restore exact ID
    bool _ownsItem;
};

/**
 * @class AddEdgeCommand
 * @brief Command to add an edge. Supports Undo/Redo.
 */
class AddEdgeCommand : public QUndoCommand
{
public:
    AddEdgeCommand(QGraphicsScene* scene, Graph* graph, Vertex* source, Vertex* target, int weight, int price, QUndoCommand* parent = nullptr);
    ~AddEdgeCommand();

    void undo() override;
    void redo() override;

    // Serialization
    int id() const override { return CMD_ADD_EDGE; }
    void save(QDataStream& out) const;
    static AddEdgeCommand* create(QDataStream& in, QGraphicsScene* scene, Graph* graph);

private:
    QGraphicsScene* _scene;
    Graph* _graph;
    Edge* _edge;
    Vertex* _source;
    Vertex* _target;
    bool _ownsItem;
};

/**
 * @class RemoveItemCommand
 * @brief Command to remove an item. Supports Undo/Redo.
 */
class RemoveItemCommand : public QUndoCommand
{
public:
    RemoveItemCommand(QGraphicsScene* scene, Graph* graph, QGraphicsItem* item, QUndoCommand* parent = nullptr);
    ~RemoveItemCommand();

    void undo() override;
    void redo() override;

    // Serialization
    int id() const override { return CMD_REMOVE_ITEM; }
    void save(QDataStream& out) const;
    static RemoveItemCommand* create(QDataStream& in, QGraphicsScene* scene, Graph* graph, int version = 3);

private:
    QGraphicsScene* _scene;
    Graph* _graph;
    QGraphicsItem* _item;
    
    // Pointers for runtime
    Vertex* _vertexRef = nullptr;
    Edge* _edgeRef = nullptr;
    
    // Data for serialization (since pointers are invalid after save/load)
    int _itemId = -1; // Vertex ID
    int _edgeSrcId = -1;
    int _edgeTgtId = -1;
    // V3 Fix: Save edge attributes to disambiguate multiple edges between same vertices
    int _edgeWeight = -1; 
    int _edgePrice = -1;

    bool _isVertex = false;

    QList<Edge*> _connectedEdges; 
    bool _ownsItem;
};

#endif // COMMANDS_H
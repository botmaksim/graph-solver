#ifndef VERTEX_H
#define VERTEX_H
#pragma once

#include <QGraphicsItem>
#include <QPainter>
#include <QLineF>
#include <cmath>

/**
 * @class Vertex
 * @brief Represents a node in the graph, displayed as a circle on the scene.
 */
class Vertex : public QGraphicsItem
{
    friend class Graph;
    friend class LayoutSolver; // Allow physics solver to access internal state directly
private:
    static const int _Radius = 15;
    float _x, _y;
    int _index = 0;
    QString _name;
    QColor _color;

    // Physics state for dynamic layout
    double _vx = 0.0;
    double _vy = 0.0;

public:
    static bool _showIdentifiers; ///< Global toggle for showing text labels

    Vertex();
    
    /**
     * @brief Constructs a new Vertex.
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param name Label of the vertex.
     * @param color Initial color.
     */
    Vertex(float x, float y, QString name = "", QColor color = Qt::white)
        : _x(x) , _y(y), _name(name), _color(color)
    {
        setPos(x, y);
        setFlag(ItemIsMovable);
        setFlag(ItemSendsGeometryChanges);
        setZValue(2);
        SetColorByName(); // Ensure color is correct on creation
    }

    ~Vertex()
    {

    }

    /**
     * @brief Gets the internal index/ID of the vertex.
     */
    int GetIndex() const { return _index; }

    /**
     * @brief Sets the internal index/ID of the vertex.
     * @param index New index.
     */
    void SetIndex(int index) { _index = index; }

    /**
     * @brief Gets the name of the vertex.
     * @return The name string.
     */
    QString GetName()
    {
        return _name;
    }

    /**
     * @brief Sets the name of the vertex and auto-updates color based on name type.
     * @param name The new name.
     */
    void SetName(const QString name)
    {
        _name = name;
        SetColorByName(); // Automatically update color when name changes
        update();
    }

    /**
     * @brief Gets the current color.
     * @return QColor object.
     */
    QColor GetColor()
    {
        return _color;
    }

    /**
     * @brief Sets the vertex color.
     * @param color New color.
     */
    void SetColor(const QColor color)
    {
        _color = color;
        update();
    }

    /**
     * @brief Sets color based on name (S=Cyan, T=Magenta, else White).
     */
    void SetColorByName()
    {
        // We set the base fill color here.
        // Note: The paint method handles the forced border coloring for S/T.
        if (_name.startsWith("S"))
            _color = Qt::cyan;
        else if (_name.startsWith("T"))
            _color = Qt::magenta;
        else
            _color = Qt::white;

        update();
    }

    /** @brief Returns bounding rectangle. */
    QRectF boundingRect() const override
    {
        return QRectF(-_Radius - 2, -_Radius - 2, 2 * _Radius + 4, 2 * _Radius + 4);
    }

    /** @brief Paints the vertex on the scene. */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        // 1. Draw Circle Fill
        painter->setBrush(_color);

        // 2. Logic for Border (Highlighting Source/Target always)
        QPen pen;
        if (_name.startsWith("S")) {
            pen.setColor(Qt::blue); // Darker blue border for Source
            pen.setWidth(3);
        } else if (_name.startsWith("T")) {
            pen.setColor(Qt::red); // Darker red/magenta border for Target
            pen.setWidth(3);
        } else {
            pen.setColor(Qt::black);
            pen.setWidth(2);
        }
        
        painter->setPen(pen);
        painter->drawEllipse(-_Radius, -_Radius, 2 * _Radius, 2 * _Radius);

        // 3. Draw Text if enabled
        if (_showIdentifiers) {
            painter->setPen(Qt::black);
            QFont font = painter->font();
            font.setBold(true);
            font.setPointSize(10);
            painter->setFont(font);
            
            // Ensure text is drawn over the circle
            painter->drawText(boundingRect(), Qt::AlignCenter, _name);
        }
    }

    /** @brief Handles geometry changes (position updates). */
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override
    {
        if (change == ItemPositionChange) {
            // Placeholder for grid snapping or bounds checking if needed later
        }
        return QGraphicsItem::itemChange(change, value);
    }
};

#endif // VERTEX_H
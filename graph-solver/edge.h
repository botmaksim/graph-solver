#ifndef EDGE_H
#define EDGE_H
#pragma once

#include "vertex.h"

/**
 * @class Edge
 * @brief Represents a directed connection between two vertices.
 */
class Edge : public QGraphicsItem
{
public:
    inline static int _mode = 1; ///< Display mode: 1=Weight/Price, 2=Flow.
    static const int _Arrow_size = 15; ///< Size of the arrow head.
    
    Vertex* _source; ///< Source Vertex.
    Vertex* _target; ///< Target Vertex.
    int _weight; ///< Capacity or Weight.
    int _price;  ///< Cost per unit of flow.
    int _flow = 0; ///< Current flow.
    QColor _color = Qt::black; ///< Drawing color.

    Edge* reverse = nullptr; ///< Pointer to reverse edge (for residual graphs).

public:
    Edge();

    /**
     * @brief Constructs an Edge.
     * @param source Source vertex.
     * @param target Target vertex.
     * @param color Line color.
     * @param weight Edge weight/capacity.
     * @param price Edge price/cost.
     */
    Edge(Vertex* source, Vertex* target, QColor color = Qt::black, int weight = 1, int price = 0)
        : _source(source), _target(target), _weight(weight), _price(price), _color(color)
    {
        prepareGeometryChange();
    }

    ~Edge()
    {
        _source = nullptr;
        _target = nullptr;

        if (reverse)
        {
            reverse->reverse = nullptr;
            delete reverse;
            reverse = nullptr;
        }
    }

    /** @return The weight/capacity of the edge. */
    int GetWeight()
    {
        return _weight;
    }

    /** @param weight New weight. */
    void SetWeight(int weight)
    {
        _weight = weight;
        update();
    }

    /** @return The price/cost of the edge. */
    int GetPrice()
    {
        return _price;
    }

    /** @param price New price. */
    void SetPrice(int price)
    {
        _price = price;
        update();
    }

    /** @return Current color of the edge. */
    QColor GetColor()
    {
        return _color;
    }

    /** @param color New color. */
    void SetColor(const QColor color)
    {
        _color = color;
        update();
    }

    /** @brief Returns the bounding rectangle for QGraphicsScene. */
    QRectF boundingRect() const override
    {
        if (!_source || !_target) return QRectF();

        qreal extra = _Arrow_size + 20; // Allow space for arrow and text
        return QRectF(_source->pos(), _target->pos()).normalized()
            .adjusted(-extra, -extra, extra, extra);
    }

    /** @brief Paints the edge (line and arrow) on the scene. */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        if (!_source || !_target) return;

        QLineF line(_source->pos(), _target->pos());
        if (line.length() == 0) return;

        painter->setPen(QPen(_color, 2, Qt::SolidLine, Qt::RoundCap));
        painter->drawLine(line);

        QPointF text_pos = (_target->pos() * 3 + _source->pos()) / 4;
        double angle = std::atan2(line.dy(), line.dx());
        QPointF arrow_p1 = text_pos - QPointF(
                               cos(angle + M_PI / 6) * _Arrow_size,
                               sin(angle + M_PI / 6) * _Arrow_size
                               );
        QPointF arrow_p2 = text_pos - QPointF(
                               cos(angle - M_PI / 6) * _Arrow_size,
                               sin(angle - M_PI / 6) * _Arrow_size
                               );

        painter->drawPolygon(QPolygonF() << text_pos << arrow_p1 << arrow_p2);
        
        // Offset text slightly to avoid overlapping the arrow
        QPointF normal(line.dy() / line.length(), -line.dx() / line.length());
        text_pos += normal * 20;

        painter->setPen(Qt::darkGreen);
        QFont font = painter->font();
        font.setPointSize(10);
        painter->setFont(font);

        if (_mode == 1)
            painter->drawText(QRectF(text_pos.x() - 50, text_pos.y() - 15, 100, 30), Qt::AlignCenter, QString::number(_weight) + "/" + QString::number(_price));
        if (_mode == 2)
            painter->drawText(QRectF(text_pos.x() - 50, text_pos.y() - 15, 100, 30), Qt::AlignCenter, QString::number(_weight) + "/" + QString::number(_price) + "/" + QString::number(_flow));
    }
};

#endif // EDGE_H
#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H
#pragma once

#include "QGraphicsScene"
#include "QGraphicsSceneMouseEvent"

/**
 * @class MyGraphicsScene
 * @brief Custom graphics scene to handle mouse events and item selection.
 * 
 * Acts as the bridge between raw Qt mouse events and the application logic,
 * handling different interaction modes (Moving, Adding Vertices, Adding Edges).
 */
class MyGraphicsScene : public QGraphicsScene
{
public:
    MyGraphicsScene();
    
    /** 
     * @brief Sets the index of the item type to add.
     * @param index Context-dependent index (e.g. type of vertex).
     */
    void SetItemIndex(int index);
    
    /** 
     * @brief Sets the interaction mode for mouse clicks.
     * 
     * @param index The mode ID:
     * - 0: Edit/Move Mode (Select items)
     * - 1: Add Vertex Mode
     * - 2: Add Edge Mode
     * - 3: Remove Item Mode
     * - 4: Algorithm Result Mode (Interaction disabled)
     */
    void SetItemMode(int index);
    
    /** @brief Sets the active state (used to trigger UI updates in MainWindow loop). */
    void setActive(bool v);
    
    /** @brief Returns active state. True if a significant event (click) occurred. */
    bool isActive();
    
    /** 
     * @brief Handle mouse press to select items or record click position. 
     * @param event The mouse event details.
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    
    /** @brief Gets the last clicked point in scene coordinates. */
    QPointF getPoint();
    
    /** @brief Gets the currently selected item (Vertex or Edge). */
    QGraphicsItem* getSelectedItem();


private:
    QPointF point;
    int item_index = 0;
    int item_mode = -1;
    bool active = false;
    QGraphicsItem* sel_item;
};

#endif // MYGRAPHICSSCENE_H
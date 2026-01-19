#ifndef MYVIEW_H
#define MYVIEW_H

#include <QWidget>
#include <QUndoStack>
#include <QResizeEvent>

#include "mygraphicsscene.h"
#include "graph.h"

namespace Ui {
class MyView;
}

/**
 * @class MyView
 * @brief Widget containing the graphics view and scene for a single graph page.
 * Maintains its own UndoStack.
 */
class MyView : public QWidget
{
    Q_OBJECT
    friend class MainWindow;

public:
    /**
     * @brief Constructor.
     * @param parent Parent widget.
     */
    explicit MyView(QWidget *parent = nullptr);
    ~MyView();

protected:
    /** 
     * @brief Handles resize events to update the scene rectangle. 
     * Ensures layout solver bounds match the visible area.
     */
    void resizeEvent(QResizeEvent* event) override;

private:
    MyGraphicsScene* scene = nullptr;
    Graph* graph = nullptr;
    QUndoStack* undoStack = nullptr; // Per-page undo stack
    
    Vertex* pot_source = nullptr;
    bool normal_coloring = true;
    int item_mode = 0;
    int vertex_mode = 0;
    int item_index = 0;
    Ui::MyView *ui;
};

#endif // MYVIEW_H
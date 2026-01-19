#include "myview.h"
#include "ui_myview.h"
#include <spdlog/spdlog.h>

MyView::MyView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyView)
{
    ui->setupUi(this);

    scene = new MyGraphicsScene();
    ui->graphicsView->setScene(scene);
    
    // Hide scrollbars
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    // Set initial scene rect based on current (likely small) size
    // resizeEvent will correct this when layout settles
    ui->graphicsView->setSceneRect(-width()/2, -height()/2, width(), height());
    scene->setSceneRect(-width()/2, -height()/2, width(), height());

    graph = new Graph();
    undoStack = new QUndoStack(this); // Init stack
    pot_source = nullptr;

    spdlog::debug("MyView constructed");
}

MyView::~MyView()
{
    delete graph;
    delete scene;
    delete ui;
    spdlog::debug("MyView destroyed");
}

void MyView::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    
    // Update the scene rectangle to match the new view size.
    // We keep (0,0) in the center.
    if (ui && ui->graphicsView) {
        // Use a slightly smaller rect to ensure no scrollbars trigger due to borders
        // Subtracting small buffer prevents scrollbars from flickering
        int w = ui->graphicsView->width() - 4; 
        int h = ui->graphicsView->height() - 4;
        
        // CRITICAL FIX: Set the Scene's rect to match the View's rect.
        // This ensures scene->sceneRect() returns the fixed view bounds
        // which LayoutSolver uses to constrain vertices.
        QRectF newRect(-w / 2, -h / 2, w, h);
        ui->graphicsView->setSceneRect(newRect);
        scene->setSceneRect(newRect);
        
        spdlog::trace("View resized. New SceneRect: [{}, {}, {}, {}]", newRect.x(), newRect.y(), newRect.width(), newRect.height());
    }
}
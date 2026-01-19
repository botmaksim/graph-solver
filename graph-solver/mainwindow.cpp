#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <spdlog/spdlog.h>
#include <QKeySequence>
#include <QImage>
#include <QPainter>
#include <QComboBox> 

void MainWindow::updateScene()
{
    if (cur_view->scene->isActive())
    {
        switch (cur_view->item_mode) {
        case 0:
            UpdateInfo();
            break;
        case 1:
            AddVertex();
            break;
        case 2:
            AddEdge();
            break;
        case 3:
            RemoveItem();
            break;
        }
        cur_view->scene->setActive(false);
    }

    cur_view->scene->update();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- Renaming UI Element (Requirement 2) ---
    // Assuming index 0 was "Move Item", we rename it to "Edit Item"
    ui->modeComboBox->setItemText(0, tr("Edit Item"));

    // --- Undo / Redo Setup ---
    // Global Actions, connections are dynamic based on view
    undoAction = new QAction(tr("Undo"), this);
    undoAction->setShortcuts(QKeySequence::Undo);
    undoAction->setVisible(false); 

    redoAction = new QAction(tr("Redo"), this);
    redoAction->setShortcuts(QKeySequence::Redo);
    redoAction->setVisible(false); 

    // --- Layout & Export Actions ---
    layoutAction = new QAction(tr("Auto Layout"), this);
    layoutAction->setCheckable(true);
    connect(layoutAction, &QAction::triggered, this, &MainWindow::ToggleLayoutAnimation);

    exportAction = new QAction(tr("Export Image"), this);
    connect(exportAction, &QAction::triggered, this, &MainWindow::exportImage);

    // --- Show Identifiers Action ---
    toggleIdAction = new QAction(tr("Show IDs"), this);
    toggleIdAction->setCheckable(true);
    toggleIdAction->setChecked(Vertex::_showIdentifiers); // Sync with static default
    connect(toggleIdAction, &QAction::triggered, this, &MainWindow::ToggleIdentifiers);

    // --- Toolbar Creation ---
    mainToolBar = new QToolBar("Main Toolbar", this);
    addToolBar(Qt::TopToolBarArea, mainToolBar);
    mainToolBar->addAction(undoAction);
    mainToolBar->addAction(redoAction);
    mainToolBar->addSeparator();
    mainToolBar->addAction(layoutAction);
    mainToolBar->addAction(toggleIdAction); // Add checkmark to toolbar
    mainToolBar->addAction(exportAction);

    // --- Timer Setup ---
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateScene()));
    timer->start(10);

    layoutTimer = new QTimer(this);
    connect(layoutTimer, &QTimer::timeout, this, &MainWindow::applyLayoutStep);

    // --- View Setup ---
    AddView();
    sel_view = cur_view;

    v_edit = new VertexEditor();
    e_edit = new EdgeEditor();
    a_edit = new AlgorithmEditor();

    ui->stackedEditorWidget->addWidget(v_edit);
    ui->stackedEditorWidget->addWidget(e_edit);
    ui->stackedEditorWidget->addWidget(a_edit);

    connect(v_edit, &VertexEditor::VertexInfoChanged,
            this, &MainWindow::SetVertexAttributes);

    connect(e_edit, &EdgeEditor::EdgeInfoChanged,
            this, &MainWindow::SetEdgeAttributes);

    connect(a_edit, &AlgorithmEditor::ExecuteAlgorithm,
            this, &MainWindow::ExecuteAlgorithm);

    connect(a_edit, &AlgorithmEditor::ChangedAlgorithm,
            this, &MainWindow::PrepareGraphWithChangedAlgorithm);

    ChangeMode(0);

    setWindowTitle("Graph Solver");
    spdlog::debug("MainWindow constructed");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --- ID Toggle Implementation ---
void MainWindow::ToggleIdentifiers()
{
    Vertex::_showIdentifiers = toggleIdAction->isChecked();
    spdlog::info("Identifiers visibility toggled: {}", Vertex::_showIdentifiers);
    cur_view->scene->update(); // Force redraw
}

// --- Layout Implementation ---
void MainWindow::ToggleLayoutAnimation()
{
    if (layoutAction->isChecked()) {
        spdlog::info("Layout Animation Started");
        // Use a higher starting temperature for more movement capability
        layoutTemp = 50.0f; 
        layoutTimer->start(30); // ~30fps
    } else {
        spdlog::info("Layout Animation Stopped");
        layoutTimer->stop();
    }
}

void MainWindow::applyLayoutStep()
{
    if (cur_view && cur_view->graph) {
        QRectF bounds = cur_view->scene->sceneRect();
        LayoutSolver::CalculateStep(cur_view->graph, bounds, layoutTemp);
        
        // Slower cooling for longer stabilization
        if (layoutTemp > 1.0f) layoutTemp *= 0.99f; // Slower cooling
    }
}

// --- Export Implementation ---
void MainWindow::exportImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Image"), QDir::homePath(), tr("PNG Files (*.png)"));
    if (fileName.isEmpty()) return;

    if (!fileName.endsWith(".png", Qt::CaseInsensitive))
        fileName += ".png";

    // Calculate the bounding box of all items with some padding
    QRectF rect = cur_view->scene->itemsBoundingRect();
    // Add padding (20px)
    rect.adjust(-20, -20, 20, 20);

    QImage image(rect.size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::white);
    
    QPainter painter(&image);
    // Render only the relevant part of the scene
    cur_view->scene->render(&painter, QRectF(), rect);
    
    if (image.save(fileName)) {
        spdlog::info("Graph exported to image: {}", fileName.toStdString());
        QMessageBox::information(this, "Success", "Image saved successfully.");
    } else {
        spdlog::error("Failed to save image to {}", fileName.toStdString());
        QMessageBox::critical(this, "Error", "Failed to save image.");
    }
}

// ... Existing methods below ...

void MainWindow::AddView()
{
    if (ui->stackedSceneWidget->count() >= 10)
    {
        spdlog::warn("Attempted to add view beyond limit (10).");
        QMessageBox::critical(this, "Error", "It's forbidden to open more than 10 pages");
        return;
    }

    MyView* view_container = new MyView();
    ui->stackedSceneWidget->addWidget(view_container);

    ui->totalPageSpinBox->setValue(ui->stackedSceneWidget->count());
    SwitchToView(ui->stackedSceneWidget->count() - 1);
    spdlog::debug("New view added. Total views: {}", ui->stackedSceneWidget->count());
}

void MainWindow::RemoveView()
{
    if (ui->stackedSceneWidget->count() <= 1)
    {
        spdlog::warn("Attempted to delete the last page.");
        QMessageBox::critical(this, "Error", "It's forbidden to delete last page");
        return;
    }

    // Stack is owned by view, will be deleted with view
    ui->stackedSceneWidget->removeWidget(cur_view);

    delete cur_view;
    cur_view = nullptr;

    ui->totalPageSpinBox->setValue(ui->stackedSceneWidget->count());
    SwitchToView(qMin(cur_index, ui->stackedSceneWidget->count() - 1));
}

void MainWindow::SwitchToView(int index)
{
    cur_index = index;
    ui->stackedSceneWidget->setCurrentIndex(index);

    ui->curPageSpinBox->setMaximum(ui->totalPageSpinBox->value());
    ui->selPageSpinBox->setMaximum(ui->totalPageSpinBox->value());
    ui->curPageSpinBox->setValue(index + 1);

    cur_view = dynamic_cast<MyView*>(ui->stackedSceneWidget->currentWidget());
    
    // Switch Undo/Redo Context
    if (cur_view) {
        undoAction->disconnect();
        redoAction->disconnect();
        
        // Connect actions to the current view's stack
        connect(undoAction, &QAction::triggered, cur_view->undoStack, &QUndoStack::undo);
        connect(cur_view->undoStack, &QUndoStack::canUndoChanged, undoAction, &QAction::setVisible);
        
        connect(redoAction, &QAction::triggered, cur_view->undoStack, &QUndoStack::redo);
        connect(cur_view->undoStack, &QUndoStack::canRedoChanged, redoAction, &QAction::setVisible);
        
        // Refresh visibility state
        undoAction->setVisible(cur_view->undoStack->canUndo());
        redoAction->setVisible(cur_view->undoStack->canRedo());
    }

    ChangeMode(cur_mode);
}

void MainWindow::AddVertex()
{
    int x = cur_view->scene->getPoint().x();
    int y = cur_view->scene->getPoint().y();

    QString name = v_edit->GetName();

    // Push to CURRENT view's stack
    QUndoCommand* addCmd = new AddVertexCommand(cur_view->scene, cur_view->graph, QPointF(x, y), name);
    cur_view->undoStack->push(addCmd);
    
    sel_v = nullptr;
}

void MainWindow::AddEdge()
{
    if (Vertex* v = dynamic_cast<Vertex*>(cur_view->scene->getSelectedItem()))
    {
        if (cur_view->pot_source == nullptr)
        {
            cur_view->pot_source = v;
            cur_view->pot_source->SetColor(Qt::blue);
            return;
        }
        else if (v != cur_view->pot_source)
        {
            if (cur_view->graph->HasEdge(cur_view->pot_source, v)) {
                // Changed from WARN to INFO, as this is a user feedback, not a system error
                spdlog::info("User attempted to add duplicate edge between {} and {}. Action blocked.", 
                             cur_view->pot_source->GetName().toStdString(), v->GetName().toStdString());
                ui->infoLable->setText("Edge already exists between these vertices!");
                
                cur_view->pot_source->SetColorByName();
                cur_view->pot_source = nullptr;
                return;
            }

            // Push to CURRENT view's stack
            QUndoCommand* addEdgeCmd = new AddEdgeCommand(cur_view->scene, cur_view->graph, 
                                                         cur_view->pot_source, v, 
                                                         e_edit->weight, e_edit->price);
            cur_view->undoStack->push(addEdgeCmd);
        }
    }
    if (cur_view->pot_source)
    {
        cur_view->pot_source->SetColorByName();
        cur_view->pot_source = nullptr;
    }
}

void MainWindow::RemoveItem()
{
    QGraphicsItem* item = cur_view->scene->getSelectedItem();
    if (!item) return;

    if (dynamic_cast<Vertex*>(item) || dynamic_cast<Edge*>(item)) {
        QUndoCommand* removeCmd = new RemoveItemCommand(cur_view->scene, cur_view->graph, item);
        cur_view->undoStack->push(removeCmd);
    }
}

void MainWindow::ShowGraph()
{
    for (Vertex* v : cur_view->graph->_vertices)
    {
        cur_view->scene->addItem(v);

        sel_v = v;
        SetVertexAttributes(v->GetName());
        sel_v = nullptr;
    }

    for (Edge* e : cur_view->graph->_edges)
    {
        cur_view->scene->addItem(e);

        sel_e = e;
        SetEdgeAttributes(e->_weight, e->_price);
        sel_e = nullptr;
    }
}

void MainWindow::UpdateInfo()
{
    sel_v = nullptr;
    sel_e = nullptr;

    if (Vertex* v = dynamic_cast<Vertex*>(cur_view->scene->getSelectedItem()))
    {
        sel_v = v;
        v_edit->SetInfo(v->GetName());
        ui->stackedEditorWidget->setCurrentIndex(1);
    }
    else if (Edge* e = dynamic_cast<Edge*>(cur_view->scene->getSelectedItem()))
    {
        sel_e = e;
        e_edit->SetInfo(e->GetWeight(), e->GetPrice());
        ui->stackedEditorWidget->setCurrentIndex(2);
    }
    else
        ui->stackedEditorWidget->setCurrentIndex(0);
}

void MainWindow::ChangeMode(int new_mode)
{
    ui->flowReqiredSpinBox->hide();

    cur_mode = new_mode;

    sel_v = nullptr;
    sel_e = nullptr;
    ui->stackedEditorWidget->setCurrentIndex(0);

    if (cur_view->pot_source)
    {
        cur_view->pot_source->SetColorByName();
        cur_view->pot_source = nullptr;

        cur_view->scene->update();
    }

    if (new_mode == 1)
        ui->stackedEditorWidget->setCurrentIndex(1);

    if (new_mode == 2)
        ui->stackedEditorWidget->setCurrentIndex(2);

    if (!cur_view->normal_coloring && new_mode != 4)
        ChangeToNormalColoring();

    //cur_view->item_mode != 4 && new_mode == 4
    if (new_mode == 4)
    {
        PrepareSourcesAndTargets();
        ui->stackedEditorWidget->setCurrentIndex(3);
    }

    cur_view->scene->SetItemMode(new_mode);
    cur_view->item_mode = new_mode;

    ui->infoLable->setText("");
}

void MainWindow::ChangeToNormalColoring()
{
    for (Vertex* v : cur_view->graph->_vertices)
        v->SetColorByName();

    for (Edge* e : cur_view->graph->_edges)
        e->SetColor(Qt::black);

    cur_view->normal_coloring = true;
    ChangeEdgeMode(1);

    cur_view->scene->update();
}

void MainWindow::PrepareSourcesAndTargets()
{
    cur_view->graph->_sources.clear();
    cur_view->graph->_targets.clear();

    for (Vertex* v : cur_view->graph->_vertices)
    {
        if (v->GetName().startsWith("S"))
            cur_view->graph->_sources.push_back(v);
        else if (v->GetName().startsWith("T"))
            cur_view->graph->_targets.push_back(v);
    }
}

void MainWindow::ChangeEdgeMode(int new_mode)
{
    for (Edge* e : cur_view->graph->_edges)
    {
        e->_mode = new_mode;
    }

    //cur_view->scene->update();
}

QVector<QColor> MainWindow::GenerateColorPalette(int ammount)
{
    QVector<QColor> colors;
    for (int i = 0; i < ammount; ++i)
    {
        qreal hue = static_cast<qreal>(i) / ammount;
        colors.append(QColor::fromHsvF(hue, 0.8, 1.0));
    }

    return colors;
}

void MainWindow::FindShortestPath()
{
    spdlog::info("Algorithm started: Shortest Path (Dijkstra)");
    if (cur_view->graph->_sources.size() != 1 || cur_view->graph->_targets.size() != 1)
    {
        spdlog::error("Invalid graph state for Dijkstra: requires exactly 1 source and 1 target.");
        ui->infoLable->setText("There must be exactly 1 source and only 1 target!");
        return;
    }

    int counter = 0;
    int weight = 0;

    for (Edge* e : ShortestPathSolver::Dijkstra(*cur_view->graph,
                                                cur_view->graph->_sources[0],
                                                cur_view->graph->_targets[0]))
    {
        ++counter;
        weight += e->_weight;
        e->SetColor(Qt::green);
    }

    if (counter) {
        spdlog::info("Shortest Path found. Length: {}, Weight: {}", counter, weight);
        ui->infoLable->setText("Successfully found a minimal path and painted it green. It consists of " +
                               QString::number(counter) + " edges and its total weight is " + QString::number(weight));
    } else {
        // Changed from WARN to INFO, this is a standard result
        spdlog::info("No path found between source and target.");
        ui->infoLable->setText("Unfortunately there is no path between source and target");
    }
}

void MainWindow::FindMinimalSpanningForest()
{
    spdlog::info("Algorithm started: Minimal Spanning Forest (Kruskal)");
    int counter = 0;
    int weight = 0;

    for (Edge* e : MinSpanningForestSolver::Kruskal(*cur_view->graph))
    {
        ++counter;
        weight += e->_weight;
        e->SetColor(Qt::green);
    }

    if (counter)
        ui->infoLable->setText("Successfully found a minimal spanning forest and painted it green. It consists of " +
                               QString::number(counter) + " edges and its total weight is " + QString::number(weight));
    else
        ui->infoLable->setText("Unfortunately there is no spanning forest in this graph");
}

void MainWindow::FindMaximalMathcingInBipartite()
{
    spdlog::info("Algorithm started: Max Matching (Hopcroft-Karp)");
    int counter = 0;

    for (Edge* e : MatchingSolver::HopcroftKarp(*cur_view->graph))
    {
        ++counter;
        e->SetColor(Qt::green);
    }

    if (counter)
        ui->infoLable->setText("Successfully found a maximal matching and painted it green. It consists of " +
                               QString::number(counter) + " pairs.");
    else
        ui->infoLable->setText("Unfortunately this graph isn't bipartite.");
}

void MainWindow::FindWeakComponents()
{
    spdlog::info("Algorithm started: Weak Components");
    QHash<Vertex*, int> coloring = ConnectivitySolver::WeakComponents(*cur_view->graph);
    int weak_components = ColoringSolver::ChromaticNumber(*cur_view->graph, coloring);
    QVector<QColor> colors = GenerateColorPalette(weak_components);
    for (Vertex* v : cur_view->graph->_vertices)
        v->SetColor(colors[coloring[v]]);

    ui->infoLable->setText("Found " + QString::number(weak_components) + " weak components. " +
                           "Vertices are colored to represent components.");

    if (weak_components <= 1)
        ui->infoLable->setText(ui->infoLable->text() + " Graph IS weakly connected.");
    else
        ui->infoLable->setText(ui->infoLable->text() + " Graph is NOT weakly connected.");
}

void MainWindow::FindStrongComponents()
{
    spdlog::info("Algorithm started: Strong Components (Kosaraju)");
    QHash<Vertex*, int> coloring = ConnectivitySolver::Kosaraju(*cur_view->graph);
    int strong_components = ColoringSolver::ChromaticNumber(*cur_view->graph, coloring);
    QVector<QColor> colors = GenerateColorPalette(strong_components);
    for (Vertex* v : cur_view->graph->_vertices)
        v->SetColor(colors[coloring[v]]);

    ui->infoLable->setText("Found " + QString::number(strong_components) + " strong components. " +
                           "Vertices are colored to represent components.");

    if (strong_components <= 1)
        ui->infoLable->setText(ui->infoLable->text() + " Graph IS strongly connected.");
    else
        ui->infoLable->setText(ui->infoLable->text() + " Graph is NOT strongly connected.");
}

void MainWindow::FindColoring()
{
    spdlog::info("Algorithm started: Graph Coloring (DSatur)");
    QHash<Vertex*, int> coloring = ColoringSolver::DSatur(*cur_view->graph);
    int colors_num = ColoringSolver::ChromaticNumber(*cur_view->graph, coloring);
    QVector<QColor> colors = GenerateColorPalette(colors_num);
    for (Vertex* v : cur_view->graph->_vertices)
        v->SetColor(colors[coloring[v]]);

    ui->infoLable->setText("Chromatic number is " + QString::number(colors_num) + ". " +
                           "Vertices are colored to represent the valid coloring.");
}

void MainWindow::FindEulerian()
{
    spdlog::info("Algorithm started: Eulerian Path");
    for (Edge* e : EulerianSolver::FindEulirianPath(*cur_view->graph))
        e->SetColor(Qt::green);

    if (EulerianSolver::IsEulerian(*cur_view->graph))
        ui->infoLable->setText("Graph is Eulerian. An Eulerian Cycle exists and is painted green.");
    else if (EulerianSolver::IsHalfEulerian(*cur_view->graph))
        ui->infoLable->setText("Graph is Half-Eulerian. An Eulerian Path exists and is painted green.");
    else
        ui->infoLable->setText("Graph is NOT Eulerian. No Eulerian Path or Cycle exists.");
}

void MainWindow::FindMaxFlowAndMinCut()
{
    spdlog::info("Algorithm started: Max Flow / Min Cut");
    if (cur_view->graph->_sources.size() < 1 || cur_view->graph->_targets.size() < 1)
    {
        spdlog::error("Invalid graph: At least 1 source and 1 target required.");
        ui->infoLable->setText("There must be at least 1 source and at least 1 target!");
        return;
    }

    QPair<int, QList<Edge*>> result =
        MaxFlowSolver::MultyMaxFlow(*cur_view->graph,
                                    cur_view->graph->_sources,
                                    cur_view->graph->_targets);

    int counter = 0;

    for (Edge* e : result.second)
    {
        ++counter;
        e->SetColor(Qt::red);
    }

    if (counter)
        ui->infoLable->setText("Maximum Flow: " + QString::number(result.first) +
                           ". Minimal Cut consists of " + QString::number(counter) + " edges (colored red).");
    else
        ui->infoLable->setText("Maximum Flow is 0. No path exists.");

    ChangeEdgeMode(2);
}

void MainWindow::FindIsomorphism()
{
    spdlog::info("Algorithm started: Isomorphism (VF2)");
    QList<QPair<Vertex*, Vertex*>> isomorphism = IsomorphismSolver::VF2Algorithm(*cur_view->graph, *sel_view->graph);
    if (!isomorphism.size())
    {
        ui->infoLable->setText("Graphs are NOT isomorphic.");
        return;
    }

    int size = cur_view->graph->_vertices.size();
    QVector<QColor> colors = GenerateColorPalette(size);
    for (auto it : isomorphism)
    {
        --size;
        it.first->SetColor(colors[size]);
        it.second->SetColor(colors[size]);
    }

    sel_view->normal_coloring = false;

    ui->infoLable->setText("Graphs ARE isomorphic. Corresponding vertices are matched by color.");
}

void MainWindow::FindMinCostFlow()
{
    spdlog::info("Algorithm started: Min Cost Flow");
    if (cur_view->graph->_sources.size() != 1 || cur_view->graph->_targets.size() != 1)
    {
        spdlog::error("Invalid graph: Exactly 1 source and 1 target required.");
        ui->infoLable->setText("There must be exactly 1 source and only 1 target!");
        return;
    }

    QPair<int, int> result;
    result = MinCostFlowSolver::MinCostFlow(*cur_view->graph, cur_view->graph->_sources[0],
                                            cur_view->graph->_targets[0], ui->flowReqiredSpinBox->value());

    if (result.first)
        ui->infoLable->setText("Flow pushed: " + QString::number(result.first)
                           + ". Total Cost: " + QString::number(result.second));
    else
        ui->infoLable->setText("No flow could be pushed (likely 0 capacity or no path).");

    ChangeEdgeMode(2);
}

void MainWindow::ExecuteAlgorithm(int index)
{
    if (!cur_view->normal_coloring)
        ChangeToNormalColoring();

    switch (index) {
    case 0:
        FindShortestPath();
        break;
    case 1:
        FindMinimalSpanningForest();
        break;
    case 2:
        FindMaximalMathcingInBipartite();
        break;
    case 3:
        FindWeakComponents();
        break;
    case 4:
        FindStrongComponents();
        break;
    case 5:
        FindColoring();
        break;
    case 6:
        FindEulerian();
        break;
    case 7:
        FindMaxFlowAndMinCut();
        break;
    case 8:
        FindMinCostFlow();
        break;
    case 9:
        FindIsomorphism();
        break;
    }


    cur_view->scene->update();
    cur_view->normal_coloring = false;
}

void MainWindow::SetVertexAttributes(QString name)
{
    if (sel_v)
    {
        sel_v->SetName(name);
        // Color logic is now handled internally by Vertex::SetName -> Vertex::SetColorByName
    }
}

void MainWindow::SetEdgeAttributes(int weight, int price)
{
    if (sel_e)
    {
        sel_e->SetWeight(weight);
        sel_e->SetPrice(price);
    }
}

void MainWindow::PrepareGraphWithChangedAlgorithm(int index)
{
    if (index == 8)
        ui->flowReqiredSpinBox->show();
    else
        ui->flowReqiredSpinBox->hide();

    ui->infoLable->setText("");

    if (!cur_view->normal_coloring)
        ChangeToNormalColoring();
}

void MainWindow::on_comboBox_activated(int index)
{
    cur_view->scene->SetItemIndex(index);
    cur_view->item_index = index;

    if (!cur_view->normal_coloring)
        ChangeToNormalColoring();
}

void MainWindow::on_modeComboBox_currentIndexChanged(int index)
{
    ChangeMode(index);
}

void MainWindow::on_leftPageButton_clicked()
{
    if (--cur_index == -1)
        cur_index = ui->stackedSceneWidget->count() - 1;

    SwitchToView(cur_index);
}

void MainWindow::on_rightPageButton_clicked()
{
    if (++cur_index == ui->stackedSceneWidget->count())
        cur_index = 0;

    SwitchToView(cur_index);
}

void MainWindow::on_addPageButton_clicked()
{
    AddView();
}

void MainWindow::on_closePageButton_clicked()
{
    RemoveView();
}

void MainWindow::on_openPageButton_clicked()
{
    // Important: We don't delete cur_view->graph here anymore because
    // LoadFromBinary handles reset logic.
    
    QString file_name = QFileDialog::getOpenFileName(this, "Load the graph", QDir::homePath(), "Graph Files (*.graph)");

    if (file_name.isEmpty())
        return;

    // Load with history
    switch (cur_view->graph->LoadFromBinary(file_name, cur_view->undoStack, cur_view->scene))
    {
    case 1:
        QMessageBox::critical(this, "Error", "Cannot open a file");
        spdlog::error("Failed to open file: {}", file_name.toStdString());
        break;
    case 2:
        QMessageBox::critical(this, "Error", "Wrong file format");
        spdlog::error("Wrong file format: {}", file_name.toStdString());
        break;
    case 3:
        QMessageBox::critical(this, "Error", "Unsupported version of file");
        spdlog::error("Unsupported version: {}", file_name.toStdString());
        break;
    default:
        spdlog::info("Graph loaded from {}", file_name.toStdString());
        // Sync actions after load
        undoAction->setVisible(cur_view->undoStack->canUndo());
        redoAction->setVisible(cur_view->undoStack->canRedo());
        break;
    }

    ShowGraph();
}

void MainWindow::on_savePageButton_clicked()
{
    QString file_name = QFileDialog::getSaveFileName(this, "Save the graph", QDir::homePath(), "Graph Files (*.graph)");

    if (file_name.isEmpty())
        return;

    if (!file_name.endsWith(".graph", Qt::CaseInsensitive))
        file_name += ".graph";

    // Save with history
    switch (cur_view->graph->SaveToBinary(file_name, cur_view->undoStack))
    {
    case 1:
        QMessageBox::critical(this, "Error", "Cannot create a file");
        spdlog::error("Failed to save file: {}", file_name.toStdString());
        break;
    default:
        spdlog::info("Graph saved to {}", file_name.toStdString());
        break;
    }
}

void MainWindow::on_curPageSpinBox_valueChanged(int arg1)
{
    SwitchToView(arg1 - 1);
}

void MainWindow::on_selPageSpinBox_valueChanged(int arg1)
{
    sel_view = dynamic_cast<MyView*>(ui->stackedSceneWidget->widget(arg1 - 1));
}

void MainWindow::on_flowReqiredSpinBox_valueChanged(int arg1)
{

}
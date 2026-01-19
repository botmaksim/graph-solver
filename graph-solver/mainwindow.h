#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>
#include <QString>
#include <QStackedWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QUndoStack>
#include <QToolBar>

#include "myview.h"
#include "vertexeditor.h"
#include "edgeeditor.h"
#include "algorithmeditor.h"
#include "mygraphicsscene.h"
#include "graph.h"
#include "shortestpathsolver.h"
#include "minspanningforestsolver.h"
#include "matchingsolver.h"
#include "euleriansolver.h"
#include "coloringsolver.h"
#include "maxflowsolver.h"
#include "isomorphismsolver.h"
#include "mincostflowsolver.h"
#include "commands.h"
#include "layoutsolver.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief The main application window handling UI interactions, scenes, and algorithm execution.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    /** @brief Updates the scene (Timer callback). Handles interaction modes. */
    void updateScene();
    
    /** @brief Calculates one step of the Force-Directed Layout (Timer callback). */
    void applyLayoutStep(); 
    
    /** @brief Exports the current view to a PNG image file. */
    void exportImage();
    
    /** @brief Toggles the visibility of vertex identifiers. */
    void ToggleIdentifiers();

public:
    /** @brief Constructor. */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /** @brief Adds a new graph view tab. */
    void AddView();
    
    /** @brief Removes the current graph view tab. */
    void RemoveView();
    
    /** @brief Switches to a specific view tab. */
    void SwitchToView(int index);

    /** @brief Adds a vertex at the last clicked position using Command pattern. */
    void AddVertex();
    
    /** @brief Adds an edge between selected source and target using Command pattern. */
    void AddEdge();
    
    /** @brief Removes the selected item using Command pattern. */
    void RemoveItem();
    
    /** @brief Refreshes the scene with items from the graph logic. */
    void ShowGraph();

    /** @brief Updates the sidebar editors based on selected item. */
    void UpdateInfo();
    
    /** @brief Changes the interaction mode (Move, Add Vertex, Add Edge, etc.). */
    void ChangeMode(int new_mode);

    /** @brief Resets vertex colors to default. */
    void ChangeToNormalColoring();
    
    /** @brief Identifies Source and Target vertices based on name prefix. */
    void PrepareSourcesAndTargets();
    
    /** @brief Changes edge rendering mode (text display). */
    void ChangeEdgeMode(int new_mode);

    /** @brief Generates a palette of distinct colors. */
    QVector<QColor> GenerateColorPalette(int ammount);

    /** @brief Executes the Shortest Path algorithm (Dijkstra). */
    void FindShortestPath();

    /** @brief Executes the Minimal Spanning Forest algorithm (Kruskal). */
    void FindMinimalSpanningForest();

    /** @brief Executes the Maximal Matching algorithm for Bipartite graphs (Hopcroft-Karp). */
    void FindMaximalMathcingInBipartite();

    /** @brief Finds Weakly Connected Components. */
    void FindWeakComponents();

    /** @brief Finds Strongly Connected Components (Kosaraju). */
    void FindStrongComponents();

    /** @brief Colors the graph (DSatur). */
    void FindColoring();

    /** @brief Finds Eulerian Path or Cycle. */
    void FindEulerian();

    /** @brief Calculates Maximum Flow and Min Cut. */
    void FindMaxFlowAndMinCut();

    /** @brief Checks for Graph Isomorphism against the selected view. */
    void FindIsomorphism();

    /** @brief Calculates Minimum Cost Flow. */
    void FindMinCostFlow();

public slots:
    /** @brief Slot to execute algorithm by index. */
    void ExecuteAlgorithm(int index);
    
    /** @brief Slot to update selected vertex attributes. */
    void SetVertexAttributes(QString name);
    
    /** @brief Slot to update selected edge attributes. */
    void SetEdgeAttributes(int weight, int price);
    
    /** @brief Slot to prepare UI for a specific algorithm. */
    void PrepareGraphWithChangedAlgorithm(int index);
    
    /** @brief Slot to toggle auto-layout animation. */
    void ToggleLayoutAnimation();

private slots:
    void on_comboBox_activated(int index);
    void on_modeComboBox_currentIndexChanged(int index);
    void on_leftPageButton_clicked();
    void on_rightPageButton_clicked();
    void on_addPageButton_clicked();
    void on_closePageButton_clicked();
    void on_openPageButton_clicked();
    void on_savePageButton_clicked();
    void on_curPageSpinBox_valueChanged(int arg1);
    void on_selPageSpinBox_valueChanged(int arg1);

    void on_flowReqiredSpinBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QTimer *timer; // for Scene update
    QTimer *layoutTimer; // for Force Directed Layout
    float layoutTemp; // Temperature for layout annealing

    MyView* cur_view;
    int cur_index;
    int cur_mode;

    MyView* sel_view;

    VertexEditor* v_edit;
    EdgeEditor* e_edit;
    AlgorithmEditor* a_edit;

    Vertex* sel_v = nullptr;
    Edge* sel_e = nullptr;
    
    // Actions only, stacks are in Views
    QAction* undoAction;
    QAction* redoAction;
    QAction* layoutAction;
    QAction* exportAction;
    QAction* toggleIdAction;
    QToolBar* mainToolBar;
};
#endif // MAINWINDOW_H
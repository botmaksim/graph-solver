#ifndef ALGORITHMEDITOR_H
#define ALGORITHMEDITOR_H

#include <QWidget>

namespace Ui {
class AlgorithmEditor;
}

/**
 * @class AlgorithmEditor
 * @brief Widget for selecting and executing graph algorithms.
 */
class AlgorithmEditor : public QWidget
{
    Q_OBJECT
    friend class MainWindow;
public:
    /**
     * @brief Constructor.
     * @param parent Parent widget.
     */
    explicit AlgorithmEditor(QWidget *parent = nullptr);
    ~AlgorithmEditor();

private slots:
    void on_executeButton_clicked();

    void on_solversComboBox_currentIndexChanged(int index);

    void on_solversComboBox_activated(int index);

private:
    Ui::AlgorithmEditor *ui;

signals:
    /**
     * @brief Signal emitted when an algorithm execution is requested.
     * @param index Index of the selected algorithm.
     */
    void ExecuteAlgorithm(int index);

    /**
     * @brief Signal emitted when the selected algorithm changes (to update UI Hints).
     * @param index Index of the selected algorithm.
     */
    void ChangedAlgorithm(int index);
};

#endif // ALGORITHMEDITOR_H
#ifndef VERTEXEDITOR_H
#define VERTEXEDITOR_H

#include <QWidget>
#include <QString>

namespace Ui {
class VertexEditor;
}

/**
 * @class VertexEditor
 * @brief Widget for editing Vertex properties (Name/ID, Source/Target status).
 */
class VertexEditor : public QWidget
{
    Q_OBJECT
    friend class MainWindow;

public:
    /**
     * @brief Constructor.
     * @param parent Parent widget.
     */
    explicit VertexEditor(QWidget *parent = nullptr);
    ~VertexEditor();
    
    /** @brief Process changes in UI and emit signal. */
    void InfoChanged();

    /** @brief Returns the current name stored in the editor. */
    QString GetName() const;

public slots:
    /**
     * @brief Update editor with vertex info.
     * @param name Name of the vertex.
     */
    void SetInfo(QString name);

private slots:
    void on_sourceCheckBox_stateChanged(int arg1);
    void on_targetCheckBox_stateChanged(int arg1);
    void on_idSpinBox_textChanged(const QString &arg1);

private:
    Ui::VertexEditor *ui;
    QString name = "0";
    int vertice_type = 0;

signals:
    /**
     * @brief Signal emitted when vertex info changes.
     * @param name New name.
     */
    void VertexInfoChanged(QString name);
};

#endif // VERTEXEDITOR_H
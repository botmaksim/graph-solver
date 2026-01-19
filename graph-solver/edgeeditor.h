#ifndef EDGEEDITOR_H
#define EDGEEDITOR_H

#include <QWidget>

namespace Ui {
class EdgeEditor;
}

/**
 * @class EdgeEditor
 * @brief Widget for editing edge properties (Weight, Price).
 */
class EdgeEditor : public QWidget
{
    Q_OBJECT
    friend class MainWindow;

public:
    /**
     * @brief Constructor.
     * @param parent Parent widget.
     */
    explicit EdgeEditor(QWidget *parent = nullptr);
    ~EdgeEditor();

public slots:
    /**
     * @brief Update the editor fields with current edge info.
     * @param weight Edge weight.
     * @param price Edge price.
     */
    void SetInfo(int weight, int price);

private slots:
    void on_weightSpinBox_valueChanged(int arg1);
    void on_priceSpinBox_valueChanged(int arg1);

private:
    Ui::EdgeEditor *ui;
    int weight = 0;
    int price = 0;

signals:
    /**
     * @brief Signal emitted when edge properties are changed in the UI.
     * @param weight New weight.
     * @param price New price.
     */
    void EdgeInfoChanged(int weight, int price);
};

#endif // EDGEEDITOR_H
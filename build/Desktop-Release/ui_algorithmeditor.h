/********************************************************************************
** Form generated from reading UI file 'algorithmeditor.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ALGORITHMEDITOR_H
#define UI_ALGORITHMEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AlgorithmEditor
{
public:
    QGridLayout *gridLayout;
    QComboBox *solversComboBox;
    QPushButton *executeButton;

    void setupUi(QWidget *AlgorithmEditor)
    {
        if (AlgorithmEditor->objectName().isEmpty())
            AlgorithmEditor->setObjectName("AlgorithmEditor");
        AlgorithmEditor->resize(250, 100);
        gridLayout = new QGridLayout(AlgorithmEditor);
        gridLayout->setObjectName("gridLayout");
        solversComboBox = new QComboBox(AlgorithmEditor);
        solversComboBox->addItem(QString());
        solversComboBox->addItem(QString());
        solversComboBox->addItem(QString());
        solversComboBox->addItem(QString());
        solversComboBox->addItem(QString());
        solversComboBox->addItem(QString());
        solversComboBox->addItem(QString());
        solversComboBox->addItem(QString());
        solversComboBox->addItem(QString());
        solversComboBox->addItem(QString());
        solversComboBox->setObjectName("solversComboBox");

        gridLayout->addWidget(solversComboBox, 0, 0, 1, 1);

        executeButton = new QPushButton(AlgorithmEditor);
        executeButton->setObjectName("executeButton");

        gridLayout->addWidget(executeButton, 1, 0, 1, 1);


        retranslateUi(AlgorithmEditor);

        QMetaObject::connectSlotsByName(AlgorithmEditor);
    } // setupUi

    void retranslateUi(QWidget *AlgorithmEditor)
    {
        AlgorithmEditor->setWindowTitle(QCoreApplication::translate("AlgorithmEditor", "Form", nullptr));
        solversComboBox->setItemText(0, QCoreApplication::translate("AlgorithmEditor", "Shortest path", nullptr));
        solversComboBox->setItemText(1, QCoreApplication::translate("AlgorithmEditor", "Min spanning forest", nullptr));
        solversComboBox->setItemText(2, QCoreApplication::translate("AlgorithmEditor", "Max matching in bipartite", nullptr));
        solversComboBox->setItemText(3, QCoreApplication::translate("AlgorithmEditor", "Weak components", nullptr));
        solversComboBox->setItemText(4, QCoreApplication::translate("AlgorithmEditor", "Strong components", nullptr));
        solversComboBox->setItemText(5, QCoreApplication::translate("AlgorithmEditor", "Chromatic number", nullptr));
        solversComboBox->setItemText(6, QCoreApplication::translate("AlgorithmEditor", "Eulerian check", nullptr));
        solversComboBox->setItemText(7, QCoreApplication::translate("AlgorithmEditor", "Max flow", nullptr));
        solversComboBox->setItemText(8, QCoreApplication::translate("AlgorithmEditor", "Min cost flow", nullptr));
        solversComboBox->setItemText(9, QCoreApplication::translate("AlgorithmEditor", "Isomorphism", nullptr));

        executeButton->setText(QCoreApplication::translate("AlgorithmEditor", "Execute Algorithm", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AlgorithmEditor: public Ui_AlgorithmEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ALGORITHMEDITOR_H

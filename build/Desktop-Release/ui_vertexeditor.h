/********************************************************************************
** Form generated from reading UI file 'vertexeditor.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VERTEXEDITOR_H
#define UI_VERTEXEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VertexEditor
{
public:
    QGridLayout *gridLayout;
    QCheckBox *targetCheckBox;
    QCheckBox *sourceCheckBox;
    QLabel *idLabel;
    QSpinBox *idSpinBox;

    void setupUi(QWidget *VertexEditor)
    {
        if (VertexEditor->objectName().isEmpty())
            VertexEditor->setObjectName("VertexEditor");
        VertexEditor->resize(250, 100);
        gridLayout = new QGridLayout(VertexEditor);
        gridLayout->setObjectName("gridLayout");
        targetCheckBox = new QCheckBox(VertexEditor);
        targetCheckBox->setObjectName("targetCheckBox");

        gridLayout->addWidget(targetCheckBox, 3, 1, 1, 1);

        sourceCheckBox = new QCheckBox(VertexEditor);
        sourceCheckBox->setObjectName("sourceCheckBox");

        gridLayout->addWidget(sourceCheckBox, 3, 0, 1, 1);

        idLabel = new QLabel(VertexEditor);
        idLabel->setObjectName("idLabel");

        gridLayout->addWidget(idLabel, 1, 0, 1, 1);

        idSpinBox = new QSpinBox(VertexEditor);
        idSpinBox->setObjectName("idSpinBox");

        gridLayout->addWidget(idSpinBox, 2, 0, 1, 2);


        retranslateUi(VertexEditor);

        QMetaObject::connectSlotsByName(VertexEditor);
    } // setupUi

    void retranslateUi(QWidget *VertexEditor)
    {
        VertexEditor->setWindowTitle(QCoreApplication::translate("VertexEditor", "Form", nullptr));
        targetCheckBox->setText(QCoreApplication::translate("VertexEditor", "Target", nullptr));
        sourceCheckBox->setText(QCoreApplication::translate("VertexEditor", "Source", nullptr));
        idLabel->setText(QCoreApplication::translate("VertexEditor", "Identifier:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VertexEditor: public Ui_VertexEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VERTEXEDITOR_H

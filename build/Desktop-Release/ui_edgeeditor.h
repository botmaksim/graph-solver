/********************************************************************************
** Form generated from reading UI file 'edgeeditor.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDGEEDITOR_H
#define UI_EDGEEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EdgeEditor
{
public:
    QGridLayout *gridLayout;
    QSpinBox *weightSpinBox;
    QSpinBox *priceSpinBox;
    QLabel *weightLabel;
    QLabel *priceLabel;

    void setupUi(QWidget *EdgeEditor)
    {
        if (EdgeEditor->objectName().isEmpty())
            EdgeEditor->setObjectName("EdgeEditor");
        EdgeEditor->resize(250, 100);
        gridLayout = new QGridLayout(EdgeEditor);
        gridLayout->setObjectName("gridLayout");
        weightSpinBox = new QSpinBox(EdgeEditor);
        weightSpinBox->setObjectName("weightSpinBox");

        gridLayout->addWidget(weightSpinBox, 2, 0, 1, 1);

        priceSpinBox = new QSpinBox(EdgeEditor);
        priceSpinBox->setObjectName("priceSpinBox");
        priceSpinBox->setMaximum(2);

        gridLayout->addWidget(priceSpinBox, 2, 1, 1, 1);

        weightLabel = new QLabel(EdgeEditor);
        weightLabel->setObjectName("weightLabel");

        gridLayout->addWidget(weightLabel, 1, 0, 1, 1);

        priceLabel = new QLabel(EdgeEditor);
        priceLabel->setObjectName("priceLabel");

        gridLayout->addWidget(priceLabel, 1, 1, 1, 1);


        retranslateUi(EdgeEditor);

        QMetaObject::connectSlotsByName(EdgeEditor);
    } // setupUi

    void retranslateUi(QWidget *EdgeEditor)
    {
        EdgeEditor->setWindowTitle(QCoreApplication::translate("EdgeEditor", "Form", nullptr));
        weightLabel->setText(QCoreApplication::translate("EdgeEditor", "Weight:", nullptr));
        priceLabel->setText(QCoreApplication::translate("EdgeEditor", "Price:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EdgeEditor: public Ui_EdgeEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDGEEDITOR_H

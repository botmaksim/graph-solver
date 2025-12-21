/********************************************************************************
** Form generated from reading UI file 'myview.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYVIEW_H
#define UI_MYVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyView
{
public:
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView;

    void setupUi(QWidget *MyView)
    {
        if (MyView->objectName().isEmpty())
            MyView->setObjectName("MyView");
        MyView->resize(1280, 900);
        gridLayout = new QGridLayout(MyView);
        gridLayout->setObjectName("gridLayout");
        graphicsView = new QGraphicsView(MyView);
        graphicsView->setObjectName("graphicsView");

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);


        retranslateUi(MyView);

        QMetaObject::connectSlotsByName(MyView);
    } // setupUi

    void retranslateUi(QWidget *MyView)
    {
        MyView->setWindowTitle(QCoreApplication::translate("MyView", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MyView: public Ui_MyView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYVIEW_H

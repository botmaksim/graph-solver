/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_4;
    QComboBox *modeComboBox;
    QStackedWidget *stackedSceneWidget;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLabel *curPageLabel;
    QSpinBox *curPageSpinBox;
    QLabel *selPageLabel;
    QSpinBox *selPageSpinBox;
    QLabel *totalPageLabel;
    QSpinBox *totalPageSpinBox;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_5;
    QLabel *infoLable;
    QSpinBox *flowReqiredSpinBox;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QPushButton *savePageButton;
    QPushButton *openPageButton;
    QPushButton *rightPageButton;
    QPushButton *closePageButton;
    QPushButton *leftPageButton;
    QPushButton *addPageButton;
    QStackedWidget *stackedEditorWidget;
    QWidget *emptyPage;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1600, 900);
        MainWindow->setStyleSheet(QString::fromUtf8("QWidget {\n"
"        background-color: #fdfdfd;\n"
"        color: #202020;\n"
"        font-family: Segoe UI, sans-serif;\n"
"        font-size: 14px;\n"
"    }\n"
"\n"
"    QLabel {\n"
"    font-family: \"Segoe UI\", Arial, sans-serif;\n"
"    font-size: 16px;\n"
"    font-weight: 500;\n"
"    color: #202020;\n"
"}\n"
"\n"
"    QLabel:hover {\n"
"        border: 1px dashed #999;\n"
"        background-color: #f0f8ff;\n"
"    }\n"
"\n"
"    QSpinBox {\n"
"        border: 1px solid #ccc;\n"
"        border-radius: 4px;\n"
"        padding: 2px 6px 2px 6px;\n"
"        background-color: #ffffff;\n"
"        selection-background-color: #cceeff;\n"
"    }\n"
"\n"
"    QSpinBox::up-arrow {\n"
"        content: \"\342\226\262\";\n"
"        color: #555;\n"
"        font-size: 10px;\n"
"    }\n"
"\n"
"    QSpinBox::down-arrow {\n"
"        content: \"\342\226\274\";\n"
"        color: #555;\n"
"        font-size: 10px;\n"
"    }\n"
"\n"
"    QSpinBox::up-button:hover, QSpinBox::down-button:hover {\n"
"        backg"
                        "round-color: #e6f7ff;\n"
"    }\n"
"\n"
"    QPushButton {\n"
"        background-color: #e0f0ff;\n"
"        border: 1px solid #99ccff;\n"
"        border-radius: 6px;\n"
"        padding: 4px 10px;\n"
"    }\n"
"\n"
"    QPushButton:hover {\n"
"        background-color: #cce4ff;\n"
"    }\n"
"\n"
"    QPushButton:pressed {\n"
"        background-color: #b3d8ff;\n"
"    }\n"
"\n"
"    QCheckBox {\n"
"        spacing: 6px;\n"
"    }\n"
"\n"
"    QCheckBox:hover {\n"
"        color: #0055aa;\n"
"    }"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout_6 = new QGridLayout(centralwidget);
        gridLayout_6->setObjectName("gridLayout_6");
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName("gridLayout_4");
        gridLayout_4->setContentsMargins(-1, -1, 0, -1);
        modeComboBox = new QComboBox(centralwidget);
        modeComboBox->addItem(QString());
        modeComboBox->addItem(QString());
        modeComboBox->addItem(QString());
        modeComboBox->addItem(QString());
        modeComboBox->addItem(QString());
        modeComboBox->setObjectName("modeComboBox");

        gridLayout_4->addWidget(modeComboBox, 2, 0, 1, 1);

        stackedSceneWidget = new QStackedWidget(centralwidget);
        stackedSceneWidget->setObjectName("stackedSceneWidget");

        gridLayout_4->addWidget(stackedSceneWidget, 0, 1, 5, 1);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName("gridLayout_2");
        curPageLabel = new QLabel(groupBox);
        curPageLabel->setObjectName("curPageLabel");

        gridLayout_2->addWidget(curPageLabel, 0, 0, 1, 1);

        curPageSpinBox = new QSpinBox(groupBox);
        curPageSpinBox->setObjectName("curPageSpinBox");
        curPageSpinBox->setMinimum(1);
        curPageSpinBox->setMaximum(1);

        gridLayout_2->addWidget(curPageSpinBox, 0, 1, 1, 1);

        selPageLabel = new QLabel(groupBox);
        selPageLabel->setObjectName("selPageLabel");

        gridLayout_2->addWidget(selPageLabel, 1, 0, 1, 1);

        selPageSpinBox = new QSpinBox(groupBox);
        selPageSpinBox->setObjectName("selPageSpinBox");
        selPageSpinBox->setMinimum(1);
        selPageSpinBox->setMaximum(1);

        gridLayout_2->addWidget(selPageSpinBox, 1, 1, 1, 1);

        totalPageLabel = new QLabel(groupBox);
        totalPageLabel->setObjectName("totalPageLabel");

        gridLayout_2->addWidget(totalPageLabel, 2, 0, 1, 1);

        totalPageSpinBox = new QSpinBox(groupBox);
        totalPageSpinBox->setObjectName("totalPageSpinBox");
        totalPageSpinBox->setEnabled(true);
        totalPageSpinBox->setWrapping(false);
        totalPageSpinBox->setReadOnly(true);
        totalPageSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        totalPageSpinBox->setAccelerated(false);
        totalPageSpinBox->setKeyboardTracking(true);
        totalPageSpinBox->setMinimum(1);
        totalPageSpinBox->setMaximum(10);

        gridLayout_2->addWidget(totalPageSpinBox, 2, 1, 1, 1);


        gridLayout_4->addWidget(groupBox, 1, 0, 1, 1);

        groupBox_4 = new QGroupBox(centralwidget);
        groupBox_4->setObjectName("groupBox_4");
        gridLayout_5 = new QGridLayout(groupBox_4);
        gridLayout_5->setObjectName("gridLayout_5");
        infoLable = new QLabel(groupBox_4);
        infoLable->setObjectName("infoLable");
        infoLable->setLayoutDirection(Qt::LeftToRight);
        infoLable->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        infoLable->setWordWrap(true);

        gridLayout_5->addWidget(infoLable, 1, 0, 1, 1);

        flowReqiredSpinBox = new QSpinBox(groupBox_4);
        flowReqiredSpinBox->setObjectName("flowReqiredSpinBox");
        flowReqiredSpinBox->setMinimum(1);

        gridLayout_5->addWidget(flowReqiredSpinBox, 2, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_4, 4, 0, 1, 1);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName("gridLayout");
        savePageButton = new QPushButton(groupBox_2);
        savePageButton->setObjectName("savePageButton");
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("document-save");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        }
        savePageButton->setIcon(icon);

        gridLayout->addWidget(savePageButton, 2, 1, 1, 1);

        openPageButton = new QPushButton(groupBox_2);
        openPageButton->setObjectName("openPageButton");
        QIcon icon1;
        iconThemeName = QString::fromUtf8("document-open");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QString::fromUtf8("."), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        }
        openPageButton->setIcon(icon1);

        gridLayout->addWidget(openPageButton, 2, 0, 1, 1);

        rightPageButton = new QPushButton(groupBox_2);
        rightPageButton->setObjectName("rightPageButton");
        QIcon icon2;
        iconThemeName = QString::fromUtf8("go-next");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon2 = QIcon::fromTheme(iconThemeName);
        } else {
            icon2.addFile(QString::fromUtf8("."), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        }
        rightPageButton->setIcon(icon2);

        gridLayout->addWidget(rightPageButton, 0, 1, 1, 1);

        closePageButton = new QPushButton(groupBox_2);
        closePageButton->setObjectName("closePageButton");
        QIcon icon3;
        iconThemeName = QString::fromUtf8("edit-clear");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon3 = QIcon::fromTheme(iconThemeName);
        } else {
            icon3.addFile(QString::fromUtf8("."), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        }
        closePageButton->setIcon(icon3);

        gridLayout->addWidget(closePageButton, 1, 1, 1, 1);

        leftPageButton = new QPushButton(groupBox_2);
        leftPageButton->setObjectName("leftPageButton");
        QIcon icon4;
        iconThemeName = QString::fromUtf8("go-previous");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon4 = QIcon::fromTheme(iconThemeName);
        } else {
            icon4.addFile(QString::fromUtf8("."), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        }
        leftPageButton->setIcon(icon4);

        gridLayout->addWidget(leftPageButton, 0, 0, 1, 1);

        addPageButton = new QPushButton(groupBox_2);
        addPageButton->setObjectName("addPageButton");
        QIcon icon5;
        iconThemeName = QString::fromUtf8("list-add");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon5 = QIcon::fromTheme(iconThemeName);
        } else {
            icon5.addFile(QString::fromUtf8("."), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        }
        addPageButton->setIcon(icon5);

        gridLayout->addWidget(addPageButton, 1, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_2, 0, 0, 1, 1);

        stackedEditorWidget = new QStackedWidget(centralwidget);
        stackedEditorWidget->setObjectName("stackedEditorWidget");
        emptyPage = new QWidget();
        emptyPage->setObjectName("emptyPage");
        emptyPage->setEnabled(false);
        stackedEditorWidget->addWidget(emptyPage);

        gridLayout_4->addWidget(stackedEditorWidget, 3, 0, 1, 1);

        gridLayout_4->setRowStretch(0, 1);
        gridLayout_4->setRowStretch(1, 1);
        gridLayout_4->setRowStretch(2, 1);
        gridLayout_4->setRowStretch(3, 2);
        gridLayout_4->setRowStretch(4, 6);
        gridLayout_4->setColumnStretch(0, 1);
        gridLayout_4->setColumnStretch(1, 4);

        gridLayout_6->addLayout(gridLayout_4, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1600, 25));
        MainWindow->setMenuBar(menubar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        modeComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Move items", nullptr));
        modeComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Add vertex", nullptr));
        modeComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Add edge", nullptr));
        modeComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Delete items", nullptr));
        modeComboBox->setItemText(4, QCoreApplication::translate("MainWindow", "Execute algorithm", nullptr));

        modeComboBox->setCurrentText(QCoreApplication::translate("MainWindow", "Move items", nullptr));
        groupBox->setTitle(QString());
        curPageLabel->setText(QCoreApplication::translate("MainWindow", "Current page number:", nullptr));
        selPageLabel->setText(QCoreApplication::translate("MainWindow", "Selected page number:", nullptr));
        totalPageLabel->setText(QCoreApplication::translate("MainWindow", "Total number of pages:", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "Inforamation", nullptr));
        infoLable->setText(QString());
        groupBox_2->setTitle(QString());
        savePageButton->setText(QString());
        openPageButton->setText(QString());
        rightPageButton->setText(QString());
        closePageButton->setText(QString());
        leftPageButton->setText(QString());
        addPageButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

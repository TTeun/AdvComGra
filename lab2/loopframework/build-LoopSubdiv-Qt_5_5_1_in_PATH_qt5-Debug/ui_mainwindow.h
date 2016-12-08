/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include "mainview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *OptionsGB;
    QSpinBox *SubdivSteps;
    QLabel *SubdivLabel;
    QPushButton *LoadOBJ;
    QGroupBox *displayGB;
    QSlider *reflDensitySlider;
    QComboBox *shaderComboBox;
    QLabel *label;
    QSpinBox *curScalingSB;
    QLabel *label_2;
    QLabel *label_3;
    QPushButton *rotResetButton;
    QComboBox *drawModeComboBox;
    QLabel *label_4;
    MainView *MainDisplay;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1366, 768);
        MainWindow->setStyleSheet(QLatin1String("/* Groupbox */\n"
"\n"
"QGroupBox {\n"
"    border: 1px solid #DDD;\n"
"    border-radius: 9px;\n"
"    margin-top: 9px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px 0 3px;\n"
"}"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(6, 6, 6, 6);
        OptionsGB = new QGroupBox(centralWidget);
        OptionsGB->setObjectName(QStringLiteral("OptionsGB"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(OptionsGB->sizePolicy().hasHeightForWidth());
        OptionsGB->setSizePolicy(sizePolicy);
        OptionsGB->setMinimumSize(QSize(220, 0));
        SubdivSteps = new QSpinBox(OptionsGB);
        SubdivSteps->setObjectName(QStringLiteral("SubdivSteps"));
        SubdivSteps->setEnabled(false);
        SubdivSteps->setGeometry(QRect(20, 90, 181, 22));
        SubdivSteps->setMaximum(8);
        SubdivLabel = new QLabel(OptionsGB);
        SubdivLabel->setObjectName(QStringLiteral("SubdivLabel"));
        SubdivLabel->setGeometry(QRect(20, 70, 181, 16));
        LoadOBJ = new QPushButton(OptionsGB);
        LoadOBJ->setObjectName(QStringLiteral("LoadOBJ"));
        LoadOBJ->setGeometry(QRect(20, 30, 181, 21));
        displayGB = new QGroupBox(OptionsGB);
        displayGB->setObjectName(QStringLiteral("displayGB"));
        displayGB->setEnabled(false);
        displayGB->setGeometry(QRect(10, 220, 191, 291));
        reflDensitySlider = new QSlider(displayGB);
        reflDensitySlider->setObjectName(QStringLiteral("reflDensitySlider"));
        reflDensitySlider->setEnabled(false);
        reflDensitySlider->setGeometry(QRect(10, 190, 160, 18));
        reflDensitySlider->setMinimum(1);
        reflDensitySlider->setOrientation(Qt::Horizontal);
        shaderComboBox = new QComboBox(displayGB);
        shaderComboBox->setObjectName(QStringLiteral("shaderComboBox"));
        shaderComboBox->setGeometry(QRect(10, 110, 151, 33));
        label = new QLabel(displayGB);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 170, 161, 17));
        curScalingSB = new QSpinBox(displayGB);
        curScalingSB->setObjectName(QStringLiteral("curScalingSB"));
        curScalingSB->setEnabled(false);
        curScalingSB->setGeometry(QRect(130, 220, 45, 27));
        curScalingSB->setMinimum(-5);
        curScalingSB->setMaximum(5);
        label_2 = new QLabel(displayGB);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 90, 111, 17));
        label_3 = new QLabel(displayGB);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 220, 111, 17));
        rotResetButton = new QPushButton(displayGB);
        rotResetButton->setObjectName(QStringLiteral("rotResetButton"));
        rotResetButton->setGeometry(QRect(10, 260, 151, 27));
        drawModeComboBox = new QComboBox(displayGB);
        drawModeComboBox->setObjectName(QStringLiteral("drawModeComboBox"));
        drawModeComboBox->setGeometry(QRect(10, 50, 151, 33));
        label_4 = new QLabel(displayGB);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 30, 131, 17));

        horizontalLayout->addWidget(OptionsGB);

        MainDisplay = new MainView(centralWidget);
        MainDisplay->setObjectName(QStringLiteral("MainDisplay"));

        horizontalLayout->addWidget(MainDisplay);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        OptionsGB->setTitle(QApplication::translate("MainWindow", "Options", 0));
        SubdivLabel->setText(QApplication::translate("MainWindow", "Loop subdivision steps", 0));
        LoadOBJ->setText(QApplication::translate("MainWindow", "Load OBJ file", 0));
        displayGB->setTitle(QApplication::translate("MainWindow", "Display", 0));
        shaderComboBox->clear();
        shaderComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Normal Shading", 0)
         << QApplication::translate("MainWindow", "Gaussian Curvature", 0)
         << QApplication::translate("MainWindow", "Flat Shading", 0)
         << QApplication::translate("MainWindow", "Reflection Lines", 0)
        );
        label->setText(QApplication::translate("MainWindow", "Reflection Line Density", 0));
        label_2->setText(QApplication::translate("MainWindow", "Shading Options", 0));
        label_3->setText(QApplication::translate("MainWindow", "Curvature Scaling", 0));
        rotResetButton->setText(QApplication::translate("MainWindow", "Reset Rotation", 0));
        drawModeComboBox->clear();
        drawModeComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Wireframe", 0)
         << QApplication::translate("MainWindow", "Surface", 0)
         << QApplication::translate("MainWindow", "Point Cloud", 0)
        );
        label_4->setText(QApplication::translate("MainWindow", "Draw mode", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

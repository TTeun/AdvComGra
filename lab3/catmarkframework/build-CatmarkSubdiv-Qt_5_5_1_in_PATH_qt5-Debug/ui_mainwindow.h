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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include "mainview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *SettingsGB;
    QSpinBox *SubdivSteps;
    QPushButton *ImportOBJ;
    QLabel *StepsLabel;
    QCheckBox *wireFrameCB;
    QCheckBox *limitPointsCB;
    QCheckBox *quadPatchCB;
    QCheckBox *controlMeshCB;
    QDoubleSpinBox *sharpnessSlider;
    QLabel *label;
    QGroupBox *quadPatchGB;
    QSpinBox *outerLevelSB;
    QSpinBox *innerLevelSB;
    QLabel *innerLevelLabel;
    QLabel *outerLevelLabel;
    QCheckBox *gridLinesCB;
    QPushButton *applySharpnessPB;
    QCheckBox *showModelCB;
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
        SettingsGB = new QGroupBox(centralWidget);
        SettingsGB->setObjectName(QStringLiteral("SettingsGB"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SettingsGB->sizePolicy().hasHeightForWidth());
        SettingsGB->setSizePolicy(sizePolicy);
        SettingsGB->setMinimumSize(QSize(220, 0));
        SettingsGB->setCheckable(false);
        SubdivSteps = new QSpinBox(SettingsGB);
        SubdivSteps->setObjectName(QStringLiteral("SubdivSteps"));
        SubdivSteps->setGeometry(QRect(20, 375, 181, 29));
        ImportOBJ = new QPushButton(SettingsGB);
        ImportOBJ->setObjectName(QStringLiteral("ImportOBJ"));
        ImportOBJ->setGeometry(QRect(20, 40, 181, 28));
        StepsLabel = new QLabel(SettingsGB);
        StepsLabel->setObjectName(QStringLiteral("StepsLabel"));
        StepsLabel->setGeometry(QRect(20, 345, 181, 20));
        wireFrameCB = new QCheckBox(SettingsGB);
        wireFrameCB->setObjectName(QStringLiteral("wireFrameCB"));
        wireFrameCB->setGeometry(QRect(30, 120, 87, 22));
        wireFrameCB->setChecked(true);
        limitPointsCB = new QCheckBox(SettingsGB);
        limitPointsCB->setObjectName(QStringLiteral("limitPointsCB"));
        limitPointsCB->setGeometry(QRect(20, 415, 87, 22));
        quadPatchCB = new QCheckBox(SettingsGB);
        quadPatchCB->setObjectName(QStringLiteral("quadPatchCB"));
        quadPatchCB->setGeometry(QRect(30, 180, 161, 22));
        controlMeshCB = new QCheckBox(SettingsGB);
        controlMeshCB->setObjectName(QStringLiteral("controlMeshCB"));
        controlMeshCB->setGeometry(QRect(30, 150, 131, 22));
        sharpnessSlider = new QDoubleSpinBox(SettingsGB);
        sharpnessSlider->setObjectName(QStringLiteral("sharpnessSlider"));
        sharpnessSlider->setGeometry(QRect(20, 490, 62, 27));
        label = new QLabel(SettingsGB);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 460, 111, 17));
        quadPatchGB = new QGroupBox(SettingsGB);
        quadPatchGB->setObjectName(QStringLiteral("quadPatchGB"));
        quadPatchGB->setEnabled(false);
        quadPatchGB->setGeometry(QRect(20, 215, 166, 121));
        outerLevelSB = new QSpinBox(quadPatchGB);
        outerLevelSB->setObjectName(QStringLiteral("outerLevelSB"));
        outerLevelSB->setGeometry(QRect(10, 50, 41, 27));
        outerLevelSB->setValue(4);
        innerLevelSB = new QSpinBox(quadPatchGB);
        innerLevelSB->setObjectName(QStringLiteral("innerLevelSB"));
        innerLevelSB->setEnabled(false);
        innerLevelSB->setGeometry(QRect(10, 20, 41, 27));
        innerLevelSB->setValue(4);
        innerLevelLabel = new QLabel(quadPatchGB);
        innerLevelLabel->setObjectName(QStringLiteral("innerLevelLabel"));
        innerLevelLabel->setGeometry(QRect(60, 55, 91, 17));
        outerLevelLabel = new QLabel(quadPatchGB);
        outerLevelLabel->setObjectName(QStringLiteral("outerLevelLabel"));
        outerLevelLabel->setEnabled(false);
        outerLevelLabel->setGeometry(QRect(60, 25, 101, 17));
        gridLinesCB = new QCheckBox(quadPatchGB);
        gridLinesCB->setObjectName(QStringLiteral("gridLinesCB"));
        gridLinesCB->setEnabled(false);
        gridLinesCB->setGeometry(QRect(10, 85, 87, 22));
        applySharpnessPB = new QPushButton(SettingsGB);
        applySharpnessPB->setObjectName(QStringLiteral("applySharpnessPB"));
        applySharpnessPB->setGeometry(QRect(95, 490, 106, 27));
        showModelCB = new QCheckBox(SettingsGB);
        showModelCB->setObjectName(QStringLiteral("showModelCB"));
        showModelCB->setGeometry(QRect(30, 90, 131, 22));
        showModelCB->setChecked(true);

        horizontalLayout->addWidget(SettingsGB);

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
        SettingsGB->setTitle(QApplication::translate("MainWindow", "Settings", 0));
        ImportOBJ->setText(QApplication::translate("MainWindow", "Import OBJ file", 0));
        StepsLabel->setText(QApplication::translate("MainWindow", "Catmull-Clark steps", 0));
        wireFrameCB->setText(QApplication::translate("MainWindow", "Wireframe", 0));
        limitPointsCB->setText(QApplication::translate("MainWindow", "To limit", 0));
        quadPatchCB->setText(QApplication::translate("MainWindow", "Quad Patches", 0));
        controlMeshCB->setText(QApplication::translate("MainWindow", "Control Mesh", 0));
        label->setText(QApplication::translate("MainWindow", "Sharpness", 0));
        quadPatchGB->setTitle(QApplication::translate("MainWindow", "Quad Patch Setting", 0));
        innerLevelLabel->setText(QApplication::translate("MainWindow", "Inner Level", 0));
        outerLevelLabel->setText(QApplication::translate("MainWindow", "Outer Level", 0));
        gridLinesCB->setText(QApplication::translate("MainWindow", "Grid Lines", 0));
        applySharpnessPB->setText(QApplication::translate("MainWindow", "Apply Sharpness", 0));
        showModelCB->setText(QApplication::translate("MainWindow", "Show Model", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

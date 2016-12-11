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
    QCheckBox *checkBox;
    QCheckBox *limitPointsCB;
    QCheckBox *quadPatchCB;
    QCheckBox *controlMeshCB;
    QDoubleSpinBox *sharpnessSlider;
    QLabel *label;
    QPushButton *sharpnessPB;
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
        SubdivSteps->setGeometry(QRect(20, 260, 181, 29));
        ImportOBJ = new QPushButton(SettingsGB);
        ImportOBJ->setObjectName(QStringLiteral("ImportOBJ"));
        ImportOBJ->setGeometry(QRect(20, 40, 181, 28));
        StepsLabel = new QLabel(SettingsGB);
        StepsLabel->setObjectName(QStringLiteral("StepsLabel"));
        StepsLabel->setGeometry(QRect(20, 230, 181, 20));
        checkBox = new QCheckBox(SettingsGB);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(30, 90, 87, 22));
        checkBox->setChecked(true);
        limitPointsCB = new QCheckBox(SettingsGB);
        limitPointsCB->setObjectName(QStringLiteral("limitPointsCB"));
        limitPointsCB->setGeometry(QRect(20, 300, 87, 22));
        quadPatchCB = new QCheckBox(SettingsGB);
        quadPatchCB->setObjectName(QStringLiteral("quadPatchCB"));
        quadPatchCB->setGeometry(QRect(30, 120, 161, 22));
        controlMeshCB = new QCheckBox(SettingsGB);
        controlMeshCB->setObjectName(QStringLiteral("controlMeshCB"));
        controlMeshCB->setGeometry(QRect(30, 150, 131, 22));
        sharpnessSlider = new QDoubleSpinBox(SettingsGB);
        sharpnessSlider->setObjectName(QStringLiteral("sharpnessSlider"));
        sharpnessSlider->setGeometry(QRect(20, 460, 62, 27));
        label = new QLabel(SettingsGB);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 430, 111, 17));
        sharpnessPB = new QPushButton(SettingsGB);
        sharpnessPB->setObjectName(QStringLiteral("sharpnessPB"));
        sharpnessPB->setGeometry(QRect(90, 460, 111, 27));

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
        checkBox->setText(QApplication::translate("MainWindow", "Wireframe", 0));
        limitPointsCB->setText(QApplication::translate("MainWindow", "To limit", 0));
        quadPatchCB->setText(QApplication::translate("MainWindow", "Quad Patches", 0));
        controlMeshCB->setText(QApplication::translate("MainWindow", "Control Mesh", 0));
        label->setText(QApplication::translate("MainWindow", "Sharpness", 0));
        sharpnessPB->setText(QApplication::translate("MainWindow", "Apply Sharpness", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

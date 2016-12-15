#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "objfile.h"
#include <QFileDialog>
#include "mesh.h"
#include "meshtools.h"
#include <QDoubleSpinBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {

  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void importOBJ();
  Ui::MainWindow *ui;
  void setSharpness(double value);
private:
  unsigned int currentMesh = 0;

private slots:
  void on_ImportOBJ_clicked();                 // Load object
  void on_SubdivSteps_valueChanged(int value); // Change subdivision level
  void on_wireFrameCB_toggled(bool checked);   // Wireframe mode on/off
  void on_limitPointsCB_toggled(bool checked); // Project to limit on/off
  void on_quadPatchCB_toggled(bool checked);   // Show quad patches tessellation on/off
  void on_controlMeshCB_toggled(bool checked); // Show control mesh on/off
  void on_innerLevelSB_valueChanged(int arg1); // Change inner tessellation level
  void on_outerLevelSB_valueChanged(int arg1); // Change outer tessellation level
  void on_gridLinesCB_toggled(bool checked);   // Show grid lines for quads on/off
  void on_showModelCB_toggled(bool checked);   // Hide model on/off

  void on_applySharpnessPB_released();         // Functions for setting/applying sharpness
  void on_sharpnessSlider_editingFinished();

private:
};

#endif // MAINWINDOW_H

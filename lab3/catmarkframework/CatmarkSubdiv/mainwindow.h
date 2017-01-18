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

private slots:
  void on_ImportOBJ_clicked();                 // Load object
  void on_SubdivSteps_valueChanged(int value); // Change subdivision level
  void on_wireFrameCB_toggled(bool checked);   // Wireframe mode on/off
  void on_controlMeshCB_toggled(bool checked); // Show control mesh on/off
  void on_showModelCB_toggled(bool checked);   // Hide model on/off
  void on_sharpnessSlider_editingFinished();   // Sharpness updated

private:
};

#endif // MAINWINDOW_H

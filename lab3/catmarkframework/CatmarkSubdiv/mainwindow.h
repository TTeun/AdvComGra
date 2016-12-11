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

  enum class MESH_MODE{
      MESH,  // Show subdivided mesh
      LIMIT  // Show limit mesh
  };

  void importOBJ();
  Ui::MainWindow *ui;
  void setSharpness(double value);
private:
  unsigned int currentMesh = 0;

private slots:
  void on_ImportOBJ_clicked();
  void on_SubdivSteps_valueChanged(int value);
  void on_checkBox_toggled(bool checked);
  void on_limitPointsCB_toggled(bool checked);
  void on_quadPatchCB_toggled(bool checked);
  void on_controlMeshCB_toggled(bool checked);
  void on_sharpnessSlider_editingFinished();

  void on_sharpnessPB_clicked();

private:
};

#endif // MAINWINDOW_H

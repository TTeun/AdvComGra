#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "objfile.h"
#include <QFileDialog>
#include "mesh.h"
#include "meshtools.h"
#include "mainview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {

  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  QVector<Mesh> Meshes;
  void loadOBJ();

private slots:
  void on_SubdivSteps_valueChanged(int value);
  void on_LoadOBJ_clicked();
  void on_reflDensitySlider_valueChanged(int value);
  void on_shaderComboBox_currentIndexChanged(const QString &arg1);

  void on_curScalingSB_valueChanged(int arg1);

  void on_rotResetButton_clicked();

  void on_drawModeComboBox_currentIndexChanged(const QString &arg1);

private:
  Ui::MainWindow *ui;

  void setShading(MainView::SHADER_MODE shaderMode);
  GLint meshIndex = 0;
};


#endif // MAINWINDOW_H

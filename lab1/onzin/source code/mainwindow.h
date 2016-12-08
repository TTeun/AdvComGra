#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {

  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_controlNet_toggled(bool checked);          // Show new
  void on_curvePoints_toggled(bool checked);         // Show subdivided curve
  void on_netPresets_currentIndexChanged(int index); // Load preset net
  void on_subdivMask_returnPressed();                // Load submask
  void on_subdivSteps_valueChanged(int arg1);        // Change number of subdiv steps
  void on_curvColCB_toggled(bool checked);           // Show curvature via colour
  void on_normCombCB_toggled(bool checked);          // Show normal comb
  void on_oscuCircCB_toggled(bool checked);          // Show osculating circle
  void on_oscuCircPosition_valueChanged(int value);  // Update oscu-circle position

private:
  Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H

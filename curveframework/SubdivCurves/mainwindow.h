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
  void on_controlNet_toggled(bool checked);
  void on_curvePoints_toggled(bool checked);
  void on_netPresets_currentIndexChanged(int index);
  void on_subdivMask_returnPressed();
  void on_subdivSteps_valueChanged(int arg1);

private:
  Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H

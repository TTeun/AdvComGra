#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mainview.h"
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {

	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_rotX_valueChanged(int value);
    void on_rotY_valueChanged(int value);
    void on_rotZ_valueChanged(int value);

    void on_distance_valueChanged(int value);

private:
	Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H

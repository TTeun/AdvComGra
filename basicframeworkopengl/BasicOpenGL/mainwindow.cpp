#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	:
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	qDebug() << "✓✓ MainWindow constructor";
}

MainWindow::~MainWindow()
{
	delete ui;
	qDebug() << "✗✗ MainWindow destructor";
}


void MainWindow::on_rotX_valueChanged(int value)
{
    ui->mainView->setRotation('x', value);
    ui->mainView->update();
}

void MainWindow::on_rotY_valueChanged(int value)
{
    ui->mainView->setRotation('y', value);
    ui->mainView->update();
}

void MainWindow::on_rotZ_valueChanged(int value)
{
    ui->mainView->setRotation('z', value);
    ui->mainView->update();
}


void MainWindow::on_distance_valueChanged(int value)
{
    ui->mainView->setDist(value);
    ui->mainView->update();
}

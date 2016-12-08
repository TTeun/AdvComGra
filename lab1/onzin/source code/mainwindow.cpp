#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :  QMainWindow(parent), ui(new Ui::MainWindow) {
  qDebug() << "✓✓ MainWindow constructor";
  ui->setupUi(this);

  ui->controlNet->setChecked(true);

  ui->curvePoints->setChecked(false);

  ui->netPresets->addItem("Pentagon");
  ui->netPresets->addItem("Basis");

  // Input restrictions for the Mask
  ui->subdivMask->setValidator(new QRegularExpressionValidator(QRegularExpression("(-?\\d+\\s)+(-?\\d+\\s?)")));

  // Initialise mask
  ui->mainView->setMask(ui->subdivMask->text());

}

MainWindow::~MainWindow() {
  qDebug() << "✗✗ MainWindow destructor";
  delete ui;
}

void MainWindow::on_controlNet_toggled(bool checked) {
  ui->mainView->showNet = checked;
  ui->mainView->update();
}

void MainWindow::on_curvePoints_toggled(bool checked) {
  ui->mainView->showCurvePts = checked;
  ui->mainView->refresh();
}

void MainWindow::on_netPresets_currentIndexChanged(int index) {
  if (ui->mainView->isValid()) {
    ui->mainView->presetNet(index);
  }
}

void MainWindow::on_subdivMask_returnPressed() {
  ui->mainView->setMask(ui->subdivMask->text());
  ui->mainView->refresh();
}

void MainWindow::on_subdivSteps_valueChanged(int value) {
  ui->mainView->setSteps(value);
  ui->mainView->refresh();
}

// The following functions could perhaps be factored
void MainWindow::on_curvColCB_toggled(bool checked)
{
    ui->mainView->showCurCol = checked;
    ui->mainView->refresh();
}

void MainWindow::on_oscuCircCB_toggled(bool checked)
{
    ui->mainView->showOscu = checked;
    ui->mainView->refresh();
}

void MainWindow::on_normCombCB_toggled(bool checked)
{
    ui->mainView->showComb = checked;
    ui->mainView->refresh();
}

void MainWindow::on_oscuCircPosition_valueChanged(int value)
{
    ui->mainView->setPos(value);
    ui->mainView->refresh();
}

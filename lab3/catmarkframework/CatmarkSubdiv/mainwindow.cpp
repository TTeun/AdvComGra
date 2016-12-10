#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :  QMainWindow(parent), ui(new Ui::MainWindow) {
  qDebug() << "✓✓ MainWindow constructor";
  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  qDebug() << "✗✗ MainWindow destructor";
  delete ui;

  Meshes.clear();
  Meshes.squeeze();
}

void MainWindow::importOBJ() {
  OBJFile newModel = OBJFile(QFileDialog::getOpenFileName(this, "Import OBJ File", "models/", tr("Obj Files (*.obj)")));
  Meshes.clear();
  Meshes.squeeze();
  Meshes.append( Mesh(&newModel) );

  ui->MainDisplay->updateMeshBuffers( &Meshes[0] );
  ui->MainDisplay->modelLoaded = true;

  ui->MainDisplay->update();
}

void MainWindow::on_ImportOBJ_clicked() {
  importOBJ();
  ui->SubdivSteps->setEnabled(true);
  ui->MainDisplay->controlMesh = &Meshes[0];
}

void MainWindow::on_SubdivSteps_valueChanged(int value) {
  unsigned short k;

  for (k=Meshes.size(); k<value+1; k++) {
    Meshes.append(Mesh());
    subdivideCatmullClark(&Meshes[k-1], &Meshes[k]);
  }
  currentMesh = value;

  ui->MainDisplay->updateMeshBuffers( &Meshes[value] );
  ui->limitPointsCB->setChecked(false);
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    ui->MainDisplay->wireframeMode = checked;
    ui->MainDisplay->update();
}

void MainWindow::on_limitPointsCB_toggled(bool checked)
{
    if (checked){
        limitMesh = new Mesh();
        toLimitMesh(&Meshes[currentMesh], limitMesh);
        ui->MainDisplay->updateMeshBuffers( limitMesh );
    } else {
        ui->MainDisplay->updateMeshBuffers( &Meshes[currentMesh] );
    }
}

void MainWindow::on_quadPatchCB_toggled(bool checked)
{
    ui->MainDisplay->patchMode = checked;
    ui->MainDisplay->updateMeshBuffers( &Meshes[currentMesh] );
    ui->MainDisplay->updateMatrices();
    ui->MainDisplay->update();
}

void MainWindow::on_controlMeshCB_toggled(bool checked)
{
    ui->MainDisplay->showControlMesh = checked;
    ui->MainDisplay->update();
}

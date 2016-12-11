#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :  QMainWindow(parent), ui(new Ui::MainWindow) {
  qDebug() << "✓✓ MainWindow constructor";
  ui->setupUi(this);
  ui->MainDisplay->mainWindow = this;
}

MainWindow::~MainWindow() {
  qDebug() << "✗✗ MainWindow destructor";
  ui->MainDisplay->Meshes.clear();
  ui->MainDisplay->Meshes.squeeze();

  delete ui;
}

void MainWindow::importOBJ() {
  OBJFile newModel = OBJFile(QFileDialog::getOpenFileName(this, "Import OBJ File", "models/", tr("Obj Files (*.obj)")));
  ui->MainDisplay->Meshes.clear();
  ui->MainDisplay->Meshes.squeeze();
  ui->MainDisplay->Meshes.append( Mesh(&newModel) );

  ui->MainDisplay->updateMeshBuffers( &ui->MainDisplay->Meshes[0] );
  ui->MainDisplay->modelLoaded = true;

  ui->MainDisplay->update();
}

void MainWindow::on_ImportOBJ_clicked() {
  ui->MainDisplay->firstPass = true;
  importOBJ();
  ui->SubdivSteps->setEnabled(true);
}

void MainWindow::on_SubdivSteps_valueChanged(int value) {
  unsigned short k;

  for (k=ui->MainDisplay->Meshes.size(); k<value+1; k++) {
    ui->MainDisplay->Meshes.append(Mesh());
    subdivideCatmullClark(&ui->MainDisplay->Meshes[k-1], &ui->MainDisplay->Meshes[k]);
  }
  currentMesh = value;
  ui->MainDisplay->currentMesh = value;

  ui->MainDisplay->updateMeshBuffers( &ui->MainDisplay->Meshes[value] );
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
        ui->MainDisplay->limitMesh = new Mesh();
        toLimitMesh(&ui->MainDisplay->Meshes[currentMesh], ui->MainDisplay->limitMesh);
        ui->MainDisplay->updateMeshBuffers( ui->MainDisplay->limitMesh );
    } else {
        ui->MainDisplay->updateMeshBuffers( &ui->MainDisplay->Meshes[currentMesh] );
    }
}

void MainWindow::on_quadPatchCB_toggled(bool checked)
{
    ui->MainDisplay->patchMode = checked;
    ui->MainDisplay->updateMeshBuffers( &ui->MainDisplay->Meshes[currentMesh] );
    ui->MainDisplay->updateMatrices();
    ui->MainDisplay->update();
}

void MainWindow::on_controlMeshCB_toggled(bool checked)
{
    ui->MainDisplay->showControlMesh = checked;
    ui->MainDisplay->update();
}

void MainWindow::setSharpness(double value){
    ui->sharpnessSlider->blockSignals(true);
    ui->sharpnessSlider->setValue(value);
    ui->sharpnessSlider->blockSignals(false);
}

void MainWindow::on_sharpnessSlider_editingFinished()
{
    HalfEdge *currentEdge;
    currentEdge = &ui->MainDisplay->Meshes[0].HalfEdges[ui->MainDisplay->selected_index];
    currentEdge->sharpness = ui->sharpnessSlider->value();
    currentEdge->twin->sharpness = ui->sharpnessSlider->value();
}

void MainWindow::on_sharpnessPB_clicked()
{
    qDebug() << "Re dividing surface";
    ui->MainDisplay->Meshes.resize(1);
    unsigned short k;
    int value = ui->SubdivSteps->value();
    for (k=ui->MainDisplay->Meshes.size(); k<value+1; k++) {
      ui->MainDisplay->Meshes.append(Mesh());
      subdivideCatmullClark(&ui->MainDisplay->Meshes[k-1], &ui->MainDisplay->Meshes[k]);
    }
    currentMesh = value;
    ui->MainDisplay->currentMesh = value;

    ui->MainDisplay->updateMeshBuffers( &ui->MainDisplay->Meshes[value] );
    ui->limitPointsCB->setChecked(false);

}

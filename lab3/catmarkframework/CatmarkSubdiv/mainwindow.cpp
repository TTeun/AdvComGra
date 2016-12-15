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

  ui->ImportOBJ->setEnabled(false);

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
  ui->MainDisplay->currentMeshIndex = value;

  ui->MainDisplay->updateMeshBuffers( &ui->MainDisplay->Meshes[value] );
  ui->MainDisplay->buildQuadMesh();
  ui->limitPointsCB->setChecked(false);
}

void MainWindow::on_wireFrameCB_toggled(bool checked)
{
    ui->MainDisplay->wireframeMode = checked;
    ui->MainDisplay->update();
}

void MainWindow::on_limitPointsCB_toggled(bool checked)
{
    ui->MainDisplay->limitShown = checked;

    if (checked){
        ui->MainDisplay->limitMesh = new Mesh();
        toLimitMesh(&ui->MainDisplay->Meshes[currentMesh], ui->MainDisplay->limitMesh); // Implementation in meshtools.cpp
        ui->MainDisplay->updateMeshBuffers( ui->MainDisplay->limitMesh );
        ui->MainDisplay->buildQuadMesh(); // Quad mesh needs to be rebuild in case we want to swap between subdiv or limit mesh

    } else {
        ui->MainDisplay->updateMeshBuffers( &ui->MainDisplay->Meshes[currentMesh] );
        ui->MainDisplay->buildQuadMesh();
    }

}

void MainWindow::on_quadPatchCB_toggled(bool checked)
{
    ui->MainDisplay->showQuadPatch = checked;
    ui->MainDisplay->updateMeshBuffers( &ui->MainDisplay->Meshes[currentMesh] );

    ui->MainDisplay->buildQuadMesh();

    // Show the quad ui features
    ui->quadPatchGB->setEnabled(checked);
    ui->innerLevelSB->setEnabled(checked);
    ui->outerLevelSB->setEnabled(checked);
    ui->innerLevelLabel->setEnabled(checked);
    ui->outerLevelLabel->setEnabled(checked);
    ui->gridLinesCB->setEnabled(checked);

    ui->MainDisplay->updateMatrices();
    ui->MainDisplay->update();
}

void MainWindow::on_controlMeshCB_toggled(bool checked)
{
    ui->MainDisplay->showControlMesh = checked;
    ui->MainDisplay->update();
}

void MainWindow::on_gridLinesCB_toggled(bool checked)
{
    ui->MainDisplay->showGridLines = checked;
    ui->MainDisplay->update();
}

void MainWindow::on_showModelCB_toggled(bool checked)
{
    ui->MainDisplay->showModel = checked;
    ui->MainDisplay->update();
}

void MainWindow::on_innerLevelSB_valueChanged(int arg1)
{
    ui->MainDisplay->tessLevelInner = arg1;
    ui->MainDisplay->updateMatrices();
}

void MainWindow::on_outerLevelSB_valueChanged(int arg1)
{
    ui->MainDisplay->tessLevelOuter = arg1;
    ui->MainDisplay->updateMatrices();
}

// The sharpness handling functions
void MainWindow::on_applySharpnessPB_released()
{
    ui->MainDisplay->Meshes.resize(1);
    ui->MainDisplay->Meshes.squeeze();

    unsigned short k;
    int value = ui->SubdivSteps->value();
    for (k=ui->MainDisplay->Meshes.size(); k<value+1; k++) {
      ui->MainDisplay->Meshes.append(Mesh());
      subdivideCatmullClark(&ui->MainDisplay->Meshes[k-1], &ui->MainDisplay->Meshes[k]);
    }

    currentMesh = value;
    ui->MainDisplay->currentMeshIndex = value;

    ui->MainDisplay->updateMeshBuffers( &ui->MainDisplay->Meshes[value] );
    ui->limitPointsCB->setChecked(false);
}

void MainWindow::setSharpness(double value){
    ui->sharpnessSlider->setValue(value);
}

void MainWindow::on_sharpnessSlider_editingFinished()
{
    HalfEdge *currentEdge;
    currentEdge = &ui->MainDisplay->Meshes[0].HalfEdges[ui->MainDisplay->selected_index];
    currentEdge->sharpness = ui->sharpnessSlider->value();
    currentEdge->twin->sharpness = ui->sharpnessSlider->value();

}



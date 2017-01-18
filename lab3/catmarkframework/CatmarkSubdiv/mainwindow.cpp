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

//  ui->ImportOBJ->setEnabled(false);

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

  ui->MainDisplay->updateMeshBuffers( &ui->MainDisplay->Meshes[value] );
}

void MainWindow::on_wireFrameCB_toggled(bool checked)
{
    ui->MainDisplay->wireframeMode = checked;
    ui->MainDisplay->update();
}

void MainWindow::on_controlMeshCB_toggled(bool checked)
{
    ui->MainDisplay->showControlMesh = checked;
    ui->MainDisplay->update();
}

void MainWindow::on_showModelCB_toggled(bool checked)
{
    ui->MainDisplay->showModel = checked;
    ui->MainDisplay->update();
}

void MainWindow::setSharpness(double value){
    ui->sharpnessSlider->setValue(value);
}

void MainWindow::on_sharpnessSlider_editingFinished()
{
  if (ui->MainDisplay->selected_index >=  ui->MainDisplay->Meshes[0].HalfEdges.size() || (ui->MainDisplay->selected_index <= 0))
    return;

  if(ui->MainDisplay->selected_index_vert>-1){
      Vertex *selectedVertex;
      selectedVertex = &ui->MainDisplay->Meshes[0].Vertices[ui->MainDisplay->selected_index_vert];
      selectedVertex->sharpness = ui->sharpnessSlider->value();
      qDebug()<<"Sharpness:";
      qDebug()<< selectedVertex->sharpness;
  }else{
  HalfEdge *currentEdge;
  currentEdge = &ui->MainDisplay->Meshes[0].HalfEdges[ui->MainDisplay->selected_index];
  currentEdge->sharpness = ui->sharpnessSlider->value();
  currentEdge->twin->sharpness = currentEdge->sharpness;
    }
  ui->MainDisplay->buildCtrlMesh();
  ui->MainDisplay->Meshes.resize(1);
  ui->MainDisplay->Meshes.squeeze();

  unsigned short k;
  int value = ui->SubdivSteps->value();
  for (k=ui->MainDisplay->Meshes.size(); k<value+1; k++) {
    ui->MainDisplay->Meshes.append(Mesh());
    subdivideCatmullClark(&ui->MainDisplay->Meshes[k-1], &ui->MainDisplay->Meshes[k]);
  }

  currentMesh = value;
  ui->MainDisplay->updateMeshBuffers( &ui->MainDisplay->Meshes[value] );
}

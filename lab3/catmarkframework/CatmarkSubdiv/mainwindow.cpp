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
  importOBJ();
  ui->SubdivSteps->setEnabled(true);
}

void MainWindow::on_SubdivSteps_valueChanged(int value) {
  unsigned short k;

  for (k=ui->MainDisplay->Meshes.size(); k<value+1; k++) {
    ui->MainDisplay->Meshes.append(Mesh());
    subdivideCatmullClark(&ui->MainDisplay->Meshes[k-1], &ui->MainDisplay->Meshes[k]);
  }

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

void MainWindow::on_relfCheckBox_toggled(bool checked)
{
    ui->MainDisplay->useReflLines = checked;
    qDebug()<<"Refl toogled";
    ui->MainDisplay->update();
}
void MainWindow::on_reflDensitySlider_valueChanged(int value)
{ // Change the reflection line density
    ui->MainDisplay->reflectionDensity = (float)value;
    ui->MainDisplay->update();
}

void MainWindow::setSharpness(double value){
    ui->sharpnessSlider->setValue(value);
}

void MainWindow::on_sharpnessSlider_editingFinished()
{
  // Once user is done editing the sharpness, we need to send this sharpness to edge or vertex
  int vertIndex = ui->MainDisplay->selected_index_vert;
  int edgeIndex = ui->MainDisplay->selected_index;


  if(ui->MainDisplay->selected_index_vert>-1){
      //Setting the sharpness for a selected vertex

  if (vertIndex > -1){ // A vertex was selected when editing sharpness

      Vertex *selectedVertex;
      selectedVertex = &ui->MainDisplay->Meshes[0].Vertices[vertIndex]; // Set sharpness of vertex
      selectedVertex->sharpness = ui->sharpnessSlider->value();

      qDebug()<<"Sharpness:";
      qDebug()<< selectedVertex->sharpness;
  }else{
      //Setting the sharpness for a selected edge
  HalfEdge *currentEdge;
  currentEdge = &ui->MainDisplay->Meshes[0].HalfEdges[ui->MainDisplay->selected_index];
  currentEdge->sharpness = ui->sharpnessSlider->value();
  currentEdge->twin->sharpness = currentEdge->sharpness;
    }

  } else if (edgeIndex > -1){ // An edge was selected
      HalfEdge *currentEdge;
      currentEdge = &ui->MainDisplay->Meshes[0].HalfEdges[edgeIndex];
      currentEdge->sharpness = ui->sharpnessSlider->value();
      currentEdge->twin->sharpness = currentEdge->sharpness;
  }

  ui->MainDisplay->buildCtrlMesh();

  // We now have to redo the subdivision
  ui->MainDisplay->Meshes.resize(1);
  ui->MainDisplay->Meshes.squeeze();

  int value = ui->SubdivSteps->value();
  on_SubdivSteps_valueChanged(value);
}

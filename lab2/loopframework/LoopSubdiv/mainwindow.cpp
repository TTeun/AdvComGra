#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  qDebug() << "✓✓ MainWindow constructor";
  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  qDebug() << "✗✗ MainWindow destructor";
  delete ui;

  Meshes.clear();
  Meshes.squeeze();
}

void MainWindow::loadOBJ() {
  OBJFile newModel = OBJFile(QFileDialog::getOpenFileName(this, "Import OBJ File", "models/", tr("Obj Files (*.obj)")));
  Meshes.clear();
  Meshes.squeeze();
  Meshes.append( Mesh(&newModel) );

  ui->MainDisplay->updateMeshBuffers( &Meshes[0] );
  ui->MainDisplay->modelLoaded = true;
  ui->displayGB->setEnabled(true);

  ui->MainDisplay->update();
}

void MainWindow::on_SubdivSteps_valueChanged(int value) {
  unsigned short k;

  for (k=Meshes.size(); k<value+1; k++) {
    Meshes.append(Mesh());
    subdivideLoop(&Meshes[k-1], &Meshes[k]);
  }

  ui->MainDisplay->updateMeshBuffers( &Meshes[value] );
  meshIndex = value;
}

void MainWindow::on_LoadOBJ_clicked() {
  loadOBJ();
  ui->LoadOBJ->setEnabled(false);
  ui->SubdivSteps->setEnabled(true);
}

void MainWindow::on_reflDensitySlider_valueChanged(int value)
{ // Change the reflection line density
    ui->MainDisplay->reflectionDensity = value;
    ui->MainDisplay->uniformUpdateRequired = true;
    ui->MainDisplay->update();
}

void MainWindow::on_shaderComboBox_currentIndexChanged(const QString &arg1)
{ // Change the type of shading in use
    ui->MainDisplay->shaderMode = arg1 == "Normal Shading"     ? MainView::MAIN :
                                  arg1 == "Flat Shading"       ? MainView::FLAT :
                                  arg1 == "Gaussian Curvature" ? MainView::GAUSS :
                                  arg1 == "Reflection Lines"   ? MainView::REFLECTION :
                                                                 MainView::MAIN;

    // Disable/enable refl. density slider depending on whether or not reflection line is in use
    ui->reflDensitySlider->setEnabled(arg1 == "Reflection Lines");
    ui->curScalingSB->setEnabled(arg1 == "Gaussian Curvature");    // Same for Gaussian curvature

    if (arg1 == "Reflection Lines") // When reflection lines is choosen, it is probably best to swap to Surface viewing mode
        ui->drawModeComboBox->setCurrentText("Surface");

    ui->MainDisplay->updateMatrices();
    ui->MainDisplay->updateColours(&Meshes[meshIndex]);
}

void MainWindow::on_curScalingSB_valueChanged(int arg1)
{ // Change the scaling of the curvature
    ui->MainDisplay->curScaling = arg1;
    ui->MainDisplay->updateMatrices();

    ui->MainDisplay->updateColours(&Meshes[meshIndex]);
}

void MainWindow::on_rotResetButton_clicked()
{ // Reset the rotation to original position
    ui->MainDisplay->rotX = 0.0;
    ui->MainDisplay->rotY = 0.0;
    ui->MainDisplay->updateMatrices();
}

void MainWindow::on_drawModeComboBox_currentIndexChanged(const QString &arg1)
{ // Change the drawing mode
    ui->MainDisplay->drawMode = arg1 == "Surface"     ? MainView::SURFACE     :
                                arg1 == "Wireframe"   ? MainView::WIRE_MESH   :
                                arg1 == "Point Cloud" ? MainView::POINT_CLOUD :
                                                        MainView::WIRE_MESH;
    ui->MainDisplay->updateMatrices();

}

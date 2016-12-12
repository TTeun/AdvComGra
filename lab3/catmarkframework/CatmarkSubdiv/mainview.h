#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLDebugLogger>

#include <QOpenGLShaderProgram>

#include <QMouseEvent>
#include "mesh.h"
#include "mainwindow.h"

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {

  Q_OBJECT

public:
  MainView(QWidget *Parent = 0);
  ~MainView();

  MainWindow *mainWindow;
  float rotX = 0, rotY = 0;

  size_t currentMesh = 0;
  bool modelLoaded;
  bool wireframeMode;
  bool patchMode = false;
  bool showControlMesh = false;

  float FoV;
  float dispRatio;

  bool firstPass = true;


  enum MODE{
      LIMITMESH,
      DIVMESH
  };

  void updateMatrices();
  void updateUniforms();
  void updateMeshBuffers(Mesh *currentMesh);

  QVector<Mesh> Meshes;
  Mesh *limitMesh;
  void *setSharpnessSlider(double sharpness);
  int selected_index = -1;

  float tessLevelInner = 4.0;
  float tessLevelOuter = 4.0;
  bool showGridLines = false;

protected:
  void initializeGL();
  void resizeGL(int newWidth, int newHeight);
  void paintGL();

  unsigned int maxInt;

  void renderMesh();

  void mousePressEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent*);
  void wheelEvent(QWheelEvent* event);
  void keyPressEvent(QKeyEvent* event);

private:
  QOpenGLDebugLogger* debugLogger;

  QMatrix4x4 modelViewMatrix, projectionMatrix;
  QMatrix3x3 normalMatrix;

  // Uniforms
  GLint uniModelViewMatrix, uniProjectionMatrix, uniNormalMatrix;
  GLint ctrlUniModelViewMatrix, ctrlUniProjectionMatrix, ctrlUniNormalMatrix;
  GLint tessUniModelViewMatrix, tessUniProjectionMatrix, tessUniNormalMatrix;
  GLint uniTessLevelInner;
  GLint uniTessLevelOuter;
  GLint uniShowGridLines;



  // ---

  QOpenGLShaderProgram* mainShaderProg, *tessShaderProg, *controlMeshShader;

  GLuint meshVAO, meshCoordsBO, meshNormalsBO, meshIndexBO;
  GLuint ctrlVAO, ctrlCoordsBO, ctrlIndexBO, ctrlColourBO;
  GLuint slctVAO, slctCoordsBO, slctIndexBO, slctColourBO;
  unsigned int meshIBOSize;

  // ---

  QVector<QVector3D> vertexCoords;
  QVector<QVector3D> vertexNormals;
  QVector<unsigned int> polyIndices;

  QVector<QVector3D> ctrlCoords;
  QVector<QVector3D> ctrlColours;
  QVector<unsigned int> ctrlIndices;

  QVector<QVector3D> slctCoords;
  QVector<QVector3D> slctColours = {QVector3D(1.0, 0.0, 0.0), QVector3D(1.0, 0.0, 0.0)};
  QVector<unsigned int> slctlIndices = {0, 1};

  void createShaderPrograms();
  void createBuffers();  
  void buildCtrlMesh();

  QVector2D lastPos;
  bool rotating = false;
private slots:
  void onMessageLogged( QOpenGLDebugMessage Message );

};

#endif // MAINVIEW_H

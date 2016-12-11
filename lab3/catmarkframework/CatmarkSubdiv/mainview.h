#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLDebugLogger>

#include <QOpenGLShaderProgram>

#include <QMouseEvent>
#include "mesh.h"

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {

  Q_OBJECT

public:
  MainView(QWidget *Parent = 0);
  ~MainView();

  float rotX = 0, rotY = 0;

  size_t currentMesh = 0;
  bool modelLoaded;
  bool wireframeMode;
  bool patchMode = false;
  bool showControlMesh = false;

  float FoV;
  float dispRatio;

  bool firstPass = true;

  bool uniformUpdateRequired;

  void updateMatrices();
  void updateUniforms();
  void updateMeshBuffers(Mesh* currentMesh);

  QVector<Mesh> Meshes;

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
  QVector<QVector3D> slctColours;
  QVector<unsigned int> slctlIndices;

  void createShaderPrograms();
  void createBuffers();  

  QVector2D lastPos;
  bool rotating = false;
  int selected_index = -1;
private slots:
  void onMessageLogged( QOpenGLDebugMessage Message );

};

#endif // MAINVIEW_H

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

  bool modelLoaded;
  bool wireframeMode;
  bool showControlMesh = true; // Show the control mesh

  float FoV;
  float dispRatio;

  bool firstPass = true;    // When the model is just loaded, we build the control mesh (only once)

  void updateMatrices();
  void updateUniforms();
  void updateMeshBuffers(Mesh *currentMesh);

  QVector<Mesh> Meshes;     // Moved here from the MainWindow class for slightly easier jumpin between meshes

  // Tessellation level
  float tessLevelInner = 4.0;
  float tessLevelOuter = 4.0;
  bool showGridLines = false;

  bool showModel = true; // To hide model, can be useful when setting sharpness on the control mesh

  bool useReflLines = false;
  float reflectionDensity = 10.0;
  // This if setting the sharpness
  void *setSharpnessSlider(double sharpness);
  int selected_index = -1;
  int selected_index_vert = -1;
  void buildCtrlMesh();  // Build the control mesh. Happens only once on firstPass, so can private


protected:
  void initializeGL();
  void resizeGL(int newWidth, int newHeight);
  void paintGL();

  unsigned int maxInt;

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
  GLint uniModelViewMatrix, uniProjectionMatrix, uniNormalMatrix, uniModelViewMatrixRefl, uniProjectionMatrixRefl,uniNormalMatrixRefl;
  GLint ctrlUniModelViewMatrix, ctrlUniProjectionMatrix, ctrlUniNormalMatrix,uniReflDensity;
  GLint uniShowGridLines;

  // ---

  QOpenGLShaderProgram* mainShaderProg, *controlMeshShader, *reflShaderProg;

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

  // These are for line selection. Always one line, so the colour vector is a constant vector of size 2, as is the indices vector
  QVector<QVector3D> slctCoords;
  QVector<QVector3D> slctColours = {QVector3D(1.0, 0.0, 0.0), QVector3D(1.0, 0.0, 0.0)};
  QVector<unsigned int> slctlIndices = {0, 1};

  //Used for vertex selection.
  QVector<QVector3D> slctColoursVert = {QVector3D(1.0, 0.0, 0.0), QVector3D(1.0, 0.0, 0.0),QVector3D(1.0, 0.0, 0.0),
                                    QVector3D(1.0, 0.0, 0.0),QVector3D(1.0, 0.0, 0.0), QVector3D(1.0, 0.0, 0.0)};
  QVector<unsigned int>slctlIndicesVert = {0,1,2,3,4,5};

  void createShaderPrograms();
  void createBuffers();  

  QVector2D lastPos;     // Keeping track of position for mouse rotation
  bool rotating = false;

private slots:
  void onMessageLogged( QOpenGLDebugMessage Message );

};

#endif // MAINVIEW_H

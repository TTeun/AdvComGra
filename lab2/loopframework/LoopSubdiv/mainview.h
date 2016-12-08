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

  bool modelLoaded;

  int selected_index = -1;

  float FoV;
  float dispRatio;

  bool uniformUpdateRequired;

  void updateMatrices();
  void updateUniforms();
  void updateMeshBuffers(Mesh* currentMesh);

  float reflectionDensity = 10.0;

  enum SHADER_MODE{
      MAIN,
      REFLECTION,
      FLAT,
      GAUSS
  };
  SHADER_MODE shaderMode = MAIN;

  enum DRAW_MODE{
      SURFACE,
      WIRE_MESH,
      POINT_CLOUD
  };
  DRAW_MODE drawMode = WIRE_MESH;


  void updateColours(Mesh* currentMesh);
  int curScaling = 0;
protected:
  QVector2D lastPos;

  void initializeGL();
  void resizeGL(int newWidth, int newHeight);
  void paintGL();

  void renderMesh();

  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent*);
  void mouseMoveEvent(QMouseEvent* event);
  void wheelEvent(QWheelEvent* event);


private:
  QOpenGLDebugLogger* debugLogger;

  QMatrix4x4 modelViewMatrix, projectionMatrix;
  QMatrix3x3 normalMatrix;

  // Uniforms
  GLint uniModelViewMatrix, uniProjectionMatrix, uniNormalMatrix, uniReflDensity;
  // ---

  QOpenGLShaderProgram* mainShaderProg, *reflShaderProg, *flatShaderProg;


  GLuint meshVAO, meshCoordsBO, meshNormalsBO, meshIndexBO, meshColourBO;
  unsigned int meshIBOSize;

  // ---

  QVector<QVector3D> vertexCoords;
  QVector<QVector3D> vertexColours;
  QVector<QVector3D> vertexNormals;
  QVector<unsigned int> polyIndices;

  void createShaderPrograms();
  void createBuffers();  

  bool rotating = false;

  double root(double value);
private slots:
  void onMessageLogged( QOpenGLDebugMessage Message );

};


#endif // MAINVIEW_H

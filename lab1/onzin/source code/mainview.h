#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLWidget>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>

#include <QVector2D>
#include <QMouseEvent>

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {

  Q_OBJECT

public:
  MainView(QWidget *parent = 0);
  ~MainView();

  void clearArrays(); // Clears net
  void clearTemps();  // Clears all other arrays

  void presetNet(unsigned short preset);
  void updateBuffers();

  bool showNet, showCurvePts = false;
  bool showCurCol = false, showOscu = false, showComb = false; // Curvatures
  short int selectedPt;
  short int findClosest(float x, float y);

  void setMask(QString stringMask);
  void setSteps(int value);

  void refresh(); // updateBuffers + update

  void setPos(int value); // Set position of osculating circle

protected:
  void initializeGL();
  void paintGL();

  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void keyPressEvent(QKeyEvent *event);

private:
  QOpenGLShaderProgram *mainShaderProg, *curShaderProg, *combShaderProg;
  QOpenGLDebugLogger* debugLogger;
  GLuint steps =0;
  QVector<QVector2D> netCoords, subCoords, tmpCoords, oscuCoords;
  QVector<QVector3D> netColours, subColours, oscuColours;


  GLuint netVAO, netCoordsBO, netColourBO;
  GLuint subVAO, subCoordsBO, subColourBO;
  GLuint oscuVAO, oscuCoordsBO, oscuColourBO;

  void createShaderPrograms();
  void createBuffers();
  void  makeCircle();
  int pos = 50; // Initial circle position = halfway curve

  bool updateUniformsRequired;
  bool shouldDelete;
  //GLint uni...

  void updateUniforms();

  QVector<short int> subdivMask, firstStencil, secondStencil;
  float normalizeValue;
  QVector2D applyStencil(GLuint stencil, GLuint firstPt);

  int  recur(int s0, int constant); // A recurrence relation that shows up several times

private slots:
  void onMessageLogged( QOpenGLDebugMessage Message );

};

#endif // MAINVIEW_H

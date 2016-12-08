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

  void clearArrays();
  void presetNet(unsigned short preset);
  void updateBuffers();

  bool showNet, showCurvePts;
  short int selectedPt;
  short int findClosest(float x, float y);

  void setMask(QString stringMask);

protected:
  void initializeGL();
  void paintGL();

  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void keyPressEvent(QKeyEvent *event);

private:
  QOpenGLDebugLogger* debugLogger;

  QVector<QVector2D> netCoords;

  QOpenGLShaderProgram* mainShaderProg;
  GLuint netVAO, netCoordsBO;

  void createShaderPrograms();
  void createBuffers();

  bool updateUniformsRequired;
  //GLint uni...

  void updateUniforms();

  QVector<short int> subdivMask, firstStencil, secondStencil;
  float normalizeValue;

private slots:
  void onMessageLogged( QOpenGLDebugMessage Message );

};

#endif // MAINVIEW_H

#include "mainview.h"
#include "math.h"

MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
  qDebug() << "✓✓ MainView constructor";

  selectedPt = -1;
}

MainView::~MainView() {
  qDebug() << "✗✗ MainView destructor";

  clearArrays();

  glDeleteBuffers(1, &netCoordsBO);
  glDeleteVertexArrays(1, &netVAO);

  delete mainShaderProg;

  debugLogger->stopLogging();
}

// ---

void MainView::createShaderPrograms() {

  // Qt approach
  mainShaderProg = new QOpenGLShaderProgram();
  mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
  mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");

  mainShaderProg->link();

//  uni... = glGetUniformLocation(mainShaderProg->programId(), "...");
}

void MainView::createBuffers() {

  // Pure OpenGL
  glGenVertexArrays(1, &netVAO);
  glBindVertexArray(netVAO);

  glGenBuffers(1, &netCoordsBO);
  glBindBuffer(GL_ARRAY_BUFFER, netCoordsBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);
}

void MainView::updateBuffers() {

  glBindBuffer(GL_ARRAY_BUFFER, netCoordsBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D)*netCoords.size(), netCoords.data(), GL_DYNAMIC_DRAW);

  update();

}

void MainView::updateUniforms() {

  //  glUniform...();

  updateUniformsRequired = false;
}

void MainView::clearArrays() {

  // As of Qt 5.6, clear() does not release the memory anymore. Use e.g. squeeze()
  netCoords.clear();
  netCoords.squeeze();
}

// ---

void MainView::initializeGL() {

  initializeOpenGLFunctions();
  qDebug() << ":: OpenGL initialized";

  debugLogger = new QOpenGLDebugLogger();
  connect( debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage ) ), this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ), Qt::DirectConnection );

  if ( debugLogger->initialize() ) {
    qDebug() << ":: Logging initialized";
    debugLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
    debugLogger->enableMessages();
  }

  // If the application crashes here, try setting "MESA_GL_VERSION_OVERRIDE = 4.1"
  // and "MESA_GLSL_VERSION_OVERRIDE = 410" in Projects (left panel) -> Build Environment

  QString glVersion;
  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
  qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

  // Enable depth buffer
  glEnable(GL_DEPTH_TEST);
  // Default is GL_LESS
  glDepthFunc(GL_LEQUAL);

  // ---

  createShaderPrograms();
  createBuffers();

  presetNet(0);
}

void MainView::paintGL() {

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mainShaderProg->bind();

  if (updateUniformsRequired) {
    updateUniforms();
  }

  if (showNet) {
    glBindVertexArray(netVAO);

    // Draw control net
    glDrawArrays(GL_LINE_STRIP, 0, netCoords.size());
    glPointSize(8.0);
    glDrawArrays(GL_POINTS, 0, netCoords.size());

    // Highlight selected control point
    if (selectedPt > -1) {
      glPointSize(12.0);
      glDrawArrays(GL_POINTS, selectedPt, 1);
    }

    glBindVertexArray(0);
  }

  if (showCurvePts) {
    // ...
  }

  mainShaderProg->release();
}

// ---

void MainView::presetNet(unsigned short preset) {

  selectedPt = -1;
  clearArrays();

  switch (preset) {
  case 0:
    // 'Pentagon'
    netCoords.reserve(5);
    netCoords.append(QVector2D(-0.25, -0.5));
    netCoords.append(QVector2D(-0.75, 0.0));
    netCoords.append(QVector2D(-0.25, 0.75));
    netCoords.append(QVector2D(0.75, 0.5));
    netCoords.append(QVector2D(0.5, -0.75));
    break;
  case 1:
    // 'Basis'
    netCoords.reserve(9);
    netCoords.append(QVector2D(-1.0, -0.25));
    netCoords.append(QVector2D(-0.75, -0.25));
    netCoords.append(QVector2D(-0.5, -0.25));
    netCoords.append(QVector2D(-0.25, -0.25));
    netCoords.append(QVector2D(0.0, 0.50));
    netCoords.append(QVector2D(0.25, -0.25));
    netCoords.append(QVector2D(0.5, -0.25));
    netCoords.append(QVector2D(0.75, -0.25));
    netCoords.append(QVector2D(1.0, -0.25));
    break;
  }

  updateBuffers();

}

void MainView::setMask(QString stringMask) {

  subdivMask.clear();

  QString trimmedMask;
  trimmedMask = stringMask.trimmed();

  // Convert to sequence of integers
  QTextStream intSeq(&trimmedMask);
  while (!intSeq.atEnd()) {
    int k;
    intSeq >> k;
    subdivMask.append(k);
  }

  // Stencils represent affine combinations (i.e. they should sum to unity)
  normalizeValue = 0;

  firstStencil.clear();
  secondStencil.clear();

  for (int k=0; k<subdivMask.size(); k++) {
    if (k % 2) {
      normalizeValue += subdivMask[k];
      firstStencil.append(subdivMask[k]);
    }
    else {
      secondStencil.append(subdivMask[k]);
    }
  }

  qDebug() << ":: Extracted stencils" << firstStencil << "and" << secondStencil;
}

// ---

void MainView::mousePressEvent(QMouseEvent *event) {

  // In order to allow keyPressEvents:
  setFocus();

  float xRatio, yRatio, xScene, yScene;

  xRatio = (float)event->x() / width();
  yRatio = (float)event->y() / height();

  // By default, the drawing canvas is the square [-1,1]^2:
  xScene = (1-xRatio)*-1 + xRatio*1;
  // Note that the origin of the canvas is in the top left corner (not the lower left).
  yScene = yRatio*-1 + (1-yRatio)*1;

  switch (event->buttons()) {
  case Qt::LeftButton:
    if (selectedPt > -1) {
      // De-select control point
      selectedPt = -1;
      setMouseTracking(false);
      update();
    }
    else {
      // Add new control point
      netCoords.append(QVector2D(xScene, yScene));
      updateBuffers();
    }
    break;
  case Qt::RightButton:
    // Select control point
    selectedPt = findClosest(xScene, yScene);
    update();
    break;
  }

}

void MainView::mouseMoveEvent(QMouseEvent *event) {

  if (selectedPt > -1) {
    float xRatio, yRatio, xScene, yScene;

    xRatio = (float)event->x() / width();
    yRatio = (float)event->y() / height();

    xScene = (1-xRatio)*-1 + xRatio*1;
    yScene = yRatio*-1 + (1-yRatio)*1;

    // Update position of the control point
    netCoords[selectedPt] = QVector2D(xScene, yScene);
    updateBuffers();
  }

}

void MainView::keyPressEvent(QKeyEvent *event) {

  // Only works when the widget has focus!

  switch(event->key()) {
  case 'G':
    if (selectedPt > -1) {
      // Grab selected control point
      setMouseTracking(true);
    }
    break;
  case 'X':
    if (selectedPt > -1) {
      // Remove selected control point
      netCoords.remove(selectedPt);
      selectedPt = -1;
      updateBuffers();
    }
    break;
  }

}

short int MainView::findClosest(float x, float y) {

  short int ptIndex;
  float currentDist, minDist = 4;

  for (int k=0; k<netCoords.size(); k++) {
    currentDist = pow((netCoords[k].x()-x),2) + pow((netCoords[k].y()-y),2);
    if (currentDist < minDist) {
      minDist = currentDist;
      ptIndex = k;
    }
  }

  return ptIndex;

}

// ---

void MainView::onMessageLogged( QOpenGLDebugMessage Message ) {
  qDebug() << " → Log:" << Message;
}

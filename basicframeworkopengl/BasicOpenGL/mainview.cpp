#include "mainview.h"
#include "math.h"
#include <iostream>
#include <fstream>
#include <string>

MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
  qDebug() << "✓✓ MainView constructor";
}

MainView::~MainView() {

  clearArrays();

  glDeleteBuffers(1, &fanCoordsBO);
  glDeleteBuffers(1, &fanColourBO);
  glDeleteBuffers(1, &fanIndexBO);
  glDeleteVertexArrays(1, &fanVAO);

  delete mainShaderProg;

  debugLogger->stopLogging();

  qDebug() << "✗✗ MainView destructor";
}

// ---

void MainView::createShaderPrograms() {

  // Qt approach
  mainShaderProg = new QOpenGLShaderProgram();
  mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          ":/shaders/vertshader.glsl");
  mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":/shaders/fragshader.glsl");

  mainShaderProg->link();

  // Qt wrappers
  // uniModelViewMatrix = mainShaderProg->uniformLocation("modelviewmatrix");
  // uniProjectionMatrix = mainShaderProg->uniformLocation("projectionmatrix");

  // Pure OpenGL
  uniModelViewMatrix =
      glGetUniformLocation(mainShaderProg->programId(), "modelviewmatrix");
  uniProjectionMatrix =
      glGetUniformLocation(mainShaderProg->programId(), "projectionmatrix");
}

void MainView::createBuffers() {

  // Pure OpenGL
  glGenVertexArrays(1, &fanVAO);
  glBindVertexArray(fanVAO);

  glGenBuffers(1, &fanCoordsBO);
  glBindBuffer(GL_ARRAY_BUFFER, fanCoordsBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glGenBuffers(1, &fanColourBO);
  glBindBuffer(GL_ARRAY_BUFFER, fanColourBO);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glGenBuffers(1, &fanIndexBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fanIndexBO);

  //

  glBindVertexArray(0);
}

void MainView::updateBuffers() {
    clearArrays();

    std::ifstream fstr;
    fstr.open("horse.off");
    if (not fstr.is_open()){
        qDebug() << "File not opened.";
        return;
    }

    std::string str;
    fstr >> str;
    if (str != "OFF"){
        qDebug() << "File is not an OFF file.";
        return;
    }

    unsigned int n_vertexes;
    unsigned int n_faces;
    unsigned int edges;
    fstr >> n_vertexes >> n_faces >> edges;

   float x, y, z;
   for (GLuint vtx = 0; vtx < n_vertexes; ++vtx){
       fstr >> x >> y >> z;
       modelCoords.append(QVector3D(x,y,z));
       modelColours.append(QVector3D(.4, 0.8, 0.6));
   }

   unsigned int i0, i1, i2, temp;
   for (GLuint fce = 0; fce < n_faces; ++fce){
       fstr >> temp >> i0 >> i1 >> i2;
       modelIndices.append(i0);
       modelIndices.append(i1);
       modelIndices.append(i2);

       if (i0 > n_faces || i1 > n_faces || i2 > n_faces)
           qDebug() << i0 << i1 << i2;
   }

  glBindBuffer(GL_ARRAY_BUFFER, fanCoordsBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) * modelCoords.size(),
               modelCoords.data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, fanColourBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) * modelColours.size(),
               modelColours.data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fanIndexBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(unsigned short) * modelIndices.size(), modelIndices.data(),
               GL_DYNAMIC_DRAW);

}


void MainView::setRotation(char coor, int value){
    if (coor == 'x'){
        rot_x = value;
    } else if (coor == 'y'){
        rot_y = value;
    } else if (coor == 'z'){
        rot_z = value;
    }
    updateMatrices();
}

void MainView::updateMatrices() {
  modelViewMatrix.setToIdentity();

  modelViewMatrix.translate(0.0, 0.0, -dist);
  modelViewMatrix.rotate(rot_x, QVector3D(1.0, 0.0, 0.0));
  modelViewMatrix.rotate(rot_y, QVector3D(0.0, 1.0, 0.0));
  modelViewMatrix.rotate(rot_z, QVector3D(0.0, 0.0, 1.0));

  projectionMatrix.setToIdentity();
  projectionMatrix.perspective(90.0, (float)width() / height(), 0.2, 1000.0);

  updateUniformsRequired = true;
}

void MainView::updateUniforms() {

  // Qt wrappers
  // mainShaderProg->setUniformValue(uniModelViewMatrix, modelViewMatrix);
  // mainShaderProg->setUniformValue(uniProjectionMatrix, projectionMatrix);

  // Pure OpenGL
  glUniformMatrix4fv(uniModelViewMatrix, 1, false, modelViewMatrix.data());
  glUniformMatrix4fv(uniProjectionMatrix, 1, false, projectionMatrix.data());

  updateUniformsRequired = false;
}

void MainView::clearArrays() {

  modelCoords.clear();
  modelCoords.squeeze();
  modelColours.clear();
  modelColours.squeeze();
  modelIndices.clear();
  modelIndices.squeeze();
}

// ---

QVector3D MainView::convertHSLtoRGB(float H, float S, float L) {

  float C, Hp, X, m;
  QVector3D preRGB;

  // Grey values, S=0.0, so C=0.0
  C = (1.0 - abs(2.0 * L - 1.0)) * S;
  m = L - (C / 2.0);
  Hp = (H * 360.0) / 60.0;
  X = C * (1.0 - abs(fmod(Hp, 2.0) - 1.0));

  if (Hp > 5.0) {
    preRGB = QVector3D(C, 0.0, X);
  } else if (Hp > 4.0) {
    preRGB = QVector3D(X, 0.0, C);
  } else if (Hp > 3.0) {
    preRGB = QVector3D(0.0, X, C);
  } else if (Hp > 2.0) {
    preRGB = QVector3D(0.0, C, X);
  } else if (Hp > 1.0) {
    preRGB = QVector3D(X, C, 0.0);
  } else if (Hp >= 0.0) {
    preRGB = QVector3D(C, X, 0.0);
  }

  return preRGB + QVector3D(m, m, m);
}

// ---

void MainView::initializeGL() {

  qDebug() << ":: Initializing OpenGL";
  initializeOpenGLFunctions();

  debugLogger = new QOpenGLDebugLogger();
  connect(debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this,
          SLOT(onMessageLogged(QOpenGLDebugMessage)), Qt::DirectConnection);

  if (debugLogger->initialize()) {
    qDebug() << ":: Logging initialized";
    debugLogger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
    debugLogger->enableMessages();
  }

  QString glVersion;
  glVersion = reinterpret_cast<const char *>(glGetString(GL_VERSION));
  qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

  // Enable depth buffer
  glEnable(GL_DEPTH_TEST);
  // Default is GL_LESS
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glPointSize(3);

  createShaderPrograms();
  createBuffers();
  updateBuffers();
  updateMatrices();
}

void MainView::resizeGL(int newWidth, int newHeight) { updateMatrices(); }

void MainView::paintGL() {

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mainShaderProg->bind();

  if (updateUniformsRequired)
    updateUniforms();


      glBindVertexArray(fanVAO);

//      glDrawElements(GL_LINE_STRIP, modelIndices.size() - 1, GL_UNSIGNED_SHORT, nullptr);
      glDrawElements(GL_TRIANGLES, modelIndices.size() - 1, GL_UNSIGNED_SHORT, nullptr);


  glBindVertexArray(0);

  mainShaderProg->release();
}

// ---

void MainView::onMessageLogged(QOpenGLDebugMessage Message) {
  qDebug() << " → Log:" << Message;
}

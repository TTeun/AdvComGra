#include "mainview.h"
#include <QtMath>

MainView::MainView(QWidget *Parent) : QOpenGLWidget(Parent) {
  qDebug() << "✓✓ MainView constructor";

  modelLoaded = false;

  FoV = 60.0;
}

MainView::~MainView() {
  qDebug() << "✗✗ MainView destructor";

  glDeleteBuffers(1, &meshCoordsBO);
  glDeleteBuffers(1, &meshNormalsBO);
  glDeleteBuffers(1, &meshColourBO);
  glDeleteBuffers(1, &meshIndexBO);
  glDeleteVertexArrays(1, &meshVAO);

  debugLogger->stopLogging();

  delete mainShaderProg;
  delete reflShaderProg;
  delete flatShaderProg;
}

void MainView::createShaderPrograms() {
  qDebug() << ".. createShaderPrograms";

  mainShaderProg = new QOpenGLShaderProgram();
  mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
  mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");

  mainShaderProg->link();

  // Shader program to handle reflection lines
  reflShaderProg = new QOpenGLShaderProgram();
  reflShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
  reflShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/reflshader.glsl");

  reflShaderProg->link();

  // Shader program for simple flat shading. Can be nice sometimes when viewing the mesh as points or lines
  flatShaderProg = new QOpenGLShaderProgram();
  flatShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
  flatShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/flatfragshader.glsl");

  flatShaderProg->link();


  uniModelViewMatrix = glGetUniformLocation(reflShaderProg->programId(), "modelviewmatrix");
  uniProjectionMatrix = glGetUniformLocation(reflShaderProg->programId(), "projectionmatrix");
  uniNormalMatrix = glGetUniformLocation(reflShaderProg->programId(), "normalmatrix");

  // Here we find the uniform variable corresponding to the reflection line density
  uniReflDensity = glGetUniformLocation(reflShaderProg->programId(), "refldensity");
}

void MainView::createBuffers() {

  qDebug() << ".. createBuffers";

  glGenVertexArrays(1, &meshVAO);
  glBindVertexArray(meshVAO);

  glGenBuffers(1, &meshCoordsBO);
  glBindBuffer(GL_ARRAY_BUFFER, meshCoordsBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glGenBuffers(1, &meshNormalsBO);
  glBindBuffer(GL_ARRAY_BUFFER, meshNormalsBO);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glGenBuffers(1, &meshColourBO);
  glBindBuffer(GL_ARRAY_BUFFER, meshColourBO);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glGenBuffers(1, &meshIndexBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIndexBO);

  glBindVertexArray(0);
}

void MainView::updateMeshBuffers(Mesh* currentMesh) {

  qDebug() << ".. updateBuffers";

  unsigned int k;
  unsigned short m;
  HalfEdge* currentEdge;

  vertexCoords.clear();
  vertexCoords.reserve(currentMesh->Vertices.size());

  for (k=0; k<currentMesh->Vertices.size(); k++) {
    vertexCoords.append(currentMesh->Vertices[k].coords);
  }

  vertexNormals.clear();
  vertexNormals.reserve(currentMesh->Vertices.size());

  for (k=0; k<currentMesh->Faces.size(); k++) {
    currentMesh->setFaceNormal(&currentMesh->Faces[k]);
  }

  for (k=0; k<currentMesh->Vertices.size(); k++) {
    vertexNormals.append( currentMesh->computeVertexNormal(&currentMesh->Vertices[k]) );
  }

  polyIndices.clear();
  polyIndices.reserve(currentMesh->HalfEdges.size() + currentMesh->Faces.size());

  for (k=0; k<currentMesh->Faces.size(); k++) {
    currentEdge = currentMesh->Faces[k].side;
    for (m=0; m<3; m++) {
      polyIndices.append(currentEdge->target->index);
      currentEdge = currentEdge->next;
    }
  }

  updateColours(currentMesh);

  glBindBuffer(GL_ARRAY_BUFFER, meshCoordsBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexCoords.size(), vertexCoords.data(), GL_DYNAMIC_DRAW);

  qDebug() << " → Updated meshCoordsBO";

  glBindBuffer(GL_ARRAY_BUFFER, meshNormalsBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexNormals.size(), vertexNormals.data(), GL_DYNAMIC_DRAW);

  qDebug() << " → Updated meshNormalsBO";

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIndexBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*polyIndices.size(), polyIndices.data(), GL_DYNAMIC_DRAW);

  qDebug() << " → Updated meshIndexBO";

  meshIBOSize = polyIndices.size();

  update();
}

double MainView::root(double value){
    // A simple scaling function used to spread out the values of the Gaussian curvature
    for (int i = 0; i < curScaling; ++i)
        value = sqrt(value);
    for (int i = curScaling; i < 0; ++i)
        value = value * value;

    return value;
}

void MainView::updateColours(Mesh* currentMesh){
    // Here we determine the colours of the vertices.
    vertexColours.clear();
    vertexColours.squeeze();

    if (shaderMode == GAUSS){
        QVector<float> deficits;   // Will hold angle deficits for all vertices.
        float maxDeficit = 0.0;
        float minDeficit = 1000.0; // Used to normalize deficits to range [-1,1]

        Vertex *currentVertex;
        HalfEdge* currentEdge;
        int k;
        float faceAngle;
        float totalAngle;
        for (int i = 0; i < vertexCoords.size(); ++i){

            totalAngle = 0.0;
            currentVertex = &currentMesh->Vertices[i];
            currentEdge = currentVertex->out;

            for (k=0; k<currentVertex->val; k++) {

                faceAngle = acos( fmax(-1.0, QVector3D::dotProduct(
                                           (currentEdge->target->coords - currentVertex->coords).normalized(),
                                           (currentEdge->prev->twin->target->coords - currentVertex->coords).normalized() ) ) );

                totalAngle += faceAngle;
                currentEdge = currentEdge->twin->next;

            }
            deficits.append( 6.2832 - totalAngle );
            maxDeficit = qMax(maxDeficit, deficits[i]);
            minDeficit = qMin(minDeficit, deficits[i]);
        }

        for (int i = 0; i < vertexCoords.size(); ++i){
            if (deficits[i] < 0) // Negative deficit
                vertexColours.append(QVector3D(0.0, 1 - root(deficits[i] / minDeficit), root(deficits[i] / minDeficit)));
            else                 // positive deficit (aka surplus)
                vertexColours.append(QVector3D(root(deficits[i] / maxDeficit), 1 - root(deficits[i] / maxDeficit), 0.0));
        }
    }
    else {        // When de not apply Gaussian shading, we simply pick a nice colour
        for (int i = 0; i < vertexCoords.size(); ++i)
            vertexColours.append(QVector3D(0.53, 0.80, 0.87));
    }

    // Bind the colours
    glBindBuffer(GL_ARRAY_BUFFER, meshColourBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexColours.size(), vertexColours.data(), GL_DYNAMIC_DRAW);

    qDebug() << " → Updated meshColoursBO";

}

void MainView::updateMatrices() {

  modelViewMatrix.setToIdentity();
  modelViewMatrix.translate(QVector3D(0.0, 0.0, -3.0));
  modelViewMatrix.scale(QVector3D(1.0, 1.0, 1.0));

  // Rotate around y-axis first
  modelViewMatrix.rotate(rotX, 0.0, 1.0, 0.0);

  // To now rotate about apparent x-axis, i.e., the axis from left to right on the screen, we must
  // multiply (1.0, 0.0, 0.0) by the inverse modelview matrix. 4th coordinate is 0.0 -> no translation
  QVector4D dir = modelViewMatrix.inverted() * QVector4D(1.0, 0.0, 0.0, 0.0);
  modelViewMatrix.rotate(rotY, dir[0], dir[1], dir[2]);

  projectionMatrix.setToIdentity();
  projectionMatrix.perspective(FoV, dispRatio, 0.2, 8.0);

  normalMatrix = modelViewMatrix.normalMatrix();

  uniformUpdateRequired = true;
  update();

}

void MainView::updateUniforms() {

  // mainShaderProg should be bound at this point!

  glUniformMatrix4fv(uniModelViewMatrix, 1, false, modelViewMatrix.data());
  glUniformMatrix4fv(uniProjectionMatrix, 1, false, projectionMatrix.data());
  glUniformMatrix3fv(uniNormalMatrix, 1, false, normalMatrix.data());

  if (shaderMode == REFLECTION) // If not in REFLECTION mode, this variable is not in any shader
      glUniform1f(uniReflDensity, reflectionDensity);

}

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

  QString glVersion;
  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
  qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

  // Enable depth buffer
  glEnable(GL_DEPTH_TEST);
  // Default is GL_LESS
  glDepthFunc(GL_LEQUAL);

  createShaderPrograms();
  createBuffers();
  updateMatrices();
}

void MainView::resizeGL(int newWidth, int newHeight) {
  qDebug() << ".. resizeGL";

  dispRatio = (float)newWidth/newHeight;
  updateMatrices();

}

void MainView::paintGL() {

  if (modelLoaded) {

    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QOpenGLShaderProgram *shaderProg;
    // Here we use a switch to select the shader program to use
    switch (shaderMode) {
    case REFLECTION:
        shaderProg = reflShaderProg;
        break;
    case FLAT:
        shaderProg = flatShaderProg;
        break;
    default:
        shaderProg = mainShaderProg;
        break;
    }
    shaderProg->bind();

    if (uniformUpdateRequired) {
      updateUniforms();
      uniformUpdateRequired = false;
    }

    renderMesh();

    shaderProg->release();
  }
}

void MainView::renderMesh() {

    if (not modelLoaded){
        return;
    }
    glBindVertexArray(meshVAO);

    if (drawMode != SURFACE) { //  For lines or point we do not need GL_FILL
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }
    else {
      glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    glPointSize(2.0); // In case we are drawing a point cloud
    GLenum mode = drawMode == POINT_CLOUD ? GL_POINTS : GL_TRIANGLES;
    glDrawElements(mode, meshIBOSize, GL_UNSIGNED_INT, 0);

    if (selected_index > -1){ // This is to show the point selected by the user
        glPointSize(10.0);
        glDrawArrays(GL_POINTS, selected_index, 1);
    }
    glBindVertexArray(0);

}

double sq(double a){ // No comment necessary
    return a*a;
}

void MainView::mousePressEvent(QMouseEvent* event) {
  setFocus();
  float xRatio, yRatio, xScene, yScene;
  xRatio = (float)event->x() / width();
  yRatio = (float)event->y() / height();

  // Get the mouse position in NDS
  xScene = 2 * xRatio - 1;
  yScene = 1 - 2 * yRatio;

  if (event->button() == Qt::RightButton){ // If Right button pressed
      lastPos[0] = xScene; // Store the position of mouse
      lastPos[1] = yScene;

      rotating = true; // And start rotating
  }
  if (event->button() == Qt::LeftButton){ // If Left button pressed

      // We send a ray out into the scene and find the closest vertex to it

      // Unproject the ray from NDS
      QVector4D ray_eye = (projectionMatrix.inverted()) * QVector4D(xScene, yScene, -1.0, 1.0);
      ray_eye[2] = -1.0;
      ray_eye[3] = 0.0; //  The ray is a direction, not a position

      // Invert the modelview
      QMatrix4x4 MV = modelViewMatrix.inverted();
      QVector4D ray_f = MV * ray_eye;

      // This is the direction the ray shoots in
      QVector3D ray_final = (QVector3D(ray_f[0], ray_f[1], ray_f[2])).normalized();

      // This is the origin of the ray in world coordinates
      QVector4D lastCol = MV.column(3);
      QVector3D origin = QVector3D(lastCol[0], lastCol[1], lastCol[2]);

      // We now look for the vertex that minimizes the distance to this ray
      float dist, minDist = 1000.0;
      int minIndex = 0;
      for (int i = 0; i < vertexCoords.size(); ++i){
          dist = vertexCoords[i].distanceToLine(origin, ray_final); // Nice Qt helper function
          if (dist < minDist){
              minDist = dist;
              minIndex = i;
          }
      }
      selected_index = minDist < 0.03 ? minIndex : -1;
      updateMatrices();
  }
}

void MainView::mouseMoveEvent(QMouseEvent* event) {
  setFocus();
  if (rotating){    // If we are rotating currently
      float xRatio, yRatio, xScene, yScene;

      xRatio = (float)event->x() / width();
      yRatio = (float)event->y() / height();

      xScene = 2 * xRatio - 1;
      yScene = 1 - 2 * yRatio;

      QVector2D dir;
      dir[0] = lastPos.x() - xScene; // Find how much the mouse moved
      dir[1] = lastPos.y() - yScene;

      rotX -= 200 * dir[0]; // Scale it, and update the rotaion values
      rotY += 200 * dir[1];

      updateMatrices();

      lastPos[0] = xScene; // Store the new position as the last position
      lastPos[1] = yScene;
  }
}

void MainView::mouseReleaseEvent(QMouseEvent*) {
  setFocus();
  rotating = false; // Stop rotating
}

void MainView::wheelEvent(QWheelEvent* event) {
  FoV -= event->delta() / 60.0;
  updateMatrices();
}

void MainView::onMessageLogged( QOpenGLDebugMessage Message ) {
  qDebug() << " → Log:" << Message;
}

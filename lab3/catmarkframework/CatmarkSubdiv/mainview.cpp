#include "mainview.h"

MainView::MainView(QWidget *Parent) : QOpenGLWidget(Parent) {
  qDebug() << "✓✓ MainView constructor";

  modelLoaded = false;
  wireframeMode = true;

  FoV = 60.0;
}

MainView::~MainView() {
  qDebug() << "✗✗ MainView destructor";

  glDeleteBuffers(1, &meshCoordsBO);
  glDeleteBuffers(1, &meshNormalsBO);
  glDeleteBuffers(1, &meshIndexBO);
  glDeleteVertexArrays(1, &meshVAO);

  glDeleteBuffers(1, &ctrlCoordsBO);
  glDeleteBuffers(1, &ctrlIndexBO);
  glDeleteVertexArrays(1, &ctrlVAO);

  glDeleteBuffers(1, &slctCoordsBO);
  glDeleteBuffers(1, &slctIndexBO);
  glDeleteVertexArrays(1, &slctVAO);

  debugLogger->stopLogging();

  delete mainShaderProg;
  delete tessShaderProg;
  delete controlMeshShader;
}

// ---

void MainView::createShaderPrograms() {
  qDebug() << ".. createShaderPrograms";

  mainShaderProg = new QOpenGLShaderProgram();
  mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
  mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");

  mainShaderProg->link();

  controlMeshShader = new QOpenGLShaderProgram();
  controlMeshShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
  controlMeshShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/controlfragshader.glsl");

  controlMeshShader->link();

  tessShaderProg = new QOpenGLShaderProgram();
  tessShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshaderts.glsl");          // vertex shader without projection
  tessShaderProg->addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/shaders/tcshader.glsl");
  tessShaderProg->addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/shaders/teshader.glsl");
  tessShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");

  tessShaderProg->link();

  uniModelViewMatrix = glGetUniformLocation(mainShaderProg->programId(), "modelviewmatrix");
  uniProjectionMatrix = glGetUniformLocation(mainShaderProg->programId(), "projectionmatrix");
  uniNormalMatrix = glGetUniformLocation(mainShaderProg->programId(), "normalmatrix");
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

  glGenBuffers(1, &meshIndexBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIndexBO);


  glBindVertexArray(0);

  // Control Mesh

  glGenVertexArrays(1, &ctrlVAO);
  glBindVertexArray(ctrlVAO);

  glGenBuffers(1, &ctrlCoordsBO);
  glBindBuffer(GL_ARRAY_BUFFER, ctrlCoordsBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glGenBuffers(1, &ctrlColourBO);
  glBindBuffer(GL_ARRAY_BUFFER, ctrlColourBO);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glGenBuffers(1, &ctrlIndexBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctrlIndexBO);

  // VAO for showing the selected line

  glGenVertexArrays(1, &slctVAO);
  glBindVertexArray(slctVAO);

  glGenBuffers(1, &slctCoordsBO);
  glBindBuffer(GL_ARRAY_BUFFER, slctCoordsBO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glGenBuffers(1, &slctColourBO);
  glBindBuffer(GL_ARRAY_BUFFER, slctColourBO);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glGenBuffers(1, &slctIndexBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, slctIndexBO);


  glBindVertexArray(0);
}

void MainView::buildCtrlMesh(){
    firstPass = false;
    unsigned int k, n, m;
    HalfEdge* currentEdge;
    ctrlCoords.clear();
    ctrlCoords.reserve(Meshes[0].Vertices.size());

    ctrlIndices.clear();
    ctrlIndices.reserve(Meshes[0].HalfEdges.size() + Meshes[0].Faces.size());

    for (k=0; k<(GLuint)Meshes[0].Vertices.size(); k++) {
      ctrlCoords.append(Meshes[0].Vertices[k].coords);
      ctrlColours.append(QVector3D(0.6, 0.8, 0.0));
    }
    for (k=0; k<(GLuint)Meshes[0].Faces.size(); k++) {
        n = Meshes[0].Faces[k].val;
        currentEdge = Meshes[0].Faces[k].side;
        for (m=0; m<n; m++) {
            if (currentEdge->index < currentEdge->twin->index){
                ctrlIndices.append(currentEdge->twin->target->index);
                ctrlIndices.append(currentEdge->target->index);
            }
            currentEdge = currentEdge->next;
        }
        ctrlIndices.append(maxInt);
    }

    glBindBuffer(GL_ARRAY_BUFFER, ctrlCoordsBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*ctrlCoords.size(), ctrlCoords.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, ctrlColourBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*ctrlColours.size(), ctrlColours.data(), GL_DYNAMIC_DRAW);

    qDebug() << " → Updated ctrlCoordsBO";

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctrlIndexBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*ctrlIndices.size(), ctrlIndices.data(), GL_DYNAMIC_DRAW);

}

void MainView::updateMeshBuffers(Mesh *currentMesh) {

  qDebug() << ".. updateBuffers";

  unsigned int k;
  unsigned short n, m;
  HalfEdge* currentEdge;

  if (firstPass){
      buildCtrlMesh();
  }

  vertexCoords.clear();
  vertexCoords.reserve(currentMesh->Vertices.size());

  for (k=0; k<(GLuint)currentMesh->Vertices.size(); k++) {
    vertexCoords.append(currentMesh->Vertices[k].coords);
  }

  vertexNormals.clear();
  vertexNormals.reserve(currentMesh->Vertices.size());

  for (k=0; k<(GLuint)currentMesh->Faces.size(); k++) {
    currentMesh->setFaceNormal(&currentMesh->Faces[k]);
  }

  for (k=0; k<(GLuint)currentMesh->Vertices.size(); k++) {
    vertexNormals.append( currentMesh->computeVertexNormal(&currentMesh->Vertices[k]) );
  }

  polyIndices.clear();
  polyIndices.reserve(currentMesh->HalfEdges.size() + currentMesh->Faces.size());

  if (patchMode) // Indexing for quad patches is different and we skip ngons with n != 4
      for (k=0; k<(GLuint)currentMesh->Faces.size(); k++) {
          n = currentMesh->Faces[k].val;
          if (n == 4){
              currentEdge = currentMesh->Faces[k].side;
              polyIndices.append(currentEdge->target->index);
              polyIndices.append(currentEdge->prev->target->index);
              polyIndices.append(currentEdge->next->target->index);
              polyIndices.append(currentEdge->next->next->target->index);

              polyIndices.append(maxInt);
          }
      }
  else
      for (k=0; k<(GLuint)currentMesh->Faces.size(); k++) {
          n = currentMesh->Faces[k].val;
          currentEdge = currentMesh->Faces[k].side;
          for (m=0; m<n; m++) {
              polyIndices.append(currentEdge->target->index);
              currentEdge = currentEdge->next;
          }
          polyIndices.append(maxInt);
      }





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

  QString glVersion;
  glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
  qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

  // Enable depth buffer
  glEnable(GL_DEPTH_TEST);
  // Default is GL_LESS
  glDepthFunc(GL_LEQUAL);

  glEnable(GL_PRIMITIVE_RESTART);
  maxInt = ((unsigned int) -1);
  glPrimitiveRestartIndex(maxInt);

  // ---

  createShaderPrograms();
  createBuffers();

  // ---

  updateMatrices();
}

void MainView::resizeGL(int newWidth, int newHeight) {

  qDebug() << ".. resizeGL";

  dispRatio = (float)newWidth/newHeight;
  updateMatrices();

}

void MainView::paintGL() {

  if (modelLoaded) {

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QOpenGLShaderProgram *shaderProg = patchMode ? tessShaderProg : mainShaderProg;
    shaderProg->bind();

    if (uniformUpdateRequired) {
      updateUniforms();
      uniformUpdateRequired = false;
    }

    renderMesh();
    shaderProg->release();
    if (showControlMesh)
    {
        controlMeshShader->bind();
        updateUniforms();
        glBindVertexArray(ctrlVAO);
        glDrawElements(GL_LINES, ctrlIndices.size(), GL_UNSIGNED_INT, 0);

        if (selected_index != -1){
            glBindVertexArray(slctVAO);
            glDrawElements(GL_LINES, slctCoords.size(), GL_UNSIGNED_INT, 0);
        }
        controlMeshShader->release();
    }
  }
}

// ---

void MainView::renderMesh() {

    glBindVertexArray(meshVAO);
    if (patchMode){
        glPatchParameteri(GL_PATCH_VERTICES, 4);
        glDrawElements(GL_PATCHES, meshIBOSize, GL_UNSIGNED_INT, 0);
    }
    else
    {
        if (wireframeMode) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            glDrawElements(GL_LINE_LOOP, meshIBOSize, GL_UNSIGNED_INT, 0);
        }
        else {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glDrawElements(GL_TRIANGLE_FAN, meshIBOSize, GL_UNSIGNED_INT, 0);
        }
    }

    glBindVertexArray(0);

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

      // We send a ray out into the scene and find the closest edge to it

      QVector4D ray_eye = (projectionMatrix.inverted()) * QVector4D(xScene, yScene, -1.0, 1.0);
      ray_eye[2] = -1.0;
      ray_eye[3] = 0.0; //  The ray is a direction, not a position
      QMatrix4x4 MV = modelViewMatrix.inverted();
      QVector4D ray_f = MV * ray_eye;
      QVector3D ray_final = (QVector3D(ray_f[0], ray_f[1], ray_f[2])).normalized();
      QVector4D lastCol = MV.column(3);
      QVector3D origin = QVector3D(lastCol[0], lastCol[1], lastCol[2]);

      // ray = origin + t * ray_final, with -infty < t < infty
      // HalfEdge = p0 + s * (p1 - p0), with 0 <= s <= 1

      float dist, minDist = 1000.0;
      int minIndex = 0;

      QVector3D n, n2, p1, p2, d1, d2;
      float s;
      HalfEdge *currentEdge;
      d2 = ray_final;
      p2 = origin;
      for (int i = 0; i < Meshes[0].HalfEdges.size(); ++i)
      {
          currentEdge = &Meshes[0].HalfEdges[i];
          d1 =  currentEdge->twin->target->coords - currentEdge->target->coords;

          n = QVector3D::crossProduct(ray_final, d1);
          n = n.normalized();

          n2 = QVector3D::crossProduct(n, d2);
          p1 = currentEdge->target->coords;

          s = QVector3D::dotProduct((p2 - p1), n2) / QVector3D::dotProduct(d1, n2);
          if ((s < 0) || (s > 1))
              dist = 1000.0;
          else {
              dist = QVector3D::dotProduct(n, (origin - p1));
              dist = dist < 0 ? (-dist) : dist;
          }

          if (dist < minDist){
              minDist = dist;
              minIndex = i;
          }
      }

      selected_index = minIndex;

      mainWindow->setSharpness(Meshes[0].HalfEdges[selected_index].sharpness);
      updateMatrices();

      slctCoords.clear();
      slctCoords.squeeze();

      slctCoords.append(Meshes[0].HalfEdges[selected_index].target->coords);
      slctCoords.append(Meshes[0].HalfEdges[selected_index].twin->target->coords);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, slctIndexBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*slctlIndices.size(), slctlIndices.data(), GL_DYNAMIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, slctCoordsBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*slctCoords.size(), slctCoords.data(), GL_DYNAMIC_DRAW);

      glBindBuffer(GL_ARRAY_BUFFER, slctColourBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*slctColours.size(), slctColours.data(), GL_DYNAMIC_DRAW);

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

// ---

void MainView::wheelEvent(QWheelEvent* event) {
  FoV -= event->delta() / 60.0;
  updateMatrices();
}

void MainView::keyPressEvent(QKeyEvent* event) {
  switch(event->key()) {
  case 'Z':
    wireframeMode = !wireframeMode;
    update();
    break;
  }
}

// ---

void MainView::onMessageLogged( QOpenGLDebugMessage Message ) {
  qDebug() << " → Log:" << Message;
}

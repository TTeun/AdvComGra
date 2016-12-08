#include "mainview.h"
#include "math.h"

MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
  qDebug() << "✓✓ MainView constructor";

  selectedPt = -1;
}

MainView::~MainView() {
  qDebug() << "✗✗ MainView destructor";

  clearArrays();

  // Delete vertex arrays
  glDeleteBuffers(1, &netCoordsBO);
  glDeleteVertexArrays(1, &netVAO);

  glDeleteBuffers(1, &subCoordsBO);
  glDeleteVertexArrays(1, &subVAO);

  glDeleteBuffers(1, &oscuCoordsBO);
  glDeleteVertexArrays(1, &oscuVAO);

  // Delete colour arrays
  glDeleteBuffers(1, &netColourBO);
  glDeleteBuffers(1, &oscuColourBO);
  glDeleteBuffers(1, &subColourBO);

  // delete shader programs
  delete mainShaderProg;
  delete curShaderProg;
  delete combShaderProg;

  debugLogger->stopLogging();
}

// ---

void MainView::createShaderPrograms() {

    // Main shader program
    mainShaderProg = new QOpenGLShaderProgram();
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");

    mainShaderProg->link();

    // Curvature shader program showing coloured subdiv curve
    curShaderProg = new QOpenGLShaderProgram();
    curShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
    curShaderProg->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/curvatureColShader.glsl");
    curShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");

    // Curvature shader program showing normal comb
    combShaderProg = new QOpenGLShaderProgram();
    combShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
    combShaderProg->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/normalCombShader.glsl");
    combShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");

    combShaderProg->link();

  //  uni... = glGetUniformLocation(mainShaderProg->programId(), "...");
}

void MainView::createBuffers() {

    // Original points
    glGenVertexArrays(1, &netVAO);
    glBindVertexArray(netVAO);

    glGenBuffers(1, &netCoordsBO);
    glBindBuffer(GL_ARRAY_BUFFER, netCoordsBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &netColourBO);
    glBindBuffer(GL_ARRAY_BUFFER, netColourBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Subdivision array
    glGenVertexArrays(1, &subVAO);
    glBindVertexArray(subVAO);

    glGenBuffers(1, &subCoordsBO);
    glBindBuffer(GL_ARRAY_BUFFER, subCoordsBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &subColourBO);
    glBindBuffer(GL_ARRAY_BUFFER, subColourBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);


    // Osculating circle array
    glGenVertexArrays(1, &oscuVAO);
    glBindVertexArray(oscuVAO);

    glGenBuffers(1, &oscuCoordsBO);
    glBindBuffer(GL_ARRAY_BUFFER, oscuCoordsBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &oscuColourBO);
    glBindBuffer(GL_ARRAY_BUFFER, oscuColourBO);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
}

void MainView::refresh(){
    update();
    updateBuffers();
    selectedPt = -1; // Deselect point when options are (de)selected etc. Just personal preference
}

QVector2D MainView::applyStencil(GLuint stencilNum, GLuint firstPt)
{
    // Applies either the first or second stencil starting at point firstPt

    QVector<short int> const &stencil = stencilNum == 1 ? firstStencil : secondStencil; // will not change during function,
                                                                                        // and reference so no redundant copying


    GLuint size = tmpCoords.size();
    QVector2D result(0.0,0.0);
    for (int it = 0; it < stencil.size(); ++it)
        result += stencil[it] * tmpCoords[((int)(it + firstPt)) % size];

    // The modulus here is just for safety to ensure we dont go beyond the size of the vertices

    return result / normalizeValue;
}



float sq(float r){ // Self explanatory
    return r * r;
}

void MainView::updateBuffers() {

    clearTemps();

    if (normalizeValue == 0.0)
        qDebug() << "Warning Stencil sums to zero";

    tmpCoords = netCoords;
    int i;
    for (GLuint it = 0; it < steps; ++it){
        subCoords.clear();
        subCoords.squeeze();

        subCoords.append( tmpCoords[0] ); // include starting point
        for (i = 0; i < tmpCoords.size() - secondStencil.size() + 1; ++i)
        {
            subCoords.append( applyStencil(1, i) );
            subCoords.append( applyStencil(2, i) );
        }
        if (secondStencil.size() > firstStencil.size())
            subCoords.append( applyStencil(1, i));

        subCoords.append( tmpCoords.last() ); // include last point

        if (it < steps - 1)            // For all but the last iteration...
            tmpCoords.swap(subCoords); // Swap subCoords into tmpCoords so process can be iterated.
                                       // (Probably) faster than tmpCoords = subCoords -> no redundant copying
     }

    for (int i = 0; i < subCoords.size(); ++i)
        subColours.append(QVector3D(1.0, 0.0, 0.3));

    if (showOscu)
        makeCircle();


    // Bind all the buffers
    glBindBuffer(GL_ARRAY_BUFFER, netCoordsBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D)*netCoords.size(), netCoords.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, netColourBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) *netColours.size(), netColours.data(), GL_DYNAMIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, subCoordsBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D)*subCoords.size(), subCoords.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, subColourBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) *subColours.size(), subColours.data(), GL_DYNAMIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, oscuCoordsBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QVector2D)*oscuCoords.size(), oscuCoords.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, oscuColourBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D) *oscuColours.size(), oscuColours.data(), GL_DYNAMIC_DRAW);

    update();

}

void  MainView::makeCircle(){
    // This function takes three points and constructs the unique circle connecting these.
    // This could not be done nicely in a geometry shader because we want to show only one oscu circle,
    // whereas a geometry shader would draw the circle for all points

    if (steps == 0) // No subdivision -> no real use in showing oscu circle
        return;

    // pos gives the position on the scale of 0-100
    int index = (subCoords.size() * pos) / 100;
    // Bound the index so we do not attempt to make the circle on the first or last point
    index = qBound(1, index, subCoords.size() - 2);

    float x1 = subCoords[index - 1][0]; // extract coordinates
    float y1 = subCoords[index - 1][1];
    float x2 = subCoords[index    ][0];
    float y2 = subCoords[index    ][1];
    float x3 = subCoords[index + 1][0];
    float y3 = subCoords[index + 1][1];

    // The arduous and opaque process of determining the coefficients r, h and k for the circle
    // r^2 = (x-h)^2 + (y-k)^2 passing through p1, p2 and p3

    // We use the fact that r^2 = (xi-h)^2 = (yi-k)^2 for all i = 1,2,3.
    // ==> (xi-h)^2 + (yi-k)^2 = (xj-h)^2 + (yj-k)^2 for all i,j = 1,2,3.
    // Solve this to obtain the solution below
    float a12 = sq(x1) - sq(x2) + sq(y1) - sq(y2);
    float a23 = sq(x2) - sq(x3) + sq(y2) - sq(y3);
    float x12 = x1 - x2;
    float x23 = x2 - x3;
    float y12 = y1 - y2;
    float y23 = y2 - y3;

    float h = (a12 / (2* y12) - a23 / (2* y23)) / ((x12 / y12) - (x23 / y23));
    float k = (a12 - 2 * h * x12) / (2 * y12) ;
    float r = sqrtf(sq(x1-h) + sq(y1-k));
    // Math :D
    QVector2D center = QVector2D(h, k);

    oscuCoords.reserve(630);
    oscuCoords.append(center);            // Used later to draw center of oscu circle
    oscuCoords.append(subCoords[index]);  // Used later to oscu circle contact point

    for (int i = 0; i < 628; i++)    // 100 vertices per radian
        oscuCoords.append(center + r * QVector2D(sin(i / 100.0), cos(i / 100.0)));

    for (int i = 0; i < oscuCoords.size(); ++i)
        oscuColours.append( QVector3D(0.4, 0.4, 0.4));
}

void MainView::setPos(int value){
    pos = value;
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

void MainView::clearTemps() {
    // Clears all arrays but the net array
    tmpCoords.clear();
    tmpCoords.squeeze();
    subCoords.clear();
    subCoords.squeeze();
    oscuCoords.clear();
    oscuCoords.squeeze();
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

    glClearColor(0.0,0.0,0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    if (updateUniformsRequired) {
      updateUniforms();
    }

    if (showNet)
    { // Show the net
        mainShaderProg->bind();
        glBindVertexArray(netVAO);
        glDrawArrays(GL_LINE_STRIP, 0, netCoords.size());

        glPointSize(8.0);
        glDrawArrays(GL_POINTS, 0, netCoords.size());

        // Highlight selected control point
        if (selectedPt > -1) {
          glPointSize(12.0);
          glDrawArrays(GL_POINTS, selectedPt, 1);
        }

      glBindVertexArray(0);
      mainShaderProg->release();

    }

    if (steps > 0) // Show curve points, curvature visualization etc only when subdivision is applied.
    {
        if (showComb)
        { // Show the normal comb

            combShaderProg->bind(); // Load appropriate shader program
            glBindVertexArray(subVAO);
            glDrawArrays(GL_LINE_STRIP_ADJACENCY, 0, subCoords.size());
            combShaderProg->release();
        }

        if (showOscu)
        { // Show the osculating circle

            mainShaderProg->bind();
            glBindVertexArray(oscuVAO);
            glDrawArrays(GL_LINE_STRIP, 2, oscuCoords.size() - 2);

            glPointSize(8.0);
            glDrawArrays(GL_POINTS, 0, 2); // contact point and circle center

            glBindVertexArray(0);
            mainShaderProg->release();
        }

        if (showCurvePts)
        { // Show the curved points

            // Choose the appropriate shader program
            QOpenGLShaderProgram* shaderProg = showCurCol ? curShaderProg : mainShaderProg;
            shaderProg->bind();

            // Choose the appropriate draw mode
            GLuint mode = showCurCol ? GL_LINE_STRIP_ADJACENCY : GL_LINE_STRIP;

            // Draw subdivided curve
            glBindVertexArray(subVAO);
            glDrawArrays(mode, 0, subCoords.size());

            shaderProg->release();

            if (selectedPt > -1)
            { // If point is selected, show the two points bounding its influence region

                mainShaderProg->bind();

                // When moving a net point, we can find the corresponding point in subCoords by the recurrence relation in MainView::recur
                int ptIndex = recur(selectedPt, 2 - firstStencil.size());
                // Similarly
                int leftInfl = recur(0, (subdivMask.size() - 1) / 2);
                int rightInfl = recur(0, subdivMask.size() / 2);

                // We clamp the points to be inside the appropriate range. Also we decrement / increment
                // the point indices by one, so we reach the 'outer points' that remain stationary.
                int leftPt  = qMax((int)(ptIndex - leftInfl - 1), 0);
                int rightPt = qMin((int)(ptIndex + rightInfl + 1), subCoords.size() -1);

                glPointSize(13.0);
                glDrawArrays(GL_POINTS, leftPt, 1);
                glDrawArrays(GL_POINTS, rightPt, 1);

                shaderProg->release();
            }

        }
    }
}

int MainView::recur(int s0, int constant)
{   // recurrence relation s_i+1 = 2 * s_i + constant
    for (unsigned int i = 0; i < steps; ++i)
    {
        s0 <<= 1;        // ( s0 *= 2)
        s0 += constant;
    }
    return s0;
}

// ---

void MainView::setSteps(int value){ // set subdivision steps
    steps = value;
}


void MainView::presetNet(unsigned short preset) {

  selectedPt = -1;
  clearArrays();

  switch (preset) {
  case 0:
    // 'Pentagon'
    netCoords.reserve(5);
    netCoords.append(QVector2D(-0.25, -0.5));
    netCoords.append(QVector2D(-0.72, 0.0));
    netCoords.append(QVector2D(-0.25, 0.73));
    netCoords.append(QVector2D(0.79, 0.5));
    netCoords.append(QVector2D(0.5, -0.73));
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
  netColours.clear();
  netColours.squeeze();
  for (int i = 0; i < netCoords.size(); ++i)
      netColours.append(QVector3D(0.4, 0.5, 0.5));

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
      netColours.append(QVector3D(0.18, 0.309, 0.309));
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

  short int ptIndex = 0;
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

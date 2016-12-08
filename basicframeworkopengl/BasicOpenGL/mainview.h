#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLWidget>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>

#include <QVector3D>

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {

	Q_OBJECT

public:
	MainView(QWidget * parent = 0);
	~MainView();

	QVector3D convertHSLtoRGB(float H, float S, float L);
    void updateBuffers();
	void clearArrays();

    void setRotation(char coor, int value);
    void setDist(int value){
        dist = static_cast<float>(value / 20.0);
        updateMatrices();
    }

protected:
	void initializeGL();
	void resizeGL(int newWidth, int newHeight);
	void paintGL();

private:
	QOpenGLDebugLogger*debugLogger;

	QOpenGLShaderProgram*mainShaderProg;
    GLuint fanVAO, fanCoordsBO, fanColourBO, fanIndexBO;

    GLuint rot_x = 0;
    GLuint rot_y = 0;
    GLuint rot_z = 0;

    GLfloat dist = 0.0;

    void createShaderPrograms();
	void createBuffers();
    void updateMatrices();
    void updateUniforms();

	bool updateUniformsRequired;

    QVector<QVector3D> modelCoords;
    QVector<QVector3D> modelColours;
    QVector<unsigned short> modelIndices;

	GLint uniModelViewMatrix, uniProjectionMatrix;
	QMatrix4x4 modelViewMatrix, projectionMatrix;

private slots:
	void onMessageLogged( QOpenGLDebugMessage Message );

};

#endif // MAINVIEW_H

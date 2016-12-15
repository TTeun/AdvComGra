#ifndef SHADERHANDLER_H
#define SHADERHANDLER_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>

class ShaderHandler : public QObject, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT

public:

    ShaderHandler(QObject *Parent = 0);
    ~ShaderHandler();
    QOpenGLShaderProgram* mainShaderProg, *tessShaderProg, *controlMeshShader;
    void initShaders();

    QMatrix4x4 *modelViewMatrix, *projectionMatrix;
    QMatrix3x3 *normalMatrix;

    void updateMainUniforms();
    void updateTessUniforms();
    void updateCtrlUniforms();

    float tessLevelInner = 4.0;
    float tessLevelOuter = 4.0;
    bool showGridLines = false;


private:
    GLint uniModelViewMatrix, uniProjectionMatrix, uniNormalMatrix;
    GLint ctrlUniModelViewMatrix, ctrlUniProjectionMatrix, ctrlUniNormalMatrix;
    GLint tessUniModelViewMatrix, tessUniProjectionMatrix, tessUniNormalMatrix;
    GLint uniTessLevelInner;
    GLint uniTessLevelOuter;
    GLint uniShowGridLines;

};

#endif // SHADERHANDLER_H

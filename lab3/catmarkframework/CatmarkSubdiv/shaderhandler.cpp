#include "shaderhandler.h"

ShaderHandler::ShaderHandler(QWidget *Parent)
    : QOpenGLWidget(Parent)
{
    modelViewMatrix = mvMatrix;
    projectionMatrix = pMatrix;
    normalMatrix = nMatrix;
}

ShaderHandler::~ShaderHandler(){
    delete mainShaderProg;
    delete tessShaderProg;
    delete controlMeshShader;

    modelViewMatrix = nullptr;
    projectionMatrix = nullptr;
    normalMatrix = nullptr;
}

void ShaderHandler::initShaders(){class MainView;

    mainShaderProg = new QOpenGLShaderProgram();
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
    mainShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");

    mainShaderProg->link();

    controlMeshShader = new QOpenGLShaderProgram();
    controlMeshShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/ctrl_vertshader.glsl");
    controlMeshShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/ctrl_fragshader.glsl");

    controlMeshShader->link();

    tessShaderProg = new QOpenGLShaderProgram();
    tessShaderProg->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/tess_vertshader.glsl");
    tessShaderProg->addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/shaders/tess_ctrlshader.glsl");
    tessShaderProg->addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/shaders/tess_evalshader.glsl");
    tessShaderProg->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/tess_geomshader.glsl");
    tessShaderProg->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/tess_fragshader.glsl");

    tessShaderProg->link();

    uniModelViewMatrix = glGetUniformLocation(mainShaderProg->programId(), "modelviewmatrix");
    uniProjectionMatrix = glGetUniformLocation(mainShaderProg->programId(), "projectionmatrix");
    uniNormalMatrix = glGetUniformLocation(mainShaderProg->programId(), "normalmatrix");

    ctrlUniModelViewMatrix = glGetUniformLocation(controlMeshShader->programId(), "modelviewmatrix");
    ctrlUniProjectionMatrix = glGetUniformLocation(controlMeshShader->programId(), "projectionmatrix");
    ctrlUniNormalMatrix = glGetUniformLocation(controlMeshShader->programId(), "normalmatrix");

    tessUniModelViewMatrix = glGetUniformLocation(tessShaderProg->programId(), "modelviewmatrix");
    tessUniProjectionMatrix = glGetUniformLocation(tessShaderProg->programId(), "projectionmatrix");
    tessUniNormalMatrix = glGetUniformLocation(tessShaderProg->programId(), "normalmatrix");
    uniTessLevelInner = glGetUniformLocation(tessShaderProg->programId(), "TessLevelInner");
    uniTessLevelOuter = glGetUniformLocation(tessShaderProg->programId(), "TessLevelOuter");
    uniShowGridLines = glGetUniformLocation(tessShaderProg->programId(), "showGridLines");
}

void ShaderHandler::updateTessUniforms(){
    tessShaderProg->bind();
    glUniform1f(uniTessLevelInner, tessLevelInner);
    glUniform1f(uniTessLevelOuter, tessLevelOuter);
    glUniform1f(uniShowGridLines, showGridLines);
    glUniformMatrix4fv(tessUniModelViewMatrix, 1, false, modelViewMatrix->data());
    glUniformMatrix4fv(tessUniProjectionMatrix, 1, false, projectionMatrix->data());
    glUniformMatrix3fv(tessUniNormalMatrix, 1, false, normalMatrix->data());
    tessShaderProg->release();
}

void ShaderHandler::updateMainUniforms(){
    mainShaderProg->bind();
    glUniformMatrix4fv(uniModelViewMatrix, 1, false, modelViewMatrix->data());
    glUniformMatrix4fv(uniProjectionMatrix, 1, false, projectionMatrix->data());
    glUniformMatrix3fv(uniNormalMatrix, 1, false, normalMatrix->data());
    mainShaderProg->release();
}

void ShaderHandler::updateCtrlUniforms(){
    controlMeshShader->bind();
    glUniformMatrix4fv(ctrlUniModelViewMatrix, 1, false, modelViewMatrix->data());
    glUniformMatrix4fv(ctrlUniProjectionMatrix, 1, false, projectionMatrix->data());
    glUniformMatrix3fv(ctrlUniNormalMatrix, 1, false, normalMatrix->data());
    controlMeshShader->release();

}

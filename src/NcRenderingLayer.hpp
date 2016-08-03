#ifndef DATARENDERINGLAYER_H
#define DATARENDERINGLAYER_H

#include <GL/glew.h>
#include <QString>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "NcLoader.hpp"
#include "NcQuickView.hpp"
#include "NcMapView.hpp"

class NcQuickView;
class NcMapView;

class NcRenderingLayer
{
public:
    NcRenderingLayer();
    ~NcRenderingLayer();
    void init(NcMapView *view);
    void render(NcMapView *view);

private:
    GLuint                tex;
    QOpenGLShaderProgram* shader;
    GLuint                vao;
    GLuint                vbo;
    GLuint                colorbuffer;
};

#endif // DATARENDERINGLAYER_H

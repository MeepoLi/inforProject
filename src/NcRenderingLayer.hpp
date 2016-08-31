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
	void rectangle(float x1, float y1, float x2, float y2, int idx);
private:
    GLuint                tex;
    QOpenGLShaderProgram* shader;
    QOpenGLShaderProgram* rect_shader;

    GLuint                vao;
    GLuint                vbo;
    GLuint                colorbuffer;
	GLuint	rect_buffer[3];
	GLuint	rect_color[3];
	bool rectangle_flag;
};

#endif // DATARENDERINGLAYER_H

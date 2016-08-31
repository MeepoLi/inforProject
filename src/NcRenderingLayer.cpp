#include "NcRenderingLayer.hpp"
#include "QMatrix4x4D.hpp"
#include "OpenGLFuncs.hpp"

#include <QDebug>
#include <cassert>


NcRenderingLayer::NcRenderingLayer()
{

}

NcRenderingLayer::~NcRenderingLayer()
{
    delete shader;
}

void NcRenderingLayer::init(NcMapView *view)
{
    // shaders
	QSurfaceFormat glf = QSurfaceFormat::defaultFormat();
	qDebug()<<"Glversion ="<<glf.version();
    this->shader = new QOpenGLShaderProgram();
    this->shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/simple.vert");
    this->shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/color.frag");
    this->shader->link();

	this->rect_shader =  new QOpenGLShaderProgram();
    this->rect_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shaders/simple.vert");
    this->rect_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":shaders/rect_color.frag");
    this->rect_shader->link();
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->colorbuffer);

//    QPointF origin = QPointF(0,0);//view->getOrigin();
//    QPointF wTopRight  = view->cam()->geo2world(view->getGeoBounds().bottomLeft()) - origin;
//    QPointF wBottomRight = view->cam()->geo2world(view->getGeoBounds().bottomRight()) - origin;
//    QPointF wTopLeft     = view->cam()->geo2world(view->getGeoBounds().topLeft()) - origin;
//    QPointF wBottomLeft  = view->cam()->geo2world(view->getGeoBounds().topRight()) - origin;
//    QPointF wTopLeft  = QVector3DD(253522375.1, 432502876, -9.968698493).toPointF() - origin;
//    QPointF wBottomLeft = QVector3DD(253522375.1, 432234063.2, -9.968698493).toPointF() - origin;
//    QPointF wTopRight     = QVector3DD(253880792.2, 432502876, -9.968698493).toPointF() - origin;
//    QPointF wBottomRight  = QVector3DD(253880792.2, 432234063.2, -9.968698493).toPointF() - origin;

//    qDebug() << wBottomLeft << wBottomRight << wTopLeft << wTopRight;
    float z = 0;
    float points[] = {
//        wBottomLeft.x(),-wBottomLeft.y(), z,
//        0, 0, z,
//        wBottomRight.x(),-wBottomRight.y(),  z,
//        wTopRight.x(), -wTopRight.y(),  z,
//        wBottomLeft.x(),-wBottomLeft.y(), z,
//        wTopRight.x(), -wTopRight.y(),  z,
//        wTopLeft.x(), -wTopLeft.y(), z
//        0.0f,1.0f, z,
//        1.0f,1.0f, z,
//        1.0f,0.0f, z,
//        0.0f,1.0f, z,
//        1.0f,0.0f, z,
//        0.0f,0.0f, z
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 1.0, 0.0,
        0.0, 1.0, 0.0
    };

    float colors[] = {
        1,0,0,
        0,0,1,
        1,0,0,
        1,0,0,
        1,0,1,
        1,0,0
    };

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, this->colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // texture
    createTexImage2D(view->getWidth(), view->getHeight(), GL_R32F, GL_RED, GL_FLOAT, &this->tex, (void*)view->getData().data());

//	this->rectangle(0.2,0.2,0.5,0.5);
}
void NcRenderingLayer::rectangle(float x1, float y1, float x2, float y2, int idx)
{

	float rec_points[] = {
		x1, y1, 0,
		x1, y2, 0,
		x1, y2, 0,
		x2, y2, 0,
		x2, y2, 0,
		x2, y1, 0,
		x2, y1, 0,
		x1, y1, 0,
		
	};
	float rec_color[] = {
		1,1,0,
		1,1,0,
		1,1,0,
		1,1,0,
		1,1,0,
		1,1,0,
		1,1,0,
		1,1,0
	};
	this->rectangle_flag = true;
    glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &rect_buffer[idx]);
	glGenBuffers(1, &rect_color[idx]);

	glBindBuffer(GL_ARRAY_BUFFER, this->rect_buffer[idx]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rec_points),rec_points, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->rect_color[idx]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rec_color), rec_color, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);



//	qDebug()<<"draw rect";
	
}	
void NcRenderingLayer::render(NcMapView *view)
{


	QMatrix4x4D mvp = view->cam()->getModelViewProjectionMatrix();
	if (this->rectangle_flag){

		this->rect_shader->bind();
		glBindVertexArray( this->vao );
		glEnableVertexAttribArray(0);
    	glUniformMatrix4dv(rect_shader->uniformLocation("mvp"),1, GL_FALSE, (mvp).constData());
		for (int idx=0; idx<3; idx ++){
			glBindBuffer(GL_ARRAY_BUFFER, rect_buffer[idx]);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, rect_color[idx]);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		    glUniform1i(this->rect_shader->uniformLocation("colorFlag"), idx);
			glDrawArrays(GL_LINES, 0, 8);
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
//    glUniform2d(shader->uniformLocation("size"), view->getWBounds().width(), view->getWBounds().height());
		this->rect_shader->release();
	}

     this->shader->bind();
//    qDebug() << view->cam()->getModelViewProjectionMatrix();

//    view->cam()->glTransform(WORLD_ZOOM_LEVEL, view->getWBounds().center());
//    view->cam()->glTransform(WORLD_ZOOM_LEVEL, view->getWBounds().center(), QPointF(view->getWBounds().width(), view->getWBounds().height()));
//    QMatrix4x4D proj = view->cam()->getProjectionMatrix();
//    QMatrix4x4D mv = view->cam()->getModelViewMatrix();
//	mvp.setToIdentity();
//    mv.setToIdentity();
//    mv.translate(-view->getOrigin().x(), -view->getOrigin().y(), 0);
//    mv.translate(1,0.5,0);
//    mv.scale(1000);
//    qDebug() << "......." << qSetRealNumberPrecision(10) << view->getWBounds().width() << view->getWBounds().height();
//    mv.scale(view->getWBounds().width(), view->getWBounds().height(), 1);
//    proj.setToIdentity();
//    proj.perspective(45, 1, 0, 1000);
//    proj.data()[14] = 0;

//    QMatrix4x4D t;
//    t.translate(view->getWBounds().center().x()/2, view->getWBounds().center().y()/2);
//    t.scale(view->getWBounds().width(), view->getWBounds().height());
//    t.translate(-view->getWBounds().center().x()/2, -view->getWBounds().center().y()/2);
//    mvp.setToIdentity();

//    mvp = proj * mv;// * t;
//    qDebug() << mvp;

//    mvp.setToIdentity();
//    qDebug() << mv;

//    QPointF wCenter  = view->cam()->geo2world(view->getGeoBounds().center());
//    QVector4D wCenter3 = QVector4D(wCenter.x(), wCenter.y(),0,1);
//    qDebug() << mvp * QVector4D(view->getWBounds().width(), view->getWBounds().height(),1,1) * QVector4D(0,0,0,1);

//    qDebug() << qSetRealNumberPrecision(10) << (mv.inverted()*proj.inverted()) * QVector4D(0,0,1,1);
//    qDebug() << qSetRealNumberPrecision(10) << (mv.inverted()*proj.inverted()) * QVector4D(0,1,1,1);
//    qDebug() << qSetRealNumberPrecision(10) << (mv.inverted()*proj.inverted()) * QVector4D(1,0,1,1);
//    qDebug() << qSetRealNumberPrecision(10) << (mv.inverted()*proj.inverted()) * QVector4D(1,1,1,1);

    glUniformMatrix4dv(shader->uniformLocation("mvp"),1, GL_FALSE, (mvp).constData());
//    glUniform2d(shader->uniformLocation("size"), view->getWBounds().width(), view->getWBounds().height());
    glUniform1i(shader->uniformLocation("tex"), 0);


    glActiveTexture(GL_TEXTURE0);
//working code below
    glBindTexture(GL_TEXTURE_2D, this->tex);

	glBindVertexArray( this->vao );
	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, this->colorbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);



    glBindTexture(GL_TEXTURE_2D, 0);

//    qDebug() << glGetError();

    this->shader->release();
	
}

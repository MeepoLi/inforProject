#ifndef CAMERA_INTERACTOR_HPP
#define CAMERA_INTERACTOR_HPP

#include <GL/glew.h>
#include <QPointF>
#include <QOpenGLShaderProgram>
#include "QVector3DD.hpp"

class SimpleCamera;
class Camera;
class QKeyEvent;
class QMouseEvent;
class NcQuickView;
class NcMapView;

class CameraInteractor
{
public:
    CameraInteractor(NcMapView *mv, SimpleCamera *cam);
    SimpleCamera *getCamera() const;
    QPointF getLastPos();
    void    setCamera(SimpleCamera *cam);
	void	setRectangle(std::vector<float> *rectangle[3]);
    virtual ~CameraInteractor();
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual bool isInteracting() const;

protected:
    NcMapView   * view;
    SimpleCamera   * baseCamera;
    int        action;
    QPointF    lastPos;
	QPointF	   mousePos;
    QPointF    screenPos;
	QPointF	   mouseClickPos;
	float      focusDistance;
	std::vector<float> *rect[3];
	int	rectangle_idx;
	bool rectangle_flag;
};

inline SimpleCamera * CameraInteractor::getCamera() const
{
    return this->baseCamera;
}

inline QPointF CameraInteractor::getLastPos()
{
    return lastPos;
}

#endif

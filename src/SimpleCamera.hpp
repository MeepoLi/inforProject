#ifndef SIMPLECAMERA_H
#define SIMPLECAMERA_H

#include <QPointF>
#include "QMatrix4x4D.hpp"
#define CAMERA_MATRIX(Name)                       \
public:                                           \
const QMatrix4x4D & get##Name##Matrix() const {  \
return this->mat##Name;                        \
}                                                \
protected:                                        \
QMatrix4x4D mat##Name; 

class SimpleCamera
{
public:
    SimpleCamera(const QRectF &nvp, int dpr, QPointF p);
    void setWorldCenter(const QPointF &center);
	void pan(const QPointF &anchor, const QPointF &screenDiff);
 	void updateMousePosition(const QPointF mousePosition);
 	void zoom(const QPointF &anchor, double diff);
    const QPointF screen2world(const QPointF &screen);
    const QMatrix4x4D& getModelViewProjectionMatrix();
    void updateViewport();
    void update();
    void setNormalizedViewport(const QRectF &nvp);
    void setViewport(int width, int height, int dpr);
 	const QRectF &getNativeViewport() const;
    QPointF getworldCoordinate(const QPointF mousePosition);
public:	
	CAMERA_MATRIX(Projection);
	CAMERA_MATRIX(ModelView);
	CAMERA_MATRIX(ModelViewProjection);
	CAMERA_MATRIX(MapToScreen);
	CAMERA_MATRIX(ScreenToMap);
	CAMERA_MATRIX(Normal);
protected:
 	QPointF worldCenter;
private:
//    QMatrix4x4D matProj;
//    QMatrix4x4D matModelView;
//    QMatrix4x4D matMapToScreen;
    int devicePixelRatio;
    float zoomLevel;
    QRectF screenViewport;
    QRectF normalizedViewport;
    QRectF nativeViewport;
    QPointF anchor;
    QPointF lookAt;
    QPointF pos;
	QPointF mousePos;
};

#endif // SIMPLECAMERA_H

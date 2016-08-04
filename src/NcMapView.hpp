#ifndef NCMAPVIEW_H
#define NCMAPVIEW_H


#include <GL/glew.h>
#include <QGLWidget>

#include "Camera.hpp"
#include "CameraInteractor.hpp"
#include "SimpleCamera.hpp"
#include "NcRenderingLayer.hpp"
#include <QQmlProperty>
class NcRenderingLayer;

class NcMapView : public QGLWidget
{
public:
    NcMapView(const QString &filename, const QRectF &vp, QWidget *parent=NULL);

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void updateView();
    void setupViewport(int width, int height);
    SimpleCamera *cam();
    const QPointF &getOrigin();
    const QRectF &getGeoBounds();
	const int getTimestamp();
    int getWidth();
    int getHeight();
    void updateData();
    const std::vector<float>& getData();
    const QRectF &getWBounds();
signals:

public slots:

private:
    CameraInteractor *interactor;
    SimpleCamera *cams;
    NcLoader *ncLoader;
    NcRenderingLayer *ncRenderingLayer;
    QRectF geoBounds;
    QRectF wBounds;
    QPointF origin;
    float zoomFactor;
    std::vector<float> data;
    int dataWidth;
    int dataHeight;
    int currentTimestep;
};

#endif // NCMAPVIEW_H

#ifndef NCVIEW_H
#define NCVIEW_H

#include <GL/glew.h>
#include <QGLWidget>
#include <QVector>
#include <QRectF>
#include <QQuickView>
#include <QGuiApplication>
#include <QQuickItem>

#include "NcMapView.hpp"
#include "NcLoader.hpp"
#include "NcRenderingLayer.hpp"
#include "Camera.hpp"
#include "SimpleCamera.hpp"
#include "CameraInteractor.hpp"

class NcRenderingLayer;

class NcQuickView : public QQuickView
{
    Q_OBJECT
public:
    NcQuickView(const QString &filename, const QRectF &vp, QQuickWindow *parent=NULL);
    virtual ~NcQuickView();
    static void ensureOpenGLFormat();
    void initializeGL();
    void resizeEvent(QResizeEvent *e);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    // qml signal calls
    Q_INVOKABLE void callShowHideChart(){
        emit(showHideChart());
    }

signals:
    void showHideChart();

public slots:
    void paintGL();

private:
    NcMapView* map;
};

#endif // NCVIEW_H

#include "NcQuickView.hpp"

#include <QKeyEvent>
#include <QQmlContext>

#include <algorithm>
#include <iostream>

void NcQuickView::ensureOpenGLFormat()
{
    QSurfaceFormat glf = QSurfaceFormat::defaultFormat();
    glf.setVersion(4,1);
    glf.setProfile(QSurfaceFormat::CoreProfile);
    glf.setSamples(4);
    glf.setSwapBehavior(QSurfaceFormat::SingleBuffer);
    glf.setRedBufferSize(8);
    glf.setGreenBufferSize(8);
    glf.setBlueBufferSize(8);
    glf.setDepthBufferSize(8);

    qDebug() << glf.version();
    QSurfaceFormat::setDefaultFormat(glf);
}

NcQuickView::NcQuickView(const QString &filename, const QRectF &vp, QQuickWindow* parent)
    : QQuickView(parent)
{
    // QuickView setup
    this->setResizeMode(QQuickView::SizeRootObjectToView);
    this->setClearBeforeRendering(false);

    // Call paintGL before render QML
    QObject::connect(this, SIGNAL( beforeRendering() ), SLOT( paintGL() ), Qt::DirectConnection);

    // window creation
    this->map = new NcMapView(filename,vp);
    // root object
    this->rootContext()->setContextProperty("ncQuickView", this);
}

NcQuickView::~NcQuickView()
{
    delete this->map;
}

void NcQuickView::initializeGL()
{
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

    this->map->initializeGL();
}

void NcQuickView::paintGL()
{

    static bool firstTime = true;
    if(firstTime) {
        this->initializeGL();
        firstTime = false;
    }

    this->map->paintGL();

    // avoids erros wrong OpenGL states on QML rendering
    this->resetOpenGLState();
}

void NcQuickView::resizeEvent(QResizeEvent *e)
{
    this->map->resize(e->size().width(), e->size().height());
    this->map->resizeGL(e->size().width(), e->size().height());

    QQuickView::resizeEvent(e);
    NcQuickView::update();
}


void NcQuickView::mousePressEvent(QMouseEvent *event)
{
    QQuickView::mousePressEvent(event);
    if (event->isAccepted()) return;

//    qWarning()<< event->pos().x();
//	qWarning()<< event->pos().y();
    this->map->mousePressEvent(event);
    NcQuickView::update();
}

void NcQuickView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QQuickView::mouseDoubleClickEvent(event);
    if (event->isAccepted()) return;

//    doubleClickPos = event->pos();
//    doubleClicked = true;
    NcQuickView::update();
}

void NcQuickView::mouseMoveEvent(QMouseEvent *event)
{
    QQuickView::mouseMoveEvent(event);
    if (event->isAccepted()) return;


    this->map->mouseMoveEvent(event);
    NcQuickView::update();
}

void NcQuickView::mouseReleaseEvent(QMouseEvent *event)
{
    QQuickView::mouseReleaseEvent(event);
    if (event->isAccepted()) return;

    this->map->mouseReleaseEvent(event);
    NcQuickView::update();
}


void NcQuickView::keyPressEvent(QKeyEvent *event)
{
    QQuickView::keyPressEvent(event);
    if (event->isAccepted()) return;
	QObject *obj = this->rootObject();
	QObject *timeLine = obj->findChild<QObject*>("ball");
	
    switch(event->key()){
         default:
            this->map->keyPressEvent(event);
    }

	int timeStamp = this->map->getTimestamp();
	if (timeLine)
		timeLine->setProperty("x",timeStamp * 10);	
	else qDebug()<<"didnt find QML component";
    NcQuickView::update();
}

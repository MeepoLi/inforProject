#include "NcMapView.hpp"

NcMapView::NcMapView(const QString &filename, const QRectF &vp, QWidget *parent)
    : QGLWidget(parent)
{
    this->ncLoader = new NcLoader();
    this->ncLoader->loadNCFile(filename);
    this->ncLoader->printInfo();

    this->ncRenderingLayer = new NcRenderingLayer();
    this->zoomFactor = 1.0f;
    this->cams = new SimpleCamera(vp, this->window()->devicePixelRatio(), QPointF(0,0));
//	this->cams->setWorldCenter(QPointF(0.5,0.5));
//    this->cams->loadFromFile("../../datasets/camera.txt");
//    this->cams->setCenter(QPointF(40.712508,-73.999128));
    this->interactor = new CameraInteractor(this, this->cams);
//    this->origin = this->cam()->geo2world(this->geoBounds.topLeft());

}

void NcMapView::initializeGL() {
    this->currentTimestep = 0;
    this->updateData();
}

void NcMapView::resizeGL(int width, int height) {
    this->setupViewport(width, height);
}

void NcMapView::keyPressEvent(QKeyEvent *event) {
    if (this->interactor)
        this->interactor->keyPressEvent(event);
    event->accept();
    int key = event->key();
    switch (key) {
    case Qt::Key_Left:
    {
        currentTimestep = std::max(0, currentTimestep-1);
        this->updateData();
        this->updateView();
    }
        break;
    case Qt::Key_Right:
    {
        currentTimestep++;
        this->updateData();
        this->updateView();
    }
        break;
    }
}

void NcMapView::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->setupViewport(this->width(), this->height());

    this->cam()->updateViewport();

    this->ncRenderingLayer->render(this);
}

void NcMapView::mousePressEvent(QMouseEvent *event)
{
    if (this->interactor)
        this->interactor->mousePressEvent(event);
}

void NcMapView::mouseReleaseEvent(QMouseEvent *event)
{
    if(this->interactor)
        this->interactor->mouseReleaseEvent(event);
}

void NcMapView::mouseMoveEvent(QMouseEvent *event)
{
    if(this->interactor)
        this->interactor->mouseMoveEvent(event);
}

void NcMapView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (this->interactor)
        this->interactor->mouseDoubleClickEvent(event);
}

void NcMapView::updateView()
{
    this->update();
}

void NcMapView::setupViewport(int width, int height)
{
    this->cam()->setViewport(width, height, this->window()->devicePixelRatio());
    QRectF vp = this->cam()->getNativeViewport();
    glViewport(vp.x(), vp.y(), vp.width(), vp.height());
}

SimpleCamera *NcMapView::cam()
{
    return this->cams;
}

const QPointF &NcMapView::getOrigin()
{
    return this->origin;
}

const QRectF& NcMapView::getGeoBounds()
{
    return this->geoBounds;
}

const QRectF& NcMapView::getWBounds()
{
    return this->wBounds;
}

int NcMapView::getWidth()
{
    return this->dataWidth;
}

int NcMapView::getHeight()
{
    return this->dataHeight;
}

void NcMapView::updateData()
{
    data = std::vector<float>();
    std::vector<float> latBounds = std::vector<float>();
    std::vector<float> lonBounds = std::vector<float>();

    int lats = this->ncLoader->getSize("latitude");
    int lons = this->ncLoader->getSize("longitude");
    this->ncLoader->getDataFromNC("discharge", {this->currentTimestep, 0, 0}, {1, lats, lons}, data);
    this->ncLoader->getDataFromNC("latitude", {0}, {lats}, latBounds);
    this->ncLoader->getDataFromNC("longitude", {0}, {lons}, lonBounds);

    // data
    this->dataWidth = lons;
    this->dataHeight = lats;
    auto dataMinmax = std::minmax_element(data.begin(), data.end());
    float min = *dataMinmax.first;
    float max = *dataMinmax.second;

//    max = 8460.657227;
    min = 0;
    for(int i=0; i<data.size(); ++i) {
        if(data[i] == -9999)
            data[i] = -1;
        else
            data[i] = (data[i] - min) / (max-min);
    }

    qDebug() << qSetRealNumberPrecision(10) << max << min;

    qDebug() << "Current timestep" << this->currentTimestep;

    // geo bounds
    auto latMinmax = std::minmax_element(latBounds.begin(), latBounds.end());
    auto lonMinmax = std::minmax_element(lonBounds.begin(), lonBounds.end());
    this->geoBounds.setCoords(*latMinmax.first, *lonMinmax.first, *latMinmax.second, *lonMinmax.second);
    qDebug() << "Geo Bounds:" << this->geoBounds.bottomLeft() << this->geoBounds.bottomRight() << this->geoBounds.topLeft() << this->geoBounds.topRight();

    // world bounds
//    QPointF wTopRight    = this->cam()->geo2world(this->geoBounds.topRight());
//    QPointF wBottomRight = this->cam()->geo2world(this->geoBounds.bottomRight());
//    QPointF wTopLeft     = this->cam()->geo2world(this->geoBounds.topLeft());
//    QPointF wBottomLeft  = this->cam()->geo2world(this->geoBounds.bottomLeft());
//    this->wBounds.setCoords(wBottomRight.x(), wTopLeft.y(), wTopLeft.x(), wBottomRight.y());
//    qDebug() << "World bounds:" << qSetRealNumberPrecision(10) << this->wBounds.bottomLeft() << this->wBounds.bottomRight() << this->wBounds.topLeft() << this->wBounds.topRight();

    this->ncRenderingLayer->init(this);
}

const std::vector<float>& NcMapView::getData()
{
    return this->data;
}

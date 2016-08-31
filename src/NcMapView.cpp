#include "NcMapView.hpp"

NcMapView::NcMapView(const QString &filename, const QRectF &vp, QWidget *parent)
    : QGLWidget(parent)
{
    this->ncLoader = new NcLoader();
    this->ncLoader->loadNCFile(filename);
    this->ncLoader->printInfo();
	this->currentTimestep = 0;
    this->ncRenderingLayer = new NcRenderingLayer();
    this->zoomFactor = 1.0f;
    this->cams = new SimpleCamera(vp, this->window()->devicePixelRatio(), QPointF(0,0));
//	this->cams->setWorldCenter(QPointF(0.5,0.5));
//   this->cams->loadFromFile("../../datasets/camera.txt");
//   this->cams->setCenter(QPointF(40.712508,-73.999128));
    this->interactor = new CameraInteractor(this, this->cams);
	this->rectangle[0] = new std::vector<float>(4,0);
	this->rectangle[1] = new std::vector<float>(4,0);
	this->rectangle[2] = new std::vector<float>(4,0);

	this->interactor->setRectangle(this->rectangle);
//   this->origin = this->cam()->geo2world(this->geoBounds.topLeft());
	this->setBaseSize(800,600);
}

void NcMapView::initializeGL() {
//	qDebug()<<"initialGL------MAP VIEW";
   // this->updateData();

}

void NcMapView::resizeGL(int width, int height) {
  this->setupViewport(width, height);
}
void NcMapView::keyReleaseEvent(QKeyEvent *event) {
	if (this->interactor)
        this->interactor->keyReleaseEvent(event); 
}

void NcMapView::keyPressEvent(QKeyEvent *event) {
    if (this->interactor)
        this->interactor->keyPressEvent(event);
    event->accept();
    int key = event->key();	
	switch (key) {
	    case Qt::Key_Left:
    	    currentTimestep = std::max(0, currentTimestep-1);
       		this->updateData();
       	 	this->updateView();
        	break;
   		case Qt::Key_Right:
        	currentTimestep++;
        	this->updateData();
        	this->updateView();
        	break;
//		case Qt::Key_A:
//			qDebug()<<(*this->rectangle)[0]<<","<<(*this->rectangle)[1]<<","<<(*this->rectangle)[2]<<","<<(*this->rectangle)[3];
//			break;
    }
}

void NcMapView::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->setupViewport(this->width(), this->height());

    this->cam()->updateViewport();

    this->ncRenderingLayer->render(this);
	for (int idx = 0; idx < 3; idx ++)
		this->ncRenderingLayer->rectangle((*this->rectangle[idx])[0],(*this->rectangle[idx])[1],(*this->rectangle[idx])[2],(*this->rectangle[idx])[3], idx);
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
void NcMapView::setTimeStamp(int t)
{
	if (t!=this->currentTimestep)
	{
		this->currentTimestep = t;
		this->updateData();

		this->updateView();
	}
}
int NcMapView::getTimestamp()
{
	return this->currentTimestep;
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
int NcMapView::getTimelength()
{
	return this->ncLoader->getSize("time");
}
std::vector< std::vector<float>> NcMapView::getAreaSum()
{
	int lats = this->ncLoader->getSize("latitude");
	int lons = this->ncLoader->getSize("longitude");
	
	if (this->mapData.size() == 0)
	{
		int timeSize = this->ncLoader->getSize("time");
		std::vector<float> areaSum;
			
		std::vector<float> d = std::vector<float>();
		for (int t = 0; t<timeSize;t++){
			qDebug()<<"Get data of time: "<<t;
			this->ncLoader->getDataFromNC("discharge", {t, 0, 0}, {1, lats, lons}, d);
			this->mapData.push_back(d);
		}	
	}	
	std::vector< std::vector<float>> retValue;
	for (int idx = 0; idx < 3; idx++){
		std::vector<float> areaSum;
		int lati1,lati2,long1,long2;
		if ((*this->rectangle[idx])[0]< (*this->rectangle[idx])[2])
		{
			long1 = ( (*this->rectangle[idx])[0] > 1 ? 1 :  (*this->rectangle[idx])[0] )  * lons;
			long2 = ( (*this->rectangle[idx])[2] > 1 ? 1 :  (*this->rectangle[idx])[2] )  * lons;
		} 
		else
		{
			long2 = ( (*this->rectangle[idx])[0] > 1 ? 1 :  (*this->rectangle[idx])[0] )  * lons;
			long1 = ( (*this->rectangle[idx])[2] > 1 ? 1 :  (*this->rectangle[idx])[2] )  * lons;
		}
// left top = 0,0
 /*		if ((*this->rectangle[idx])[0]< (*this->rectangle[idx])[2])
		{
			lati2 = lats -  ( (*this->rectangle[idx])[0] > 1 ? 1 :  (*this->rectangle[idx])[0] )  * lats;
			lati1 = lats - ( (*this->rectangle[idx])[2] > 1 ? 1 :  (*this->rectangle[idx])[2] )  * lats;
		} 
		else
		{
			lati1 = lats - ( (*this->rectangle[idx])[0] > 1 ? 1 :  (*this->rectangle[idx])[0] )  * lats;
			lati2 = lats - ( (*this->rectangle[idx])[2] > 1 ? 1 :  (*this->rectangle[idx])[2] )  * lats;
		} */
	
		if ((*this->rectangle[idx])[1]< (*this->rectangle[idx])[3])
		{
			lati1 = ( (*this->rectangle[idx])[1] > 1 ? 1 :  (*this->rectangle[idx])[1] )  * lats;
			lati2 = ( (*this->rectangle[idx])[3] > 1 ? 1 :  (*this->rectangle[idx])[3] )  * lats;
		} 
		else
		{
			lati2 = ( (*this->rectangle[idx])[1] > 1 ? 1 :  (*this->rectangle[idx])[1] )  * lats;
			lati1 = ( (*this->rectangle[idx])[3] > 1 ? 1 :  (*this->rectangle[idx])[3] )  * lats;
		}
	qDebug()<<"(lat = "<<lati1<<" lon = "<<long1<<")-> (lat = "<<lati2<<" lon ="<<long2<<")";
			for (int t = 0; t<this->mapData.size();t++){
				float sum = 0;
				int start = lati1 * lons + long1;
				int end   = lati2 * lons + long2;
				start = start < 0 ? 0 : start;
				start = start > this->mapData[t].size() - 1 ? this->mapData[t].size()-1 : start;

				end = end < 0 ? 0 : end;
				end = end > this->mapData[t].size() - 1 ? this->mapData[t].size()-1 : end;
		//		qDebug()<<start<<"-->"<<end;

				for (int i = start; i <= end ; i ++ )
				if (this->mapData[t][i]!=-9999)
					sum+=this->mapData[t][i];
				areaSum.push_back(sum);
		}
	retValue.push_back(areaSum);
	}
	return retValue;
//	return this->areaSum;
}
void NcMapView::updateData()
{

    data = std::vector<float>();
    std::vector<float> latBounds = std::vector<float>();
    std::vector<float> lonBounds = std::vector<float>();

	std::vector<float> test = std::vector<float>();


    int lats = this->ncLoader->getSize("latitude");
    int lons = this->ncLoader->getSize("longitude");
//	this->ncLoader->getDataFromNC("discharge", {this->currentTimestep, 0, 0}, {1, lats, lons}, data);
	this->ncLoader->getDataFromNC("discharge", {this->currentTimestep, 0, 0}, {1, lats, lons}, data);
 	qDebug()<<"lats ="<< lats<<", lons="<<lons;
    
    this->ncLoader->getDataFromNC("latitude", {0}, {lats}, latBounds);
    this->ncLoader->getDataFromNC("longitude", {0}, {lons}, lonBounds);

	this->ncLoader->getDataFromNC("average",{this->currentTimestep}, {5}, test);
	qDebug()<<"average ="<<test[0];
    // data
    this->dataWidth = lons;
    this->dataHeight = lats;
    auto dataMinmax = std::minmax_element(data.begin(), data.end());
    float min = *dataMinmax.first;
    float max = *dataMinmax.second;
	qDebug()<< data.size();
//    max = 8460.657227;
    min = 0;
    for(int i=0; i<data.size(); ++i) {
        if(data[i] == -9999)
            data[i] = -1;
        else
            data[i] = (data[i] - min) / (max-min);

//		areaSum+=data[i];
//	qDebug()<<"i = "<<i<<" value = "<<data[i];
	
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

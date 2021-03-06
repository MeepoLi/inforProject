#include "NcQuickView.hpp"
#include "NcGui.hpp"
#include <QKeyEvent>
#include <QQmlContext>

#include <algorithm>
#include <iostream>
//#include <QtCharts>

void NcQuickView::ensureOpenGLFormat()
{
    
	//QGLFormat glFormat;
 	//glFormat.setVersion(3,2);
	//glFormat.setProfile(QGLFormat::CoreProfile);
	//QGLFormat::setDefaultFormat(glFormat); 

    QSurfaceFormat glf = QSurfaceFormat::defaultFormat();
    glf.setVersion(4,1);
    glf.setProfile(QSurfaceFormat::CoreProfile);
//	glf.setOption(QSurfaceFormat::DebugContext);	   
    glf.setSamples(4);
    glf.setSwapBehavior(QSurfaceFormat::SingleBuffer);
    glf.setRedBufferSize(8);
    glf.setGreenBufferSize(8);
    glf.setBlueBufferSize(8);
    glf.setDepthBufferSize(8);

 //   qDebug() <<"glversion = "<< glf.version();
    QSurfaceFormat::setDefaultFormat(glf);
//	QGLFormat::setDefaultFormat(glFormat);

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
//	qDebug()<<"no load";
    this->map = new NcMapView(filename,vp);

    // root object
    this->rootContext()->setContextProperty("ncQuickView", this);


}
void NcQuickView::initializeQML()
{
	QObject *obj =  this->rootObject();
	if (obj) {
	
		obj->setProperty("width", this->width());
		obj->setProperty("height", this->height());
		qDebug()<< this->width()<<"*"<<this->height();

		QObject *timeBar = obj->findChild<QObject*>("timeBar");
		int timeLength = this->map->getTimelength();;
		qDebug()<< "timeL = " << timeLength;
		if (timeBar)
			timeBar->setProperty("n",timeLength);	
		else 
			qDebug()<<"Time Bar Not Find";
	}
	else qDebug()<<"Root Not Find";

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


    this->map->updateData();//for testing
//this->map->initializeGL();

//this->map->resizeGL(this->width(),this->height());    
//NcQuickView::update();

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
    qDebug()<<"Size = "<<e->size();
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
//    qWarning()<< event->pos().y();
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
	QObject *obj = this->rootObject();
	QObject *timeLine = obj->findChild<QObject*>("slideBar");
	int timeStamp = QQmlProperty::read(timeLine, "timeStamp").toInt();

//    qDebug()<<"mouseMoving"<<timeStamp;
	this->map->setTimeStamp(timeStamp);
	
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

//void NcQuickView::setChartView(QtCharts::QChartView chartview)
//{
//	this->qchartview = chartview;
//}
void NcQuickView::keyReleaseEvent(QKeyEvent *event)
{
	QQuickView::keyReleaseEvent(event);
	if (event->isAccepted()) return;

	this->map->keyReleaseEvent(event);
	NcQuickView::update();

}

void NcQuickView::keyPressEvent(QKeyEvent *event)
{
    QQuickView::keyPressEvent(event);
    if (event->isAccepted()) return;
	QObject *obj = this->rootObject();
	QObject *timeLine = obj->findChild<QObject*>("slideBar");
	
    switch(event->key()){
		case Qt::Key_C:
			{
				break;
			}
		case Qt::Key_M:
			{	
				std::vector< std::vector<float> > areaSum = this->map->getAreaSum();
				QObject *qchartView = obj->findChild<QObject*>("chart");
			
				QMetaObject::invokeMethod(qchartView, "clearLine");//, Q_RETURN_ARG(QVariant, retValue), Q_ARG(QVariant, i), Q_ARG(QVariant,areaSum[i]));I
				qchartView->setProperty("maxX",(int) areaSum[0].size());

				float maxY = INT_MIN, minY = INT_MAX;
				for (int idx = 0; idx < 3; idx ++)
					for (int i=0;i<areaSum[idx].size();i++)
						{
							maxY = maxY<areaSum[idx][i]?areaSum[idx][i]:maxY;
							minY = minY>areaSum[idx][i]?areaSum[idx][i]:minY;
						}
				qDebug()<<minY<<" "<<maxY;
				qchartView->setProperty("minY",(int)minY*0.9);
				qchartView->setProperty("maxY",(int)maxY*1.1);
				
				for (int idx = 0 ; idx < 3 ; idx++)
				{			
	
					QVariant retValue;	

					
					for (int i=0;i<areaSum[idx].size();i++)
					{
						if (idx == 0) QMetaObject::invokeMethod(qchartView, "createXYPoint1", Q_RETURN_ARG(QVariant, retValue), Q_ARG(QVariant, i), Q_ARG(QVariant,areaSum[idx][i]));
						if (idx == 1) QMetaObject::invokeMethod(qchartView, "createXYPoint2", Q_RETURN_ARG(QVariant, retValue), Q_ARG(QVariant, i), Q_ARG(QVariant,areaSum[idx][i]));
						if (idx == 2) QMetaObject::invokeMethod(qchartView, "createXYPoint3", Q_RETURN_ARG(QVariant, retValue), Q_ARG(QVariant, i), Q_ARG(QVariant,areaSum[idx][i]));	
					}
					qDebug()<<retValue.toString();
				}
				
					qDebug()<<"qchart is fixed";
				
				break;
			}
        default:
			this->map->keyPressEvent(event);
    }
/*
	int timeStamp = this->map->getTimestamp();
	if (timeLine)
	//	timeLine->setProperty("x",timeStamp * 10);	
	//	timeLine->setProperty("timeStamp",timeStamp);	
	
	else qDebug()<<"didnt find QML component";
  */
    NcQuickView::update();
}

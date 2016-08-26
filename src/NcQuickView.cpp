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
void NcQuickView::keyPressEvent(QKeyEvent *event)
{
    QQuickView::keyPressEvent(event);
    if (event->isAccepted()) return;
	QObject *obj = this->rootObject();
	QObject *timeLine = obj->findChild<QObject*>("slideBar");
	
    switch(event->key()){
		case  Qt::Key_M:
			{	
			//	std::vector<float> areaSum = this->map->getAreaSum();
				//QtCharts::QChartView  *qchartView = obj->findChild<QtCharts::QChartView *>("chart");
				QObject *qchartobj = obj->findChild<QObject*>("chart");
				//QtCharts::QChartView *qchartView = qobject_cast<QtCharts::QChartView*>(qchartobj);
				
				QtCharts::QChartView *qchartview;
				//qchartview = NcGui::getChartView();
					if (!qchartview) {
					qDebug()<<"qchart not found";
					return;
				}
							
				QtCharts::QChart *chart = new QtCharts::QChart();
				chart->createDefaultAxes();
				QtCharts::QValueAxis *axis = new QtCharts::QValueAxis;
				axis->setTickCount(10);
				chart->addAxis(axis, Qt::AlignBottom);
				chart->setTitle("suc");
				QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
				qchartobj = chartView;

//				QtCharts::QChart *qchart = new QtCharts::QChart();

				
		//		QtCharts::QChartView *chartview = new QtCharts::QChartView(qchart);
	//			chartview->setParent(obj);
				
//				QtCharts::QLineSeries* series = new QtCharts::QLineSeries();
				//for (int i = 0; i < areaSum.size(); i++)
				//	series->append(i, areaSum[i]);
//				qchart->chart()->createDefaultAxes();
//			    qchart->chart()->addSeries(series);
//				if (!lineseries) {
//					qDebug()<<"line not found";
//					return;
//				}
			
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

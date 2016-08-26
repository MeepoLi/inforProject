#include <QDebug>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QString>

#include <map>
#include <netcdf.h>

#include "basegui.hpp"
#include "NcGui.hpp"
#include "NcRenderingLayer.hpp"
#include "QtCharts"
int main(int argc, char *argv[])
{
    // Application creation.
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
//    engine.addImportPath(QStringLiteral("../src/qml/jbQuick/Charts"));
//    engine.addImportPath(QString("qrc:/qml"));
/*	QtCharts::QChart *chart = new QtCharts::QChart();
	chart->createDefaultAxes();

	chart->setTitle("suc");	
	QtCharts::QLineSeries *series = new QtCharts:: QLineSeries();
	series->append(0,6);
	series->append(2,4);
	series->append(3,8);
	chart->addSeries(series);

*/
    NcQuickView::ensureOpenGLFormat();

    NcGui *gui = new NcGui();
    gui->run();
    gui->connectSlots();

//	QtCharts::QChartView *chartView = gui->getChartView();
//	QMainWindow wd;
//	wd.setCentralWidget(chartView);
//	wd.show();


    app.exec();

    return 0;
}

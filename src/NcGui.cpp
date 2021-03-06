#include "NcGui.hpp"
#include "GLUT/glut.h"
NcGui::NcGui(): fullscreen(false)
{


    qputenv("QSG_RENDER_LOOP", "windows");

    this->viewport[0] = this->viewport[1] = 0;
    this->viewport[2] = this->viewport[3] = 1;

    // window creation
    QString filename = "../../datasets/Discharge_NCEP_Pristine_dTS2004.nc";
    this->view = new NcQuickView(filename, QRectF(viewport[0], viewport[1], viewport[2]-viewport[0], viewport[3]-viewport[1]));
//    this->view->setGeometry(200,200,800,600);
	this->view->setGeometry(0,0,800,800);
	
 // chart dialog
 //   this->dlg = new ChartView();
 //   this->dlg->setGeometry(250,250,640,480);
}


NcGui::~NcGui()
{
    delete this->view;
//    delete this->dlg;
}

void NcGui::run()
{
    // main window
    this->view->setSource(QUrl("qrc:/qml/Main.qml"));
//	this->view->setChartView(this->chartView);
   // chart dialog
    //this->dlg->load(QUrl("qrc:/html/index.html"));
	// Set fullscreen
    if (fullscreen)
        this->view->showFullScreen();
    else{
        this->view->show();
    }
    // Additional setup
    this->view->raise();
	this->view->initializeQML();
}

void NcGui::connectSlots()
{
    // chart toggle
 //   QObject::connect(this->view, SIGNAL(showHideChart()), this->dlg, SLOT(showHideToggle()));
}

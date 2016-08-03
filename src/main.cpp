#include <QDebug>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QString>

#include <map>
#include <netcdf.h>

#include "basegui.hpp"
#include "NcGui.hpp"
#include "NcRenderingLayer.hpp"

int main(int argc, char *argv[])
{
    // Application creation.
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
//    engine.addImportPath(QStringLiteral("../src/qml/jbQuick/Charts"));
//    engine.addImportPath(QString("qrc:/qml"));

    NcQuickView::ensureOpenGLFormat();

    NcGui *gui = new NcGui();
    gui->run();
    gui->connectSlots();

    app.exec();

    return 0;
}

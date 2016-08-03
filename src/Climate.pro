TEMPLATE = app

QT += qml quick widgets opengl core webkitwidgets quickwidgets

TARGET = Climate
CONFIG += console
CONFIG -= app_bundle

SOURCES += main.cpp \
    NcRenderingLayer.cpp \
    NcLoader.cpp \
    QMatrix4x4D.cpp \
    QVector3DD.cpp \
    Camera.cpp \
    CameraInteractor.cpp \
    OpenGLFuncs.cpp \
    SimpleCamera.cpp \
    NcQuickView.cpp \
    NcMapView.cpp \
    NcGui.cpp \
    ChartView.cpp


# Unix configuration
unix:!macx{
    QMAKE_CXXFLAGS += -std=c++0x
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_LFLAGS   += -fopenmp

    INCLUDEPATH += -I ./
    INCLUDEPATH += -I /usr/local/include/

    LIBS += -L/usr/local/lib/ -lGLEW -lnetcdf_c++4
}
# OsX configuration
macx{
    QMAKE_CXXFLAGS += -I/usr/local/opt/qt5/include -stdlib=libc++ -std=c++11 -Wno-c++11-narrowing
    QMAKE_LFLAGS   += -I/usr/local/opt/qt5/include -stdlib=libc++

    INCLUDEPATH += -I ./
    INCLUDEPATH += -I /usr/local/include/

    LIBS += -L/usr/local/lib/ -lGLEW -lnetcdf_c++4
}

QML_IMPORT_PATH += qml

HEADERS += \
    NcRenderingLayer.hpp \
    NcLoader.hpp \
    QMatrix4x4D.hpp \
    QVector3DD.hpp \
    Camera.hpp \
    BaseGui.hpp \
    CameraInteractor.hpp \
    BaseGui.hpp \
    OpenGLFuncs.hpp \
    SimpleCamera.hpp \
    NcMapView.hpp \
    NcQuickView.hpp \
    NcGui.hpp \
    ChartView.hpp \
    basegui.hpp

include(deployment.pri)

RESOURCES += \
    NcView.qrc \
    NcGui.qrc

DISTFILES += \
    deployment.pri \
    html/index.html

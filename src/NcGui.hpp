#ifndef NCVIEWGUI_H
#define NCVIEWGUI_H

#include "basegui.hpp"
#include "NcQuickView.hpp"
//#include "ChartView.hpp"

class NcGui : public BaseGui
{
public:
    NcGui();
    ~NcGui();

    void run();
    void connectSlots();
private:
    // main window
    NcQuickView *view;
    // chart window
//    ChartView* dlg;

    // parameters
    bool fullscreen;
    float viewport[4];
    QString filename;

};

#endif // NCVIEWGUI_H

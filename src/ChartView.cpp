#include "ChartView.hpp"

ChartView::ChartView():
    QWebView()
{
    // setup frame
    initFrame();
}

ChartView::~ChartView()
{
}

void ChartView::initFrame()
{
    // Enable console
    this->frame = this->page()->mainFrame();
    // Make the d3 interface object availlable on javascript
    this->frame->addToJavaScriptWindowObject("Chart",this);

    // Scroll
    this->frame->setScrollBarPolicy(Qt::Vertical,Qt::ScrollBarAlwaysOff);
    this->frame->setScrollBarPolicy(Qt::Horizontal,Qt::ScrollBarAlwaysOff);

    // Enables console on right click
    QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

    // Rezize the window
    this->resize(660,535);
    // hide close button
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}

void ChartView::showHideToggle()
{
    if(this->isVisible())
        this->hide();
    else
        this->show();
}


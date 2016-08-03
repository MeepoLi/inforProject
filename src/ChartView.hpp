#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtWebKitWidgets>

class ChartView : public QWebView
{
    Q_OBJECT

public:
    ChartView();
    ~ChartView();

    // initializes the frame
    void initFrame();

public slots:
    void showHideToggle();

protected:
    QWebFrame *frame;
};

#endif // CHARTVIEW_H

#ifndef CUSTOMPLOTWINDOW_H
#define CUSTOMPLOTWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h"

namespace Ui {
class CustomPlotWindow;
}

class CustomPlotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CustomPlotWindow(QWidget *parent = 0);
    ~CustomPlotWindow();

    void setupDemo(int demoIndex);
    void setupQuadraticDemo(QCustomPlot *customPlot);
    void setupSimpleDemo(QCustomPlot *customPlot);
    void setupSincScatterDemo(QCustomPlot *customPlot);
    void setupScatterStyleDemo(QCustomPlot *customPlot);
    void setupLineStyleDemo(QCustomPlot *customPlot);
    void setupScatterPixmapDemo(QCustomPlot *customPlot);
    void setupDateDemo(QCustomPlot *customPlot);
    void setupTextureBrushDemo(QCustomPlot *customPlot);
    void setupMultiAxisDemo(QCustomPlot *customPlot);
    void setupLogarithmicDemo(QCustomPlot *customPlot);
    void setupRealtimeDataDemo(QCustomPlot *customPlot);
    void setupParametricCurveDemo(QCustomPlot *customPlot);
    void setupBarChartDemo(QCustomPlot *customPlot);
    void setupStatisticalDemo(QCustomPlot *customPlot);
    void setupSimpleItemDemo(QCustomPlot *customPlot);
    void setupItemDemo(QCustomPlot *customPlot);
    void setupStyledDemo(QCustomPlot *customPlot);
    void setupAdvancedAxesDemo(QCustomPlot *customPlot);
    void setupColorMapDemo(QCustomPlot *customPlot);
    void setupFinancialDemo(QCustomPlot *customPlot);

    void setupPlayground(QCustomPlot *customPlot);

private slots:
    void realtimeDataSlot();
    void bracketDataSlot();
    void screenShot();
    void allScreenShots();

private:
    Ui::CustomPlotWindow *ui;
    QString demoName;
    QTimer dataTimer;
    QCPItemTracer *itemDemoPhaseTracer;
    int currentDemoIndex;

};

#endif // CUSTOMPLOTWINDOW_H

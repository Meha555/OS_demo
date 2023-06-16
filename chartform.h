#ifndef CHARTFORM_H
#define CHARTFORM_H

#include <QWidget>
#include <QStringList>
#include <QTimer>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QtCharts>
#include "analysewindow.h"
#include "betterchartview.h"

#include "statgatherer.h"

#define MAX_X 1000
#define MAX_Y 1000

QT_CHARTS_USE_NAMESPACE

struct ChartParam; // 添加前向声明，否则报错'ChartParam' does not name a type;

namespace Ui {
class ChartForm;
}

class ChartForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChartForm(const ChartParam &param,QWidget *parent = nullptr);
    ~ChartForm();

public slots:
    void initLineChart();
    void initPieChart();
    void initBarChart();
    void initPercentBar();
    void drawLineChart();
    void drawBarChart();
    void drawPieChart();
    void drawPercentBar();

private:
    Ui::ChartForm *ui;

    QTimer *timeRefresh;
    QString chartName;
    QChart* chart;
    QLineSeries* lineSeries;

    StatGatherer* data_source;

};

#endif // CHARTFORM_H

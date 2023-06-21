#ifndef CHARTFORM_H
#define CHARTFORM_H

#include <QWidget>
#include <QStringList>
#include <QTimer>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QtCharts>
#include <QList>
#include <QStringList>
#include <algorithm>
#include "analysewindow.h"
#include "betterchartview.h"
//#include "utils.hpp"
#include "statgatherer.h"
//#include "chartstat.h"
#include "chartdatachangedtrend.h"
#include "chartdatadistribution.h"
#include "chartthreadstatetrend.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class ChartForm;
}

class ChartDataChangedTrend;
class ChartDataDistribution;
class ChartThreadStateTrend;

class AnalyseWindow;
class MainWindow;

class ChartForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChartForm(QWidget *parent = nullptr);
    explicit ChartForm(const ChartParam &param, QWidget *parent = nullptr);
    ~ChartForm();

    void initialize(QWidget* parent);
    ChartParam getParam() const;
    void setParam(const ChartParam &value);
    void drawFromJson(QString& str);
    void setGrid(const int x, const int y);

public slots:
    void initLineChart(int n, const QStringList& names, const QString& x_name, const QString& y_name);
    void initPieChart(int n, const QStringList& names);
    void drawChart_type1();
    void drawChart_type2();
    void drawChart_type3();

private:
    Ui::ChartForm *ui;

    ChartParam param;
    QTimer *timerRefresh;
    QString chartName;
    QChart* chart;
    QList<QLineSeries*> lineSeries;
    QPieSeries* pieSeries;
    qint64 count = 0;
    int max_x;
    int max_y;
    int interval = 100;

public:
    ChartDataChangedTrend* draft1;
    ChartDataDistribution* draft2;
    ChartThreadStateTrend* draft3;
};

#endif // CHARTFORM_H

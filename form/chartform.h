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
#include "analysewindow.h"
#include "betterchartview.h"
#include "statgatherer.h"
#include "chartdatachangedtrend.h"
#include "chartdatadistribution.h"
#include "chartthreadstatetrend.h"
#include "chartstat.h"

#define MAX_X 1000
#define MAX_Y 1000

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class ChartForm;
}

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

public slots:
    void initLineChart(int n, const QStringList& names, const QString& x_name, const QString& y_name);
    void initPieChart(int n, const QStringList& names);
    void drawChart_type1();
    void drawChart_type2();
    void drawChart_type3();

private:
    Ui::ChartForm *ui;

    ChartParam param;
    QTimer *timeRefresh;
    QString chartName;
    QChart* chart;
    QList<QLineSeries*> lineSeries;
    QPieSeries* pieSeries;

public:
    ChartDataChangedTrend* draft1;
    ChartDataDistribution* draft2;
    ChartThreadStateTrend* draft3;
};

#endif // CHARTFORM_H

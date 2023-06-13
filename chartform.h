#ifndef CHARTFORM_H
#define CHARTFORM_H

#include <QWidget>
#include <QString>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QtCharts>
#include "analysewindow.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class ChartForm;
}

class ChartForm : public QWidget
{
    Q_OBJECT

public:
//    explicit ChartForm(QWidget *parent = nullptr);
    explicit ChartForm(const QString &type,QWidget *parent = nullptr);
    ~ChartForm();

    QString getType() const;
    void setType(const QString &type);

public slots:
    void iniPieChart();
    void iniBarChart();
    void iniPercentBar();
    void drawBarChart();
    void drawPieChart();
    void drawPercentBar();

private:
    Ui::ChartForm *ui;
    QString type;
//    QChartView* chartView;
    QChart* chart;
};

#endif // CHARTFORM_H

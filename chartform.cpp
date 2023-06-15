#include "chartform.h"
#include "ui_chartform.h"

extern QStringList profiles;

ChartForm::ChartForm(const ChartParam &param, QWidget* parent)
    : QWidget(parent), ui(new Ui::ChartForm) {
    ui->setupUi(this);
    timeRefresh = new QTimer(this);
    timeRefresh->start(100);
    chartName = param.buf + "-" + profiles[param.type];
    qDebug()<<chartName;
    this->setWindowTitle(chartName);
    connect(dynamic_cast<AnalyseWindow*>(parent)->comboxStyle,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,[this](int i){
        ui->chartView->chart()->setTheme(QChart::ChartTheme(i));
    });
    switch (param.type) {
    case DATA_CHANGE_TREND:{//Buffer数据量变化趋势【曲线图+柱状/饼图】
        initLineChart();
        connect(timeRefresh,&QTimer::timeout,this,&ChartForm::drawLineChart);

//        initPieChart();
//        connect(timeRefresh,&QTimer::timeout,this,&ChartForm::drawPieChart);

//        initBarChart();
//        connect(timeRefresh,&QTimer::timeout,this,&ChartForm::drawBarChart);
    }
    case DATA_DISTRIBUTION:{//Buffer数据量分布【柱状/饼图】
        initPieChart();
        connect(timeRefresh,&QTimer::timeout,this,&ChartForm::drawPieChart);

//        initBarChart();
//        connect(timeRefresh,&QTimer::timeout,this,&ChartForm::drawBarChart);
    }
    case PUT_OUT_TREND:{//取出/放入数据变化趋势【曲线图】
        initLineChart();
        connect(timeRefresh,&QTimer::timeout,this,&ChartForm::drawLineChart);
    }
    case THREAD_STATE_TREND:{//线程状态变化趋势【曲线图】
        initLineChart();
        connect(timeRefresh,&QTimer::timeout,this,&ChartForm::drawLineChart);
    }
    }
}

ChartForm::~ChartForm() {
    delete ui;
}

// 初始化柱状图
void ChartForm::initBarChart() {
    QChart* chart = new QChart();  // 创建chart
    chart->setTitle("Barchart演示");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    ui->chartView->setChart(chart);  // 为ChartView设置chart
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

// 绘制柱状图
void ChartForm::drawBarChart() {
    //     QChart *chart =ui->chartViewBar->chart(); //获取ChartView关联的chart
    //     chart->removeAllSeries(); //删除所有序列

    //    chart->removeAxis(chart->axisX()); //删除坐标轴
    //    chart->removeAxis(chart->axisY()); //删除坐标轴

    ////创建三个QBarSet数据集,从数据模型的表头获取Name
    //    QBarSet *setMath = new QBarSet(theModel->horizontalHeaderItem(colNoMath)->text());
    //    QBarSet *setChinese = new QBarSet(theModel->horizontalHeaderItem(colNoChinese)->text());
    //    QBarSet *setEnglish= new QBarSet(theModel->horizontalHeaderItem(colNoEnglish)->text());

    //    QLineSeries *Line = new QLineSeries(); //创建一个QLineSeries序列用于显示平均分
    //    Line->setName(theModel->horizontalHeaderItem(colNoAverage)->text());
    //    QPen    pen;
    //    pen.setColor(Qt::red);
    //    pen.setWidth(2);
    //    Line->setPen(pen);
    ////    lineseries->setPointLabelsVisible(true);
    ////    lineseries->setPointLabelsFormat("@yPoint");

    //    for(int i=0;i<theModel->rowCount();i++)
    //    {//从数据模型获取数据
    //        setMath->append(theModel->item(i,colNoMath)->text().toInt()); //数学
    //        setChinese->append(theModel->item(i,colNoChinese)->text().toInt()); //语文
    //        setEnglish->append(theModel->item(i,colNoEnglish)->text().toInt()); //英语
    //        Line->append(QPointF(i,theModel->item(i,colNoAverage)->text().toFloat()));  //平均分
    //    }

    ////创建一个柱状图序列 QBarSeries, 并添加三个数据集
    //    QBarSeries *series = new QBarSeries();
    //    series->append(setMath);
    //    series->append(setChinese);
    //    series->append(setEnglish);
    ////    series->setLabelsVisible(true);
    ////    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);// LabelsCenter
    ////    series->setLabelsPosition(QAbstractBarSeries::LabelsCenter);// LabelsCenter

    ////    lineseries->setPointLabelsVisible(true);
    ////    lineseries->setPointLabelsFormat("@yPoint");

    //    chart->addSeries(series); //添加柱状图序列
    //    chart->addSeries(Line); //添加折线图序列

    ////用于横坐标在字符串列表,即学生姓名
    //    QStringList categories;
    //    for (int i=0;i<theModel->rowCount();i++)
    //        categories <<theModel->item(i,colNoName)->text();

    ////用于柱状图的坐标轴
    //    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    //    axisX->append(categories); //添加横坐标文字列表
    ////    chart->createDefaultAxes();
    //    chart->setAxisX(axisX, series); //设置横坐标
    //    chart->setAxisX(axisX, Line);//设置横坐标
    //    axisX->setRange(categories.at(0), categories.at(categories.count()-1)); //这只坐标轴范围

    ////数值型坐标作为纵轴
    //    QValueAxis *axisY = new QValueAxis;
    //    axisY->setRange(0, 100);
    //    axisY->setTitleText("分数");
    //    axisY->setTickCount(6);//11
    //    axisY->setLabelFormat("%.0f"); //标签格式
    ////    axisY->setGridLineVisible(false);
    ////    axisY->setMinorTickCount(4);
    //    chart->setAxisY(axisY, series); //为
    //    chart->setAxisY(axisY, Line);

    ////    lineseries->setPointLabelsVisible(true);
    ////    lineseries->setPointLabelsFormat("@yPoint");

    //    chart->legend()->setVisible(true); //显示图例
    //    chart->legend()->setAlignment(Qt::AlignBottom); //图例显示在下方
}

// 初始化饼图
void ChartForm::initPieChart() {
    chart = new QChart();
    chart->setTitle("饼图");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

// 绘制饼图
void ChartForm::drawPieChart() {
    QChart* chart = ui->chartView->chart();  // 获取chart对象
    chart->removeAllSeries();                // 删除所有序列

    QPieSeries* series = new QPieSeries();  // 创建饼图序列

    for (int i = 1; i <= 3; i++)  // 添加分块数据
    {
        series->append("Buffer" + QString::number(i), 25);  // 添加一个饼图分块数据,标签，数值
    }

    QPieSlice* slice;             // 饼图分块
    for (int i = 0; i <= 2; i++)  // 设置每个分块的标签文字
    {
        slice = series->slices().at(i);                                       // 获取分块
        slice->setLabel(slice->label() + QString::asprintf(": %.0f, %.1f%%",  // 设置分块的标签
                                                           slice->value(), slice->percentage() * 100));
        // 信号与槽函数关联，鼠标落在某个分块上时，此分块弹出
        connect(slice, SIGNAL(hovered(bool)),
                this, SLOT(on_PieSliceHighlight(bool)));
    }
    slice->setExploded(true);        // 最后一个设置为exploded
    series->setLabelsVisible(true);  // 只影响当前的slices，必须添加完slice之后再设置
    chart->addSeries(series);        // 添加饼图序列
    chart->setTitle("饼图----测试一下");
    // 图例
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
}

// 百分比柱状图初始化
void ChartForm::initPercentBar() {
    QChart* chart = new QChart();
    chart->setTitle("PercentBar 演示");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

// 绘制百分比柱状图
void ChartForm::drawPercentBar() {
    //     QChart *chart =ui->chartViewPercentBar->chart();
    //     chart->removeAllSeries();
    //     chart->removeAxis(chart->axisX());
    //     chart->removeAxis(chart->axisY());

    ////创建数据集
    //    QBarSet *setMath = new QBarSet(theModel->horizontalHeaderItem(colNoMath)->text());
    //    QBarSet *setChinese = new QBarSet(theModel->horizontalHeaderItem(colNoChinese)->text());
    //    QBarSet *setEnglish= new QBarSet(theModel->horizontalHeaderItem(colNoEnglish)->text());

    //    QTreeWidgetItem *item; //节点
    //    QStringList categories;
    //    for (int i=0;i<=4;i++)
    //    {//从分数段统计数据表里获取数据，添加到数据集
    //        item=ui->treeWidget->topLevelItem(i);
    //        categories<<item->text(0);//用作横坐标的标签
    //        setMath->append(item->text(colNoMath).toFloat());
    //        setChinese->append(item->text(colNoChinese).toFloat());
    //        setEnglish->append(item->text(colNoEnglish).toFloat());
    //    }

    //    QPercentBarSeries *series = new QPercentBarSeries(); //序列
    //    series->append(setMath);
    //    series->append(setChinese);
    //    series->append(setEnglish);
    //    series->setLabelsVisible(true);//显示百分比
    ////    series->setLabelsFormat("@value人");

    //    chart->addSeries(series);

    //    QBarCategoryAxis *axisX = new QBarCategoryAxis(); //横坐标
    //    axisX->append(categories);
    //    chart->setAxisX(axisX, series);
    //    axisX->setRange(categories.at(0), categories.at(categories.count()-1));

    //    QValueAxis *axisY = new QValueAxis;//纵坐标
    //    axisY->setRange(0, 100);
    //    axisY->setTitleText("百分比");
    //    axisY->setTickCount(6);
    //    axisY->setLabelFormat("%.1f"); //标签格式
    //    chart->setAxisY(axisY, series);

    //    chart->legend()->setVisible(true);
    //    chart->legend()->setAlignment(Qt::AlignRight); //AlignBottom
}

void ChartForm::initLineChart()
{
    //初始化QChart的实例
    chart = ui->chartView->chart();  // 获取chart对象
    //初始化QSplineSeries的实例
    lineSeries = new QSplineSeries();
    //设置曲线的名称
    lineSeries->setName(chartName);
    //把曲线添加到QChart的实例chart中
    chart->addSeries(lineSeries);
    //声明并初始化X轴、两个Y轴
    QValueAxis *axisX = new QValueAxis();
    QValueAxis *axisY = new QValueAxis();
    //设置坐标轴显示的范围
    axisX->setMin(0);
    axisX->setMax(MAX_X);
    axisY->setMin(0);
    axisY->setMax(MAX_Y);
    //设置坐标轴上的格点
    axisX->setTickCount(10);
    axisY->setTickCount(10);
    //设置坐标轴显示的名称
    axisX->setTitleText("X-Test");
    axisY->setTitleText("Y-Test");
    //设置网格不显示
    axisY->setGridLineVisible(false);
    //下方：Qt::AlignBottom，左边：Qt::AlignLeft
    //右边：Qt::AlignRight，上方：Qt::AlignTop
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    //把曲线关联到坐标轴
    lineSeries->attachAxis(axisX);
    lineSeries->attachAxis(axisY);
    //把chart显示到窗口上
//    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

void ChartForm::drawLineChart()
{
    static int count = 0;
        if(count > MAX_X)
        {
            //当曲线上最早的点超出X轴的范围时，剔除最早的点，
            lineSeries->removePoints(0,lineSeries->count() - MAX_X);
            // 更新X轴的范围
            chart->axisX()->setMin(count - MAX_X);
            chart->axisX()->setMax(count);
        }
        //增加新的点到曲线末端
        lineSeries->append(count, rand()%65);//随机生成0到65的随机数
        count ++;
}

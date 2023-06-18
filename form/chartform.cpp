#include "chartform.h"
#include "ui_chartform.h"

extern QStringList profiles;

// For Json
ChartForm::ChartForm(QWidget *parent): QWidget(parent)
{
    initialize(parent);
}

// For Real-Time
ChartForm::ChartForm(const ChartParam &param, QWidget* parent)
    : QWidget(parent), ui(new Ui::ChartForm), param(param) {
    ui->setupUi(this);
    initialize(parent);

    chartName = param.buf + "-" + profiles[param.type];
    this->setWindowTitle(chartName);

    switch (param.type) {
    case DATA_CHANGE_TREND:{//Buffer数据量变化趋势【曲线图】
        initLineChart(3,{"Buffer1","Buffer2","Buffer3"},tr("时间(ms)"),tr("数据数量"));
        connect(timeRefresh,&QTimer::timeout,this,&ChartForm::drawChart_type1);
        break;
    }
    case DATA_DISTRIBUTION:{//Buffer数据量分布【饼图】
        initPieChart(3, {"Buffer1","Buffer2","Buffer3"});
        connect(timeRefresh,&QTimer::timeout,this,&ChartForm::drawChart_type2);
        break;
    }
    case THREAD_STATE_TREND:{//线程状态变化趋势【曲线图】
        initLineChart(3,{"PUT","MOVE","GET"},tr("时间(ms)"),tr("阻塞数量"));
        connect(timeRefresh,&QTimer::timeout,this,&ChartForm::drawChart_type3);
        break;
    }
    }
}

ChartForm::~ChartForm() {
    delete ui;
    delete draft1;
    delete draft2;
    delete draft3;
}

void ChartForm::initialize(QWidget* parent)
{
    draft1 = new ChartDataChangedTrend();
    draft2 = new ChartDataDistribution();
    draft3 = new ChartThreadStateTrend();
    timeRefresh = new QTimer(this);
    timeRefresh->start(100);
    connect(dynamic_cast<AnalyseWindow*>(parent)->comboxStyle,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,[this](int i){
        ui->chartView->chart()->setTheme(QChart::ChartTheme(i));
    });
}

void ChartForm::drawFromJson(QString& str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject obj = doc.object();
    QString type = obj["param"].toObject()["type"].toString();
    if(type == "type1") {
        draft1->readJson(str);
        initLineChart(3,{"Buffer1","Buffer2","Buffer3"},tr("时间(ms)"),tr("数据数量"));
        connect(timeRefresh,&QTimer::timeout,this,&ChartForm::drawChart_type1);
    }else if(type == "type2"){
        draft2->readJson(str);
        initPieChart(3, {"Buffer1","Buffer2","Buffer3"});qDebug()<<"initPieChart";
        connect(timeRefresh,&QTimer::timeout,this,&ChartForm::drawChart_type2);
    }else if(type == "type3"){
        draft3->readJson(str);
        initLineChart(3,{"PUT","MOVE","GET"},tr("时间(ms)"),tr("阻塞数量"));
        connect(timeRefresh,&QTimer::timeout,this,&ChartForm::drawChart_type3);
    }
}

void ChartForm::initLineChart(int n, const QStringList& names, const QString& x_name, const QString& y_name)
{
    chart = new QChart();
    for(int i = 0;i < n;i++){
        lineSeries.append(new QSplineSeries());
        lineSeries[i]->setName(names[i]);
        chart->addSeries(lineSeries[i]);
    }
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
    axisX->setTitleText(x_name);
    axisY->setTitleText(y_name);
    //设置网格不显示
    axisY->setGridLineVisible(false);
    //下方：Qt::AlignBottom，左边：Qt::AlignLeft
    //右边：Qt::AlignRight，上方：Qt::AlignTop
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    for(int i = 0;i < n;i++){
//        lineSeries.append(new QSplineSeries());
        lineSeries[i]->attachAxis(axisX);
        lineSeries[i]->attachAxis(axisY);
//        lineSeries[i]->setName(names[i]);
//        chart->addSeries(lineSeries[i]);
    }
    //把chart显示到窗口上
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

void ChartForm::initPieChart(int n, const QStringList& names) {
    chart = new QChart();
    pieSeries = new QPieSeries();
    for (int i = 0; i < n; i++)  // 添加初始分块数据
    {
//        QPieSlice* pieSlice = new QPieSlice();
//        pieSlice->setLabel(names[i]);
        pieSeries->append(names[i],0);  // 添加一个饼图分块数据,标签
        // 信号与槽函数关联，鼠标移入、移出某个分块时，此分块弹出、收回
        connect(pieSeries->slices()[i], &QPieSlice::hovered, this, [this](bool show){
            static_cast<QPieSlice*>(sender())->setExploded(show);
        });
    }
    pieSeries->slices().back()->setExploded(true);  // 最后一个设置为exploded，与切片分离
    pieSeries->setLabelsVisible(true);  // 只影响当前的slices，必须添加完slice之后再设置
    chart->addSeries(pieSeries);        // 添加饼图序列
    chart->setTitle(chartName);
    // 图例
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

void ChartForm::drawChart_type1()
{
    auto& data = draft1->_data_source;
    for(int count = 0; count < data->getData_num(); count++){
        //当曲线上最早的点超出X轴的范围时
        for(auto& line : lineSeries){
            if(count > MAX_X) {
                // 剔除最早的点
                line->removePoints(0,line->count() - MAX_X);
                // 更新X轴的范围
                chart->axes(Qt::Horizontal).back()->setMin(count - MAX_X);
                chart->axes(Qt::Horizontal).back()->setMax(count);
            }
        }
        //增加新的点到曲线末端
        lineSeries[0]->append(data->time_staps[count], data->buffer1_data[count]);
        lineSeries[1]->append(data->time_staps[count], data->buffer2_data[count]);
        lineSeries[2]->append(data->time_staps[count], data->buffer3_data[count]);
    }
}

void ChartForm::drawChart_type2() {
    auto& data = draft2->_data_source;
//    chart->removeAllSeries();     // 删除所有序列
    qreal sum = 0;
    for(int& num : data->cur_num) sum += num;qDebug()<<sum;
    auto calculator = [&sum,&data](int i) -> qreal {
        if(sum == 0) return 0;
        else return static_cast<qreal>(data->cur_num[i]) / sum;
    };
    for(int i = 0;i < pieSeries->slices().size();i++) {
        auto& slice = pieSeries->slices()[i];
        slice->setValue(calculator(i));qDebug()<<slice->value();
        slice->setLabel(slice->label() + QString::asprintf(": %.0f, %.1f%%",slice->value(),slice->percentage() * 100));qDebug()<<"22222";
    }
}

void ChartForm::drawChart_type3()
{
    auto& data = draft3->_data_source;
    for(int count = 0; count < data->getData_num(); count++){
        //当曲线上最早的点超出X轴的范围时
        for(auto& line : lineSeries){
            if(count > MAX_X) {
                // 剔除最早的点
                line->removePoints(0,line->count() - MAX_X);
                // 更新X轴的范围
                chart->axes(Qt::Horizontal).back()->setMin(count - MAX_X);
                chart->axes(Qt::Horizontal).back()->setMax(count);
            }
        }
        //增加新的点到曲线末端
        lineSeries[0]->append(data->time_staps[count], data->put_blocked_num[count]);
        lineSeries[1]->append(data->time_staps[count], data->move_blocked_num[count]);
        lineSeries[2]->append(data->time_staps[count], data->get_blocked_num[count]);
    }
}

ChartParam ChartForm::getParam() const
{
    return param;
}

void ChartForm::setParam(const ChartParam &value)
{
    param = value;
}

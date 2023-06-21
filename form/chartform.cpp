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
        setGrid(50,std::max({draft1->_data_source->buffers[0]->capacity,
                             draft1->_data_source->buffers[1]->capacity,
                             draft1->_data_source->buffers[2]->capacity}));
        initLineChart(3,{"Buffer1","Buffer2","Buffer3"},tr("时间(s)"),tr("数据数量"));
        connect(timerRefresh,&QTimer::timeout,this,&ChartForm::drawChart_type1);
        break;
    }
    case DATA_DISTRIBUTION:{//Buffer数据量分布【饼图】
        initPieChart(3, {"Buffer1","Buffer2","Buffer3"});
        connect(timerRefresh,&QTimer::timeout,this,&ChartForm::drawChart_type2);
        break;
    }
    case THREAD_STATE_TREND:{//线程状态变化趋势【曲线图】
        setGrid(50,std::max({draft3->_data_source->config->put_num,
                             draft3->_data_source->config->move_num,
                             draft3->_data_source->config->get_num}));
        disconnect(static_cast<AnalyseWindow*>(parent),nullptr,timerRefresh,nullptr);
        disconnect(static_cast<AnalyseWindow*>(parent),nullptr,timerRefresh,nullptr);
        initLineChart(3,{"PUT","MOVE","GET"},tr("时间(ms)"),tr("阻塞数量"));
        connect(timerRefresh,&QTimer::timeout,this,&ChartForm::drawChart_type3);
        timerRefresh->start(interval);
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
    timerRefresh = new QTimer(this);
    connect(static_cast<AnalyseWindow*>(parent)->comboxStyle,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,[this](int i){
        ui->chartView->chart()->setTheme(QChart::ChartTheme(i));
    });
    connect(static_cast<AnalyseWindow*>(parent),&AnalyseWindow::sigStart,timerRefresh,[this](){timerRefresh->start(interval);},Qt::DirectConnection);
    connect(static_cast<AnalyseWindow*>(parent),&AnalyseWindow::sigStop,timerRefresh,&QTimer::stop,Qt::DirectConnection);
}

void ChartForm::drawFromJson(QString& str)
{
    if(param.type == DATA_CHANGE_TREND) {
        draft1->readJson(str);
        initLineChart(3,{"Buffer1","Buffer2","Buffer3"},tr("时间(ms)"),tr("数据数量"));
        auto& data = draft1->_data_source;
        for(; count < data->getData_num(); count++){
            //当曲线上最早的点超出X轴的范围时
            for(auto& line : lineSeries){
                if(count > max_x) {
                    // 剔除最早的点
                    line->removePoints(0,line->count() - max_x);
                    // 更新X轴的范围
                    chart->axes(Qt::Horizontal).back()->setRange(count - max_x,count);
                }
            }
            //增加新的点到曲线末端
            lineSeries[0]->append(data->time_staps[count], data->buffer1_data[count]);
            lineSeries[1]->append(data->time_staps[count], data->buffer2_data[count]);
            lineSeries[2]->append(data->time_staps[count], data->buffer3_data[count]);
        }
    }else if(param.type == DATA_DISTRIBUTION){
        draft2->readJson(str);
        initPieChart(3, {"Buffer1","Buffer2","Buffer3"});
        auto& data = draft2->_data_source;
        for (;count < data->getData_num();count++) {
            pieSeries->slices().at(0)->setValue(data->buffer1_data[count]);
            pieSeries->slices().at(1)->setValue(data->buffer1_data[count]);
            pieSeries->slices().at(2)->setValue(data->buffer2_data[count]);
        }
        for(int i = 0;i < pieSeries->slices().size();i++) {
            auto& slice = pieSeries->slices().at(i);
            slice->setLabel(QString::asprintf("Buffer%d: 数据量: %.0f,占比: %.1f%%",i+1,slice->value(),slice->percentage() * 100));
        }
    }else if(param.type == THREAD_STATE_TREND){
        draft3->readJson(str);
        initLineChart(3,{"PUT","MOVE","GET"},tr("时间(ms)"),tr("阻塞数量"));
        auto& data = draft3->_data_source;
        for(; count < data->getData_num(); count++){
            //当曲线上最早的点超出X轴的范围时
            for(auto& line : lineSeries){
                if(count > max_x) {
                    // 剔除最早的点
                    line->removePoints(0,line->count() - max_x);
                    // 更新X轴的范围
                    chart->axes(Qt::Horizontal).back()->setRange(count - max_x,count);
                }
            }
            //增加新的点到曲线末端
            lineSeries[0]->append(data->time_staps[count], data->put_blocked_num[count]);
            lineSeries[1]->append(data->time_staps[count], data->move_blocked_num[count]);
            lineSeries[2]->append(data->time_staps[count], data->get_blocked_num[count]);
        }
    }
}

void ChartForm::setGrid(const int x, const int y)
{
    max_x = x;
    max_y = y + 1;
    interval = draft1->_data_source->interval;
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
    axisX->setRange(0,max_x);
    axisY->setRange(-1,max_y);
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
        lineSeries[i]->attachAxis(axisX);
        lineSeries[i]->attachAxis(axisY);
    }
    //把chart显示到窗口上
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);
}

void ChartForm::initPieChart(int n, const QStringList& names) {
    chart = new QChart();
    pieSeries = new QPieSeries();
    for (int i = 0; i < n; i++) { // 添加初始分块数据
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
    //当曲线上最早的点超出X轴的范围时
    for(auto& line : lineSeries){
        if(count > max_x) {
            // 剔除最早的点，保留最近的max_x个点
            line->removePoints(0,line->count() - max_x);
            // 更新X轴的范围
            chart->axes(Qt::Horizontal).back()->setRange(count - max_x,count);
        }
    }
    //增加新的点到曲线末端
    for(int i = 0;i < 3;i++){
        lineSeries[i]->append(count,data->buffers[i]->cur_num);
    }
    count++;
}

void ChartForm::drawChart_type2() {
    auto& data = draft2->_data_source;
//    chart->removeAllSeries();     // 删除所有序列
//    qreal sum = 0;
//    for(auto& buf : data->buffers) sum += buf->cur_num;
//    auto calculator = [&sum,&data](int i) -> qreal {
//        if(sum == 0) return 0;
//        else return static_cast<qreal>(data->buffers.at(i)->cur_num) / sum;
//    };
    for(int i = 0;i < pieSeries->slices().size();i++) {
        auto& slice = pieSeries->slices().at(i);
        slice->setValue(data->buffers.at(i)->cur_num);
        slice->setLabel(QString::asprintf("Buffer%d: 数据量: %.0f,占比: %.1f%%",i+1,slice->value(),slice->percentage() * 100));
    }
    data->buf1_cur_num.append(data->buffers.at(0)->cur_num);
    data->buf2_cur_num.append(data->buffers.at(1)->cur_num);
    data->buf3_cur_num.append(data->buffers.at(2)->cur_num);
}

void ChartForm::drawChart_type3()
{
    auto& data = draft3->_data_source;
    //当曲线上最早的点超出X轴的范围时
    for(auto& line : lineSeries){
        if(count > max_x) {
            // 剔除最早的点，保留最近的max_x个点
            line->removePoints(0,line->count() - max_x);
            // 更新X轴的范围
            chart->axes(Qt::Horizontal).back()->setRange(count - max_x,count);
        }
    }
    //增加新的点到曲线末端
    lineSeries[0]->append(count, data->getPut_blocked_num());
    lineSeries[1]->append(count, data->getMove_blocked_num());
    lineSeries[2]->append(count, data->getPut_blocked_num());
    count++;
}

ChartParam ChartForm::getParam() const
{
    return param;
}

void ChartForm::setParam(const ChartParam &value)
{
    param = {
        .buf = value.buf,
        .type = value.type
    };
}

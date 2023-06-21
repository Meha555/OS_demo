#include "analysewindow.h"
#include "ui_analysewindow.h"

QStringList profiles = {"Buffer数据量变化趋势", "Buffer数据量分布", "线程状态变化趋势"};

static const QString help = "<b>图表类型：</b><br>"
                            "- Buffer数据量变化趋势<br>"
                            "- Buffer数据量分布<br>"
                            "- 线程状态变化趋势<br><br>"
                            "<b>使用说明：</b><br>"
                            "- 鼠标框选可以放大指定区域<br>"
                            "- 鼠标右键可以缩小<br>"
                            "- 鼠标滚轮可以缩放<br>"
                            "- 鼠标中键可以拖动";


AnalyseWindow::AnalyseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalyseWindow)
{
    ui->setupUi(this);
    labPut = new QLabel("PUT线程阻塞数量: 0",this);
    labPut->setMinimumWidth(250);
    labMove = new QLabel("MOVE线程阻塞数量: 0",this);
    labMove->setMinimumWidth(250);
    labGet = new QLabel("GET线程阻塞数量: 0",this);
    labGet->setMinimumWidth(250);
    statLED = new LedLabel(this);
    statLED->setText("当前状态: ");
    ui->statusbar->addWidget(labPut);
    ui->statusbar->addWidget(labMove);
    ui->statusbar->addWidget(labGet);
    ui->statusbar->addPermanentWidget(statLED);

    // 哪个Buffer: bf1 bf2 all
    QLabel* lab1 = new QLabel("Buffer: ",this);
    comboxBuffer = new QComboBox(this);
    // 什么数据统计组合套餐（绑定了图表类型）
    QLabel* lab2 = new QLabel("统计类型: ",this);
    comboxProfile = new QComboBox(this);
    // 什么图表风格
    QLabel* lab3 = new QLabel("图表风格: ",this);
    comboxStyle = new QComboBox(this);
    // OK & Close
//    btnOK = new QPushButton(this);
//    btnCancel = new QPushButton(this);
//    btnHelp = new QPushButton(this);
    btnOK = new QtMaterialRaisedButton(this);
    btnClose = new QtMaterialRaisedButton(this);
    btnHelp = new QtMaterialRaisedButton(this);
    btnOK->setRole(Material::Secondary);
    btnClose->setRole(Material::Primary);
    btnOK->setText("绘制");
    btnClose->setText("关闭全部");
    btnHelp->setText("使用说明");
    btnOK->setFixedSize(100, 30);
    btnClose->setFixedSize(100,30);
//    comboxProfile->addItem("")
    comboxBuffer->addItem("Buffer1");
    comboxBuffer->addItem("Buffer2");
    comboxBuffer->addItem("Buffer3");
    comboxBuffer->addItem("All");

    comboxProfile->addItems(profiles);

    comboxStyle->addItem("Light", QChart::ChartThemeLight);
    comboxStyle->addItem("Blue Cerulean", QChart::ChartThemeBlueCerulean);
    comboxStyle->addItem("Dark", QChart::ChartThemeDark);
    comboxStyle->addItem("Brown Sand", QChart::ChartThemeBrownSand);
    comboxStyle->addItem("Blue NCS", QChart::ChartThemeBlueNcs);
    comboxStyle->addItem("High Contrast", QChart::ChartThemeHighContrast);
    comboxStyle->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
    comboxStyle->addItem("Qt", QChart::ChartThemeQt);

    connect(btnOK, &QPushButton::clicked, this, &AnalyseWindow::createTab);
    connect(btnClose, &QPushButton::clicked, this, [this](){
        for (int i = 0; i < ui->tabWidget->count(); ++i) { // 获取所有选项卡的个数，并逐一关闭之
            ui->tabWidget->widget(i)->close();
        }
    });
    connect(btnHelp,&QPushButton::clicked,this,[this](){
        QMessageBox::information(this, "帮助信息", help);
    });
    MainWindow* ptr = static_cast<MainWindow*>(parent);
    connect(ptr,&MainWindow::sigPause,this,[this](){statLED->setStatus(BLOCK);emit sigStop();},Qt::DirectConnection);
    connect(ptr,&MainWindow::sigStop,this,[this](){statLED->setStatus(TERMINATED);emit sigStop();},Qt::DirectConnection);
    connect(ptr,&MainWindow::sigRun,this,[this](){statLED->setStatus(RUNNING);emit sigStart();},Qt::DirectConnection);
    qDebug()<<"======PUT====== "<<ptr->gatherer->getPut_blocked_num();
    qDebug()<<"======MOVE===== "<<ptr->gatherer->getMove_blocked_num();
    qDebug()<<"======GET====== "<<ptr->gatherer->getGet_blocked_num();
//    connect(ptr->timerWalker,&QTimer::timeout,this,[this,&ptr](){
//        labPut->setText(QString("PUT线程阻塞数量: %1").arg(ptr->gatherer->getPut_blocked_num()));
//        labMove->setText(QString("MOVE线程阻塞数量: %1").arg(ptr->gatherer->getMove_blocked_num()));
//        labGet->setText(QString("GET线程阻塞数量: %1").arg(ptr->gatherer->getGet_blocked_num()));
//        qDebug()<<"======PUT====== "<<ptr->gatherer->getPut_blocked_num();
//        qDebug()<<"======MOVE===== "<<ptr->gatherer->getMove_blocked_num();
//        qDebug()<<"======GET====== "<<ptr->gatherer->getGet_blocked_num();
//    },Qt::DirectConnection);

    // 创建栅格布局，以坐标形式限定各组件的位置
    QGridLayout* btnLayout = new QGridLayout;
    btnLayout->setSpacing(5);
    btnLayout->addWidget(lab1, 0, 0);
    btnLayout->addWidget(comboxBuffer, 0, 1);
    btnLayout->addWidget(lab3, 0, 2);
    btnLayout->addWidget(comboxStyle, 0, 3);
    btnLayout->addWidget(lab2, 1, 0);
    btnLayout->addWidget(comboxProfile, 1, 1);
    btnLayout->addWidget(btnOK, 1, 2);
    btnLayout->addWidget(btnClose, 1, 3);
    QWidget* widget = new QWidget(this);
    widget->setLayout(btnLayout);
    ui->toolBar->addWidget(widget);
    ui->toolBar->addWidget(btnHelp);
//    ui->tabWidget->setTabsClosable(true);
//    ui->tabWidget->setUsesScrollButtons(true);

}

AnalyseWindow::~AnalyseWindow()
{
    delete ui;
}

void AnalyseWindow::createTab()
{
    ChartParam param{
        .buf = comboxBuffer->currentText(),
        .type = static_cast<ProfileType>(comboxProfile->currentIndex())
    };

    //创建ChartForm窗体，并在tabWidget中显示
    ChartForm *chartForm = new ChartForm(param,this); //不指定父窗口，单独用show()方法显示
    chartForm->setAttribute(Qt::WA_DeleteOnClose); //关闭时自动删除

    int cur = ui->tabWidget->addTab(chartForm
            ,param.buf + "-" + profiles[param.type]+QString::number(ui->tabWidget->count()));
    ui->tabWidget->setCurrentIndex(cur);
    ui->tabWidget->setVisible(true);
}

void AnalyseWindow::on_tabWidget_tabCloseRequested(int index)
{
    if (index >= 0) ui->tabWidget->widget(index)->close();
}

void AnalyseWindow::on_actSelectData_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,"选择数据源",QDir::currentPath(),"Json文件(*.json)");
    if(fileName.isEmpty()){
        QMessageBox::warning(this,"文件选择错误","文件路径为空！");
        return;
    }
    QFile aFile(fileName);
    if(!aFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this,"文件错误","打开文件失败");
        return;
    }
    QTextStream aStream(&aFile);
    aStream.setCodec("UTF-8");
    QString str = aStream.readAll();
    aFile.close();
    ChartForm *chartForm = new ChartForm(this); //不指定父窗口，单独用show()方法显示
    chartForm->setAttribute(Qt::WA_DeleteOnClose); //关闭时自动删除
    chartForm->setParam(ChartStat::recognize(QJsonDocument::fromJson(str.toUtf8()).object()));
    int cur = ui->tabWidget->addTab(chartForm
            ,chartForm->getParam().buf + "-" + profiles[chartForm->getParam().type]+QString::number(ui->tabWidget->count()));
    ui->tabWidget->setCurrentIndex(cur);
    ui->tabWidget->setVisible(true);
    chartForm->drawFromJson(str);
}

void AnalyseWindow::on_actSnapShot_triggered()
{
    QString defaultFileName = QString("data_%1.json").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss"));

    savePath = QFileDialog::getSaveFileName(this, "选择保存路径", defaultFileName, "json files");

    QFile aFile(savePath);
    if(!aFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::warning(this,"文件错误","创建文件失败");
        return;
    }

    ProfileType idx = static_cast<ProfileType>(comboxProfile->currentIndex());

    switch(idx){
    case DATA_CHANGE_TREND:{
        ChartDataChangedTrend* chart = new ChartDataChangedTrend();
        aFile.write(chart->writeJson());
        break;
    }
    case DATA_DISTRIBUTION:{
        ChartDataDistribution* chart = new ChartDataDistribution();
        aFile.write(chart->writeJson());
        break;
    }
    case THREAD_STATE_TREND:{
        ChartThreadStateTrend* chart = new ChartThreadStateTrend();
        aFile.write(chart->writeJson());
        break;
    }
    }

    aFile.close();
}



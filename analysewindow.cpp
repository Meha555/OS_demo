#include "analysewindow.h"
#include "ui_analysewindow.h"

QStringList profiles = {"Buffer数据量变化趋势", "Buffer数据量分布", "取出/放入数据变化趋势", "线程状态变化趋势"};

AnalyseWindow::AnalyseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalyseWindow)
{
    ui->setupUi(this);
    QLabel* labPut = new QLabel("PUT线程阻塞数量: 0",this);
    labPut->setMinimumWidth(250);
    QLabel* labMove = new QLabel("MOVE线程阻塞数量: 0",this);
    labMove->setMinimumWidth(250);
    QLabel* labGet = new QLabel("GET线程阻塞数量: 0",this);
    labGet->setMinimumWidth(250);
    LedLabel* statLED = new LedLabel(this);
    statLED->setText("当前状态: ");
    statLED->setColor("grey");
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
    // OK & Cancel
//    btnOK = new QPushButton(this);
//    btnCancel = new QPushButton(this);
//    btnHelp = new QPushButton(this);
    btnOK = new QtMaterialRaisedButton(this);
    btnCancel = new QtMaterialRaisedButton(this);
    btnHelp = new QtMaterialRaisedButton(this);
    btnOK->setRole(Material::Secondary);
    btnCancel->setRole(Material::Primary);
    btnOK->setText("绘制");
    btnCancel->setText("关闭全部");
    btnHelp->setText("图表说明");
    btnOK->setFixedSize(100, 30);
    btnCancel->setFixedSize(100,30);
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
    connect(btnCancel, &QPushButton::clicked, this, [this](){
        for (int i = 0; i < ui->tabWidget->count(); ++i) { // 获取所有选项卡的个数，并逐一关闭之
            ui->tabWidget->widget(i)->close();
        }
    });
    connect(btnHelp,&QPushButton::clicked,this,[this](){
        QMessageBox::information(this, "帮助信息", "这是一条帮助信息。");
    });

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
    btnLayout->addWidget(btnCancel, 1, 3);
//    ui->toolBar->addWidget(lab1);
//    ui->toolBar->addWidget(comboxBuffer);
//    ui->toolBar->addWidget(lab2);
//    ui->toolBar->addWidget(comboxProfile);
//    ui->toolBar->addWidget(lab3);
//    ui->toolBar->addWidget(comboxStype);
//    ui->toolBar->addWidget(btnOK);
//    ui->toolBar->addWidget(btnCancel);
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
    ChartParam parm{
        .buf = comboxBuffer->currentText(),
        .type = static_cast<ProfileType>(comboxProfile->currentIndex())
    };

    //创建ChartForm窗体，并在tabWidget中显示
    ChartForm *chartForm = new ChartForm(parm,this); //不指定父窗口，单独用show()方法显示
    chartForm->setAttribute(Qt::WA_DeleteOnClose); //关闭时自动删除

    int cur=ui->tabWidget->addTab(chartForm,
            QString::asprintf("图表 %d",ui->tabWidget->count()));
    ui->tabWidget->setCurrentIndex(cur);
    ui->tabWidget->setVisible(true);
}

void AnalyseWindow::on_tabWidget_tabCloseRequested(int index)
{
    if (index >= 0) ui->tabWidget->widget(index)->close();
}

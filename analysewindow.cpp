#include "analysewindow.h"
#include "ui_analysewindow.h"

AnalyseWindow::AnalyseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnalyseWindow)
{
    ui->setupUi(this);
    // 哪个Buffer: bf1 bf2 all
    QLabel* lab1 = new QLabel(this);
    lab1->setText("Buffer: ");
    comboxBuffer = new QComboBox(this);
    // 什么数据统计组合套餐（绑定了图表类型）
    QLabel* lab2 = new QLabel(this);
    lab2->setText("统计类型: ");
    comboxProfile = new QComboBox(this);
    // 什么图表风格
    QLabel* lab3 = new QLabel(this);
    lab3->setText("图表风格: ");
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
    btnOK->setText("确定");
    btnCancel->setText("取消");
    btnHelp->setText("图表说明");
    btnOK->setFixedSize(100, 30);
    btnCancel->setFixedSize(100,30);
//    comboxProfile->addItem("")
    comboxBuffer->addItem("Buffer1");
    comboxBuffer->addItem("Buffer2");
    comboxBuffer->addItem("Buffer3");

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
        comboxBuffer->clear();comboxProfile->clear();comboxStyle->clear();
    });
    connect(btnHelp,&QPushButton::clicked,this,[this](){
        QMessageBox::information(this, "帮助信息", "这是一条帮助信息。");
    });

    // 创建栅格布局，以坐标形式限定各组件的位置
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(lab1, 0, 0);
    layout->addWidget(comboxBuffer, 0, 1);
    layout->addWidget(lab3, 0, 2);
    layout->addWidget(comboxStyle, 0, 3);
    layout->addWidget(lab2, 1, 0);
    layout->addWidget(comboxProfile, 1, 1);
    layout->addWidget(btnOK, 1, 2);
    layout->addWidget(btnCancel, 1, 3);
//    ui->toolBar->addWidget(lab1);
//    ui->toolBar->addWidget(comboxBuffer);
//    ui->toolBar->addWidget(lab2);
//    ui->toolBar->addWidget(comboxProfile);
//    ui->toolBar->addWidget(lab3);
//    ui->toolBar->addWidget(comboxStype);
//    ui->toolBar->addWidget(btnOK);
//    ui->toolBar->addWidget(btnCancel);
    QWidget* widget = new QWidget(this);
    widget->setLayout(layout);
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
    //创建ChartForm窗体，并在tabWidget中显示
    ChartForm *chartForm = new ChartForm("Pie",this); //不指定父窗口，单独用show()方法显示
//    chartForm->setType("Pie"); // 设置为绘制饼状图，这里应放Profile类型
    chartForm->setAttribute(Qt::WA_DeleteOnClose); //关闭时自动删除
    chartForm->setWindowTitle("xxx图表");
    //    chartForm->setWindowFlag(Qt::Widget,true);
    //    chartForm->setWindowFlag(Qt::CustomizeWindowHint,true);
    //    chartForm->setWindowFlag(Qt::WindowMinMaxButtonsHint,false);

    //    chartForm->setWindowState(Qt::WindowMaximized);
    //    chartForm->setWindowOpacity(0.5);
    //    chartForm->setWindowModality(Qt::WindowModal);

    //    chartForm->show(); //在单独的窗口中显示
    //    chartForm->setWindowTitle("基于QWidget的窗口，无父窗口，关闭时删除");
    int cur=ui->tabWidget->addTab(chartForm,
            QString::asprintf("图表 %d",ui->tabWidget->count()));
    ui->tabWidget->setCurrentIndex(cur);
    ui->tabWidget->setVisible(true);
}

void AnalyseWindow::on_tabWidget_tabCloseRequested(int index)
{
    if (index >= 0) ui->tabWidget->widget(index)->close();
}

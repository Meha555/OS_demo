#include "mainwindow.h"
#include "ui_mainwindow.h"

extern int speed_coefficient;

bool first_start = true;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 界面相关设置
    label1 = new QLabel("操作速度控制：", this);
    slider = new QSlider(Qt::Horizontal, this);
    label2 = new QLabel("慢 ", this);
    labelSpeed = new QLabel(QString(" 当前速度：%1x").arg(static_cast<qreal>(speed_coefficient/1000),0,'g',2));
    labelSpeed->setFixedSize(140,20);
    labelSpeed->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // 设置大小策略为固定大小
    label3 = new QLabel(" 快", this);
    slider->setMinimum(MIN_SPEED);      // 设置滑动条的最小值
    slider->setMaximum(MAX_SPEED);   // 设置滑动条的最大值
    slider->setValue(CUR_SPEED); // 设置滑动条初始值
    slider->setSingleStep(100);
    ui->toolBar->addWidget(label1);
    ui->toolBar->addWidget(label2);
    ui->toolBar->addWidget(slider);
    ui->toolBar->addWidget(label3);
    ui->toolBar->addWidget(labelSpeed);

    ui->actPasue->setEnabled(false);
    ui->actStart->setEnabled(false);
    ui->actRestart->setEnabled(false);
    ui->actTerminate->setEnabled(false);

    // 绘制进度球
    QTimer* _pTimerUpdate = new QTimer(this);_pTimerUpdate->start(2000);
    connect(_pTimerUpdate, &QTimer::timeout, ui->buffer1, &ProgressBall::updateProgressBall);
    connect(_pTimerUpdate, &QTimer::timeout, ui->buffer2, &ProgressBall::updateProgressBall);
    connect(_pTimerUpdate, &QTimer::timeout, ui->buffer3, &ProgressBall::updateProgressBall);
    _pTimerUpdate->start(20);

    // 信号相关
    connect(slider,&QSlider::valueChanged,this,&MainWindow::changeSpeed);
//    connect(_pTimerUpdate, &QTimer::timeout, this, [this](){
//        qlonglong t = buffer1->cur_num+buffer2->cur_num+buffer3->cur_num;
//        result.collectResult(t,)
//    });

}

MainWindow::~MainWindow()
{
    delete ui;
    delete empty1;
    delete empty2;
    delete empty3;
    delete full1;
    delete full2;
    delete full3;
    delete mutex1;
    delete mutex2;
    delete mutex3;
}

void MainWindow::changeSpeed(int val){
    speed_coefficient = val;
    labelSpeed->setText(QString(" 当前速度：%1x").arg(static_cast<qreal>(speed_coefficient)/1000,0,'g',2));
#ifdef _DEBUG
    qDebug()<<speed_coefficient;
#endif
}

// 设置参数
void MainWindow::setConfig()
{
    // 创建3个缓冲区
    buffer1 = new Buffer(this);
    buffer2 = new Buffer(this);
    buffer3 = new Buffer(this);

    // 初始化缓冲区数据
    buffer1->setCapacity(config.getBuffer1_size());
    buffer1->setFree_space_num(buffer1->capacity);
    buffer2->setCapacity(config.getBuffer2_size());
    buffer2->setFree_space_num(buffer2->capacity);
    buffer3->setCapacity(config.getBuffer2_size());
    buffer3->setFree_space_num(buffer2->capacity);

    // 初始化各信号量
    empty1 = new Semaphore(buffer1->capacity);
    full1 = new Semaphore(0);
    empty2 = new Semaphore(buffer2->capacity);
    full2 = new Semaphore(0);
    empty3 = new Semaphore(buffer3->capacity);
    full3 = new Semaphore(0);
//    empty1 = new QSemaphore(buffer1->capacity);
//    full1 = new QSemaphore(0);
//    empty2 = new QSemaphore(buffer2->capacity);
//    full2 = new QSemaphore(0);
//    empty3 = new QSemaphore(buffer3->capacity);
//    full3 = new QSemaphore(0);
    mutex1 = new QMutex;
    mutex2 = new QMutex;
    mutex3 = new QMutex;

    ui->actPasue->setEnabled(true);
    ui->actStart->setEnabled(true);
    ui->actRestart->setEnabled(true);
    ui->actTerminate->setEnabled(true);

    ui->label_c_id->setText(QString::number(config.c_id));
    ui->label_buffer1_size->setText(QString::number(config.buffer1_size));
    ui->label_buffer2_size->setText(QString::number(config.buffer2_size));
    ui->label_buffer3_size->setText(QString::number(config.buffer3_size));
    ui->label_put_num->setText(QString::number(config.put_num));
    ui->label_put_speed->setText(QString::number(config.put_speed));
    ui->label_move_num->setText(QString::number(config.move_num));
    ui->label_move_speed->setText(QString::number(config.move_speed));
    ui->label_get_num->setText(QString::number(config.get_num));
    ui->label_get_speed->setText(QString::number(config.get_speed));
}



void MainWindow::on_actSetConfig_triggered()
{
    // 弹出对话框来设置信号量
    if(configFrom == nullptr)
        configFrom = new ConfigForm(this);
    configFrom->setWindowFlags(configFrom->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    // 接收设置
    connect(configFrom,&ConfigForm::sendConfig,this,[this](Config& cfg){config = cfg;});

    if(configFrom->exec() == QDialog::Accepted){
        // 设置参数
        setConfig();
    }
//    config.configInfo();
}

void MainWindow::on_actStart_triggered()
{
    if(first_start){
        // 初始化各线程，并启动
        for(int i=1;i<=config.put_num;i++){
#ifdef _DEBUG
            qDebug()<<"启动1个PUT";
#endif
            displayLogTextSys("启动1个PUT");
            Operation* put = new Operation(PUT,config.put_speed,this);
            connect(this,&MainWindow::destroy,put,[put](){put->quit();put->wait();put->deleteLater();});
            put->start();
        }
        for(int i=1;i<=(config.move_num>>1);i++) {
#ifdef _DEBUG
            qDebug()<<"启动2个MOVE";
#endif
            displayLogTextSys("启动2个MOVE");
            Operation* move1 = new Operation(MOVE1,config.move_speed,this);
            connect(this,&MainWindow::destroy,move1,[move1](){move1->quit();move1->wait();move1->deleteLater();});
            Operation* move2 = new Operation(MOVE2,config.move_speed,this);
            connect(this,&MainWindow::destroy,move2,[move2](){move2->quit();move2->wait();move2->deleteLater();});
            move1->start();move2->start();
        }
        for(int i=1;i<=config.put_num;i++){
#ifdef _DEBUG
            qDebug()<<"启动1个GET";
#endif
            displayLogTextSys("启动1个GET");
            Operation* get = new Operation(GET,config.get_speed,this);
            connect(this,&MainWindow::destroy,get,[get](){get->quit();get->wait();get->deleteLater();});
            get->start();
        }
        first_start = false;
    }
    else{
        Operation::resumeThread();
    }
}

void MainWindow::on_actPasue_triggered()
{
    Operation::pauseThread();
}

void MainWindow::on_actTerminate_triggered()
{
    first_start = true;
    Operation::terminateThread();
}

void MainWindow::on_actRestart_triggered()
{
    Operation::terminateThread();
    first_start = true;
    on_actStart_triggered();
}

void MainWindow::on_actImportConfig_triggered()
{
    if(dao_configForm == nullptr)
        dao_configForm = new DaoConfigForm(this);
//    dao_configForm->setWindowFlags(dao_configForm->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    // 接收设置
    connect(dao_configForm,&DaoConfigForm::sendConfig,this,[this](Config& cfg){config = cfg;});

    if(dao_configForm->exec() == QDialog::Accepted){
        // 设置参数
        setConfig();
        QMessageBox::information(this, "导入参数", "导入参数完成");
    }
    config.configInfo();
}

void MainWindow::displayLogTextSys(QString text,bool bold,QColor frontColor)
{
    if(ui->sysText_log->blockCount()>50) ui->sysText_log->clear();
    ui->sysText_log->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);//设置光标到文本末尾

    insertRichText(ui->sysText_log,text+"\n",bold,frontColor);
//    ui->sysText_log->insertPlainText(text+"\n");
    //移动滚动条到底部
    QScrollBar* scrollbar = ui->sysText_log->verticalScrollBar();
    if (scrollbar) {
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}

void MainWindow::displayLogText(QPlainTextEdit* edit,const QString& text) {
    if(edit->blockCount()>50) edit->clear();
    edit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);//设置光标到文本末尾

    edit->insertPlainText(text+"\n");
    //移动滚动条到底部
    QScrollBar* scrollbar = edit->verticalScrollBar();
    if (scrollbar) {
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}

void MainWindow::on_btnClear1_clicked()
{
    ui->plainText_log1->clear();
    ui->plainText_log1->verticalScrollBar()->setSliderPosition(0);
}

void MainWindow::on_btnClear2_clicked()
{
    ui->plainText_log2->clear();
    ui->plainText_log2->verticalScrollBar()->setSliderPosition(0);
}

void MainWindow::on_btnClear3_clicked()
{
    ui->plainText_log3->clear();
    ui->plainText_log3->verticalScrollBar()->setSliderPosition(0);
}

void MainWindow::insertRichText(QPlainTextEdit* edit,QString text,bool bold,QColor frontColor) {
    QTextCharFormat fmt;
//    fmt.setFontPointSize(size);//字体大小
    fmt.setFontWeight(bold ? QFont::Bold : QFont::Normal);//字体加粗,能保证不会影响到下次的设置
    fmt.setForeground(QBrush(frontColor));//字体色
    edit->mergeCurrentCharFormat(fmt);//文本框使用以上设定
    edit->insertPlainText(text);//文本框添加文本
}

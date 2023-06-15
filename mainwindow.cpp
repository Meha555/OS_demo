#include "mainwindow.h"
#include "ui_mainwindow.h"

extern int speed_coefficient;

extern int thread_put_num; //put操作个数
extern int thread_move_num; //move操作个数
extern int thread_get_num; //get操作个数

extern int putin_num; //已放入的数据个数
extern int getout_num; //已取出的数据个数

extern Statitics statics;

bool first_start = true;

void MainWindow::initalizeData()
{
    cfgDao->sqlExecute("UPDATE sqlite_sequence SET seq = 0 WHERE name='config'");
    msgDao->sqlExecute("UPDATE sqlite_sequence SET seq = 0 WHERE name='message'");
    resDao->sqlExecute("UPDATE sqlite_sequence SET seq = 0 WHERE name='result'");
}

void MainWindow::resetStatistics()
{
    speed_coefficient = 1000;//速度系数(ms)

    thread_put_num = config.put_num; //put操作个数
    thread_move_num = config.move_num; //move操作个数
    thread_get_num = config.get_num; //get操作个数

    statics.put_blocked_num = 0; //当前阻塞的put线程数
    statics.move_blocked_num = 0; //当前阻塞的move线程数
    statics.get_blocked_num = 0; //当前阻塞的get线程数

    putin_num = 0; //已放入的数据个数
    getout_num = 0; //已取出的数据个数

    ui->bufBall1->resetProgress();
    ui->bufBall2->resetProgress();
    ui->bufBall3->resetProgress();

//    ui->plainText_log1->clear();
//    ui->plainText_log2->clear();
//    ui->plainText_log3->clear();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<QTextCursor>("QTextCursor");

    cfgDao = new ConfigDaoImpl(this);
    msgDao = new MessageDaoImpl(this);
    resDao = new ResultDaoImpl(this);

    // 界面相关设置
    label1 = new QLabel("操作速度控制：", this);
//    slider = new QSlider(Qt::Horizontal, this);
    slider = new QtMaterialSlider(this);
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
    ui->actExportResult->setEnabled(false);
    ui->actAnalyse->setEnabled(false);

//    ui->btnClear1->setRole(Material::Primary);
//    ui->btnClear2->setRole(Material::Primary);
//    ui->btnClear3->setRole(Material::Primary);

    timeCounter = new QElapsedTimer();

    // 绘制进度球
    timerWalker = new QTimer(this);
// 这几个槽函数现在参数和timeout不匹配
//    connect(timerWalker, &QTimer::timeout, ui->bufBall1, &ProgressBall::updateProgress);
//    connect(timerWalker, &QTimer::timeout, ui->bufBall2, &ProgressBall::updateProgress);
//    connect(timerWalker, &QTimer::timeout, ui->bufBall3, &ProgressBall::updateProgress);

    // 信号相关
    connect(slider, &QSlider::valueChanged, this, &MainWindow::changeSpeed);
    connect(timerWalker, &QTimer::timeout, this, [this](){
        result.collectResult(buffer1->cur_num+buffer2->cur_num+buffer3->cur_num,putin_num,getout_num);
    });

    // 最后的准备工作
    initalizeData();
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
    delete timeCounter;
}

void MainWindow::changeSpeed(int val){
    speed_coefficient = val;
    labelSpeed->setText(QString(" 当前速度：%1x").arg(static_cast<qreal>(speed_coefficient)/1000,0,'g',2));
    qDebug()<<"Current Speed: "<<speed_coefficient;
}

// 每隔一段时间就将buffer的情况写入到数据库中
void MainWindow::updateRes2DB()
{
    // 写入Buffer内容
    int bf1 = msgDao->batchInsert(buffer1->buffer);
    int bf2 = msgDao->batchInsert(buffer2->buffer);
    int bf3 = msgDao->batchInsert(buffer3->buffer);
    qDebug()<<"向Buffer中插入"<<bf1+bf2+bf3<<"条记录";
    // 写入运行结果
    resDao->insert(result);
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
    buffer1->setCur_num(0);
    buffer2->setCapacity(config.getBuffer2_size());
    buffer2->setFree_space_num(buffer2->capacity);
    buffer2->setCur_num(0);
    buffer3->setCapacity(config.getBuffer3_size());
    buffer3->setFree_space_num(buffer3->capacity);
    buffer3->setCur_num(0);

    ui->bufBall1->setCapacity(config.buffer1_size);
    ui->bufBall2->setCapacity(config.buffer2_size);
    ui->bufBall3->setCapacity(config.buffer3_size);

    // 初始化各信号量
    empty1 = new QSemaphore(buffer1->capacity);
    full1 = new QSemaphore(0);
    empty2 = new QSemaphore(buffer2->capacity);
    full2 = new QSemaphore(0);
    empty3 = new QSemaphore(buffer3->capacity);
    full3 = new QSemaphore(0);
    mutex1 = new QMutex;
    mutex2 = new QMutex;
    mutex3 = new QMutex;

    ui->actStart->setEnabled(true);

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
    config.configInfo();
}

void MainWindow::on_actStart_triggered()
{
    timeCounter->start();
    timerWalker->start(20);

    if(first_start){
        resetStatistics();
        // 初始化各线程，并启动
        for(int i=1;i<=config.put_num;i++){
//            qDebug()<<"启动1个PUT";
            displayLogTextSys("启动1个PUT");
            Operation* put = new Operation(PUT,config.put_speed,this);
            connect(ui->actTerminate,&QAction::triggered,put,[put](){put->quit();put->wait();put->deleteLater();});
            connect(put, &Operation::putIn, ui->bufBall1, &ProgressBall::updateProgress,Qt::DirectConnection);
//            connect(_pTimerUpdate, &QTimer::timeout, put, [put](){put->getStatus();});
            put->start();
//            put->getTID();
        }
        int t = ceil(config.move_num/2.0);
        for(int i=1;i<=t;i++) {
//            qDebug()<<"启动2个MOVE";
            displayLogTextSys("启动2个MOVE");
            Operation* move1 = new Operation(MOVE1,config.move_speed,this);
            connect(ui->actTerminate,&QAction::triggered,move1,[move1](){move1->quit();move1->wait();move1->deleteLater();});
            connect(move1, &Operation::getOut, ui->bufBall1, &ProgressBall::updateProgress,Qt::DirectConnection);
            connect(move1, &Operation::putIn, ui->bufBall2, &ProgressBall::updateProgress,Qt::DirectConnection);
//            connect(_pTimerUpdate, &QTimer::timeout, move1, [move1](){move1->getStatus();});
            Operation* move2 = new Operation(MOVE2,config.move_speed,this);
            connect(ui->actTerminate,&QAction::triggered,move2,[move2](){move2->quit();move2->wait();move2->deleteLater();});
            connect(move2, &Operation::getOut, ui->bufBall1, &ProgressBall::updateProgress,Qt::DirectConnection);
            connect(move2, &Operation::putIn, ui->bufBall3, &ProgressBall::updateProgress,Qt::DirectConnection);
//            connect(_pTimerUpdate, &QTimer::timeout, move2, [move2](){move2->getStatus();});
            move1->start();move2->start();
//            move1->getTID();move2->getTID();
        }
        for(int i=1;i<=config.get_num;i++){
//            qDebug()<<"启动1个GET";
            displayLogTextSys("启动1个GET");
            Operation* get = new Operation(GET,config.get_speed,this);
            connect(ui->actTerminate,&QAction::triggered,get,[get](){get->quit();get->wait();get->deleteLater();});
            connect(get, &Operation::getOut, ui->bufBall2, &ProgressBall::updateProgress,Qt::DirectConnection);
            connect(get, &Operation::getOut, ui->bufBall3, &ProgressBall::updateProgress,Qt::DirectConnection);
//            connect(_pTimerUpdate, &QTimer::timeout, get, [get](){get->getStatus();});
            get->start();
//            get->getTID();
        }
        first_start = false;
        emit sigRun();
        ui->actPasue->setEnabled(true);
        ui->actRestart->setEnabled(true);
        ui->actTerminate->setEnabled(true);
    }
    else{
        Operation::resumeThread();
        emit sigRun();
    }
    ui->actAnalyse->setEnabled(false);
}

void MainWindow::on_actPasue_triggered()
{
    Operation::pauseThread();
    emit sigPause();
    ui->actAnalyse->setEnabled(true);
}

void MainWindow::on_actTerminate_triggered()
{
    first_start = true;
    Operation::terminateThread();
    emit sigStop();
    qreal avg = qreal(buffer1->cur_num+buffer2->cur_num+buffer3->cur_num) / 3;
    result.summaryResult(avg,timeCounter->elapsed());
    //将数据写回数据库
    updateRes2DB();

    ui->actExportResult->setEnabled(true);
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

void MainWindow::on_actExportResult_triggered()
{
    result.resultInfo();
    //TODO：没想好做啥
    QString fileName = QFileDialog::getSaveFileName(this,"选择保存路径",
                                                    "result.html",
                                                    "Echart files");
    if(!fileName.isEmpty()){
        //TODO: 生成echart图表
//        dao.sqlExecute(QString("insert into result values(%1,%2,%3,%4,%5,%6)")
//                       .arg(result.r_id).arg(result.run_time).arg(result.curr_data_num)
//                       .arg(result.putin_data_num).arg(result.getout_data_num).arg(result.avg_num));

        QMessageBox::information(this,"提示信息","数据导出完成，图表保存在"+fileName);
    }
}

void MainWindow::on_actAnalyse_triggered()
{
    if(analyseWindow == nullptr)
        analyseWindow = new AnalyseWindow(this);
    analyseWindow->setWindowFlags(analyseWindow->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    analyseWindow->show();
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

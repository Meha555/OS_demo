#include "mainwindow.h"
#include "ui_mainwindow.h"

bool first_start = true;

void MainWindow::initalizeData() {
    cfgDao->sqlExecute("UPDATE sqlite_sequence SET seq = 0 WHERE name='config'");
    msgDao->sqlExecute("UPDATE sqlite_sequence SET seq = 0 WHERE name='message'");
    resDao->sqlExecute("UPDATE sqlite_sequence SET seq = 0 WHERE name='result'");
}

void MainWindow::resetStatistics() {
    gatherer->speed_coefficient = 1000;  // 速度系数(ms)

    gatherer->thread_put_num = config.put_num;    // put操作个数
    gatherer->thread_move_num = config.move_num;  // move操作个数
    gatherer->thread_get_num = config.get_num;    // get操作个数

    QVector<int>(1).swap(gatherer->put_blocked_num);   // 当前阻塞的put线程数
    QVector<int>(1).swap(gatherer->move_blocked_num);  // 当前阻塞的move线程数
    QVector<int>(1).swap(gatherer->get_blocked_num);   // 当前阻塞的get线程数

    ui->bufBall1->resetProgress();
    ui->bufBall2->resetProgress();
    ui->bufBall3->resetProgress();

        ui->plainText_log1->clear();
        ui->plainText_log2->clear();
        ui->plainText_log3->clear();
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    qRegisterMetaType<QTextCursor>("QTextCursor");

    cfgDao = new ConfigDaoImpl(dbName, this);
    msgDao = new MessageDaoImpl(dbName, this);
    resDao = new ResultDaoImpl(dbName, this);

    // 界面相关设置
    label1 = new QLabel("操作速度控制：", this);
    //    slider = new QSlider(Qt::Horizontal, this);
    slider = new QtMaterialSlider(this);
    label2 = new QLabel("慢 ", this);
    gatherer = StatGatherer::instance();
    labelSpeed = new QLabel(QString(" 当前速度：%1x").arg(static_cast<qreal>(gatherer->speed_coefficient / 1000), 0, 'g', 2));
    labelSpeed->setFixedSize(140, 20);
    labelSpeed->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // 设置大小策略为固定大小
    label3 = new QLabel(" 快", this);
    slider->setRange(MIN_SPEED,MAX_SPEED);
    slider->setValue(CUR_SPEED);    // 设置滑动条初始值
    slider->setSingleStep(100);
    ui->ckboxLog->setToolTip("是否显示操作日志");
    ui->toolBar->addWidget(label1);
    ui->toolBar->addWidget(label2);
    ui->toolBar->addWidget(slider);
    ui->toolBar->addWidget(label3);
    ui->toolBar->addWidget(labelSpeed);

    ui->actPasue->setEnabled(false);
    ui->actStart->setEnabled(false);
    ui->actRestart->setEnabled(false);
    ui->actTerminate->setEnabled(false);
    ui->actExportResult->setEnabled(true);
    ui->actAnalyse->setEnabled(true);

    timeCounter = new QElapsedTimer();

    // 绘制进度球
    timerWalker = new QTimer(this);

    // 信号相关
    connect(slider, &QSlider::valueChanged, this, &MainWindow::changeSpeed);
    connect(timerWalker, &QTimer::timeout, this, [this]() {
        int putin_num = buffer1->putin_num+buffer2->putin_num+buffer3->putin_num;
        int getout_num = buffer1->getout_num+buffer2->getout_num+buffer3->getout_num;
        result.collectResult(buffer1->cur_num + buffer2->cur_num + buffer3->cur_num, putin_num, getout_num,
                             buffer1->putin_num,buffer2->putin_num,buffer3->putin_num,
                             buffer1->getout_num,buffer2->getout_num,buffer3->getout_num);
    });

    // 最后的准备工作
    initalizeData();
}

MainWindow::~MainWindow() {
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
    delete gatherer;
}

void MainWindow::changeSpeed(int val) {
    gatherer->speed_coefficient = val;
    labelSpeed->setText(QString(" 当前速度：%1x").arg(static_cast<qreal>(gatherer->speed_coefficient) / 1000, 0, 'g', 2));
    qDebug() << "当前速度: " << gatherer->speed_coefficient;
}

// 每隔一段时间就将buffer的情况写入到数据库中
void MainWindow::updateRes2DB() {
    // 写入Buffer内容
    int bf1 = msgDao->batchInsert(buffer1->buffer);
    int bf2 = msgDao->batchInsert(buffer2->buffer);
    int bf3 = msgDao->batchInsert(buffer3->buffer);
    qDebug()<<bf1<<" "<<bf2<<" "<<bf3;
    qDebug() << "向Buffer中插入" << bf1 + bf2 + bf3 << "条记录";
    // 写入运行结果
    resDao->insert(result);
}

// 设置参数
void MainWindow::setConfig() {
    // 创建3个缓冲区
    buffer1 = new Buffer(1,this);
    buffer2 = new Buffer(2,this);
    buffer3 = new Buffer(3,this);

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

    // 初始化Gatherer
    gatherer->buffers.append(buffer1);
    gatherer->buffers.append(buffer2);
    gatherer->buffers.append(buffer3);
    gatherer->setPut_blocked_num(0);
    gatherer->setMove_blocked_num(0);
    gatherer->setGet_blocked_num(0);
    gatherer->setConfig(&config);

    // For gatherer
    connect(timerWalker, &QTimer::timeout, this, [this]() {
        this->gatherer->setTime_staps(timeCounter->elapsed());
        this->gatherer->setBuffer1_data(buffer1->cur_num);
        this->gatherer->setBuffer2_data(buffer2->cur_num);
        this->gatherer->setBuffer3_data(buffer3->cur_num);
    });

    ui->bufBall1->setCapacity(config.buffer1_size);
    ui->bufBall2->setCapacity(config.buffer2_size);
    ui->bufBall3->setCapacity(config.buffer3_size);

    connect(ui->bufBall1, &ProgressBall::clicked,this,[this](){
        if(buf1Form == nullptr) buf1Form = new BufferForm(buffer1,this);
        buf1Form->setWindowTitle("Buffer1数据内容");
        buf1Form->show();
    });
    connect(ui->bufBall2, &ProgressBall::clicked,this,[this](){
        if(buf2Form == nullptr) buf2Form = new BufferForm(buffer2,this);
        buf2Form->setWindowTitle("Buffer2数据内容");
        buf2Form->show();
    });
    connect(ui->bufBall3, &ProgressBall::clicked,this,[this](){
        if(buf3Form == nullptr) buf3Form = new BufferForm(buffer3,this);
        buf3Form->setWindowTitle("Buffer3数据内容");
        buf3Form->show();
    });

    // 初始化各信号量
#ifndef _MY
    empty1 = new QSemaphore(buffer1->capacity);
    full1 = new QSemaphore(0);
    empty2 = new QSemaphore(buffer2->capacity);
    full2 = new QSemaphore(0);
    empty3 = new QSemaphore(buffer3->capacity);
    full3 = new QSemaphore(0);
#endif
#ifdef _MY
    empty1 = new Semaphore(buffer1->capacity);
    full1 = new Semaphore(0);
    empty2 = new Semaphore(buffer2->capacity);
    full2 = new Semaphore(0);
    empty3 = new Semaphore(buffer3->capacity);
    full3 = new Semaphore(0);
#endif
    mutex1 = new QMutex;
    mutex2 = new QMutex;
    mutex3 = new QMutex;

    // For gatherer
#ifdef _MY
    connect(empty1, &Semaphore::blocked, gatherer, &StatGatherer::increPut_blocked_num);    // PUT放不进去
    connect(empty2, &Semaphore::blocked, gatherer, &StatGatherer::increMove_blocked_num);   // MOVE放不进去
    connect(empty3, &Semaphore::blocked, gatherer, &StatGatherer::increMove_blocked_num);   // MOVE放不进去
    connect(full1, &Semaphore::blocked, gatherer, &StatGatherer::increMove_blocked_num);    // MOVE拿不出来
    connect(full2, &Semaphore::blocked, gatherer, &StatGatherer::increGet_blocked_num);     // GET拿不出来
    connect(full3, &Semaphore::blocked, gatherer, &StatGatherer::increGet_blocked_num);     // GET拿不出来
    connect(empty1, &Semaphore::awake, gatherer, &StatGatherer::decrePut_blocked_num);      // PUT放成功
    connect(empty2, &Semaphore::awake, gatherer, &StatGatherer::decreMove_blocked_num);     // MOVE放成功
    connect(empty3, &Semaphore::awake, gatherer, &StatGatherer::decreMove_blocked_num);     // MOVE放成功
    connect(full1, &Semaphore::awake, gatherer, &StatGatherer::decreMove_blocked_num);      // MOVE拿成功
    connect(full2, &Semaphore::awake, gatherer, &StatGatherer::decreGet_blocked_num);       // GET拿成功
    connect(full3, &Semaphore::awake, gatherer, &StatGatherer::decreGet_blocked_num);       // GET拿成功
#endif

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

void MainWindow::on_actStart_triggered() {
    timeCounter->start();
    timerWalker->start(20);
    if (first_start) {
        resetStatistics();
        // 初始化各线程，并启动
        for (int i = 1; i <= config.put_num; i++) {
            qDebug() << "启动1个PUT";
            displayLogTextSys("启动1个PUT");
            Operation* put = new Operation(PUT, config.put_speed, this);
            connect(ui->actTerminate, &QAction::triggered, put, [put]() {put->quit();put->wait();put->deleteLater(); });
//            connect(put, &Operation::putIn1, ui->bufBall1, &ProgressBall::updateProgress, Qt::DirectConnection);
            //            connect(_pTimerUpdate, &QTimer::timeout, put, [put](){put->getStatus();});
            put->start();
            //            put->getTID();
        }
        int movet = ceil(config.move_num / 2.0);
        for (int i = 1; i <= movet; i++) {
            qDebug() << "启动2个MOVE";
            displayLogTextSys("启动2个MOVE");
            Operation* move1 = new Operation(MOVE1_2, config.move_speed, this);
            connect(ui->actTerminate, &QAction::triggered, move1, [move1]() {move1->quit();move1->wait();move1->deleteLater(); });
//            connect(move1, &Operation::getOut1, ui->bufBall1, &ProgressBall::updateProgress, Qt::DirectConnection);
//            connect(move1, &Operation::putIn2, ui->bufBall2, &ProgressBall::updateProgress, Qt::DirectConnection);
            //            connect(_pTimerUpdate, &QTimer::timeout, move1, [move1](){move1->getStatus();});
            Operation* move2 = new Operation(MOVE1_3, config.move_speed, this);
            connect(ui->actTerminate, &QAction::triggered, move2, [move2]() {move2->quit();move2->wait();move2->deleteLater(); });
//            connect(move2, &Operation::getOut1, ui->bufBall1, &ProgressBall::updateProgress, Qt::DirectConnection);
//            connect(move2, &Operation::putIn3, ui->bufBall3, &ProgressBall::updateProgress, Qt::DirectConnection);
            //            connect(_pTimerUpdate, &QTimer::timeout, move2, [move2](){move2->getStatus();});
            move1->start();
            move2->start();
            //            move1->getTID();move2->getTID();
        }
        int gett = config.get_num / 2;
        int gett2 = config.get_num - (gett << 1);
        for (int i = 1; i <= gett; i++) {
            qDebug() << "启动2个GET";
            displayLogTextSys("启动2个GET");
            Operation* get2 = new Operation(GET2, config.get_speed, this);
            Operation* get3 = new Operation(GET3, config.get_speed, this);
            connect(ui->actTerminate, &QAction::triggered, get2, [get2]() {get2->quit();get2->wait();get2->deleteLater(); });
            connect(ui->actTerminate, &QAction::triggered, get3, [get3]() {get3->quit();get3->wait();get3->deleteLater(); });
//            connect(get2, &Operation::getOut2, ui->bufBall2, &ProgressBall::updateProgress, Qt::DirectConnection);
//            connect(get3, &Operation::getOut3, ui->bufBall3, &ProgressBall::updateProgress, Qt::DirectConnection);
            //            connect(_pTimerUpdate, &QTimer::timeout, get, [get](){get->getStatus();});
            get2->start();
            get3->start();
            //            get->getTID();
        }
        if (gett2) {
            qDebug() << "启动1个GET";
            displayLogTextSys("启动1个GET");
            Operation* get2 = new Operation(GET2, config.get_speed, this);
            connect(ui->actTerminate, &QAction::triggered, get2, [get2]() {get2->quit();get2->wait();get2->deleteLater(); });
            get2->start();
//            connect(get2, &Operation::getOut2, ui->bufBall2, &ProgressBall::updateProgress, Qt::DirectConnection);
        }
        first_start = false;
        emit sigRun();
    } else {
        Operation::resumeThread();
        emit sigRun();
    }
    ui->actStart->setEnabled(false);
    ui->actPasue->setEnabled(true);
    ui->actRestart->setEnabled(true);
    ui->actTerminate->setEnabled(true);
}

void MainWindow::on_actPasue_triggered() {
    Operation::pauseThread();
    emit sigPause();
    ui->actStart->setEnabled(true);
    ui->actPasue->setEnabled(false);
}

void MainWindow::on_actTerminate_triggered() {
    if(activateAnalyze){
        QMessageBox::warning(this,"线程安全警告","请先关闭数据分析窗口！");
        return;
    }
    first_start = true;
    Operation::terminateThread();
    emit sigStop();
    qreal avg = qreal(buffer1->cur_num + buffer2->cur_num + buffer3->cur_num) / 3;
    result.summaryResult(avg, timeCounter->elapsed());
    updateRes2DB();
}

void MainWindow::on_actRestart_triggered() {
    if(activateAnalyze){
        QMessageBox::warning(this,"线程安全警告","请先关闭数据分析窗口！");
        return;
    }
    Operation::terminateThread();
    emit sigStop();
    first_start = true;
    on_actStart_triggered();
}

void MainWindow::on_actSetConfig_triggered() {
    if (configFrom == nullptr) configFrom = new ConfigForm(dbName, this);
    configFrom->setWindowFlags(configFrom->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);

    // 接收设置
    connect(configFrom, &ConfigForm::sendConfig, this, [this](Config& cfg) { config = cfg; });

    if (configFrom->exec() == QDialog::Accepted) {
        // 设置参数
        setConfig();
        isConfiged = true;
    }
}

void MainWindow::on_actImportConfig_triggered() {
    if (dao_configForm == nullptr) dao_configForm = new DaoConfigForm(dbName, this);

    // 接收设置
    connect(dao_configForm, &DaoConfigForm::sendConfig, this, [this](Config& cfg) { config = cfg; });

    if (dao_configForm->exec() == QDialog::Accepted) {
        // 设置参数
        setConfig();
        QMessageBox::information(this, "导入参数", "导入参数完成");
        isConfiged = true;
    }
}

void MainWindow::on_actExportResult_triggered() {
    if (dao_resultForm == nullptr) dao_resultForm = new DaoResultForm(dbName, this);
    dao_resultForm->exec();
}

void MainWindow::on_actAnalyse_triggered() {
    if(!isConfiged){
        QMessageBox::warning(this,"缺少数据","请先设置参数！");
        return;
    }
    activateAnalyze = true;
    if (analyseWindow == nullptr) analyseWindow = new AnalyseWindow(this);
    connect(analyseWindow,&AnalyseWindow::close,this,[this](){activateAnalyze = false;});
    analyseWindow->setWindowFlags(analyseWindow->windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    analyseWindow->show();
}

void MainWindow::displayLogTextSys(QString text, bool bold, QColor frontColor) {
    if (ui->sysText_log->blockCount() > 20)
        ui->sysText_log->clear();
    ui->sysText_log->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);  // 设置光标到文本末尾

    // 只滚动QPlainTextEdit的可滚动区域，以便提升性能
    // 获取视口高度和可滚动区域高度
    int viewportHeight = ui->sysText_log->viewport()->height();
    int scrollingHeight = ui->sysText_log->document()->size().height() - viewportHeight;

    insertRichText(ui->sysText_log, text + "\n", bold, frontColor);
    //    ui->sysText_log->insertPlainText(text+"\n");
    // 移动滚动条到底部
    QScrollBar* scrollbar = ui->sysText_log->verticalScrollBar();
    if (scrollbar) {
        scrollbar->setSliderPosition(scrollingHeight);
    }
    QCoreApplication::processEvents(); // 显式声明在这里处理事件，避免程序卡顿
}

void MainWindow::displayLogText(QPlainTextEdit* edit, const QString& text) {
    if(!ui->ckboxLog->isChecked()) return;
    if (edit->blockCount() > 25) {
        edit->clear();
    }
    edit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);

    // 只滚动QPlainTextEdit的可滚动区域，以便提升性能
    // 获取视口高度和可滚动区域高度
    int viewportHeight = edit->viewport()->height();
    int scrollingHeight = edit->document()->size().height() - viewportHeight;

    edit->insertPlainText(text + "\n");

    // 移动滚动条到底部
    QScrollBar* scrollbar = edit->verticalScrollBar();
    if (scrollbar) {
        scrollbar->setSliderPosition(scrollingHeight);
    }
    QCoreApplication::processEvents(); // 显式声明在这里处理事件，避免程序卡顿
}

void MainWindow::on_btnClear1_clicked() {
    ui->plainText_log1->clear();
    ui->plainText_log1->verticalScrollBar()->setSliderPosition(0);
}

void MainWindow::on_btnClear2_clicked() {
    ui->plainText_log2->clear();
    ui->plainText_log2->verticalScrollBar()->setSliderPosition(0);
}

void MainWindow::on_btnClear3_clicked() {
    ui->plainText_log3->clear();
    ui->plainText_log3->verticalScrollBar()->setSliderPosition(0);
}

void MainWindow::insertRichText(QPlainTextEdit* edit, QString text, bool bold, QColor frontColor) {
    QTextCharFormat fmt;
    //    fmt.setFontPointSize(size);//字体大小
    fmt.setFontWeight(bold ? QFont::Bold : QFont::Normal);  // 字体加粗,能保证不会影响到下次的设置
    fmt.setForeground(QBrush(frontColor));                  // 字体色
    edit->mergeCurrentCharFormat(fmt);                      // 文本框使用以上设定
    edit->insertPlainText(text);                            // 文本框添加文本
}


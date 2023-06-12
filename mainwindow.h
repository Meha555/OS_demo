#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QScrollBar>
#include <QDateTime>
#include <QMessageBox>
#include <QSemaphore>
#include <QFileDialog>
#include <QMutex>
#include <QSlider>
#include <QLabel>
#include <QPlainTextEdit>
#include <QTextCharFormat>
#include "config.h"
#include "result.h"
#include "buffer.h"
#include "semaphore.h"
#include "op_thread.h"
#include "configform.h"
#include "daoconfigform.h"
//#include "dao.h"
#include "messagedaoimpl.h"
#include "configdaoimpl.h"

//using namespace dao;

#define _DEBUG

#ifdef _DEBUG
    #include <QDebug>
#endif

#define MIN_SPEED 50
#define MAX_SPEED 5000
#define CUR_SPEED 1000

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void insertRichText(QPlainTextEdit* edit,QString text,
                    bool bold,QColor frontColor);// 单独插入自定义插入字体格式
    void displayLogTextSys(QString text,bool bold = false,QColor frontColor = QColor("blue"));
    void displayLogText(QPlainTextEdit* edit,const QString& text);
    void initalizeData();
    void resetStatistics();

public slots:
    void setConfig();//设置config
    void changeSpeed(int val);
    void updateRes2DB();

private slots:
//    void receiveConfig(Config& cfg);
    void on_actSetConfig_triggered();

    void on_actStart_triggered();

    void on_actImportConfig_triggered();

    void on_actPasue_triggered();

    void on_actTerminate_triggered();

    void on_actRestart_triggered();

    void on_btnClear1_clicked();

    void on_btnClear2_clicked();

    void on_btnClear3_clicked();

    void on_actExportResult_triggered();

public:
    // 缓冲区
    Buffer* buffer1;
    Buffer* buffer2;
    Buffer* buffer3;

    // 同步信号量
//    Semaphore* empty1;
//    Semaphore* full1;

//    Semaphore* empty2;
//    Semaphore* full2;

//    Semaphore* empty3;
//    Semaphore* full3;
    QSemaphore* empty1;
    QSemaphore* full1;

    QSemaphore* empty2;
    QSemaphore* full2;

    QSemaphore* empty3;
    QSemaphore* full3;

    // 互斥信号量
    QMutex* mutex1;
    QMutex* mutex2;
    QMutex* mutex3;

    // 数据库句柄
//    Dao dao;
    dao::ConfigDaoImpl cfgDao;
    dao::MessageDaoImpl msgDao;
//    ResultDaoImpl resDao;

    // 设置信息
    Config config;

    // 运行结果
    Result result;

public:
    Ui::MainWindow *ui;
    QTimer*  timerWalker;      //时间段计时器
    QElapsedTimer *timeCounter; //时间差计时器
    QLabel*  label1;
    QLabel*  label2;
    QLabel*  label3;
    QLabel*  labelSpeed;
    QSlider* slider;
    ConfigForm* configFrom = nullptr;//设置信息对话框
    DaoConfigForm* dao_configForm = nullptr;//导入参数对话框
};
#endif // MAINWINDOW_H

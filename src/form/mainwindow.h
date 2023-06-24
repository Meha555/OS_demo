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
#include "daoresultform.h"
#include "analysewindow.h"
#include "bufferform.h"
#include "statgatherer.h"

//#include "dao.h"
#include "messagedaoimpl.h"
#include "configdaoimpl.h"
#include "resultdaoimpl.h"

// 美化SDK
#include "qtmaterialslider.h"

#include "utils.hpp"

#define MIN_SPEED 50
#define MAX_SPEED 5000
#define CUR_SPEED 1000

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// 前向声明
class ConfigForm;
class DaoConfigForm;
class AnalyseWindow;

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

signals:
    void sigPause();
    void sigRun();
    void sigStop();
    void sendStatictics();


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

    void on_actAnalyse_triggered();

public:
    // 缓冲区
    Buffer* buffer1;
    Buffer* buffer2;
    Buffer* buffer3;

    /* 以下采用集中式锁 */
    // 同步信号量
#ifdef _MY
    Semaphore* empty1;
    Semaphore* full1;

    Semaphore* empty2;
    Semaphore* full2;

    Semaphore* empty3;
    Semaphore* full3;
#endif
#ifndef _MY
    QSemaphore* empty1;
    QSemaphore* full1;

    QSemaphore* empty2;
    QSemaphore* full2;

    QSemaphore* empty3;
    QSemaphore* full3;
#endif

    // 互斥信号量
    QMutex* mutex1;
    QMutex* mutex2;
    QMutex* mutex3;

    // 数据库句柄
//    Dao dao;
    ConfigDaoImpl *cfgDao;
    MessageDaoImpl *msgDao;
    ResultDaoImpl *resDao;

    // 设置信息
    Config config;

    // 运行结果
    Result result;

    bool isConfiged = false;//设置了参数
    bool activateAnalyze = false;//设置了参数

public:
    Ui::MainWindow *ui;
    QTimer*  timerWalker;       //时间段计时器
    QElapsedTimer *timeCounter; //时间差计时器
    QLabel*  label1;
    QLabel*  label2;
    QLabel*  label3;
    QLabel*  labelSpeed;
//    QSlider* slider;
    QtMaterialSlider* slider;
    QString dbName = "G://Code//QT//os_demo//src//db//CP_db.db";
    StatGatherer* gatherer; // 数据收集器单例
    ConfigForm* configFrom = nullptr;//设置信息对话框
    DaoConfigForm* dao_configForm = nullptr;//导入参数对话框
    DaoResultForm* dao_resultForm = nullptr;//查询结果对话框
    AnalyseWindow* analyseWindow = nullptr;//数据分析窗口
    BufferForm* buf1Form = nullptr;
    BufferForm* buf2Form = nullptr;
    BufferForm* buf3Form = nullptr;
};
#endif // MAINWINDOW_H

#ifndef RESULT_H
#define RESULT_H

#include <QObject>
#include <QElapsedTimer>
#include <QVariant>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

#define _DEBUG

class Result
{
public:
    Result();
    Result(int cur);
    Result(QString run,int cur,int put,int get,qreal avg);
    Result(int id, QString run,int cur,int put,int get,qreal avg);
    Result(int id, QString run,int cur,int put,int get,qreal avg,int put_bf1,int put_bf2,int put_bf3,int get_bf1,int get_bf2,int get_bf3);
    ~Result() = default;

    void resultInfo();

    int getR_id() const;
    void setR_id(int value);

    QString getRun_time();
    void setRun_time(const QString& value);

    int getCurr_data_num() const;
    void setCurr_data_num(int value);

    int getPutin_data_num() const;
    void setPutin_data_num(int value);

    int getGetout_data_num() const;
    void setGetout_data_num(int value);

    qreal getAvg_num() const;
    void setAvg_num(qreal value);

public slots:
    // 运行时的即时结果：主线程中进度球的update执行时调用
    void collectResult(int cur,int putin,int getout,int put_bf1,int put_bf2,int put_bf3,int get_bf1,int get_bf2,int get_bf3);
    // 结束时的汇总结果：主线程中点击结束按钮触发
    void summaryResult(qreal avg,qint64 time);

public:
    int r_id=0;
    QString run_time; //总运行时间【最终统计】
    int curr_data_num = 0;//当前buffer中数据总数【即时统计】
    int putin_data_num = 0; //已放入的数据总数【即时统计】
    int getout_data_num = 0;//已取出的数据总数【即时统计】
    qreal avg_num = 0;//平均每个buffer中的数据个数【最终统计】

    int putin_buf1 = 0;
    int putin_buf2 = 0;
    int putin_buf3 = 0;
    int getout_buf1 = 0;
    int getout_buf2 = 0;
    int getout_buf3 = 0;
};

// 注册入元对象系统，以便QVariant使用
Q_DECLARE_METATYPE(Result)

#endif // RESULT_H

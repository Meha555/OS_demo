#ifndef RESULT_H
#define RESULT_H

#include <QElapsedTimer>
#include <QVariant>
#include <QString>
#include <QObject>

class Result
{
public:
    Result();
    Result(int cur);
    Result(QString run,int cur,int put,int get,int avg);
    ~Result() = default;

    int getR_id() const;
    void setR_id(int value);

    QString getRun_time() const;
    void setRun_time(const QString& value);

    int getCurr_data_num() const;
    void setCurr_data_num(int value);

    int getPutin_data_num() const;
    void setPutin_data_num(int value);

    int getGetout_data_num() const;
    void setGetout_data_num(int value);

    int getAvg_num() const;
    void setAvg_num(int value);

public slots:
    //运行结束时保存结果：主线程中进度球的update执行时调用
    void collectResult(int cur,int putin,int getout,int avg);

public:
    QElapsedTimer timeCounter;
    static int r_id;
private:
    QString run_time = 0; //总运行时间
    int curr_data_num = 0;//当前buffer中数据总数
    int putin_data_num = 0; //已放入的数据总数
    int getout_data_num = 0;//已取出的数据总数
    int avg_num = 0;//平均每个buffer中的数据个数

};

// 注册入元对象系统，以便QVariant使用
Q_DECLARE_METATYPE(Result)

#endif // RESULT_H

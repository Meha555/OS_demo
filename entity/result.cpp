#include "result.h"

int Result::r_id = 0;

Result::Result()
{
    r_id++;
}

Result::Result(int cur):curr_data_num(cur)
{
    r_id++;
}

#ifdef _DEBUG
#include <QDebug>
void Result::resultInfo(){
    qDebug()<<"result: "<<r_id<<' '
           <<run_time<<' '
          <<curr_data_num<<' '
         <<putin_data_num<<' '
        <<getout_data_num<<' '
         <<avg_num;
}
#endif

Result::Result(QString run, int cur, int put, int get, qreal avg)
    :run_time(run),curr_data_num(cur),putin_data_num(put),
      getout_data_num(get),avg_num(avg)
{
    r_id++;
}
// 将整个统计结果生成为JSON字符串，以便提供给Python解析后绘制Echart图表
QString Result::toJsonString()
{

}
// 运行时的即时结果：主线程中进度球的update执行时调用
void Result::collectResult(int cur, int putin, int getout)
{
    curr_data_num = cur;
    putin_data_num = putin;
    getout_data_num = getout;
}
// 结束时的汇总结果：主线程中点击结束按钮触发
void Result::summaryResult(qreal avg,qint64 time)
{
    avg_num = avg;
    run_time = QString::number(time);
}

int Result::getR_id() const
{
    return r_id;
}

void Result::setR_id(int value)
{
    r_id = value;
}

QString Result::getRun_time()
{
    return run_time;
}

int Result::getCurr_data_num() const
{
    return curr_data_num;
}

void Result::setCurr_data_num(int value)
{
    curr_data_num = value;
}

int Result::getPutin_data_num() const
{
    return putin_data_num;
}

void Result::setPutin_data_num(int value)
{
    putin_data_num = value;
}

int Result::getGetout_data_num() const
{
    return getout_data_num;
}

void Result::setGetout_data_num(int value)
{
    getout_data_num = value;
}

qreal Result::getAvg_num() const
{
    return avg_num;
}

void Result::setAvg_num(qreal value)
{
    avg_num = value;
}


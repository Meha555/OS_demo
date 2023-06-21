#ifndef STATITICS_H
#define STATITICS_H

#include <QObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
//#include "utils.hpp"
#include "statgatherer.h"

//using ProfileType = enum {DATA_CHANGE_TREND,DATA_DISTRIBUTION,THREAD_STATE_TREND};

//typedef struct{
//    QString buf;
//    ProfileType type;
//} ChartParam;

class StatGatherer;

// 绘图所需数据 - 父类
class ChartStat
{
public:
    explicit ChartStat(const ChartParam& param);
    virtual ~ChartStat() = default;//定义虚析构函数，防止内存泄漏

    virtual bool readJson(const QString& json) = 0;
    virtual void writeJson() = 0;

    void recognize(const QJsonObject& obj);//识别是哪种图

public:
    StatGatherer* _data_source;
    QJsonObject json;
    ChartParam param;
};

#endif // STATITICS_H

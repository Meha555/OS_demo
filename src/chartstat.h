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
    virtual QByteArray writeJson() = 0;

    static ChartParam recognize(const QJsonObject& obj){
        return {.buf = obj["param"].toObject()["buf"].toString(),
                .type = static_cast<ProfileType>(obj["param"].toObject()["type"].toInt())};
    }

public:
    StatGatherer* _data_source;
    QJsonObject json;
    ChartParam param;
};

#endif // STATITICS_H

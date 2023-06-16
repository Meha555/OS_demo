#ifndef STATITICS_H
#define STATITICS_H

#include <QObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include "statgatherer.h"

// 绘图所需数据 - 父类
class ChartStat
{
public:
    explicit ChartStat();

    virtual bool readJson(const QString& json) = 0;
    virtual void writeJson() = 0;

public:
    StatGatherer* _data_source;
    QJsonObject json;
};

#endif // STATITICS_H

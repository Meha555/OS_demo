#ifndef CONFIGDAOIMPL_H
#define CONFIGDAOIMPL_H

#include <QVector>
#include <QQueue>
#include "dao.h"
#include "config.h"

//namespace dao{
//    class ConfigDaoImpl;
//}

class ConfigDaoImpl : public Dao
{
public:
    ConfigDaoImpl(const QString& dbName, QObject *parent = nullptr);
    ConfigDaoImpl(const QString& dbName, QString tabName, QObject *parent = nullptr);
    QVector<Config> findAll();
    Config findByID(int c_id);
    int deleteByID(int c_id);
    int insert(Config& cfg);
};

#endif // CONFIGDAOIMPL_H

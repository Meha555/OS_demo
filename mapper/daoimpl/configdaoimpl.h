#ifndef CONFIGDAOIMPL_H
#define CONFIGDAOIMPL_H

#include <QVector>
#include <QPair>
#include "dao.h"
#include "config.h"

class ConfigDaoImpl : public Dao
{
public:
    ConfigDaoImpl()=default;
    QPair<Config,bool> findByID(int id);//需要一个bool值来判断是真数据还是假数据
    QVector<Config> findAll();
};

#endif // CONFIGDAOIMPL_H

#ifndef RESULTDAOIMPL_H
#define RESULTDAOIMPL_H

#include "dao.h"
#include "result.h"

class ResultDaoImpl : public Dao
{
public:
    ResultDaoImpl(const QString &dbName, QObject *parent = nullptr);
    ResultDaoImpl(const QString &dbName, const QString& tabName,QObject *parent = nullptr);
    QVector<Result> findAll();
    Result findByID(int r_id);
    int deleteByID(int r_id);
    int insert(Result& res);
};

#endif // RESULTDAOIMPL_H

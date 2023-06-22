#include "resultdaoimpl.h"

ResultDaoImpl::ResultDaoImpl(const QString &dbName, QObject *parent)
    : Dao(dbName,"Result",parent)
{
}

ResultDaoImpl::ResultDaoImpl(const QString &dbName, const QString& tabName, QObject* parent)
    : Dao(dbName,"Result",parent)
{
}

QVector<Result> ResultDaoImpl::findAll()
{
    qDebug()<<"Curr DB is: "<<tableName;
    QVector<Result> ret;
    if(getConnection()){
        if(sqlHandler.exec("select * from result")){
            while(sqlHandler.next()){
                Result res(
                            sqlHandler.value("r_id").toInt(),
                            sqlHandler.value("run_time").toString(),
                            sqlHandler.value("cur_num").toInt(),
                            sqlHandler.value("putin_num").toInt(),
                            sqlHandler.value("getout_num").toInt(),
                            sqlHandler.value("putin_buf1").toInt(),
                            sqlHandler.value("putin_buf2").toInt(),
                            sqlHandler.value("putin_buf3").toInt(),
                            sqlHandler.value("getout_buf1").toInt(),
                            sqlHandler.value("getout_buf2").toInt(),
                            sqlHandler.value("getout_buf3").toInt(),
                            sqlHandler.value("avg_num").toReal());//创建一个Result对象返回
                ret.push_back(res);
                res.resultInfo();
            }
        }else{
            QMessageBox::critical(0, "数据库元组查询失败",
                                  sqlHandler.lastError().text());
            qCritical()<<sqlHandler.lastError().text();
        }
        for(auto& ele: ret){
            ele.resultInfo();
        }
        sqlHandler.finish();
    }
    return ret;
}

Result ResultDaoImpl::findByID(int r_id)
{
    qDebug()<<"Curr DB is: "<<tableName;
    if(getConnection()){
        if(sqlHandler.exec("select * from result where r_id="+QString::number(r_id))){
            sqlHandler.next();
            Result res(
                        sqlHandler.value("r_id").toInt(),
                        sqlHandler.value("run_time").toString(),
                        sqlHandler.value("cur_num").toInt(),
                        sqlHandler.value("putin_num").toInt(),
                        sqlHandler.value("getout_num").toInt(),
                        sqlHandler.value("putin_buf1").toInt(),
                        sqlHandler.value("putin_buf2").toInt(),
                        sqlHandler.value("putin_buf3").toInt(),
                        sqlHandler.value("getout_buf1").toInt(),
                        sqlHandler.value("getout_buf2").toInt(),
                        sqlHandler.value("getout_buf3").toInt(),
                        sqlHandler.value("avg_num").toReal());//创建一个Result对象返回
            return res;
        }
        else{
            QMessageBox::critical(0, "数据库元组查询失败",
                                  sqlHandler.lastError().text());
            qCritical()<<sqlHandler.lastError().text();
            return Result();
        }
        sqlHandler.finish();
    }return Result();
}

int ResultDaoImpl::deleteByID(int r_id)
{
    qDebug()<<"Curr DB is: "<<tableName;
    if(getConnection()){
        if(sqlHandler.exec("delete from result where r_id="+QString::number(r_id))){
            return 1;
        }
        else{
            QMessageBox::critical(0, "数据库元组删除失败",
                                  sqlHandler.lastError().text());
            qCritical()<<sqlHandler.lastError().text();
            return EXECUTEION_ERR;
        }
        sqlHandler.finish();
    }else return CONNECTION_ERR;
}

int ResultDaoImpl::insert(Result &res)
{
    qDebug()<<"Curr DB is: "<<tableName;
    if(getConnection()){
        if(sqlHandler.exec(QString("INSERT INTO result(run_time, cur_num, putin_num, getout_num, putin_buf1, putin_buf2, putin_buf3, getout_buf1, getout_buf2, getout_buf3, avg_num) VALUES(%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11)")
                           .arg(res.run_time).arg(res.curr_data_num).arg(res.putin_data_num).arg(res.getout_data_num)
                           .arg(res.putin_buf1).arg(res.putin_buf2).arg(res.putin_buf3)
                           .arg(res.getout_buf1).arg(res.getout_buf2).arg(res.getout_buf3).arg(res.avg_num))){
            return 1;
        }
        else{
            QMessageBox::critical(0, "数据库元组插入失败",
                                  sqlHandler.lastError().text());
            qCritical()<<sqlHandler.lastError().text();
            return EXECUTEION_ERR;
        }
        sqlHandler.finish();
    }else return CONNECTION_ERR;
}

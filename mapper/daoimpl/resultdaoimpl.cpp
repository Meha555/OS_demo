#include "resultdaoimpl.h"

ResultDaoImpl::ResultDaoImpl(QObject* parent): Dao(parent)
{
    tableName = "Result";
}

ResultDaoImpl::ResultDaoImpl(const QString &tabName, QObject *parent):Dao(tabName,parent)
{}

QVector<Result> ResultDaoImpl::findAll()
{
    qDebug()<<"Curr DB is: "<<tableName;
    if(!db.isOpen()) getConnection();
    QVector<Result> ret;
    if(sqlHandler.exec("select * from result")){
        while(sqlHandler.next()){
            Result res(
                        sqlHandler.value("r_id").toInt(),
                        sqlHandler.value("run_time").toString(),
                        sqlHandler.value("cur_num").toInt(),
                        sqlHandler.value("putin_num").toInt(),
                        sqlHandler.value("getout_num").toInt(),
                        sqlHandler.value("avg_num").toReal());//创建一个Result对象返回
            ret.push_back(res);
            res.resultInfo();
        }
    }else{
        QMessageBox::critical(0, "数据库元组查询失败",
                              sqlHandler.lastError().text());
        qCritical()<<sqlHandler.lastError();
    }
    for(auto& ele: ret){
        ele.resultInfo();
    }
    return ret;
}

Result ResultDaoImpl::findByID(int r_id)
{
    qDebug()<<"Curr DB is: "<<tableName;
    if(!db.isOpen()) getConnection();
    if(sqlHandler.exec("select * from result where r_id="+QString::number(r_id))){
        Result res(
                    sqlHandler.value("r_id").toInt(),
                    sqlHandler.value("run_time").toString(),
                    sqlHandler.value("curr_data_num").toInt(),
                    sqlHandler.value("putin_data_num").toInt(),
                    sqlHandler.value("getout_data_num").toInt(),
                    sqlHandler.value("avg_num").toReal());//创建一个Result对象返回
        return res;
    }
    else{
        QMessageBox::critical(0, "数据库元组查询失败",
                              sqlHandler.lastError().text());
        qCritical()<<sqlHandler.lastError();
        return Result();
    }
}

int ResultDaoImpl::deleteByID(int r_id)
{
    qDebug()<<"Curr DB is: "<<tableName;
    if(!db.isOpen()) getConnection();
    if(sqlHandler.exec("delete from result where r_id="+QString::number(r_id))){
        return 1;
    }
    else{
        QMessageBox::critical(0, "数据库元组删除失败",
                              sqlHandler.lastError().text());
        qCritical()<<sqlHandler.lastError();
        return 0;
    }
}

int ResultDaoImpl::insert(Result &res)
{
    qDebug()<<"Curr DB is: "<<tableName;
    if(!db.isOpen()) getConnection();
    if(sqlHandler.exec(QString("INSERT INTO result(run_time,cur_num,putin_num,getout_num,avg_num) VALUES(%1,%2,%3,%4,%5)")
                       .arg(res.run_time).arg(res.curr_data_num).arg(res.putin_data_num)
                       .arg(res.getout_data_num).arg(res.avg_num))){
        return 1;
    }
    else{
        QMessageBox::critical(0, "数据库元组插入失败",
                              sqlHandler.lastError().text());
        qCritical()<<sqlHandler.lastError();
        return 0;
    }
}

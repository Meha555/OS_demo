#include "configdaoimpl.h"


QPair<Config,bool> ConfigDaoImpl::findByID(int id)
{
    QString sql = "select c_id from config where c_id = "+QString::number(id);
    QPair<Config,bool> res;
    Config cfg;
    if(sqlQuery(sql)){
        return {Config(resultSet.value("buffer1_size").toInt(),
                       resultSet.value("buffer2_size").toInt(),
                       resultSet.value("buffer3_size").toInt(),
                       resultSet.value("put_num").toInt(),
                       resultSet.value("put_speed").toInt(),
                       resultSet.value("move_num").toInt(),
                       resultSet.value("move_speed").toInt(),
                       resultSet.value("get_num").toInt(),
                       resultSet.value("get_speed").toInt()),true};
    }
    else return {Config(),false};
}

QVector<Config> ConfigDaoImpl::findAll()
{
    QString sql = "select * from config";
    QVector<Config> res;
    if(sqlQuery(sql)){
        for(int i=1;i<=resultSet.count();i++){

        }
    }
    return res;
}

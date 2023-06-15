#include "configform.h"
#include "ui_configform.h"

ConfigForm::ConfigForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigForm)
{
    ui->setupUi(this);
    ui->btnOK->setRole(Material::Secondary);
    ui->btnCancel->setRole(Material::Primary);
    cfgDao = new ConfigDaoImpl(this);
//    dao.setTableName("Config");
}

ConfigForm::~ConfigForm()
{
    delete ui;
}

void ConfigForm::on_btnOK_clicked()
{
    Config cfg(ui->lineEdit_buf1size->text().toInt(),
               ui->lineEdit_buf2size->text().toInt(),
               ui->lineEdit_buf3size->text().toInt(),
               ui->lineEdit_putNum->text().toInt(),
               ui->lineEdit_putSpeed->text().toInt(),
               ui->lineEdit_moveNum->text().toInt(),
               ui->lineEdit_moveSpeed->text().toInt(),
               ui->lineEdit_getNum->text().toInt(),
               ui->lineEdit_getSpeed->text().toInt());
    if(cfg.buffer1_size <= 0 || cfg.buffer2_size <= 0 || cfg.buffer3_size <= 0
            || cfg.put_num <= 0 || cfg.put_speed <= 0
            || cfg.move_num <= 0 || cfg.move_speed <= 0
            || cfg.get_num <= 0 || cfg.get_speed <= 0){
        QMessageBox::warning(this,"输入错误","请全部填写大于0的数据");
        return;
    }
//    dao.sqlExecute(QString("insert into config values(%1,%2,%3,%4,%5,%6,%7,%8,%9,%10)")
//                   .arg(cfg.c_id).arg(cfg.buffer1_size).arg(cfg.buffer2_size).arg(cfg.buffer3_size)
//                   .arg(cfg.put_num).arg(cfg.put_speed)
//                   .arg(cfg.move_num).arg(cfg.move_speed)
//                   .arg(cfg.get_num).arg(cfg.get_speed));
    cfgDao->insert(cfg);
    emit sendConfig(cfg);
    this->accept();
}

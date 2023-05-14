#include "configform.h"
#include "ui_configform.h"

ConfigForm::ConfigForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigForm)
{
    ui->setupUi(this);
    dao.setTableName("Config");
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
    dao.sqlExecute(QString("insert into config values(%1,%2,%3,%4,%5,%6,%7,%8,%9,%10)")
                   .arg(cfg.c_id).arg(cfg.buffer1_size).arg(cfg.buffer2_size).arg(cfg.buffer3_size)
                   .arg(cfg.put_num).arg(cfg.put_speed)
                   .arg(cfg.move_num).arg(cfg.move_speed)
                   .arg(cfg.get_num).arg(cfg.get_speed));
    emit sendConfig(cfg);
    this->accept();
}

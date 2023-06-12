#include "daoconfigform.h"
#include "ui_daoconfigform.h"

void DaoConfigForm::selectData()
{
    sqlTabModel->select();
    Config::c_id = sqlTabModel->columnCount();
    qDebug()<<"查找了数据库";
}

DaoConfigForm::DaoConfigForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DaoConfigForm)
{
    ui->setupUi(this);
    launchFlag = true;
//    dao.setTableName("Config");//设置表名
    connect(ui->tableView,&QTableView::clicked,this,[this](){qDebug()<<ui->tableView->currentIndex().row();});
//    res_set = dao.sqlQuery("select * from config");
//    for(auto ele:res_set){
//        Config t = ele.value<Config>();
//        t.configInfo();
//    }

//    sqlTabModel = new QSqlTableModel(this,dao.db);
    sqlTabModel = new QSqlTableModel(this,cfgDao.db);
//    dao.getConnection();
    cfgDao.getConnection();
    ui->tableView->setModel(sqlTabModel);
    sqlTabModel->setTable("config");//设置表名
    ui->tableView->resizeColumnsToContents();//自动调整列宽
    ui->tableView->resizeRowsToContents();//自动调整行距
    ui->tableView->setAlternatingRowColors(true);//设置行间交叉颜色
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setSelectionMode ( QAbstractItemView::SingleSelection);//禁止选择多行
    sqlTabModel->setHeaderData(0,Qt::Horizontal,"设置ID");
    sqlTabModel->setHeaderData(1,Qt::Horizontal,"Buffer1容量");
    sqlTabModel->setHeaderData(2,Qt::Horizontal,"Buffer2容量");
    sqlTabModel->setHeaderData(3,Qt::Horizontal,"Buffer3容量");
    sqlTabModel->setHeaderData(4,Qt::Horizontal,"put操作数");
    sqlTabModel->setHeaderData(5,Qt::Horizontal,"move操作数");
    sqlTabModel->setHeaderData(6,Qt::Horizontal,"get操作数");
    sqlTabModel->setHeaderData(7,Qt::Horizontal,"put操作速度");
    sqlTabModel->setHeaderData(8,Qt::Horizontal,"move操作速度");
    sqlTabModel->setHeaderData(9,Qt::Horizontal,"get操作速度");
    if(launchFlag) selectData();
}

DaoConfigForm::~DaoConfigForm()
{
    delete ui;
}

void DaoConfigForm::showEvent(QShowEvent *event)
{
    if(launchFlag) selectData();
    QDialog::showEvent(event);
}

void DaoConfigForm::on_btnChose_clicked()
{
    Config cfg;
    //查数据库，得到一个Config对象cfg
//    cfg = dao.sqlQuery("select * from config").at(ui->tableView->currentIndex().row()).value<Config>();
//    for(int i = 0;i < sqlTabModel->columnCount();i++) {
//        sqlTabModel->data(sqlTabModel->index(ui->tableView->currentIndex().row(),i)).toString();
//    }
    cfg = cfgDao.findAll().at(ui->tableView->currentIndex().row());
    emit sendConfig(cfg);
    this->accept();
}

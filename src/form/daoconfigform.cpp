#include "daoconfigform.h"
#include "ui_daoconfigform.h"

void DaoConfigForm::selectData()
{
    cfgDao->getConnection();
    sqlTabModel->select();
}

DaoConfigForm::DaoConfigForm(const QString& dbName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DaoConfigForm)
{
    ui->setupUi(this);
    ui->btnChose->setRole(Material::Secondary);
    ui->btnSubmit->setRole(Material::Primary);
    ui->btnDelete->setRole(Material::Primary);
    launchFlag = true;
    cfgDao = new ConfigDaoImpl(dbName,this);
    sqlTabModel = new QSqlTableModel(this,cfgDao->db);
    cfgDao->getConnection();
    ui->tableView->setModel(sqlTabModel);
    sqlTabModel->setTable("config");//设置表名
    ui->tableView->resizeColumnsToContents();//自动调整列宽
    ui->tableView->resizeRowsToContents();//自动调整行距
    ui->tableView->setAlternatingRowColors(true);//设置行间交叉颜色
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);//禁止选择多行
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);//设置QTableView双击可修改
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
    cfg = cfgDao->findByID(ui->tableView->currentIndex().row()+1);
    emit sendConfig(cfg);
    this->accept();
}

void DaoConfigForm::on_btnSubmit_clicked()
{
    sqlTabModel->submitAll();
//    sqlTabModel->select();
}

void DaoConfigForm::on_btnDelete_clicked()
{
    QModelIndexList selectedIndex = ui->tableView->selectionModel()->selectedIndexes();
    if (!selectedIndex.isEmpty()) {
        QMessageBox::StandardButton button = QMessageBox::question(this,
            tr("确认删除"), tr("你确定要删除所选行吗?"), QMessageBox::Yes | QMessageBox::No);
        if (button == QMessageBox::Yes) {
            //按照row从大到小排序，依次删除，否则行数会乱掉，导致ID错乱
            std::sort(selectedIndex.begin(), selectedIndex.end(),
                      [&](const QModelIndex& index1, const QModelIndex& index2)
                      {return index1.row() > index2.row();});
            for (const QModelIndex& index : selectedIndex) {
                sqlTabModel->removeRow(index.row());
            }
            sqlTabModel->submitAll();//提交修改
            sqlTabModel->select();
        }
    } else {
        QMessageBox::information(this, tr("提示"), tr("请至少选择一行进行删除！"));
    }
}

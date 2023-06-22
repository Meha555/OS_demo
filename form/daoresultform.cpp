#include "daoresultform.h"
#include "ui_daoresultform.h"

DaoResultForm::DaoResultForm(const QString& dbName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DaoResultForm)
{
    ui->setupUi(this);
    ui->btnDelete->setRole(Material::Secondary);
    resDao = new ResultDaoImpl(dbName,this);
    sqlTabModel = new QSqlTableModel(this,resDao->db);
    resDao->getConnection();
    ui->tableView->setModel(sqlTabModel);
    sqlTabModel->setTable("result");//设置表名
    ui->tableView->resizeColumnsToContents();//自动调整列宽
    ui->tableView->resizeRowsToContents();//自动调整行距
    ui->tableView->setAlternatingRowColors(true);//设置行间交叉颜色
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    sqlTabModel->setHeaderData(0,Qt::Horizontal,"结果ID");
    sqlTabModel->setHeaderData(1,Qt::Horizontal,"运行时间(ms)");
    sqlTabModel->setHeaderData(2,Qt::Horizontal,"当前剩余数据总数");
    sqlTabModel->setHeaderData(3,Qt::Horizontal,"已放入的数据总数");
    sqlTabModel->setHeaderData(4,Qt::Horizontal,"已取出的数据总数");
    sqlTabModel->setHeaderData(5,Qt::Horizontal,"已放入buf1的个数");
    sqlTabModel->setHeaderData(6,Qt::Horizontal,"已放入buf2的个数");
    sqlTabModel->setHeaderData(7,Qt::Horizontal,"已放入buf3的个数");
    sqlTabModel->setHeaderData(8,Qt::Horizontal,"已取出buf1的个数");
    sqlTabModel->setHeaderData(9,Qt::Horizontal,"已取出buf2的个数");
    sqlTabModel->setHeaderData(10,Qt::Horizontal,"已取出buf3的个数");
    sqlTabModel->setHeaderData(11,Qt::Horizontal,"平均每个Buffer中的数据个数");
    selectData();
}

void DaoResultForm::selectData()
{
    resDao->getConnection();
    sqlTabModel->select();
}

void DaoResultForm::showEvent(QShowEvent *event)
{
    selectData();
    QDialog::showEvent(event);
}

DaoResultForm::~DaoResultForm()
{
    delete ui;
}

void DaoResultForm::on_btnDelete_clicked()
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


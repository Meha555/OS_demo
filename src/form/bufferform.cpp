#include "bufferform.h"
#include "ui_bufferform.h"

BufferForm::BufferForm(Buffer* buf,QWidget *parent) : buffer(buf),QDialog(parent),
    ui(new Ui::BufferForm)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setHorizontalHeaderLabels({"线程ID","线程类型","数据内容"});
    ui->tableWidget->setColumnWidth(0,80);
    ui->tableWidget->setColumnWidth(1,80);
    ui->tableWidget->setColumnWidth(2,120);
    connect(buffer,&Buffer::update,this,&BufferForm::display,Qt::DirectConnection);
}

BufferForm::~BufferForm()
{
    delete ui;
}

void BufferForm::display()
{
    ui->tableWidget->clearContents();
    QString tid, type, data;
    int rowCount = buffer->buffer.size(); // 队列中的元素数量
    // 调整表格的行数
    ui->tableWidget->setRowCount(rowCount);
    int i = 0; // 行索引
    for (Message& msg : this->buffer->buffer) {
        if (msg.b_id == buffer->bid) {
            QTableWidgetItem* item0 = new QTableWidgetItem();
            QTableWidgetItem* item1 = new QTableWidgetItem();
            QTableWidgetItem* item2 = new QTableWidgetItem();
            item0->setText(QString::number(msg.t_id));
            switch (msg.op_type) {
                case PUT: type = "PUT"; break;
                case MOVE1_2:case MOVE1_3: type = "MOVE"; break;
                case GET2:case GET3: type = "GET"; break;
            }
            item1->setText(type);
            item2->setText(msg.data);
            item0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            ui->tableWidget->setItem(i, 0, item0);
            ui->tableWidget->setItem(i, 1, item1);
            ui->tableWidget->setItem(i, 2, item2);
            i++; // 更新行索引
        }
    }
    update();
}


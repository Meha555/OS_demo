#include "ledlabel.h"
#include "ui_ledlabel.h"

const QHash<QString,QString> LedLabel::styleSheet = {
    {"red","min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:red"},
    {"green","min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:lime"},
    {"gray","min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:grey"},
    {"yellow","min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:yellow"}
};

LedLabel::LedLabel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LedLabel)
{
    ui->setupUi(this);
    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);//改小外边距
    setLayout(ui->horizontalLayout);//需要确保LedLabel自身的布局设置是正确的，否则它可能会被隐藏或覆盖
//    pixmap = new QPixmap(16, 16);
//    pixmap->fill(Qt::transparent);
//    painter = new QPainter(pixmap);
//    painter->setRenderHint(QPainter::Antialiasing, true);
//    pen = new QPen(Qt::black);
//    pen->setWidth(1);
//    painter->setPen(*pen);
//    painter->setBrush(QBrush(Qt::gray));
//    painter->drawEllipse(0, 0, 15, 15);
//    painter->end();
//    ui->labLED->setPixmap(*pixmap);
    setStatus(TERMINATED);
}

LedLabel::~LedLabel()
{
    delete ui;
    delete pen;
    delete pixmap;
    delete painter;
}

void LedLabel::setText(const QString &text)
{
    ui->labText->setText(text);
}

void LedLabel::setStatus(const ThreadState& stat)
{
    QString color, tip;
    switch (stat) {
    case RUNNING:color = "green";tip = "运行";break;
    case BLOCK:color = "red";tip = "暂停";break;
    case TERMINATED:color = "gray";tip = "停止";break;
    }
    ui->labLED->setStyleSheet(LedLabel::styleSheet[color]);
    ui->labLED->setWhatsThis(tip);
}

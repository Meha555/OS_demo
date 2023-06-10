#include <QApplication>
#include <QTime>
#include <QtGlobal>
#include <QFont>
#include <QTextCodec>
#include "mainwindow.h"

#include <QVector>
#include "message.h"
#include "messagedaoimpl.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(code);
    QFont font(QString::fromLocal8Bit("华文中宋"),10);
    a.setFont(font);
    MainWindow w;
    w.show();
//    Dao* dao = new MessageDaoImpl("Message");
//    QVector<Message> lm = dynamic_cast<MessageDaoImpl*>(dao)->findAll();
//    qDebug()<<lm.size();
//    for(auto& ele: lm){
//        ele.messageInfo();
//    }
    return a.exec();
}

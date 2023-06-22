#include <QApplication>
#include <QTime>
#include <QtGlobal>
#include <QFont>
#include <QTextCodec>
#include "mainwindow.h"
#include "loginform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(code);
    QFont font(QString::fromLocal8Bit("华文中宋"),10);
    a.setFont(font);
    LoginForm *l = new LoginForm;
    if(l->exec() == QDialog::Accepted){
        MainWindow w;
        w.dbName = l->getDbName();
        w.show();
        delete l;
        return a.exec();
    }
//    MainWindow w;
//    w.show();
//    return a.exec();
    return 0;
}

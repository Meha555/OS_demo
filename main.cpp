#include <QApplication>
#include <QTime>
#include <QtGlobal>
#include <QFont>
#include <QTextCodec>
#include "mainwindow.h"
#include "loginform.h"

#include <QQueue>
#include <QVector>
#include "message.h"
#include "messagedaoimpl.h"
#include "resultdaoimpl.h"
#include <algorithm>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QTextCodec *code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(code);
    QFont font(QString::fromLocal8Bit("华文中宋"),10);
    a.setFont(font);
    // QFile file(QString("%1").arg(":/stylesheet/assets/stylesheet/lightblue.css"));
    // file.open(QFile::ReadOnly);
    // QString css = QLatin1String(file.readAll());
    // qApp->setStyleSheet(css);

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
    return a.exec();

    return 0;
}

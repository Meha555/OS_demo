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

//    LoginForm *l = new LoginForm;
//    if(l->exec() == QDialog::Accepted){
//        MainWindow w;
//        w.dbName = l->getDbName();
//        w.show();
//        delete l;
//        return a.exec();
//    }
    MainWindow w;
    w.show();
    return a.exec();

//    Dao* dao = new MessageDaoImpl("Message");
//    QVector<Message> lm = dynamic_cast<MessageDaoImpl*>(dao)->findAll();
//    ResultDaoImpl resDao;
//    auto t = Result("325143",10,12,13,123.123);
//    resDao.insert(t);
//    QVector<Result> lm = resDao.findAll();
//    std::for_each(lm.begin(),lm.end(),[&resDao](Result& ele)->void{ele.resultInfo();resDao.deleteByID(ele.r_id);});

//    MessageDaoImpl msgDao;
//    QQueue<Message> lm;
//    for(int i = 1;i<=5;i++){
//        lm.enqueue(Message());
//    }
//    msgDao.batchInsert(lm);
//    auto lm = msgDao.findAll();
//    for(auto& ele: lm) ele.messageInfo();
//    msgDao.deleteAll();
    return 0;
}

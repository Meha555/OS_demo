#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QMovie>
#include <QSettings>
#include <QPainter>

namespace Ui {
class LoginForm;
}

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    explicit LoginForm(QDialog *parent = nullptr);
    ~LoginForm();

    QString getDbName() const;
    void setDbName(const QString &value);

    void    readSettings(); //读取设置，从注册表
    void    writeSettings();//写入设置，从注册表

protected:
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::LoginForm *ui;
    QString dbName;
    bool    m_moving=false;//表示窗口是否在鼠标操作下移动
    QPoint  m_lastPos;  //上一次的鼠标位置
    //注册表存放路径：HKEY_CURRENT_USER/Software/{organization}/{appName}
    QString organization = "YSU";
    QString appName = "OS_P&Cdemo";
};

#endif // LOGINFORM_H

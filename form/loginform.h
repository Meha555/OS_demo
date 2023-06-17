#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>

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

protected:
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::LoginForm *ui;
    QString dbName;
    bool    m_moving=false;//表示窗口是否在鼠标操作下移动
    QPoint  m_lastPos;  //上一次的鼠标位置
};

#endif // LOGINFORM_H

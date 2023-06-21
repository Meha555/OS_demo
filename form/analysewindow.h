#ifndef ANALYSEWINDOW_H
#define ANALYSEWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QHash>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "ledlabel.h"
#include "chartform.h"
#include "op_thread.h"

// 美化SDK
#include "qtmaterialraisedbutton.h"

class LedLabel;

namespace Ui {
class AnalyseWindow;
}

class AnalyseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AnalyseWindow(QWidget *parent = nullptr);
    ~AnalyseWindow();
private slots:
    void createTab();

    void on_tabWidget_tabCloseRequested(int index);

    void on_actSelectData_triggered();

    void on_actSnapShot_triggered();

signals:
    void sigStart();
    void sigStop();

private:
    Ui::AnalyseWindow *ui;

public:
    QLabel* labPut;
    QLabel* labMove;
    QLabel* labGet;
    LedLabel* statLED;

    QComboBox* comboxBuffer;
    QComboBox* comboxProfile;
    QComboBox* comboxStyle;
//    QPushButton* btnOK;
//    QPushButton* btnCancel;
//    QPushButton* btnHelp;
    QtMaterialRaisedButton* btnOK;
    QtMaterialRaisedButton* btnClose;
    QtMaterialRaisedButton* btnHelp;

    QString savePath;//JSON保存路径
};

#endif // ANALYSEWINDOW_H

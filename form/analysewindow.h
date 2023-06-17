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
#include <QHash>
#include "chartform.h"
#include "ledlabel.h"
#include "op_thread.h"

// 美化SDK
#include "qtmaterialraisedbutton.h"

class MainWindow;

using ProfileType = enum {DATA_CHANGE_TREND,DATA_DISTRIBUTION,THREAD_STATE_TREND};

struct ChartParam{
    QString buf;
    ProfileType type;
};

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
};

#endif // ANALYSEWINDOW_H

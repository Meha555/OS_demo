#ifndef ANALYSEWINDOW_H
#define ANALYSEWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QGridLayout>
#include <QStringList>
#include <QMessageBox>
#include "chartform.h"

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
    QComboBox* comboxBuffer;
    QComboBox* comboxProfile;
    QComboBox* comboxStyle;
    QPushButton* btnOK;
    QPushButton* btnCancel;
    QPushButton* btnHelp;
};

#endif // ANALYSEWINDOW_H

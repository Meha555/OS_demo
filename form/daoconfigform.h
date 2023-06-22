#ifndef DAOCONFIGFORM_H
#define DAOCONFIGFORM_H

#include <QDialog>
#include <QTableView>
#include <QSqlTableModel>
//#include "dao.h"
#include "config.h"
#include "configdaoimpl.h"

namespace Ui {
class DaoConfigForm;
}

class DaoConfigForm : public QDialog
{
    Q_OBJECT

public:
    explicit DaoConfigForm(const QString& dbName, QWidget *parent = nullptr);
    ~DaoConfigForm();

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void sendConfig(Config& cfg);

private:
    void selectData();

private slots:
    void on_btnChose_clicked();

private:
    Ui::DaoConfigForm *ui;
    QSqlTableModel* sqlTabModel;
    QItemSelectionModel* theSelection;

    bool launchFlag = false;
    ConfigDaoImpl *cfgDao;
};

#endif // DAOCONFIGFORM_H

#ifndef DAOCONFIGFORM_H
#define DAOCONFIGFORM_H

#include <QDialog>
#include <QTableView>
#include <QSqlTableModel>
#include "dao.h"
#include "config.h"

namespace Ui {
class DaoConfigForm;
}

class DaoConfigForm : public QDialog
{
    Q_OBJECT

public:
    explicit DaoConfigForm(QWidget *parent = nullptr);
    ~DaoConfigForm();

signals:
    void sendConfig(Config& cfg);

private slots:
    void on_btnChose_clicked();

//    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::DaoConfigForm *ui;
    QSqlTableModel* sqlTabModel;
    QItemSelectionModel* theSelection;

    Dao dao;
    QVariantList res_set;
};

#endif // DAOCONFIGFORM_H

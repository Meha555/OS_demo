#ifndef DAORESULTFORM_H
#define DAORESULTFORM_H

#include <QDialog>
#include <QSqlTableModel>
#include <QItemSelectionModel>
#include "resultdaoimpl.h"

namespace Ui {
class DaoResultForm;
}

class DaoResultForm : public QDialog
{
    Q_OBJECT

public:
    explicit DaoResultForm(const QString& dbName, QWidget *parent = nullptr);
    ~DaoResultForm();

protected:
    void showEvent(QShowEvent *) override;

private slots:
    void on_btnDelete_clicked();

private:
    void selectData();

private:
    Ui::DaoResultForm *ui;
    QSqlTableModel* sqlTabModel;
    QItemSelectionModel* theSelection;
    ResultDaoImpl *resDao;
};

#endif // DAORESULTFORM_H

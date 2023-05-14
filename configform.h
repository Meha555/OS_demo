#ifndef CONFIGFORM_H
#define CONFIGFORM_H

#include <QDialog>
#include "config.h"
#include "dao.h"

namespace Ui {
class ConfigForm;
}

class ConfigForm : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigForm(QWidget *parent = nullptr);
    ~ConfigForm();

signals:
    void sendConfig(Config&);

private slots:
    void on_btnOK_clicked();

private:
    Ui::ConfigForm *ui;
    Dao dao;
};

#endif // CONFIGFORM_H

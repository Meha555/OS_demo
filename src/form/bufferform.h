#ifndef BUFFERFORM_H
#define BUFFERFORM_H

#include <QDialog>
#include "statgatherer.h"
#include "buffer.h"

class Buffer;

namespace Ui {
class BufferForm;
}

class BufferForm : public QDialog
{
    Q_OBJECT

public:
    explicit BufferForm(Buffer* buf,QWidget *parent = nullptr);
    ~BufferForm();

public slots:
    void display();

private:
    Ui::BufferForm *ui;
    Buffer* buffer;
};

#endif // BUFFERFORM_H

#ifndef LEDLABEL_H
#define LEDLABEL_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QHash>
#include <QString>

namespace Ui {
class LedLabel;
}

class LedLabel : public QWidget
{
    Q_OBJECT

public:
    explicit LedLabel(QWidget *parent = nullptr);
    ~LedLabel();

    void setText(const QString& text);
    void setColor(const QString& color);

private:
    Ui::LedLabel *ui;
    QPen* pen;
    QPixmap* pixmap;
    QPainter* painter;
    static const QHash<QString,QString> styleSheet;
};

#endif // LEDLABEL_H

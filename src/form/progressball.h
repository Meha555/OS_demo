#ifndef PROGRESSBALL_H
#define PROGRESSBALL_H

#include <QWidget>
#include <QTimer>
#include <QPoint>
#include <QColor>
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

// 通用的自定义进度球控件
class ProgressBall : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressBall(QWidget *parent = nullptr);

    int radius() const;
    void setRadius(int radius);

    int boderWidth() const;
    void setBoderWidth(int boderWidth);

    QColor boarderColor() const;
    void setBoarderColor(const QColor &boarderColor);

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &backgroundColor);

    QColor color() const;
    void setColor(const QColor &color);

    qreal capacity() const;
    void setCapacity(const qreal &capacity);

    qreal progress() const;
    void setProgress(const qreal &progress);

protected:
    void paintEvent(QPaintEvent*) override;

public slots:
    void updateProgress(qreal val);
    void updateRipple();
    void resetProgress();

private:
    qreal _capacity = 100;//提供一个默认的非零值
    qreal _progress{};// 当前进度
    int _radius = 100;// 球半径
    int _borderWidth = 10;// 边框宽度
    QColor _boarderColor = Qt::white;// 边框颜色
    QColor _backgroundColor = QColor(200,200,200,100);// 球内背景颜色
    QColor _colorLow = Qt::green
          ,_colorMedium = QColor("darkorange")
          ,_colorHigh = Qt::red;// 进度填充色
    QTimer* _pTimerUpdate{};// 定时器
    qreal _offset{};// 波纹偏移
    int parentWidth=0;
    int parentHeight=0;
};

#endif // PROGRESSBALL_H

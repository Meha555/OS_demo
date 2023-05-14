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
#include <QDebug>

// 通用的自定义进度球控件
class ProgressBall : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressBall(QWidget *parent = nullptr);

    void setProgress(double progress);
    double getProgress() const;

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

protected:
    void paintEvent(QPaintEvent*) override;

public slots:
    void updateProgressBall();

private:
    qreal _perDelta = 0.1;
    qreal _capacity = 100;
    int _radius{};// 球半径
    int _boderWidth{};// 边框宽度
    QColor _boarderColor;// 边框颜色
    QColor _backgroundColor;// 球内背景颜色
    QColor _colorLow,_colorMedium,_colorHigh;// 进度填充色
    QPoint _clickPos;// 鼠标点击位置
    QTimer* _pTimerUpdate{};// 定时器
    qreal _progress{};// 当前进度
    qreal _offset{};// 波纹偏移
};

#endif // PROGRESSBALL_H

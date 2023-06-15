#ifndef BETTERCHARTVIEW_H
#define BETTERCHARTVIEW_H

#include <QChartView>
#include <QLineSeries>
#include <QtMath>

QT_CHARTS_USE_NAMESPACE

// 自定义ChartView，支持鼠标缩放功能
class BetterChartView : public QChartView
{
    Q_OBJECT
public:
    explicit BetterChartView(QWidget* parent = nullptr);

protected:
    virtual void mouseMoveEvent(QMouseEvent *pEvent) override;
    virtual void mousePressEvent(QMouseEvent *pEvent) override;
    virtual void mouseReleaseEvent(QMouseEvent *pEvent) override;
    virtual void wheelEvent(QWheelEvent *pEvent) override;

private:
    bool m_bMiddleButtonPressed; // 鼠标中键按下标志
    QPoint m_oPrePos; // 移动前点坐标
};

#endif // BETTERCHARTVIEW_H

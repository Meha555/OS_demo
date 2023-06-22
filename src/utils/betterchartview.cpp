#include "betterchartview.h"

BetterChartView::BetterChartView(QWidget *parent)
    : QChartView(parent),m_bMiddleButtonPressed(false),m_oPrePos(0,0)
{
    this->setRubberBand(QChartView::RectangleRubberBand);
}

void BetterChartView::mouseMoveEvent(QMouseEvent *pEvent)
{
    if (m_bMiddleButtonPressed)
    {
        QPoint oDeltaPos = pEvent->pos() - m_oPrePos; // 移动的距离
        this->chart()->scroll(-oDeltaPos.x(), oDeltaPos.y()); // 滚动相应的距离
        //由于鼠标向上滚动会使滚轮距离为正，而向下滚动会使滚轮距离为负，因此这里将x取负数，y取正数。
        //保持图表的滚动方向与鼠标滚轮的滚动方向一致：向上滚动图表向下滚动，向下滚动图表向上滚动。
        m_oPrePos = pEvent->pos(); // 更新当前点
    }
    QChartView::mouseMoveEvent(pEvent);
}

void BetterChartView::mousePressEvent(QMouseEvent *pEvent)
{
    if (pEvent->button() == Qt::MiddleButton)
    {
        this->setCursor(Qt::OpenHandCursor); // 设置当前鼠标为手掌
        m_bMiddleButtonPressed = true;
        m_oPrePos = pEvent->pos(); // 持续更新当前点位
    }
    QChartView::mousePressEvent(pEvent);
}

void BetterChartView::mouseReleaseEvent(QMouseEvent *pEvent)
{
    if (pEvent->button() == Qt::MiddleButton)
    {
        m_bMiddleButtonPressed = false;
        this->setCursor(Qt::ArrowCursor);
    }
    QChartView::mouseReleaseEvent(pEvent);
}

void BetterChartView::wheelEvent(QWheelEvent *pEvent)
{
    qreal rVal = qPow(0.999, pEvent->delta()); // 设置比例
    // 1. 读取视图基本信息
    QRectF oPlotAreaRect = this->chart()->plotArea();
    QPointF oCenterPoint = oPlotAreaRect.center();
    // 2. 水平调整
    oPlotAreaRect.setWidth(oPlotAreaRect.width() * rVal);
    // 3. 竖直调整
    oPlotAreaRect.setHeight(oPlotAreaRect.height() * rVal);
    // 4.1 计算视点，视点不变，围绕中心缩放
    //QPointF oNewCenterPoint(oCenterPoint);
    // 4.2 计算视点，让鼠标点击的位置移动到窗口中心
    //QPointF oNewCenterPoint(pEvent->pos());
    // 4.3 计算视点，让鼠标点击的位置尽量保持不动(等比换算，存在一点误差)
    QPointF oNewCenterPoint(2 * oCenterPoint - pEvent->pos() - (oCenterPoint - pEvent->pos()) / rVal);
    // 5. 设置视点
    oPlotAreaRect.moveCenter(oNewCenterPoint);
    // 6. 提交缩放调整
    this->chart()->zoomIn(oPlotAreaRect);
    QChartView::wheelEvent(pEvent);
}

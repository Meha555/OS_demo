#include "progressball.h"

ProgressBall::ProgressBall(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(_radius*2+_borderWidth, _radius*2+_borderWidth);
//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);//以无边框的窗体显示
    //    this->setAttribute(Qt::WA_TranslucentBackground, true);//设置透明背景
}

void ProgressBall::paintEvent(QPaintEvent *) {
    //设置画笔
    QPen pen;
    pen.setColor(_boarderColor);
    pen.setWidth(_borderWidth);
    pen.setJoinStyle(Qt::RoundJoin);

    //设置画家
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);// 设置抗锯齿

    //使用矩形绘制圆形边框（左上角x,y坐标，大小）
    QRect border(_borderWidth*0.5, _borderWidth*0.5, _radius*2, _radius*2);
    painter.setPen(_borderWidth > 0 ? pen : Qt::NoPen);
    painter.setBrush(_backgroundColor);
    painter.drawEllipse(border);

    //设置矩形边界（左上角x,y坐标，大小）
    QRect ball = QRect(_borderWidth, _borderWidth,border.width() - _borderWidth,border.height() - _borderWidth);
    //在矩形边界内绘制圆形区域
    QPainterPath circlePath;
    circlePath.addEllipse(ball);

    painter.setPen(Qt::NoPen);
    painter.setBrush(_colorLow);

    //绘制波形曲线，并移动到初始位置(坐标原点)
    QPainterPath ripplePath1;
    QPainterPath ripplePath2;
    ripplePath1.moveTo(ball.bottomLeft());
    ripplePath2.moveTo(ball.bottomLeft());

    /* 使用正弦波，以当前水位线为坐标横轴，圆形左下角为原点建系
            y=Asin(ωx + φ) + k
    A：振幅，最高和最低的距离
    ω：角速度，用于控制周期大小，单位x中的起伏个数
    k：偏距，曲线整体上下偏移量，即水位线位置
    */
    qreal A = ball.width()*0.1;
    qreal k = (1 - _progress*0.01) * ball.height();
    qreal w = 2 * M_PI / static_cast<qreal>(ball.width());

    //在直径上绘制水位线的切线
    for (int i = ball.left(); i <= ball.right(); ++i) {
        qreal waterY1 = k + A * qSin(w * i + _offset);
        qreal waterY2 = k + A * qSin(w * i + M_PI + _offset);
        ripplePath1.lineTo(i, waterY1);
        ripplePath2.lineTo(i, waterY2);
    }

    //将切线形成的平面图形填充圆形的整个下方区域
    ripplePath1.lineTo(ball.bottomRight());
    ripplePath2.lineTo(ball.bottomRight());

    ripplePath1.lineTo(ball.bottomLeft());
    ripplePath2.lineTo(ball.bottomLeft());

    //设置水的颜色和透明度

    QColor waterColor1 = _progress < 70 ? (_progress < 40 ? _colorLow : _colorMedium) : _colorHigh;
    waterColor1.setAlpha(100);
    QColor waterColor2 = _progress < 70 ? (_progress < 40 ? _colorLow : _colorMedium) : _colorHigh;
    waterColor2.setAlpha(200);

    QPainterPath path;
    path = circlePath.intersected(ripplePath1);// 求相交区域
    painter.setBrush(waterColor1);
    painter.drawPath(path);// 绘制出相交区域

    path = circlePath.intersected(ripplePath2);
    painter.setBrush(waterColor2);
    painter.drawPath(path);

    // 绘制进度百分比
    int fontSize = ball.width()/4;
    QFont font;
    font.setFamily("Microsoft Yahei");
    font.setPixelSize(fontSize);
    font.setBold(true);

    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(ball, Qt::AlignCenter,QString("%1%").arg(QString::number(_progress*100/_capacity,'f',2)));
}

void ProgressBall::updateProgress(qreal val) {
    _progress += val;
    update();
}

qreal ProgressBall::progress() const
{
    return _progress;
}

void ProgressBall::setProgress(const qreal &progress)
{
    _progress = progress;
}

qreal ProgressBall::capacity() const
{
    return _capacity;
}

void ProgressBall::setCapacity(const qreal &capacity)
{
    _capacity = capacity;
}

QColor ProgressBall::color() const
{
    return _colorLow;
}

void ProgressBall::setColor(const QColor &color)
{
    _colorLow = color;
}

QColor ProgressBall::backgroundColor() const
{
    return _backgroundColor;
}

void ProgressBall::setBackgroundColor(const QColor &backgroundColor)
{
    _backgroundColor = backgroundColor;
}

QColor ProgressBall::boarderColor() const
{
    return _boarderColor;
}

void ProgressBall::setBoarderColor(const QColor &boarderColor)
{
    _boarderColor = boarderColor;
}

int ProgressBall::boderWidth() const
{
    return _borderWidth;
}

void ProgressBall::setBoderWidth(int boderWidth)
{
    _borderWidth = boderWidth;
}

int ProgressBall::radius() const
{
    return _radius;
}

void ProgressBall::setRadius(int radius)
{
    _radius = radius;
}

#include "Loading.h"
#include "qmath.h"
#include <QDebug>

Loading::Loading(QWidget *parent) : QDialog(parent),_i(0),_interval(50),_index(0)
{
    const QString Img_point = ":/Images/point.png";
    back_map = QPixmap(Img_point);

    //设置背景透明
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);//去掉边框，不可以X掉

    // 设置背景透明
//    setAttribute(Qt::WA_TranslucentBackground);
//    setAttribute(Qt::WA_NoSystemBackground, true); // 可选

    // 设置固定大小
    setFixedSize(220, 220);

    // 设置样式表
    setStyleSheet("QDialog { background-color: rgba(0,0,0,0); border: none; }");

    // 设置调色板
//    QPalette pal = palette();
//    pal.setColor(QPalette::Window, Qt::transparent);
//    setAutoFillBackground(true);
//    setPalette(pal);

    //46, 134, 193
    //setDotColor(QColor("#808080"));
    //setDotColor(QColor("#2E86C1"));
    //setDotColor(QColor("#3CA0E9"));
    setDotColor(QColor(60, 160, 233, 250));
    setDotCount(20);
    connect(&timer,&QTimer::timeout,this,&Loading::refresh);
    setMaxDiameter(15);
    setMinDiameter(10);
}

//********************************************** 设置部分 *************************************
//设置点的个数
void Loading::setDotCount(int count)
{
    _count = count;
}

//设置点的颜色
void Loading::setDotColor(const QColor & color)
{
    _dotColor = color;
}

//开始动画
void Loading::start()
{
    timer.setInterval(_interval);
	timer.start();
}
void Loading::stop()
{
	if (timer.isActive())
	{
		timer.stop();
	}
}
void Loading::slotStop()
{
	if (timer.isActive())
	{
		timer.stop();
	}
}
//设置最大直径
void Loading::setMaxDiameter(float max)
{
    _maxDiameter = max;
}

//设置最小直径
void Loading::setMinDiameter(float min)
{
    _minDiameter=min;
}
//********************************************** 绘制部分 *************************************
//刷新界面
void Loading::refresh()
{
    repaint();
}

void Loading::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    caculate();
}

void Loading::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing); // 关闭抗锯齿

    painter.setPen(_dotColor);
    painter.setBrush(_dotColor);

    //绘制点
    paintDot(painter);

    //绘制文字
    // 计算文本边界
    QFont font("DejaVu Serif", 15);
    painter.setFont(font);
    QFontMetrics fm(font);
    QRect textRect = fm.boundingRect(txt);

    // 计算居中位置
    int x = (this->rect().width() - textRect.width()) / 2;
    int y = (this->rect().height() + textRect.height()) / 2;

    // 绘制文本
    painter.drawText(x, y, txt);

    QDialog::paintEvent(event);  // 调用基类的 paintEvent
}

//计算绘制正方形区域
void Loading::caculate()
{
    _squareWidth = qMin(this->width(), this->height());
    float half= _squareWidth / 2;
    _centerDistance = half - _maxDiameter / 2 - 1;

    float gap = (_maxDiameter - _minDiameter) / (_count - 1) / 2;
    float angleGap = (float)360 / _count;

    locationList.clear();
    radiiList.clear();

    for(int i = 0;i < _count;i++)
    {
        radiiList << _maxDiameter / 2 - i * gap;
        float radian = qDegreesToRadians(-angleGap * i);
		locationList.append(LocationMovie(half + _centerDistance*qCos(radian), half - _centerDistance*qSin(radian)));
    }
}

//绘制圆点
void Loading::paintDot(QPainter& painter)
{
    for(int i = 0;i < _count;i++)
    {
        painter.setPen(_dotColor);
        //半径
        float radii = radiiList.at((_index + _count - i) % _count);

        //绘制圆点
        painter.drawEllipse(QPointF(locationList.at(i).x,locationList.at(i).y),radii,radii);
        //绘制正方形
        //painter.drawRect(locationList.at(i).x,locationList.at(i).y,radii,radii);
        //绘制文字
        //QFont font("DejaVu Serif",radii * 1.2, 75);
        //painter.setFont(font);
        //painter.drawText(QPointF(locationList.at(i).x, locationList.at(i).y), u8"正在调整相机位置,请等待...");
    }
    _index++;
}

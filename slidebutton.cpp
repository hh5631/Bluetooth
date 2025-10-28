#include "slidebutton.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>

slideButton::slideButton(QWidget *parent)
    : QWidget(parent)
    , m_checked(false)
    , m_pressed(false)
    , m_sliderPosition(0)
    , m_sliderRadius(10)
    , m_backgroundColorOn(QColor(0, 150, 255))
    , m_backgroundColorOff(QColor(200, 200, 200))
    , m_sliderColor(Qt::white)
{
    // setFixedSize(60, 30);
    
    m_animation = new QPropertyAnimation(this, "sliderPosition", this);
    m_animation->setDuration(200);
    m_animation->setEasingCurve(QEasingCurve::OutCubic);
    
    connect(m_animation, &QPropertyAnimation::valueChanged, this, &slideButton::updateSlider);
}

void slideButton::setChecked(bool checked)
{
    if (m_checked != checked) {
        m_checked = checked;
        
        int startPos = m_sliderPosition;
        int endPos = checked ? width() - 2 * m_sliderRadius : 0;
        
        m_animation->setStartValue(startPos);
        m_animation->setEndValue(endPos);
        m_animation->start();
        
        emit toggled(m_checked);
    }
}

void slideButton::setSliderPosition(int position)
{
    m_sliderPosition = position;
    update();
}

void slideButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制背景
    QColor bgColor = m_checked ? m_backgroundColorOn : m_backgroundColorOff;
    painter.setBrush(bgColor);
    painter.setPen(Qt::NoPen);
    
    QRect bgRect = rect().adjusted(2, 2, -2, -2);
    painter.drawRoundedRect(bgRect, height()/2, height()/2);
    
    // 绘制滑块
    painter.setBrush(m_sliderColor);
    painter.setPen(QPen(QColor(0, 0, 0, 50), 1));
    
    int sliderX = m_sliderPosition + m_sliderRadius;
    int sliderY = height() / 2;
    
    painter.drawEllipse(QPoint(sliderX, sliderY), m_sliderRadius, m_sliderRadius);
}

void slideButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressed = true;
    }
    QWidget::mousePressEvent(event);
}

void slideButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_pressed) {
        m_pressed = false;
        setChecked(!m_checked);
    }
    QWidget::mouseReleaseEvent(event);
}

void slideButton::resizeEvent(QResizeEvent *event)
{
    m_sliderRadius = qMin(width(), height()) / 2 - 4;
    if (m_checked) {
        m_sliderPosition = width() - 2 * m_sliderRadius;
    } else {
        m_sliderPosition = 0;
    }
    QWidget::resizeEvent(event);
}

void slideButton::updateSlider()
{
    update();
}

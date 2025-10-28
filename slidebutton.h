#ifndef SLIDEBUTTON_H
#define SLIDEBUTTON_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QEasingCurve>

class slideButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int sliderPosition READ sliderPosition WRITE setSliderPosition)

public:
    explicit slideButton(QWidget *parent = nullptr);
    
    bool isChecked() const { return m_checked; }
    void setChecked(bool checked);
    
    int sliderPosition() const { return m_sliderPosition; }
    void setSliderPosition(int position);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    void toggled(bool checked);

private slots:
    void updateSlider();

private:
    bool m_checked;
    bool m_pressed;
    int m_sliderPosition;
    int m_sliderRadius;
    QPropertyAnimation *m_animation;
    
    QColor m_backgroundColorOn;
    QColor m_backgroundColorOff;
    QColor m_sliderColor;
};

#endif // SLIDEBUTTON_H

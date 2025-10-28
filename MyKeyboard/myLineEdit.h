#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QObject>
#include <QLineEdit>
#include <QMouseEvent>
#include <QWidget>
#include <QDebug>


class myLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit myLineEdit(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent*);

signals:
    void myLineEditClick(QString);

};

#endif // MYLINEEDIT_H

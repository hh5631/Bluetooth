#include "myLineEdit.h"

myLineEdit::myLineEdit(QWidget *parent) : QLineEdit(parent)
{

}

void myLineEdit::mousePressEvent(QMouseEvent *myEvent)
{
    if(myEvent->button() == Qt::LeftButton)
    {
        //qDebug()<< "my lineedit is:"<<this->objectName();
        emit myLineEditClick(this->objectName());
    }
    QLineEdit::mousePressEvent(myEvent);
}

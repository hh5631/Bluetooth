#include "widget.h"

#include <QApplication>

#include <QDir>
#include <QString>
#include <QSettings>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //设置当前运行路径
    QString exePath = QCoreApplication::applicationFilePath();
    QFileInfo fileInfo(exePath);
    QString dirPath = fileInfo.absolutePath();
    qRegisterMetaType<QList<QBluetoothDeviceInfo>>("QList<QBluetoothDeviceInfo>");
    qApp->setProperty("curDirPath", QVariant(dirPath));
    qDebug() << "curDirPath: " << qApp->property("curDirPath").toString();

    //qApp->property("curDirPath").toString().toStdString();

    Widget w;
    w.show();
    return a.exec();
}

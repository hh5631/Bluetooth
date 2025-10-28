#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#ifndef MYINCLUDE_H
#define MYINCLUDE_H

#include <QObject>
#include <QListWidget>
#include <QSettings>
//#include <QWidget>
//#include <QDesktopWidget>
#include <QFile>
#include <QFileIconProvider>
//#include <QTextCodec>
#include <QMessageBox>
#include <QAbstractButton>
#include <QPushButton>
#include <QDateTime>
#include <QDate>
#include <QCoreApplication>
#include <QFileDialog>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QDirIterator>
#include <QCursor>
#include <QTimer>
//#include <QSound>
#include <QApplication>
#include <QStyleFactory>
#include <QTextStream>
#include <QMap>
#include <QStandardItemModel>
#include <QKeyEvent>
#include <QDebug>
#include <QPainter>
#include <QProcess>
//#include <QSerialPort>
//#include <QSerialPortInfo>
#include <QInputDialog>
//#include <QNetworkAccessManager>
//#include <QNetworkReply>
//#include <QNetworkRequest>
//#include <QHttpPart>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>


class Agings : public QObject
{
public:
    Agings();~Agings();

    static void waitms(int ms)
    {
        QEventLoop eventloop;
        QTimer::singleShot(ms, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
};

#endif // MYINCLUDE_H

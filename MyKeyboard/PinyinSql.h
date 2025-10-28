#ifndef PINYINSQL_H
#define PINYINSQL_H

#include <QObject>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QStringList>
#include "DatabaseManage/DatabaseManage.h"

class PinyinSql : public QObject
{
    Q_OBJECT
public:
    explicit PinyinSql(QObject *parent = nullptr);

    bool fOpenPinyinSql(QString sqlName);

    void fCloseSql();

    QStringList fSearchPinyinData(QString);

private:

    QSqlDatabase g_PinyinSqlDB;


};

#endif // PINYINSQL_H

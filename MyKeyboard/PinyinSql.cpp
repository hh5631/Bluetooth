#include "PinyinSql.h"



PinyinSql::PinyinSql(QObject *parent) : QObject(parent)
{

}

//打开数据库文件
bool PinyinSql::fOpenPinyinSql(QString sqlName)
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        qDebug() << "PinyinSql default...";
        g_PinyinSqlDB = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        qDebug() << "QSQLITE";
        g_PinyinSqlDB = QSqlDatabase::addDatabase("QSQLITE");
    }

    g_PinyinSqlDB.setDatabaseName(sqlName);
    if(g_PinyinSqlDB.open())
    {
        qDebug() << "hSql fOpenSql open sql succed!" << sqlName;
        return true;
    }
    else
    {
        qDebug() << "hSql fOpenSql open sql failed!" << sqlName;
        return false;
    }
}

//关闭数据库
void PinyinSql::fCloseSql()
{
    g_PinyinSqlDB.close();

    //QSqlDatabase::removeDatabase("pinsql");
}

//搜索拼音对应的汉字
QStringList PinyinSql::fSearchPinyinData(QString pinyin)
{
    auto pinyin_db_ptr = DATABASEMNG->getDatabaseByName("pinyin_db");
    if (pinyin_db_ptr == nullptr || !pinyin_db_ptr->isOpen())
    {
        LOG_INFO("OperateLog", "pinyin_db can not use!");
        return QStringList();
    }

    QStringList resultList;
    QSqlQuery myQuery(*pinyin_db_ptr);
    QString myStr = QString("select word from zhcn where py == '%1' order by id asc;").arg(pinyin);

    bool isSuccess = myQuery.exec(myStr);

    if(isSuccess)
    {
        while (myQuery.next())
        {
            resultList << myQuery.value(0).toString();
        }
    }
    resultList.removeDuplicates();

    //qDebug() << "PinyinSql fSearchPinyinData resultList:"  << resultList;

    return resultList;
}

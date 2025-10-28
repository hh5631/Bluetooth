#ifndef DATABASEMANAGE_H
#define DATABASEMANAGE_H

#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QFile>
#include <memory>
#include <mutex>
#include "SpdlogManage/SpdlogManage.h"

//#include "SpdlogManage/SpdlogManage.h"

class DatabaseManage
{
public:
    static DatabaseManage* Instance()
    {
        static std::recursive_mutex m_mutex;
        static DatabaseManage* _instance;

        if (!_instance)
        {
            std::lock_guard<std::recursive_mutex> guard(m_mutex);
            if (!_instance)
            {
                _instance = new DatabaseManage();
            }
        }

        return _instance;
    }

private:
    DatabaseManage();
    ~DatabaseManage(){};

public:
    std::shared_ptr<QSqlDatabase> getDatabaseByName(std::string database_name);
    int openDatabase();

private:
    std::recursive_mutex mtxDatabase;
    std::shared_ptr<QSqlDatabase> m_user_db = nullptr;
    //std::shared_ptr<QSqlDatabase> m_data_db = nullptr;
    std::shared_ptr<QSqlDatabase> m_pinyin_db = nullptr;

    QString user_db_path = "";
    //QString data_db_path = "";
    QString pinyin_db_path = "";
};

#define DATABASEMNG DatabaseManage::Instance()

#endif

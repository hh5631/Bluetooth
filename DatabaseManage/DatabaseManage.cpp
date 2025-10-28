#include "DatabaseManage.h"

DatabaseManage::DatabaseManage()
{
    QString curDirPath = qApp->property("curDirPath").toString();
    qDebug() << "DatabaseManage curDirPath:" << curDirPath;

    user_db_path = curDirPath + "/Database/userInfo.db3";
//    data_db_path = curDirPath + "/Database/dataInfo.db3";
    pinyin_db_path = curDirPath + "/Database/pinyin.db";

    //用户数据库初始化
    m_user_db = std::make_shared<QSqlDatabase>(QSqlDatabase::addDatabase("QSQLITE", "user_db"));
    m_user_db->setDatabaseName(user_db_path);

    //细胞数据库初始化
//    m_data_db = std::make_shared<QSqlDatabase>(QSqlDatabase::addDatabase("QSQLITE", "data_db"));
//    m_data_db->setDatabaseName(data_db_path);

    //键盘拼音数据库初始化
    m_pinyin_db = std::make_shared<QSqlDatabase>(QSqlDatabase::addDatabase("QSQLITE", "pinyin_db"));
    m_pinyin_db->setDatabaseName(pinyin_db_path);
}

int DatabaseManage::openDatabase()
{
    std::lock_guard<decltype(mtxDatabase)> guard(mtxDatabase);

    int ret = 0;
    if (m_user_db /*&& m_data_db*/ && m_pinyin_db)
    {
        QFile user_file(user_db_path);
        if (!user_file.exists())
        {
            //LOG_INFO("OperateLog", "./Database/userInfo.db3 is not exist!");
            qDebug() << "user db is not exist!";
            LOG_INFO("OperateLog", "user db is not exist!");
            ret += -1;
        }
        else
        {
            if (!m_user_db->open())
            {
                //LOG_INFO("OperateLog", "oepn ./Database/userInfo.db3 failed:" + m_user_db->lastError().text().toStdString());
                qDebug() << "user db can not open err:" << m_user_db->lastError().text();
                LOG_INFO("OperateLog", "user db can not open err:" + m_user_db->lastError().text().toStdString());
                ret += -2;
            }
        }

//        QFile data_file(data_db_path);
//        if (!data_file.exists())
//        {
//            //LOG_INFO("OperateLog", "./Database/dataInfo.db3 is not exist!");
//            ret += -4;
//        }
//        else
//        {
//            if (!m_data_db->open())
//            {
//                //LOG_INFO("OperateLog", "oepn ./Database/dataInfo.db3 failed:" + m_data_db->lastError().text().toStdString());
//                ret += -8;
//            }
//        }

        QFile pinyin_file(pinyin_db_path);
        if (!pinyin_file.exists())
        {
            LOG_INFO("OperateLog", "./Database/pinyin.db is not exist!");
        }
        else
        {
            if (!m_pinyin_db->open())
            {
                LOG_INFO("OperateLog", "oepn ./Database/pinyin.db failed:" + m_pinyin_db->lastError().text().toStdString());
            }
        }
    }
    else
    {
        LOG_INFO("OperateLog", "database ptr exist null!");
        ret = -100;
    }

    return ret;
}

std::shared_ptr<QSqlDatabase> DatabaseManage::getDatabaseByName(std::string database_name)
{
    std::lock_guard<decltype(mtxDatabase)> guard(mtxDatabase);

    std::shared_ptr<QSqlDatabase> cur_db_ptr = nullptr;

    if (database_name == "user_db")
        cur_db_ptr = m_user_db;
//    else if (database_name == "data_db")
//        cur_db_ptr = m_data_db;
    else if (database_name == "pinyin_db")
        cur_db_ptr = m_pinyin_db;

    return cur_db_ptr;
}

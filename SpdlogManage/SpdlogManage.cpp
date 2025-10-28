#include "SpdlogManage.h"

SpdlogManage::SpdlogManage()
{
    //初始化线程池
    spdlog::init_thread_pool(8192, 4);

    //先初始化两个日志记录器:OperateLog、ProcessLog
    setup_async_logger("OperateLog");
    setup_async_logger("ProcessLog");

//    // 方式 1：使用 spdlog 的日志接口输出
//    spdlog::info("Spdlog 版本: {}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

//    // 方式 2：使用标准输出（如果未初始化 spdlog）
//    qDebug() << "Spdlog 版本: "
//              << SPDLOG_VER_MAJOR << "."
//              << SPDLOG_VER_MINOR << "."
//              << SPDLOG_VER_PATCH;
}

SpdlogManage::~SpdlogManage()
{
    spdlog::drop_all();
}

//设置日志记录器
std::shared_ptr<spdlog::logger> SpdlogManage::setup_async_logger(const std::string& loggerName)
{
    std::lock_guard<decltype(mtxSpdMan)> guard(mtxSpdMan);

    auto get_logger = spdlog::get(loggerName);
    if (get_logger)
    {
        //已经存在直接返回存在的日志记录器指针
        return get_logger;
    }

    //创建按小时分割的日志文件
    auto hourly_sink = std::make_shared<spdlog::sinks::hourly_file_sink_mt>(qApp->property("curDirPath").toString().toStdString() + "/SysLog/" + loggerName + ".log", 0, 0);       //日志文件路径，小时，分钟

    //创建一个异步日志记录器实例
    auto logger = std::make_shared<spdlog::async_logger>(loggerName, hourly_sink, spdlog::thread_pool(), spdlog::async_overflow_policy::block);

    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] (%s:%#) %!(): %v");
//    // 使用自定义Formatter
//    logger->set_formatter(make_unique<BeijingTimeFormatter>(
//        "[%Y-%m-%d %H:%M:%S.%e] [%l] (%s:%#) %!(): %v"));

    //注册日志记录器
    spdlog::register_logger(logger);

    //设置日志级别
    logger->set_level(spdlog::level::info);

    return logger;
}

//通过名字获取日志记录器
std::shared_ptr<spdlog::logger> SpdlogManage::getLoggerByName(const std::string& loggerName)
{
    std::lock_guard<decltype(mtxSpdMan)> guard(mtxSpdMan);
    return spdlog::get(loggerName);
}

//写入日志
void SpdlogManage::logInfo(const std::string& loggerName, QString Loginfo)
{
    std::lock_guard<decltype(mtxSpdMan)> guard(mtxSpdMan);

    auto cur_logger = spdlog::get(loggerName);
    if (!cur_logger)
    {
        //若日志记录器不存在则创建
        cur_logger = setup_async_logger(loggerName);
    }

    if (cur_logger)
    {
        //cur_logger->info(Loginfo.toStdString());
        SPDLOG_LOGGER_INFO(cur_logger, Loginfo.toStdString());

        cur_logger->flush();
    }
}

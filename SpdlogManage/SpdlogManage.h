#ifndef SPDLOGMANAGE_H
#define SPDLOGMANAGE_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/hourly_file_sink.h>
#include <spdlog/async.h>
#include <string>
#include <memory>
#include <mutex>
#include <QString>
#include <QApplication>

#include <chrono>
#include <spdlog/details/log_msg.h>
#include <spdlog/formatter.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/version.h>
#include <QDebug>
#include <time.h>

#ifdef _WIN32
#define gmtime_r(timep, result) gmtime_s(result, timep)
#endif

// template<typename T, typename... Args>
// std::unique_ptr<T> make_unique(Args&&... args) {
//     return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
// }

// class BeijingTimeFormatter : public spdlog::formatter {
// public:
//     explicit BeijingTimeFormatter(const std::string& pattern) {
//         formatter_ = make_unique<spdlog::pattern_formatter>(pattern);
//     }

//     // 拷贝构造函数
//     BeijingTimeFormatter(const BeijingTimeFormatter& other) {
//         formatter_ = other.formatter_->clone();
//     }

//     void format(const spdlog::details::log_msg& msg, spdlog::memory_buf_t& dest) override {
//         auto log_time_point = msg.time;
//         auto adjusted_time_point = log_time_point + std::chrono::hours(8);
//         auto adjusted_time_t = std::chrono::system_clock::to_time_t(adjusted_time_point);

//         std::tm beijing_tm;
//         gmtime_r(&adjusted_time_t, &beijing_tm);

//         spdlog::details::log_msg adjusted_msg = msg;
//         adjusted_msg.time = adjusted_time_point;
//         formatter_->format(adjusted_msg, dest);
//     }

//     std::unique_ptr<spdlog::formatter> clone() const override {
//         return make_unique<BeijingTimeFormatter>(*this);
//     }

// private:
//     std::unique_ptr<spdlog::formatter> formatter_;
// };

class SpdlogManage
{
public:
    static SpdlogManage* Instance()
    {
        static std::recursive_mutex m_mutex;    //静态锁
        static SpdlogManage* _instance;

        if (!_instance)
        {
            std::lock_guard<std::recursive_mutex> guard(m_mutex);
            if (!_instance)
            {
                _instance = new SpdlogManage();
            }
        }

        return _instance;
    }

private:
    SpdlogManage();
    ~SpdlogManage();

public:
    //设置日志记录器
    std::shared_ptr<spdlog::logger> setup_async_logger(const std::string& loggerName);

    //通过名字获取日志记录器
    std::shared_ptr<spdlog::logger> getLoggerByName(const std::string& loggerName);

    //写入日志
    void logInfo(const std::string& loggerName, QString Loginfo);

private:
    std::recursive_mutex mtxSpdMan;
};

#define LOGMNG SpdlogManage::Instance()

#define LOG_FLUSH(logger) LOGMNG->getLoggerByName(logger)->flush();

#define LOG_INFO(logger, concent) {SPDLOG_LOGGER_INFO(LOGMNG->getLoggerByName(logger), concent); LOG_FLUSH(logger)}

#define LOG_WARN(logger, concent) {SPDLOG_LOGGER_WARN(LOGMNG->getLoggerByName(logger), concent); LOG_FLUSH(logger)}

#define LOG_ERROR(logger, concent) {SPDLOG_LOGGER_ERROR(LOGMNG->getLoggerByName(logger), concent); LOG_FLUSH(logger)}

#endif

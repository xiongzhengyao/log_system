// log.h

#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <mutex>
#include <chrono>
#include <ctime>

// 日志级别定义
enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

// 日志输出函数
void Log(LogLevel level, const std::string& message) {
    std::string levelStr;
    switch (level) {
        case LogLevel::DEBUG: levelStr = "DEBUG"; break;
        case LogLevel::INFO: levelStr = "INFO"; break;
        case LogLevel::WARN: levelStr = "WARN"; break;
        case LogLevel::ERROR: levelStr = "ERROR"; break;
        case LogLevel::FATAL: levelStr = "FATAL"; break;
    }

    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm;
    localtime_r(&now_c, &now_tm);

    char timeStr[30];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %X", &now_tm);

    std::lock_guard<std::mutex> lock(logMutex); // 使用互斥锁保证线程安全
    std::cout << "[" << timeStr << "] " << levelStr << " - " << message << std::endl;
    // 可以选择将日志输出到文件
    std::ofstream logFile("app.log", std::ios::app);
    logFile << "[" << timeStr << "] " << levelStr << " - " << message << std::endl;
    logFile.close();
}

// 格式化日志消息的函数
template<typename... Args>
std::string FormatMessage(const std::string& format, Args... args) {
    return std::string(std::vsnprintf(nullptr, 0, format.c_str(), 0) + 1,
                       '\0') +
           std::vsnprintf(const_cast<char*>(std::string().resize(
                           std::vsnprintf(nullptr, 0, format.c_str(), 0) + 1,
                           '\0').c_str()),
                          0, format.c_str(), std::make_va_list(args)...);
}

// 用于简化日志调用的宏定义
#define LOGGER(level, fmt, ...) Log(level, FormatMessage(fmt, __VA_ARGS__))

// 定义不同等级的日志宏
#define LOG_DEBUG(fmt, ...) LOGGER(LogLevel::DEBUG, fmt, __VA_ARGS__)
#define LOG_INFO(fmt, ...) LOGGER(LogLevel::INFO, fmt, __VA_ARGS__)
#define LOG_WARN(fmt, ...) LOGGER(LogLevel::WARN, fmt, __VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOGGER(LogLevel::ERROR, fmt, __VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOGGER(LogLevel::FATAL, fmt, __VA_ARGS__)

// 日志系统使用的互斥锁
extern std::mutex logMutex;

#endif // LOG_H
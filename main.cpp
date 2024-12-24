#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

// 日志级别枚举
enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

// 日志系统类
class Logger {
public:
    Logger(const std::string& filename, LogLevel level = LogLevel::INFO)
        : logFile(filename), currentLevel(level) {}

    // 设置日志级别
    void setLevel(LogLevel level) {
        currentLevel = level;
    }

    // 记录日志
    void log(LogLevel level, const std::string& message) {
        if (level >= currentLevel) {
            std::time_t now = std::time(nullptr);
            std::tm* now_tm = std::localtime(&now);
            std::string timeStr = std::to_string(now_tm->tm_year + 1900) + "-" +
                                  std::to_string(now_tm->tm_mon + 1) + "-" +
                                  std::to_string(now_tm->tm_mday) + " " +
                                  std::to_string(now_tm->tm_hour) + ":" +
                                  std::to_string(now_tm->tm_min) + ":" +
                                  std::to_string(now_tm->tm_sec);

            std::string levelStr;
            switch (level) {
                case LogLevel::DEBUG: levelStr = "DEBUG"; break;
                case LogLevel::INFO: levelStr = "INFO"; break;
                case LogLevel::WARNING: levelStr = "WARNING"; break;
                case LogLevel::ERROR: levelStr = "ERROR"; break;
                case LogLevel::CRITICAL: levelStr = "CRITICAL"; break;
            }

            std::string logMessage = "[" + timeStr + "] [" + levelStr + "] " + message + "\n";
            std::cout << logMessage; // 输出到控制台
            if (!logFile.is_open()) {
                logFile.open(filename, std::ios::out | std::ios::app); // 打开文件
            }
            logFile << logMessage; // 输出到文件
        }
    }

private:
    std::string filename;
    LogLevel currentLevel;
    std::ofstream logFile;
};

// 使用示例
int main() {
    Logger logger("app.log", LogLevel::DEBUG);

    logger.setLevel(LogLevel::INFO); // 设置日志级别为INFO

    logger.log(LogLevel::DEBUG, "This is a debug message."); // 不会被记录
    logger.log(LogLevel::INFO, "This is an info message.");
    logger.log(LogLevel::WARNING, "This is a warning message.", % d);
    logger.log(LogLevel::ERROR, "This is an error message.");
    logger.log(LogLevel::CRITICAL, "This is a critical message.");

    return 0;
}
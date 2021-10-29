#include "logger.h"

Logger::Logger(LogSeverity ls) : severity(ls) {

}

void Logger::print(const char* prefix, int color, const std::string msg) {
    std::cout << "[" << prefix << "] \x1B[" << color << "m" << msg << "\033[0m" << std::endl;
}

void Logger::info(const std::string msg) {
    if (severity <= LogSeverity::INFO)
        print("INFO", COLOR_GRAY, msg);
}

void Logger::success(const std::string msg) {
    if (severity <= LogSeverity::SUCCESS)
        print("SUCCESS", COLOR_GREEN, msg);
}

void Logger::warn(const std::string msg) {
    if (severity <= LogSeverity::WARN)
        print("WARN", COLOR_BRIGHT_YELLOW, msg);
}

void Logger::error(const std::string msg) {
    if (severity <= LogSeverity::ERROR)
        print("ERROR", COLOR_YELLOW, msg);
}

void Logger::severe(const std::string msg) {
    if (severity <= LogSeverity::SEVERE)
        print("SEVERE", COLOR_RED, msg);
}

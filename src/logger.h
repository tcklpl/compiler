#pragma once
#include <iostream>

#define COLOR_RED 31
#define COLOR_GREEN 32
#define COLOR_YELLOW 33
#define COLOR_GRAY 90
#define COLOR_BRIGHT_YELLOW 93

enum LogSeverity {
    INFO, SUCCESS, WARN, ERROR, SEVERE
};

class Logger {
    private:
        void print(const char* prefix, int color, const std::string msg);
        LogSeverity severity;
    public:
        Logger(LogSeverity ls);
        void info(const std::string msg);
        void success(const std::string msg);
        void warn(const std::string msg);
        void error(const std::string msg);
        void severe(const std::string msg);
};
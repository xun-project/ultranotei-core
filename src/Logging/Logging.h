#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <mutex>

#define TRACE(level) INTERNAL_TRACE(level, __LINE__, __FILE__, __FUNCTION__)
#define INFO TRACE(logging::Level::INFO)
#define WARNING TRACE(logging::Level::WARNING)
#define ERROR TRACE(logging::Level::ERROR)
#define CRITICAL TRACE(logging::Level::CRITICAL)

namespace logging {

enum class Level {
    TRACE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class LoggerMessage {
public:
    LoggerMessage(Level level, const std::string& category, const char* file, int line, const char* function)
        : level_(level), category_(category), file_(file), line_(line), function_(function) {}

    ~LoggerMessage() {
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);
        
        std::cout << "[" << levelToString(level_) << "] " 
                  << category_ << " " 
                  << file_ << ":" << line_ << " " 
                  << function_ << " - " 
                  << stream_.str() << std::endl;
    }

    template<typename T>
    LoggerMessage& operator<<(const T& val) {
        stream_ << val;
        return *this;
    }

private:
    static const char* levelToString(Level level) {
        switch (level) {
            case Level::TRACE: return "TRACE";
            case Level::DEBUG: return "DEBUG";
            case Level::INFO: return "INFO";
            case Level::WARNING: return "WARNING";
            case Level::ERROR: return "ERROR";
            case Level::CRITICAL: return "CRITICAL";
            default: return "UNKNOWN";
        }
    }

    Level level_;
    std::string category_;
    const char* file_;
    int line_;
    const char* function_;
    std::stringstream stream_;
};

#define INTERNAL_TRACE(level, line, file, function) \
    LoggerMessage(level, "", file, line, function)

} // namespace logging

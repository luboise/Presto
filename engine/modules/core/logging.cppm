export module presto.core.logging;

import std;
import presto.core.types;
import presto.runtime.application;

namespace spdlog {
class logger;
}

namespace Pr {

export enum LogLevel : Pr::uint8_t { TRACE, INFO, WARN, ERROR, CRITICAL };

class Logger {
    friend class Application;

   public:
    static void Log(LogLevel level, Pr::string message);
    static void CoreLog(LogLevel level, Pr::string message);

   private:
    static void init();

    static std::shared_ptr<spdlog::logger> coreLogger_;
    static std::shared_ptr<spdlog::logger> clientLogger_;
};

export template <typename... Args>
void Log(LogLevel level, std::format_string<Args...> str, Args&&... args) {
    Logger::Log(level, std::format(str, std::forward<Args>(args)...));
};

export template <>
void Log(LogLevel level, std::format_string<> str) {
    Logger::Log(level, std::format(str));
};

export template <typename T>
void Log(LogLevel level, const T& value) {
    Logger::Log(level, std::to_string(value));
};

export template <>
void Log(LogLevel level, const std::string& value) {
    Logger::Log(level, value);
};

export template <typename... Args>
void CoreLog(LogLevel level, std::format_string<Args...> str, Args&&... args) {
    Logger::CoreLog(level, std::format(str, std::forward<Args>(args)...));
};

export template <>
void CoreLog(LogLevel level, std::format_string<> str) {
    Logger::CoreLog(level, std::format(str));
};

export template <typename T>
void CoreLog(LogLevel level, const T& value) {
    Logger::CoreLog(level, std::to_string(value));
};

export template <>
void CoreLog(LogLevel level, const std::string& value) {
    Logger::CoreLog(level, value);
};

}  // namespace Pr

/*
export template <typename... Args>
void Trace(const char* str, Args&&... args) {
    Log(TRACE, std::format(str, std::move(args...)));
}

export template <typename... Args>
void Info(const char* str, Args&&... args) {
    Log(INFO, std::format(str, std::move(args...)));
}

export template <typename... Args>
void Warn(const char* str, Args&&... args) {
    Log(WARN, std::format(str, std::move(args...)));
}

export template <typename... Args>
void Error(const char* str, Args&&... args) {
    Log(ERROR, std::format(str, std::move(args...)));
}

export template <typename... Args>
void Critical(const char* str, Args&&... args) {
    Log(CRITICAL, std::format(str, std::move(args...)));
}
*/

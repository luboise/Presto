export module presto.core.logging;

import std;
import presto.types.core;

namespace spdlog {
class logger;
}

namespace Pr {

export enum LogLevel : Pr::uint8_t { TRACE, INFO, WARN, ERROR, CRITICAL };

class Logger {
   public:
    static void Log(LogLevel level, std::string_view message);
    static void CoreLog(LogLevel level, std::string_view message);

    // TODO: Fix this being publically visible
    static void init();

   private:
    static std::shared_ptr<spdlog::logger> coreLogger_;
    static std::shared_ptr<spdlog::logger> clientLogger_;
};

export template <typename... Args>
void Log(LogLevel level, std::format_string<Args...> str, Args&&... args) {
    Logger::Log(level, std::format(str, std::forward<Args>(args)...));
};

export template <typename... Args>
void CoreLog(LogLevel level, std::format_string<Args...> str, Args&&... args) {
    Logger::CoreLog(level, std::format(str, std::forward<Args>(args)...));
};

export template <typename T>
    requires std::formattable<T, char>
void Log(LogLevel level, const T& value) {
    Logger::Log(level, std::format("{}", value));
};

export template <typename T>
    requires std::formattable<T, char>
void CoreLog(LogLevel level, const T& value) {
    Logger::CoreLog(level, std::format("{}", value));
};

export void Log(LogLevel level, const char* value) {
    Logger::Log(level, std::string_view(value));
};

export void CoreLog(LogLevel level, const char* value) {
    Logger::CoreLog(level, std::string_view(value));
};

}  // namespace Pr

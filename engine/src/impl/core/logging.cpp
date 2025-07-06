module;

#include <spdlog/spdlog.h>
// DO NOT REORDER
#include <spdlog/sinks/stdout_color_sinks.h>

module presto.core.logging;

import std;

namespace Pr {
std::shared_ptr<spdlog::logger> Logger::coreLogger_;
std::shared_ptr<spdlog::logger> Logger::clientLogger_;

void Logger::init() {
    spdlog::set_pattern("%^[%T] %n: %v%$");
    coreLogger_ = spdlog::stdout_color_mt("PRESTO");
    coreLogger_->set_level(spdlog::level::trace);

    clientLogger_ = spdlog::stdout_color_mt("APP");
    clientLogger_->set_level(spdlog::level::trace);
}

void Logger::Log(LogLevel level, std::string_view message) {
    switch (level) {
        case TRACE: {
            clientLogger_->trace(message);
        }
        case INFO: {
            clientLogger_->info(message);
        }
        case WARN: {
            clientLogger_->warn(message);
        }
        case ERROR: {
            clientLogger_->error(message);
        }
        case CRITICAL: {
            clientLogger_->critical(message);
        }
    }
}

void Logger::CoreLog(LogLevel level, std::string_view message) {
    switch (level) {
        case TRACE: {
            coreLogger_->trace(message);
        }
        case INFO: {
            coreLogger_->info(message);
        }
        case WARN: {
            coreLogger_->warn(message);
        }
        case ERROR: {
            coreLogger_->error(message);
        }
        case CRITICAL: {
            coreLogger_->critical(message);
        }
    }
}

}  // namespace Pr

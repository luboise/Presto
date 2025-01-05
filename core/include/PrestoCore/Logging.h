#pragma once

#include "spdlog/spdlog.h"

namespace PrestoCore {
    class Log {
       public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
            return s_CoreLogger;
        };
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
            return s_ClientLogger;
        };

       private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}  // namespace PrestoCore

// Core log macros
#define PR_CORE_TRACE(...) \
    ::PrestoCore::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PR_CORE_INFO(...) ::PrestoCore::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PR_CORE_WARN(...) ::PrestoCore::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PR_CORE_ERROR(...) \
    ::PrestoCore::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PR_CORE_CRITICAL(...) \
    ::PrestoCore::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define PR_TRACE(...) ::PrestoCore::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PR_INFO(...) ::PrestoCore::Log::GetClientLogger()->info(__VA_ARGS__)
#define PR_WARN(...) ::PrestoCore::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PR_ERROR(...) ::PrestoCore::Log::GetClientLogger()->error(__VA_ARGS__)
#define PR_CRITICAL(...) \
    ::PrestoCore::Log::GetClientLogger()->critical(__VA_ARGS__)

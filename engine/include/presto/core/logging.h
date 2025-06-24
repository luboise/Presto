#pragma once

#include "presto/handles/core_handles.h"
#include "spdlog/logger.h"

namespace Pr {
class Log {
   public:
    static void init();

    static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
        return s_CoreLogger;
    };
    static std::shared_ptr<spdlog::logger>& GetClientLogger() {
        return s_ClientLogger;
    };

   private:
    static Ptr<spdlog::logger> s_CoreLogger;
    static Ptr<spdlog::logger> s_ClientLogger;
};
}  // namespace Pr

// Core log macros
#define PR_CORE_TRACE(...) ::Pr::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PR_CORE_INFO(...) ::Pr::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PR_CORE_WARN(...) ::Pr::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PR_CORE_ERROR(...) ::Pr::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PR_CORE_CRITICAL(...) ::Pr::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define PR_TRACE(...) ::Pr::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PR_INFO(...) ::Pr::Log::GetClientLogger()->info(__VA_ARGS__)
#define PR_WARN(...) ::Pr::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PR_ERROR(...) ::Pr::Log::GetClientLogger()->error(__VA_ARGS__)
#define PR_CRITICAL(...) ::Pr::Log::GetClientLogger()->critical(__VA_ARGS__)

#pragma once

#include "Presto/Core.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

namespace Presto {
    class PRESTO_API Log {
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
}  // namespace Presto

// Core log macros
#define PR_CORE_TRACE(...) ::Presto::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PR_CORE_INFO(...) ::Presto::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PR_CORE_WARN(...) ::Presto::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PR_CORE_ERROR(...) ::Presto::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PR_CORE_CRITICAL(...) \
    ::Presto::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define PR_TRACE(...) ::Presto::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PR_INFO(...) ::Presto::Log::GetClientLogger()->info(__VA_ARGS__)
#define PR_WARN(...) ::Presto::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PR_ERROR(...) ::Presto::Log::GetClientLogger()->error(__VA_ARGS__)
#define PR_CRITICAL(...) ::Presto::Log::GetClientLogger()->critical(__VA_ARGS__)
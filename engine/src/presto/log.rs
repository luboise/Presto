extern crate log;

pub fn trace(out: &str, var_args) {
    println!(out, var_args);
}

use log::{error, info, log, trace, warn, Level, LevelFilter};
use std::fmt;

// Initialize the logger
fn init_logger() {
    env_logger::builder().filter_level(LevelFilter::Trace).init();
}

// Function for logging at different levels
pub fn log_message(level: Level, fmt: fmt::Arguments) {
    log!(level, "{}", fmt);
}

pub fn trace_log(fmt: std::fmt::Arguments) {
    log_message(Level::Trace, fmt);
}

pub fn info_log(fmt: std::fmt::Arguments) {
    log_message(Level::Info, fmt);
}

pub fn warn_log(fmt: std::fmt::Arguments) {
    log_message(Level::Warn, fmt);
}

pub fn error_log(fmt: std::fmt::Arguments) {
    log_message(Level::Error, fmt);
}

pub fn critical_log(fmt: std::fmt::Arguments) {
    log_message(Level::Error, fmt); // Treat critical as error
}

/*
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
*/

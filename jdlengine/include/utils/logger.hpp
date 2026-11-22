#pragma once

#include <cstdlib>
#include <memory>
#include <string>

#include <spdlog/spdlog.h>


namespace jdl
{
namespace utils
{

enum LogLevel
{
    eDebug   = spdlog::level::debug,
    eInfo    = spdlog::level::info,
    eWarning = spdlog::level::warn,
    eError   = spdlog::level::err,
    eFatal   = spdlog::level::critical
};


struct LoggerConfig
{
    /** Logger name */
    std::string logger_name = "JDLEngine";
    /** Console default severity level */
    LogLevel console_level = LogLevel::eInfo;
};


class Logger
{
public:
    /**
     * @brief Initializes the logger. Must be called before any logging call.
     * @param config The logger configuration.
     */
    static void Init(const LoggerConfig& config = {});

    /**
     * @brief Sets the minimum severity level of logged messages.
     * @param level The new severity level.
     */
    static void SetLevel(LogLevel level);

    /**
     * @brief Logs a debug message.
     * @param fmt The format string.
     * @param args The format arguments.
     */
    template<typename... Args>
    static void Debug(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (s_Logger != nullptr) {
            s_Logger->debug(fmt, std::forward<Args>(args)...);
        }
    }

    /**
     * @brief Logs an info message.
     * @param fmt The format string.
     * @param args The format arguments.
     */
    template<typename... Args>
    static void Info(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (s_Logger != nullptr) {
            s_Logger->info(fmt, std::forward<Args>(args)...);
        }
    }

    /**
     * @brief Logs a warning message.
     * @param fmt The format string.
     * @param args The format arguments.
     */
    template<typename... Args>
    static void Warning(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (s_Logger != nullptr) {
            s_Logger->warn(fmt, std::forward<Args>(args)...);
        }
    }

    /**
     * @brief Logs an error message.
     * @param fmt The format string.
     * @param args The format arguments.
     */
    template<typename... Args>
    static void Error(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (s_Logger != nullptr) {
            s_Logger->error(fmt, std::forward<Args>(args)...);
        }
    }

    /**
     * @brief Logs a fatal message, then terminates the process immediately.
     *
     * No destructor is run: the operating system and the driver reclaim every
     * resource of the process, so callers do not need any cleanup on this path.
     * std::abort() also lets an attached debugger stop at the failure point.
     * @param fmt The format string.
     * @param args The format arguments.
     */
    template<typename... Args>
    [[noreturn]] static void Fatal(spdlog::format_string_t<Args...> fmt, Args&&... args)
    {
        if (s_Logger != nullptr)
        {
            s_Logger->critical(fmt, std::forward<Args>(args)...);
            // The process ends without unwinding: write the message out now.
            s_Logger->flush();
        }
        std::abort();
    }

private:
    static std::shared_ptr<spdlog::logger> s_Logger;
};

} // namespace utils
} // namespace jdl

#define JDL_DEBUG(...)  jdl::utils::Logger::Debug(__VA_ARGS__)
#define JDL_INFO(...)   jdl::utils::Logger::Info(__VA_ARGS__)
#define JDL_WARN(...)   jdl::utils::Logger::Warning(__VA_ARGS__)
#define JDL_ERROR(...)  jdl::utils::Logger::Error(__VA_ARGS__)
#define JDL_FATAL(...)  jdl::utils::Logger::Fatal(__VA_ARGS__)

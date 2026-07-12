#pragma once

#include "non_copyable.hpp"

#include <spdlog/spdlog.h>


namespace jdl
{
namespace utils
{

enum class LogLevel
{
    eTrace = spdlog::level::trace,
    eDebug = spdlog::level::debug,
    eInfo = spdlog::level::info,
    eWarning = spdlog::level::warn,
    eError = spdlog::level::err,
    eFatal = spdlog::level::critical
};

class Logger : private NonCopyable<Logger>
{
public:
    /**
     * @brief Inits the logger.
     */
    static void Init();

    /**
     * @brief Sets the logger severity level.
     * @param level The new level. All logs with a lower level will not be displayed.
     */
    static void SetLevel(LogLevel level) {
        s_Logger->set_level(static_cast<spdlog::level::level_enum>(level));
    }

    /**
     * @brief Logs a message with the TRACE level.
     */
    template<typename... Args>
    static void Trace(Args&&... args) {
        s_Logger->trace(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the DEBUG level.
     */
    template<typename... Args>
    static void Debug(Args&&... args) {
        s_Logger->debug(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the INFO level.
     */
    template<typename... Args>
    static void Info(Args&&... args) {
        s_Logger->info(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the WARNING level.
     */
    template<typename... Args>
    static void Warning(Args&&... args) {
        s_Logger->warn(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the ERROR level.
     */
    template<typename... Args>
    static void Error(Args&&... args) {
        s_Logger->error(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the FATAL level, and throws an exception.
     */
    template<typename... Args>
    static void Fatal(Args&&... args)
    {
        s_Logger->critical(std::forward<Args>(args)...);
        throw std::runtime_error("FATAL error raised");
    }

private:
    static std::shared_ptr<spdlog::logger> s_Logger;
};

} // namespace utils
} // namespace jdl

#define JDL_TRACE(...) ::jdl::utils::Logger::Trace(__VA_ARGS__)
#define JDL_DEBUG(...) ::jdl::utils::Logger::Debug(__VA_ARGS__)
#define JDL_INFO(...)  ::jdl::utils::Logger::Info(__VA_ARGS__)
#define JDL_WARN(...)  ::jdl::utils::Logger::Warning(__VA_ARGS__)
#define JDL_ERROR(...) ::jdl::utils::Logger::Error(__VA_ARGS__)
#define JDL_FATAL(...) ::jdl::utils::Logger::Fatal(__VA_ARGS__)

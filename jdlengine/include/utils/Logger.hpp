#pragma once

#include "core/Core.hpp"

#include "NonCopyable.hpp"

#include <spdlog/spdlog.h>


namespace utils
{

enum class LogLevel
{
    eTrace   = spdlog::level::trace,
    eDebug   = spdlog::level::err,
    eInfo    = spdlog::level::info,
    eWarning = spdlog::level::warn,
    eError   = spdlog::level::err,
    eFatal   = spdlog::level::critical
};

class JDL_API Logger : private NonCopyable<Logger>
{
public:
    /**
     * @brief Inits the logger.
     */
    static void Init();

    /**
     * @brief Sets the logger minimal severity level.
     * @param level The new severity level
     */
    static void SetLevel(LogLevel level);

    /**
     * @brief Logs a message with the Trace level.
     */
    template<typename... Args>
    static void Trace(Args&&... args) {
        LOGGER->trace(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the Debug level.
     */
    template<typename... Args>
    static void Debug(Args&&... args) {
        LOGGER->debug(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the Info level.
     */
    template<typename... Args>
    static void Info(Args&&... args) {
        LOGGER->info(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the Warning level.
     */
    template<typename... Args>
    static void Warning(Args&&... args) {
        LOGGER->warn(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the Error level.
     */
    template<typename... Args>
    static void Error(Args&&... args) {
        LOGGER->error(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the Fatal level, and exits the application
     */
    template<typename... Args>
    static void Fatal(Args&&... args) {
        LOGGER->critical(std::forward<Args>(args)...);
        std::terminate();
    }

private:
    static std::shared_ptr<spdlog::logger> LOGGER;
};

#define JDL_TRACE(...) ::utils::Logger::Trace(__VA_ARGS__)
#define JDL_DEBUG(...) ::utils::Logger::Debug(__VA_ARGS__)
#define JDL_INFO(...)  ::utils::Logger::Info(__VA_ARGS__)
#define JDL_WARN(...)  ::utils::Logger::Warning(__VA_ARGS__)
#define JDL_ERROR(...) ::utils::Logger::Error(__VA_ARGS__)
#define JDL_FATAL(...) ::utils::Logger::Fatal(__VA_ARGS__)

} // namespace utils

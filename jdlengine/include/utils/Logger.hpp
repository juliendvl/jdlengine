#pragma once

#include "core/Core.hpp"

#include "NonCopyable.hpp"

#include <spdlog/spdlog.h>


namespace jdl
{
namespace utils
{

enum class LogLevel
{
    kTrace   = spdlog::level::trace,
    kDebug   = spdlog::level::debug,
    kInfo    = spdlog::level::info,
    kWarning = spdlog::level::warn,
    kError   = spdlog::level::err,
    kFatal   = spdlog::level::critical
};

class JDL_API Logger : private NonCopyable<Logger>
{
public:
    /**
     * @brief Inits the logger.
     */
    static void Init();

    /**
     * @brief Sets the severity level.
     * @param level New severity level. All logs with a lower level will not be displayed.
     */
    static void SetLevel(LogLevel level);

    /**
     * @brief Logs a message with the Trace level.
     */
    template<typename... Args>
    static void Trace(Args&&... args) {
        ILogger->trace(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the Debug level.
     */
    template<typename... Args>
    static void Debug(Args&&... args) {
        ILogger->debug(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the Info level.
     */
    template<typename... Args>
    static void Info(Args&&... args) {
        ILogger->info(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the Warning level.
     */
    template<typename... Args>
    static void Warning(Args&&... args) {
        ILogger->warn(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the Error level.
     */
    template<typename... Args>
    static void Error(Args&&... args) {
        ILogger->error(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the Fatal level, and throws an exception.
     */
    template<typename... Args>
    static void Fatal(Args&&... args) {
        ILogger->critical(std::forward<Args>(args)...);
        throw std::runtime_error("Fatal error raised!");
    }

private:
    static std::shared_ptr<spdlog::logger> ILogger;
};

#define JDL_TRACE(...) jdl::utils::Logger::Trace(__VA_ARGS__)
#define JDL_DEBUG(...) jdl::utils::Logger::Debug(__VA_ARGS__)
#define JDL_INFO(...) jdl::utils::Logger::Info(__VA_ARGS__)
#define JDL_WARN(...) jdl::utils::Logger::Warning(__VA_ARGS__)
#define JDL_ERROR(...) jdl::utils::Logger::Error(__VA_ARGS__)
#define JDL_FATAL(...) jdl::utils::Logger::Fatal(__VA_ARGS__)

} // namespace utils
} // namespace jdl

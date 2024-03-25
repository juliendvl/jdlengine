#pragma once

#include "core/Core.hpp"

#include "NonCopyable.hpp"

// Temporarily disable warnings generating when including spdlog.h
#pragma warning(push)
#pragma warning(disable: 4043)
#pragma warning(disable: 4996)
#include <spdlog/spdlog.h>
#pragma warning(pop)


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
     * @brief Inits the logger. Must be called only once!
     */
    static void Init();

    /**
     * @brief Sets the logger severity level.
     * @param level Severity level. All logs with a lower level will not be displayed.
     */
    static void SetLevel(LogLevel level);

    /**
     * @brief Logs a message with the Trace level.
     */
    template<typename... Args>
    static void Trace(Args&&... args)
    {
        if (ILogger != nullptr)
            ILogger->trace(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the Debug level.
     */
    template<typename... Args>
    static void Debug(Args&&... args)
    {
        if (ILogger != nullptr)
            ILogger->debug(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the Info level.
     */
    template<typename... Args>
    static void Info(Args&&... args)
    {
        if (ILogger != nullptr)
            ILogger->info(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the Warning level.
     */
    template<typename... Args>
    static void Warning(Args&&... args)
    {
        if (ILogger != nullptr)
            ILogger->warn(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the Error level.
     */
    template<typename... Args>
    static void Error(Args&&... args)
    {
        if (ILogger != nullptr)
            ILogger->error(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the Fatal level, and raises an exception.
     */
    template<typename... Args>
    static void Fatal(Args&&... args)
    {
        if (ILogger != nullptr)
            ILogger->critical(std::forward<Args>(args)...);
        throw std::runtime_error("Fatal error raised");
    }

private:
    // Unique logger object
    static std::shared_ptr<spdlog::logger> ILogger;
};

} // namespace utils
} // namespace jdl

#define JDL_TRACE(...) ::jdl::utils::Logger::Trace(__VA_ARGS__)
#define JDL_DEBUG(...) ::jdl::utils::Logger::Debug(__VA_ARGS__)
#define JDL_INFO(...)  ::jdl::utils::Logger::Info(__VA_ARGS__)
#define JDL_WARN(...)  ::jdl::utils::Logger::Warning(__VA_ARGS__)
#define JDL_ERROR(...) ::jdl::utils::Logger::Error(__VA_ARGS__)
#define JDL_FATAL(...) ::jdl::utils::Logger::Fatal(__VA_ARGS__)

#pragma once

#include "core/Core.hpp"

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

class JDL_API Logger
{
public:
    /**
     * @brief Inits the logger instance.
     */
    static void Init();

    /**
     * @brief Sets the minimal severity level.
     * @param level Severity level. All logs with a lower level will not be displayed.
     */
    static void SetLevel(LogLevel level) {
        LOGGER->set_level(static_cast<spdlog::level::level_enum>(level));
    }

    /**
     * @brief Logs a message with the kTrace level.
     */
    template<typename... Args>
    static void Trace(Args&&... args) {
        LOGGER->trace(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the kDebug level.
     */
    template<typename... Args>
    static void Debug(Args&&... args) {
        LOGGER->debug(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the kInfo level.
     */
    template<typename... Args>
    static void Info(Args&&... args) {
        LOGGER->info(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the kWarning level.
     */
    template<typename... Args>
    static void Warning(Args&&... args) {
        LOGGER->warn(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the kError level.
     */
    template<typename... Args>
    static void Error(Args&&... args) {
        LOGGER->error(std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message with the kFatal level, and raises an exception.
     */
    template<typename... Args>
    static void Fatal(Args&&... args) {
        LOGGER->critical(std::forward<Args>(args)...);
        throw std::runtime_error("see the logger output for details");
    }

private:
    static std::shared_ptr<spdlog::logger> LOGGER;
};

#define JDL_TRACE(...) ::jdl::utils::Logger::Trace(__VA_ARGS__)
#define JDL_DEBUG(...) ::jdl::utils::Logger::Debug(__VA_ARGS__)
#define JDL_INFO(...)  ::jdl::utils::Logger::Info(__VA_ARGS__)
#define JDL_WARN(...)  ::jdl::utils::Logger::Warning(__VA_ARGS__)
#define JDL_ERROR(...) ::jdl::utils::Logger::Error(__VA_ARGS__)
#define JDL_FATAL(...) ::jdl::utils::Logger::Fatal(__VA_ARGS__)

} // namespace utils
} // namespace jdl

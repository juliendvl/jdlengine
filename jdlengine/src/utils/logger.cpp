#include "utils/logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>


namespace jdl
{
namespace utils
{

std::shared_ptr<spdlog::logger> Logger::s_Logger = nullptr;

void Logger::Init(const LoggerConfig& config)
{
    if (s_Logger == nullptr)
    {
        s_Logger = spdlog::stdout_color_mt(config.logger_name);
        s_Logger->set_level(
            static_cast<spdlog::level::level_enum>(config.console_level)
        );
        s_Logger->set_pattern("%^[%T][%n] %v%$");
    }
}

void Logger::SetLevel(LogLevel level)
{
    s_Logger->set_level(
        static_cast<spdlog::level::level_enum>(level)
    );
}

} // namespace utils
} // namespace jdl

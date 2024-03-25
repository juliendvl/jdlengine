#include "utils/Logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>


namespace jdl
{
namespace utils
{

std::shared_ptr<spdlog::logger> Logger::ILogger = nullptr;

void Logger::Init()
{
    if (ILogger == nullptr)
    {
        ILogger = spdlog::stdout_color_mt("JDLEngine");
        ILogger->set_level(spdlog::level::info);
        ILogger->set_pattern("%^[%T][%n] %v%$");
    }
}

void Logger::SetLevel(LogLevel level)
{
    if (ILogger != nullptr)
        ILogger->set_level(static_cast<spdlog::level::level_enum>(level));
}

} // namespace utils
} // namespace jdl

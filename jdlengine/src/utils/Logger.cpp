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
        ILogger->set_pattern("%^[%T][%n] %v%$");
        ILogger->set_level(spdlog::level::info);
    }
}

} // namespace utils
} // namespace jdl

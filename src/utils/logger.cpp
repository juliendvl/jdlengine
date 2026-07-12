#include "utils/logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>


namespace jdl
{
namespace utils
{

std::shared_ptr<spdlog::logger> Logger::s_Logger = nullptr;

void Logger::Init()
{
    if (s_Logger == nullptr)
    {
        s_Logger = spdlog::stdout_color_mt("JDLEngine");
        s_Logger->set_level(spdlog::level::info);
        s_Logger->set_pattern("%^[%T][%n] %v%$");
    }
}

} // namespace utils
} // namespace jdl

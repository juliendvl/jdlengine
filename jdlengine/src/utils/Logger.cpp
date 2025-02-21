#include "utils/Logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>


namespace jdl
{
namespace utils
{

std::shared_ptr<spdlog::logger> Logger::LOGGER = nullptr;

void Logger::Init()
{
    if (LOGGER == nullptr)
    {
        LOGGER = spdlog::stdout_color_mt("JDLEngine");
        LOGGER->set_level(spdlog::level::info);
        LOGGER->set_pattern("%^[%T][%n] %v%$");
    }
}

} // namespace utils
} // namespace jdl

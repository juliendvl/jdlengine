#include "utils/Logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>


namespace jdl
{
namespace utils
{

std::shared_ptr<spdlog::logger> Logger::InnerLogger = nullptr;

void Logger::Init()
{
	if (InnerLogger == nullptr)
	{
		InnerLogger = spdlog::stdout_color_mt("JDLEngine");
		InnerLogger->set_level(spdlog::level::info);
		InnerLogger->set_pattern("%^[%T][%l][%n] %v%$");
	}
}

void Logger::SetLevel(LogLevel level)
{
	InnerLogger->set_level(static_cast<spdlog::level::level_enum>(level));
}

} // namespace utils
} // namespace jdl

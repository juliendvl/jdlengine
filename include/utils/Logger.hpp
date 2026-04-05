#pragma once

#include <spdlog/spdlog.h>


namespace jdl
{
namespace utils
{

enum class LogLevel
{
	eTrace = spdlog::level::trace,
	eDebug = spdlog::level::debug,
	eInfo = spdlog::level::info,
	eWarning = spdlog::level::warn,
	eError = spdlog::level::err,
	eFatal = spdlog::level::critical
};

class Logger
{
public:
	/**
	 * @brief Inits the logger.
	 */
	static void Init();

	/**
	 * @brief Sets the logger severity level.
	 * @param level New severity level. All logs with a lower level will
	 *	not be displayed.
	 */
	static void SetLevel(LogLevel level);

	template<typename... Args>
	static void Trace(Args&&... args) {
		InnerLogger->trace(std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void Debug(Args&&... args) {
		InnerLogger->debug(std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void Info(Args&&... args) {
		InnerLogger->info(std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void Warning(Args&&... args) {
		InnerLogger->warn(std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void Error(Args&&... args) {
		InnerLogger->error(std::forward<Args>(args)...);
	}

	template<typename... Args>
	static void Fatal(Args&&... args)
	{
		InnerLogger->critical(std::forward<Args>(args)...);
		throw std::runtime_error("Fatal error raised");
	}

private:
	static std::shared_ptr<spdlog::logger> InnerLogger;
};

} // namespace utils
} // namespace jdl

#define JDL_TRACE(...)	::jdl::utils::Logger::Trace(__VA_ARGS__)
#define JDL_DEBUG(...)	::jdl::utils::Logger::Debug(__VA_ARGS__)
#define JDL_INFO(...)	::jdl::utils::Logger::Info(__VA_ARGS__)
#define JDL_WARN(...)	::jdl::utils::Logger::Warning(__VA_ARGS__)
#define JDL_ERROR(...)	::jdl::utils::Logger::Error(__VA_ARGS__)
#define JDL_FATAL(...)	::jdl::utils::Logger::Fatal(__VA_ARGS__)

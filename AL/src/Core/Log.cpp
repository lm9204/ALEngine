#include "Core/Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace ale
{
std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

void Log::init()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	s_CoreLogger = spdlog::stdout_color_mt("ALEngine");
	s_CoreLogger->set_level(spdlog::level::trace);

	s_ClientLogger = spdlog::stdout_color_mt("App");
	s_ClientLogger->set_level(spdlog::level::trace);
}

inline std::shared_ptr<spdlog::logger> &Log::getCoreLogger()
{
	return s_CoreLogger;
}

inline std::shared_ptr<spdlog::logger> &Log::getClientLogger()
{
	return s_ClientLogger;
}
} // namespace ale
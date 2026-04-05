#include "core/Application.hpp"

#include "utils/Logger.hpp"

#include <iostream>

using namespace jdl;


class JDLEngine : public core::Application
{
public:
	JDLEngine(const char* name, int width, int height)
		: core::Application(name, width, height)
	{
		JDL_TRACE("Message");
		JDL_DEBUG("Message");
		JDL_INFO("Message");
		JDL_WARN("Message");
		JDL_ERROR("Message");
	}
};


int main(int argc, char** argv)
{
	try
	{
		utils::Logger::Init();

		JDLEngine application("JDLEngine", 800, 600);
		application.run();

		return EXIT_SUCCESS;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}

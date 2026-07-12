#include <iostream>

#include "core/application.hpp"

#include "utils/logger.hpp"

using namespace jdl;


class Sandbox : public core::Application
{
public:
    Sandbox(const char* name, int width, int height)
        : core::Application(name, width, height)
    {
        JDL_TRACE("TRACE Message");
        JDL_DEBUG("DEBUG Message");
        JDL_INFO("INFO Message");
        JDL_WARN("WARN Message");
        JDL_ERROR("ERROR Message");
    }
};


int main(int argc, char** argv)
{
    try
    {
        utils::Logger::Init();

        Sandbox application("JDLEngine", 800, 600);
        application.run();

        return EXIT_SUCCESS;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

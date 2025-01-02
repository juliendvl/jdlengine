#pragma once

#include "Application.hpp"

#include "utils/Logger.hpp"


extern std::unique_ptr<jdl::core::Application> CreateApplication(const char* name, int width, int height);

int main(int argc, char** argv)
{
    try
    {
        jdl::utils::Logger::Init();

        auto application = CreateApplication("JDLEngine", 800, 600);
        application->run();

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

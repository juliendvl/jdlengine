#pragma once

#include "core/Application.hpp"


extern std::unique_ptr<jdl::core::Application> CreateApplication(const char* name, int width, int height);

int main(int argc, char** argv)
{
    try
    {
        auto application = CreateApplication("JDLEngine", 800, 600);
        application->run();

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

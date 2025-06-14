#pragma once

#include "Application.hpp"

#include <iostream>

#include "utils/Logger.hpp"


std::unique_ptr<core::Application> CreateApplication(const char* name, int width, int height);

int main(int argc, char** argv)
{
    try
    {
        utils::Logger::Init();

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

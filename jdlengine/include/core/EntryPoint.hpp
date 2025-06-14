#pragma once

#include "Application.hpp"

#include <iostream>


std::unique_ptr<core::Application> CreateApplication(const char* name, int width, int height);

int main(int argc, char** argv)
{
    auto application = CreateApplication("JDLEngine", 800, 600);
    try
    {
        application->run();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

#pragma once

#include "Application.hpp"

#include "utils/Logger.hpp"


extern std::unique_ptr<jdl::core::Application> CreateApplication(const char* name);

int main(int argc, char** argv)
{
    try
    {
        jdl::utils::Logger::Init();

        auto application = CreateApplication("JDLEngine");
        application->run();

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        return EXIT_FAILURE;
    }
}

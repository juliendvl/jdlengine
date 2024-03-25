#pragma once

#include <memory>

#include "Application.hpp"

#include "utils/Logger.hpp"


using namespace jdl;


extern std::unique_ptr<core::Application> CreateApplication(const char* name, int width, int height);

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
        return EXIT_FAILURE;
    }
}

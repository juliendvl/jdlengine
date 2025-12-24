#pragma once

#include "core/Core.hpp"
#include "core/Application.hpp"

#include "utils/Logger.hpp"

#include <iostream>

using namespace jdl;


extern std::unique_ptr<core::Application> core::CreateApplication(const core::ApplicationArgs& args);


int main(int argc, char** argv)
{
    core::ApplicationArgs args;
    args.name = "JDLEngine";
    args.width = 800;
    args.height = 600;

    try
    {
        utils::Logger::Init();

        auto application = CreateApplication(args);
        application->run();

        return EXIT_SUCCESS;
    }
    catch (const std::exception& exc)
    {
        std::cerr << exc.what() << std::endl;
        return EXIT_FAILURE;
    }
}

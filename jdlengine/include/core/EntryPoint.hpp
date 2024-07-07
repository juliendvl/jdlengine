#pragma once

#include "Application.hpp"


extern std::unique_ptr<jdl::core::Application> CreateApplication(const char* name);

int main(int argc, char** argv)
{
    try
    {
        auto application = CreateApplication("JDLEngine");

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        return EXIT_FAILURE;
    }
}

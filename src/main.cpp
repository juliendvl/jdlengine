#include <iostream>

#include "core/application.hpp"

using namespace jdl;


class Sandbox : public core::Application
{
public:
    Sandbox(const char* name, int width, int height)
        : core::Application(name, width, height)
    {
        std::cout << "Hello World!" << std::endl;
    }
};


int main(int argc, char** argv)
{
    try
    {
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

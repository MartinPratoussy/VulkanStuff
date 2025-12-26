#include <format>
#include <iostream>

#include "TriangleApp.hpp"

int main()
{
    TriangleApp app;

    try
    {
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cout << std::format("{}", e.what()) << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
#pragma once

#include <print>

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
        std::println("{}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
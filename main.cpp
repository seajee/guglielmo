#include <iostream>

#include "Application.h"

int main(void)
{
    Application app(600, 600, "OpenGL");

    if (!app.Init()) {
        std::cerr << "ERROR: Could not initialize Application" << std::endl;
        return EXIT_FAILURE;
    }

    app.Run();

    return EXIT_SUCCESS;
}

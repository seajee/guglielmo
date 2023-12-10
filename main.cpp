#include <iostream>

#include "Application.h"

int main(void)
{
    Application app(960, 540, "OpenGL");

    if (!app.Init()) {
        std::cerr << "ERROR: Could not initialize Application" << std::endl;
        return EXIT_FAILURE;
    }

    app.Run();

    return EXIT_SUCCESS;
}

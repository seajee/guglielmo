#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

class Application
{
public:
    Application(
            int windowWidth,
            int windowHeight,
            const std::string& windowTitle);
    ~Application();

    bool Init();
    void Run();
private:
    GLFWwindow* m_Window;
    int m_WindowWidth;
    int m_WindowHeight;
    std::string m_WindowTitle;
};

void APIENTRY DebugMessageCallback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam);

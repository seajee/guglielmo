#include "Application.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

Application::Application(
        int windowWidth,
        int windowHeight,
        const std::string& windowTitle)
    :
        m_WindowWidth(windowWidth),
        m_WindowHeight(windowHeight),
        m_WindowTitle(windowTitle)
{
}

Application::~Application()
{
    glfwTerminate();
}

bool Application::Init()
{
    if (!glfwInit()) {
        std::cerr << "ERROR: Could not initialize glfw" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, m_WindowTitle.c_str(), NULL, NULL);

    if (!m_Window) {
        std::cerr << "ERROR: Could not create glfw window" << std::endl;
        return false;
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cerr << "ERROR: Could not initialize glew" << std::endl;
        return false;
    }

    std::cout << "INFO: OpenGL version: " << glGetString(GL_VERSION) << std::endl;

#ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugMessageCallback, nullptr);
#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

void Application::Run()
{
    float positions[16] = {
        // positions    // textureCoords
        100.0f, 100.0f, 0.0f, 0.0f,
        200.0f, 100.0f, 1.0f, 0.0f,
        200.0f, 200.0f, 1.0f, 1.0f,
        100.0f, 200.0f, 0.0f, 1.0f
    };

    unsigned int indices[6] = {
        0, 1, 2,
        2, 3, 0
    };

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;

    layout.Push(GL_FLOAT, 2);
    layout.Push(GL_FLOAT, 2);

    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    glm::mat4 proj = glm::ortho(0.0f, (float)m_WindowWidth, 0.0f, (float)m_WindowHeight, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 0.0f));

    glm::mat4 mvp = proj * view * model;

    Shader shader("./shaders/vertex.vert", "./shaders/fragment.frag");
    shader.Bind();
    shader.SetUniformMat4f("u_MVP", mvp);

    Renderer renderer;

    Texture texture("./assets/pepe.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    while (!glfwWindowShouldClose(m_Window))
    {
        renderer.Clear();
        renderer.Draw(va, ib, shader);

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

void APIENTRY DebugMessageCallback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
{
    (void)source;
    (void)id;
    (void)length;
    (void)userParam;

    std::cerr << std::hex << "INFO: OpenGL Callback: "
        << (type == GL_DEBUG_TYPE_ERROR ? "GL_DEBUG_TYPE_ERROR" : "OTHER")
        << " type=0x" << type
        << ", severity=0x" << severity
        << ", message=" << message << std::endl;
}

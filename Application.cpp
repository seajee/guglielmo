#include "Application.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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
    glfwSwapInterval(1); // Enable VSync

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
    // Buffers
    float positions[] = {
        // Positions           TexCoords
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, // lower left
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f, // upper left
         0.5f,  0.5f, 0.0f,    1.0f, 1.0f, // upper right
         0.5f, -0.5f, 0.0f,    1.0f, 0.0f  // lower right
    };

    unsigned int indices[6] = {
        0, 2, 1,
        0, 3, 2
    };

    // Essentials
    VertexArray va;
    VertexBuffer vb(positions, 5 * 4 * sizeof(float));
    VertexBufferLayout layout;

    layout.Push(GL_FLOAT, 3);
    layout.Push(GL_FLOAT, 2);

    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    // Model view projection matrices (transform, camera, projection)
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
    //proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    proj = glm::perspective(glm::radians(45.0f), (float)m_WindowWidth / m_WindowHeight, 0.1f, 100.0f);

    // Shader
    Shader shader("./shaders/vertex.vert", "./shaders/fragment.frag");
    shader.Bind();

    // Renderer
    Renderer renderer;

    // ImGui
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.IniFilename = NULL;
    io.LogFilename = NULL;

    ImGui::StyleColorsDark();

    // Texture
    Texture texture("./assets/pepe.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    // Unbind everything
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    glm::vec3 translation(0.0f, 0.0f, 0.0f);
    float rotation = 0.0f;

    // Main loop
    while (!glfwWindowShouldClose(m_Window))
    {
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            model = glm::translate(glm::mat4(1.0f), translation);
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 mvp = proj * view * model;

            ImGui::SliderFloat3("translation", &translation.x, -10.0f, 10.0f);
            ImGui::SliderFloat("rotation", &rotation, 0.0f, 360.0f);
            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        }

        renderer.Draw(va, ib, shader);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

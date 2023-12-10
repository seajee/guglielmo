#include "Shader.h"

#include <GL/glew.h>
#include <iostream>

#include "Utils.h"

Shader::Shader(
        const std::string& vertexShaderFilePath,
        const std::string& fragmentShaderFilePath)
    :
        m_VertexShaderFilePath(vertexShaderFilePath),
        m_FragmentShaderFilePath(fragmentShaderFilePath),
        m_RendererId(0)
{
    m_RendererId = glCreateProgram();

    std::string vertexShaderSource = ReadFile(vertexShaderFilePath);
    std::string fragmentShaderSource = ReadFile(fragmentShaderFilePath);

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(m_RendererId, vs);
    glAttachShader(m_RendererId, fs);

    glLinkProgram(m_RendererId);
    glValidateProgram(m_RendererId);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererId);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererId);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int v0)
{
    glUniform1i(GetUniformLocation(name), v0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cerr << "ERROR: Could not compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader: " << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }

    int location = glGetUniformLocation(m_RendererId, name.c_str());

    if (location == -1) {
        ASSERT(location != -1 && "ERROR: Invalid uniform location");
        std::cout << "WARNING: Uniform " << name << " does not exist" << std::endl;
    }

    m_UniformLocationCache[name] = location;

    return location;
}

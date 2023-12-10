#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader(const std::string& vertexShaderFilePath,
            const std::string& fragmentShaderFilePath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& name, int v0);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
    std::string m_VertexShaderFilePath;
    std::string m_FragmentShaderFilePath;
    unsigned int m_RendererId;

    std::unordered_map<std::string, int> m_UniformLocationCache;

    unsigned int CompileShader(unsigned int type, const std::string& source);
    int GetUniformLocation(const std::string& name);
};

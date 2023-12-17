#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
    Camera(unsigned int width, unsigned int height, const glm::vec3& position,
            float fov, float nearPlane, float farPlane);

    glm::mat4 Matrix() const;
    void Inputs(GLFWwindow* window);

public:
    glm::vec3 position;

private:
    int m_Width;
    int m_Height;

    glm::vec3 m_Up;
    glm::vec3 m_Orientation;

    float m_FOV;
    float m_NearPlane;
    float m_FarPlane;

    float m_Speed;
    float m_Sensitivity;
};

#include "Camera.h"

#include <glm/ext.hpp>

Camera::Camera(unsigned int width, unsigned int height, const glm::vec3& position,
        float fov, float nearPlane, float farPlane) :
    position(position),
    m_Width(width),
    m_Height(height),
    m_FOV(fov),
    m_NearPlane(nearPlane),
    m_FarPlane(farPlane)
{
    m_Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

    m_Speed = 0.1f;
}

glm::mat4 Camera::Matrix() const
{
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    view = glm::lookAt(position, position + m_Orientation, m_Up);
    proj = glm::perspective(glm::radians(m_FOV), (float)m_Width / m_Height, m_NearPlane, m_FarPlane);

    return proj * view;
}

void Camera::Inputs(GLFWwindow* window)
{
    //  W
    // ASD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += m_Speed * m_Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position += m_Speed * -glm::normalize(glm::cross(m_Orientation, m_Up));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position += m_Speed * -m_Orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += m_Speed * glm::normalize(glm::cross(m_Orientation, m_Up));
    }

    // Up and down
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        position += m_Speed * m_Up;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        position += m_Speed * -m_Up;
    }

    // Speed
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        m_Speed = 0.4f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        m_Speed = 0.1f;
    }
}

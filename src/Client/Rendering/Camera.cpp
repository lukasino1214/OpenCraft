//
// Created by lukas on 21.12.21.
//

#include "Camera.h"
#include "../../Tracy/Tracy.hpp"

Camera::Camera(const int &width, const int &height, const glm::vec3 &position) : m_Position(position), m_Width(width), m_Height(height) {
    m_View = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
    m_Projection = glm::perspective(glm::radians(m_FOV), (float)m_Width / m_Height, nearPlane, farPlane);
}

void Camera::Inputs(GLFWwindow* window) {
    // Handles mouse inputs
    ZoneScoped;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        // Hides mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Prevents camera from jumping on the first click
        if (firstClick)
        {
            glfwSetCursorPos(window, (m_Width / 2), (m_Height / 2));
            firstClick = false;
        }

        // Handles key inputs
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            m_Position += speed * m_Orientation;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            m_Position += speed * -glm::normalize(glm::cross(m_Orientation, m_Up));
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            m_Position += speed * -m_Orientation;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            m_Position += speed * glm::normalize(glm::cross(m_Orientation, m_Up));
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            m_Position += speed * m_Up;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            m_Position += speed * -m_Up;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            speed = 0.4f;
        }
        else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
        {
            speed = 0.1f;
        }

        // Stores the coordinates of the cursor
        double mouseX;
        double mouseY;
        // Fetches the coordinates of the cursor
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
        // and then "transforms" them into degrees
        float rotX = sensitivity * (float)(mouseY - (m_Height / 2)) / m_Height;
        float rotY = sensitivity * (float)(mouseX - (m_Width / 2)) / m_Width;

        // Calculates upcoming vertical change in the Orientation
        glm::vec3 newOrientation = glm::rotate(m_Orientation, glm::radians(-rotX), glm::normalize(glm::cross(m_Orientation, m_Up)));

        // Decides whether or not the next vertical Orientation is legal or not
        if (abs(glm::angle(newOrientation, m_Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
        }

            m_Orientation = newOrientation;
        // Rotates the Orientation left and right
        m_Orientation = glm::rotate(m_Orientation, glm::radians(-rotY), m_Up);

        // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
        glfwSetCursorPos(window, (m_Width / 2), (m_Height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Makes sure the next time the camera looks around it doesn't jump
        firstClick = true;
    }

    m_View = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
}
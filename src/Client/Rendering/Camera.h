//
// Created by lukas on 21.12.21.
//

#ifndef OPENCRAFT_CAMERA_H
#define OPENCRAFT_CAMERA_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    bool firstClick = true;
    float speed = 0.1f;
    float sensitivity = 100.0f;

    // Camera constructor to set up initial values
    Camera(const int &width, const int &height, const glm::vec3 &position);

    glm::mat4 GetView() { return m_View; }
    glm::mat4 GetProjection() { return m_Projection; }

    void SetProjection(int width, int height) {
        m_Width = width;
        m_Height = height;
        m_Projection = glm::perspective(glm::radians(m_FOV), (float)m_Width / m_Height, nearPlane, farPlane);
    }

    glm::vec3 GetPosition() {
        return m_Position;
    }

    void Inputs(GLFWwindow* window);
private:
    int m_Width;
    int m_Height;
    float m_FOV = { 90.0f };
    float nearPlane = { 0.001f };
    float farPlane = { 1000.0f };

    glm::mat4 m_View = glm::mat4(1.0f);
    glm::mat4 m_Projection = glm::mat4(1.0f);

    glm::vec3 m_Position;
    glm::vec3 m_Orientation = { -1.0f, -1.0f, -1.0f };
    glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };
};


#endif //OPENCRAFT_CAMERA_H

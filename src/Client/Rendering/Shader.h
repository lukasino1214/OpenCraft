//
// Created by lukas on 21.12.21.
//

#ifndef OPENCRAFT_SHADER_H
#define OPENCRAFT_SHADER_H


#include <unordered_map>
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <iostream>

class Shader {
public:
    Shader(const std::string& vertex, const std::string& fragment);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetInt(const std::string& name, const uint32_t value);
    void SetFloat3(const std::string& name, const glm::vec3& value);
    void SetMat4(const std::string &name, const glm::mat4 &value);

private:
    uint32_t m_RendererID;
};


#endif //OPENCRAFT_SHADER_H

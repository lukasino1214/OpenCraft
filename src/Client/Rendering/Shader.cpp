//
// Created by lukas on 21.12.21.
//

#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>
#include <fstream>

#include <vector>
#include <glad/glad.h>

std::string get_file_contents(const std::string& file)
{
    std::ifstream in(file, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc) {

    auto vertexContent = get_file_contents(vertexSrc);
    auto fragementContent = get_file_contents(fragmentSrc);

    auto c_strVertex = vertexContent.c_str();
    auto c_strFragment = fragementContent.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &c_strVertex, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &c_strFragment, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    m_RendererID = glCreateProgram();
    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);
    glLinkProgram(m_RendererID);
    // check for linking errors
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_RendererID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    //glDeleteProgram(m_RendererID);
}

void Shader::Bind() const {
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::SetFloat3(const std::string& name, const glm::vec3& value) {
    glProgramUniform3f(m_RendererID, glGetUniformLocation(m_RendererID, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &value) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glProgramUniformMatrix4fv(m_RendererID, location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetInt(const std::string& name, const uint32_t value) {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glProgramUniform1i(m_RendererID, location, value);
}

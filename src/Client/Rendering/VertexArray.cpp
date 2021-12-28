//
// Created by lukas on 21.12.21.
//

#include "VertexArray.h"

#include <glad/glad.h>

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch(type) {
        case ShaderDataType::Float:     return GL_FLOAT;
        case ShaderDataType::Float2:    return GL_FLOAT;
        case ShaderDataType::Float3:    return GL_FLOAT;
        case ShaderDataType::Float4:    return GL_FLOAT;
        case ShaderDataType::Mat3:      return GL_FLOAT;
        case ShaderDataType::Mat4:      return GL_FLOAT;
        case ShaderDataType::Int:       return GL_INT;
        case ShaderDataType::Int3:      return GL_INT;
        case ShaderDataType::Int4:      return GL_INT;
        case ShaderDataType::Int2:      return GL_INT;
        case ShaderDataType::Bool:      return GL_BOOL;
        case ShaderDataType::None:      return 0;
    }
    return 0;
}

VertexArray::VertexArray() {
    glCreateVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const {
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

void VertexArray::SetVertexBuffer(const VertexBuffer &vertexBuffer) {
    glBindVertexArray(m_RendererID);
    vertexBuffer.Bind();

    uint32_t index = 0;
    const auto& layout = vertexBuffer.GetLayout();
    for (const auto& element : layout) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
        index++;
    }
}

void VertexArray::SetIndexBuffer(const IndexBuffer &indexBuffer) {
    glBindVertexArray(m_RendererID);
    indexBuffer.Bind();
}
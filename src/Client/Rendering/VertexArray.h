//
// Created by lukas on 21.12.21.
//

#ifndef OPENCRAFT_VERTEXARRAY_H
#define OPENCRAFT_VERTEXARRAY_H

#include "Buffers.h"

#include "../Base/Base.h"


class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    void SetVertexBuffer(const VertexBuffer& vertexBuffer);
    void SetIndexBuffer(const IndexBuffer& indexBuffer);
private:
    uint32_t m_RendererID;
};


#endif //OPENCRAFT_VERTEXARRAY_H

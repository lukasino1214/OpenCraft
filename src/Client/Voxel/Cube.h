//
// Created by lukas on 06.01.22.
//

#ifndef OPENCRAFT_CUBE_H
#define OPENCRAFT_CUBE_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Cube {
public:
    Cube() {
        std::vector<float> ver = {
                -0.5f, -0.5f, -0.5f,
                0.5f, -0.5f, -0.5f,
                0.5f,  0.5f, -0.5f,
                -0.5f,  0.5f, -0.5f,
                -0.5f, -0.5f,  0.5f,
                0.5f, -0.5f,  0.5f,
                0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f
        };

        std::vector<uint32_t> ind = {
                0, 1, 3, 3, 1, 2,
                1, 5, 2, 2, 5, 6,
                5, 4, 6, 6, 4, 7,
                4, 0, 7, 7, 0, 3,
                3, 2, 7, 7, 2, 6,
                4, 5, 0, 0, 5, 1
        };

        glCreateBuffers(1, &m_vbo);
        glNamedBufferStorage(m_vbo, ver.size() * sizeof(float), &ver[0], 0);

        glCreateBuffers(1, &m_ibo);
        glNamedBufferStorage(m_ibo, ind.size() * sizeof(uint32_t), &ind[0], 0);

        glCreateVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        struct vertex { glm::vec3 pos, nrm; };

        glEnableVertexAttribArray(0);

        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, pos));

        glVertexAttribBinding(0, 0);


        glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, 3 * sizeof(float));
        glVertexArrayElementBuffer(m_vao, m_ibo);
    }

    void Draw() {
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    }

private:
    uint32_t  m_vbo, m_ibo, m_vao;
};

#endif //OPENCRAFT_CUBE_H

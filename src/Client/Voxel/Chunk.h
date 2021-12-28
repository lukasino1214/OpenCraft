//
// Created by lukas on 22.12.21.
//

#ifndef OPENCRAFT_CHUNK_H
#define OPENCRAFT_CHUNK_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include "../Rendering/Shader.h"

#include "../Rendering/VertexArray.h"
#include "../../Tracy/Tracy.hpp"
#include "Block.h"

class Chunk {
public:
    Chunk(const glm::ivec3& position, const std::vector<float>& noise);

    BlockID GetBlock(const glm::ivec3& pos) {
        if(-1 < pos.x && pos.x < 16 && -1 < pos.y && pos.y < 16 && -1 < pos.z && pos.z < 16) {
            return m_VoxelData[pos.x][pos.y][pos.z].id;
        }

        return BlockID::Air;
    }


    void Render(Shader& shader) {
        if(Renderable) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, {16.0f * pos.x, 16.0f * pos.y, 16.0f * pos.z});
            shader.SetMat4("model", model);
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        }
    }

    /*~Chunk() {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
        glDeleteVertexArrays(1, &vao);
    }*/

    glm::ivec3 getPosition() { return pos; }

private:
    glm::ivec3 pos;
    std::array<std::array<std::array<Block, 16>, 16>, 16> m_VoxelData;
    glm::vec3 m_Color[16][16][16];

    uint32_t vbo, ibo, vao, count;
    bool Renderable = false;
};


#endif //OPENCRAFT_CHUNK_H

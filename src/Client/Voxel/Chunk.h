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

    struct NeighborChunks {
        Chunk *nx, *ny, *nz;
        Chunk *px, *py, *pz;
    };
    NeighborChunks neighbors;

    BlockID GetBlockID(const glm::ivec3& pos) {
       /* if(-1 < pos.x && pos.x < 16 && -1 < pos.y && pos.y < 16 && -1 < pos.z && pos.z < 16) {
            return m_VoxelData[pos.x][pos.y][pos.z].id;
        }

        return BlockID::Air;*/

        if (pos.x < 0) {
            if (!neighbors.nx) return BlockID::Air;
            return neighbors.nx->m_VoxelData[pos.x + 16][pos.y][pos.z].id;
        } else if (pos.x > 16 - 1) {
            if (!neighbors.px) return BlockID::Air;
            return neighbors.px->m_VoxelData[pos.x - 16][pos.y][pos.z].id;
        }
        if (pos.y < 0) {
            if (!neighbors.ny) return BlockID::Air;
            return neighbors.ny->m_VoxelData[pos.x][pos.y + 16][pos.z].id;
        } else if (pos.y > 16 - 1) {
            if (!neighbors.py) return BlockID::Air;
            return neighbors.py->m_VoxelData[pos.x][pos.y - 16][pos.z].id;
        }
        if (pos.z < 0) {
            if (!neighbors.nz) return BlockID::Air;
            return neighbors.nz->m_VoxelData[pos.x][pos.y][pos.z + 16].id;
        } else if (pos.z > 16 - 1) {
            if (!neighbors.pz) return BlockID::Air;
            return neighbors.pz->m_VoxelData[pos.x][pos.y][pos.z - 16].id;
        }
        return m_VoxelData[pos.x][pos.y][pos.z].id;
    }

    uint32_t GetID(const glm::ivec3& pos) {
        if(-1 < pos.x && pos.x < 16 && -1 < pos.y && pos.y < 16 && -1 < pos.z && pos.z < 16) {
            if(m_VoxelData[pos.x][pos.y][pos.z].id == BlockID::Stone) {
                return 2;
            } else if(m_VoxelData[pos.x][pos.y][pos.z].id == BlockID::Grass) {
                return 0;
            }
            else if(m_VoxelData[pos.x][pos.y][pos.z].id == BlockID::Dirt) {
                return 1;
            }
        }

        return 0;
    }


    void Render(Shader& shader) {
        if(Renderable) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, {16.0f * pos.x, 16.0f * pos.y, 16.0f * pos.z});
            shader.SetMat4("model", model);
            glBindVertexArray(vao);
            //glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
            glDrawArrays(GL_TRIANGLES, 0, count);
        }
    }

    /*~Chunk() {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
        glDeleteVertexArrays(1, &vao);
    }*/

    glm::ivec3 getPosition() { return pos; }

    std::array<std::array<std::array<Block, 16>, 16>, 16> m_VoxelData;

private:
    glm::ivec3 pos;
    glm::vec3 m_Color[16][16][16];

    uint32_t vbo, ibo, vao, count;
    bool Renderable = false;
};


#endif //OPENCRAFT_CHUNK_H

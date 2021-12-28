//
// Created by lukas on 22.12.21.
//

#include "Chunk.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

Chunk::Chunk(const glm::ivec3& position, const std::vector<float>& noise) : pos(position) {
    /*int index = 0;
    for(int x = 0; x < 16; x++) {
        for(int y = 0; y < 16; y++) {
            for(int z = 0; z < 16; z++) {
                if(noise[index++] <= 0.0f) {
                    m_VoxelData[x][y][z].id = BlockID::Dirt;
                    float r = RandomFloat(0.0f, 1.0f);
                    float g = RandomFloat(0.0f, 1.0f);
                    float b = RandomFloat(0.0f, 1.0f);
                    m_Color[x][y][z] = {r, g, b};
                } else {
                    m_VoxelData[x][y][z].id = BlockID::Air;
                }
            }
        }
    }*/

    m_VoxelData[0][0][0].id = BlockID::Dirt;
    m_VoxelData[0][0][1].id = BlockID::Dirt;
    m_VoxelData[0][1][0].id = BlockID::Dirt;
    m_VoxelData[1][0][0].id = BlockID::Dirt;
    m_VoxelData[1][1][1].id = BlockID::Dirt;
    m_VoxelData[5][5][5].id = BlockID::Dirt;

    std::vector<float> vertices;
    std::vector<uint32_t> indices;

    int i = 0;

    uint32_t vertexIndex = 0;
    for(int x = 0; x < 16; x++) {
        for(int y = 0; y < 16; y++) {
            for(int z = 0; z < 16; z++) {
                if(m_VoxelData[x][y][z].id == BlockID::Dirt) {
                    //auto color = m_Color[x][y][z];
                    float r = RandomFloat(0.0f, 1.0f);
                    float g = RandomFloat(0.0f, 1.0f);
                    float b = RandomFloat(0.0f, 1.0f);
                    glm::vec3 color = {r, g, b};

                    std::vector<float> newVoxelVerticies = {
                            -0.5f + (float)x, -0.5f + (float)y, -0.5f + (float)z, color.x, color.y, color.z,
                            0.5f + (float)x, -0.5f + (float)y, -0.5f + (float)z, color.x, color.y, color.z,
                            0.5f + (float)x,  0.5f + (float)y, -0.5f + (float)z, color.x, color.y, color.z,
                            -0.5f + (float)x,  0.5f + (float)y, -0.5f + (float)z, color.x, color.y, color.z,
                            -0.5f + (float)x, -0.5f + (float)y,  0.5f + (float)z, color.x, color.y, color.z,
                            0.5f + (float)x, -0.5f + (float)y,  0.5f + (float)z, color.x, color.y, color.z,
                            0.5f + (float)x,  0.5f + (float)y,  0.5f + (float)z, color.x, color.y, color.z,
                            -0.5f + (float)x,  0.5f + (float)y,  0.5f + (float)z, color.x, color.y, color.z
                    };

                    vertices.insert(vertices.begin(), std::begin(newVoxelVerticies), std::end(newVoxelVerticies));

                    /*uint32_t back_face1[6]    = {vertexIndex + 0, vertexIndex + 1, vertexIndex + 3, vertexIndex + 3, vertexIndex + 1, vertexIndex + 2};
                    uint32_t left_face1[6]    = {vertexIndex + 1, vertexIndex + 5, vertexIndex + 2, vertexIndex + 2, vertexIndex + 5, vertexIndex + 6};
                    uint32_t forward_face1[6] = {vertexIndex + 5, vertexIndex + 4, vertexIndex + 6, vertexIndex + 6, vertexIndex + 4, vertexIndex + 7};
                    uint32_t right_face1[6]   = {vertexIndex + 4, vertexIndex + 0, vertexIndex + 7, vertexIndex + 7, vertexIndex + 0, vertexIndex + 3};
                    uint32_t up_face1[6]      = {vertexIndex + 3, vertexIndex + 2, vertexIndex + 7, vertexIndex + 7, vertexIndex + 2, vertexIndex + 6};
                    uint32_t down_face1[6]    = {vertexIndex + 4, vertexIndex + 5, vertexIndex + 0, vertexIndex + 0, vertexIndex + 5, vertexIndex + 1};
                    indices.insert(indices.begin(), back_face1, back_face1 + 6);
                    indices.insert(indices.begin(), left_face1, left_face1 + 6);
                    indices.insert(indices.begin(), forward_face1, forward_face1 + 6);
                    indices.insert(indices.begin(), right_face1, right_face1 + 6);
                    indices.insert(indices.begin(), up_face1, up_face1 + 6);
                    indices.insert(indices.begin(), down_face1, down_face1 + 6);*/

                    i++;
                    std::cout << std::endl;
                    if(GetBlock({x-1,y,z}) == BlockID::Air) {
                        std::cout << "x-  " << "CHAD   " << i << "  x: " << x << " y: "  << y << " z: " << z << std::endl;
                        uint32_t left_face1[6]    = {vertexIndex + 1, vertexIndex + 5, vertexIndex + 2, vertexIndex + 2, vertexIndex + 5, vertexIndex + 6};
                        indices.insert(indices.begin(), left_face1, left_face1 + 6);
                    }

                    if(GetBlock({x+1,y,z}) == BlockID::Air) {
                        std::cout << "x+  " << "CHAD   " << i << "  x: " << x << " y: "  << y << " z: " << z << std::endl;
                        uint32_t right_face1[6]   = {vertexIndex + 4, vertexIndex + 0, vertexIndex + 7, vertexIndex + 7, vertexIndex + 0, vertexIndex + 3};
                        indices.insert(indices.begin(), right_face1, right_face1 + 6);
                    }

                    if(GetBlock({x,y-1,z}) == BlockID::Air) {
                        std::cout << "y-  " << "CHAD   " << i << "  x: " << x << " y: "  << y << " z: " << z << std::endl;
                        uint32_t down_face1[6]    = {vertexIndex + 4, vertexIndex + 5, vertexIndex + 0, vertexIndex + 0, vertexIndex + 5, vertexIndex + 1};
                        indices.insert(indices.begin(), down_face1, down_face1 + 6);

                    }

                    if(GetBlock({x,y+1,z}) == BlockID::Air) {
                        std::cout << "y+  " << "CHAD   " << i << "  x: " << x << " y: "  << y << " z: " << z << std::endl;
                        uint32_t up_face1[6]      = {vertexIndex + 3, vertexIndex + 2, vertexIndex + 7, vertexIndex + 7, vertexIndex + 2, vertexIndex + 6};
                        indices.insert(indices.begin(), up_face1, up_face1 + 6);
                    }

                    if(GetBlock({x,y,z-1}) == BlockID::Air) {
                        std::cout << "z-  " << "CHAD   " << i << "  x: " << x << " y: "  << y << " z: " << z << std::endl;
                        uint32_t forward_face1[6] = {vertexIndex + 5, vertexIndex + 4, vertexIndex + 6, vertexIndex + 6, vertexIndex + 4, vertexIndex + 7};
                        indices.insert(indices.begin(), forward_face1, forward_face1 + 6);
                    }

                    if(GetBlock({x,y,z+1}) == BlockID::Air) {
                        std::cout << "z+  " << "CHAD   " << i << "  x: " << x << " y: "  << y << " z: " << z << std::endl;
                        uint32_t back_face1[6]    = {vertexIndex + 0, vertexIndex + 1, vertexIndex + 3, vertexIndex + 3, vertexIndex + 1, vertexIndex + 2};
                        indices.insert(indices.begin(), back_face1, back_face1 + 6);
                    }

                    /*GetBlock({x+1,y,z});
                    GetBlock({x,y-1,z});
                    GetBlock({x,y+1,z});
                    GetBlock({x,y,z-1});
                    GetBlock({x,y,z+1});*/


                    vertexIndex += 8;
                }
            }
        }
    }

    count = indices.size();

    if(count != 0) {
        Renderable = true;
        glCreateBuffers(1, &vbo);
        glNamedBufferStorage(vbo, vertices.size() * sizeof(float), &vertices[0], 0);

        glCreateBuffers(1, &ibo);
        glNamedBufferStorage(ibo, count * sizeof(uint32_t), &indices[0], 0);

        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);

        struct vertex { glm::vec3 pos, nrm; };

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, pos));
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, nrm));

        glVertexAttribBinding(0, 0);
        glVertexAttribBinding(1, 0);


        glVertexArrayVertexBuffer(vao, 0, vbo, 0, 6 * sizeof(float));
        glVertexArrayElementBuffer(vao, ibo);
    }


}

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
    // Generate solids
    int index = 0;
    for(int x = 0; x < 16; x++) {
        for(int y = 0; y < 16; y++) {
            for(int z = 0; z < 16; z++) {
                if(noise[index++] <= 0.0f) {
                    m_VoxelData[x][y][z].id = BlockID::Solid;
                    float r = RandomFloat(0.0f, 1.0f);
                    float g = RandomFloat(0.0f, 1.0f);
                    float b = RandomFloat(0.0f, 1.0f);
                    m_Color[x][y][z] = {r, g, b};
                } else {
                    m_VoxelData[x][y][z].id = BlockID::Air;
                }
            }
        }
    }

    for(int x = 0; x < 16; x++) {
        for(int y = 0; y < 16; y++) {
            for(int z = 0; z < 16; z++) {
                if(m_VoxelData[x][y][z].is_solid()) {
                    m_VoxelData[x][y][z].id = BlockID::Grass;
                }
            }
        }
    }

    for(int x = 0; x < 16; x++) {
        for(int y = 0; y < 16; y++) {
            for(int z = 0; z < 16; z++) {
                if(m_VoxelData[x][y][z].is_solid()) {
                    //m_VoxelData[x][y][z].id = BlockID::Grass;
                    if(GetBlockID({x,y+1,z}) == BlockID::Grass) {
                        m_VoxelData[x][y][z].id = BlockID::Dirt;
                    }
                }
            }
        }
    }

    for(int x = 0; x < 16; x++) {
        for(int y = 0; y < 16; y++) {
            for(int z = 0; z < 16; z++) {
                if(m_VoxelData[x][y][z].is_solid()) {
                    //m_VoxelData[x][y][z].id = BlockID::Grass;
                    if(GetBlockID({x,y+1,z}) == BlockID::Dirt) {
                        m_VoxelData[x][y][z].id = BlockID::Stone;
                    }
                }
            }
        }
    }

    for(int x = 0; x < 16; x++) {
        for(int y = 0; y < 16; y++) {
            for(int z = 0; z < 16; z++) {
                if(m_VoxelData[x][y][z].is_solid()) {
                    //m_VoxelData[x][y][z].id = BlockID::Grass;
                    if(GetBlockID({x,y+1,z}) == BlockID::Stone) {
                        m_VoxelData[x][y][z].id = BlockID::Stone;
                    }
                }
            }
        }
    }

    /*m_VoxelData[0][0][0].id = BlockID::Dirt;
    m_VoxelData[0][0][1].id = BlockID::Dirt;
    m_VoxelData[0][1][0].id = BlockID::Dirt;
    m_VoxelData[1][0][0].id = BlockID::Dirt;
    m_VoxelData[1][1][1].id = BlockID::Dirt;
    m_VoxelData[5][5][5].id = BlockID::Dirt;*/

    std::vector<float> vertices;
    std::vector<uint32_t> indices;

    int i = 0;

    float id = 0.0f;

    uint32_t vertexIndex = 0;
    for(int x = 0; x < 16; x++) {
        for(int y = 0; y < 16; y++) {
            for(int z = 0; z < 16; z++) {
                if(m_VoxelData[x][y][z].is_solid()) {
                    //auto color = m_Color[x][y][z];
                    float r = RandomFloat(0.0f, 1.0f);
                    float g = RandomFloat(0.0f, 1.0f);
                    float b = RandomFloat(0.0f, 1.0f);
                    glm::vec3 color = {r, g, b};
                    id = (float)GetID({x, y, z});
                    if(GetBlockID({x,y,z}) == BlockID::Grass) {
                        id = (float)1;
                    }

                    i++;
                    //std::cout << std::endl;
                    if(GetBlockID({x-1,y,z}) == BlockID::Air) {
                        //std::cout << "x-  " << "CHAD   " << i << "  x: " << x << " y: "  << y << " z: " << z << std::endl;
                        std::vector<float> face = {
                                -0.5f + (float)x,  0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,      -1.0f,  0.0f,  0.0f,       0.0f,  1.0f, id, //-x
                                -0.5f + (float)x,  0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,      -1.0f,  0.0f,  0.0f,       1.0f,  1.0f, id,
                                -0.5f + (float)x, -0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,      -1.0f,  0.0f,  0.0f,       1.0f,  0.0f, id,
                                -0.5f + (float)x, -0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,      -1.0f,  0.0f,  0.0f,       1.0f,  0.0f, id,
                                -0.5f + (float)x, -0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,      -1.0f,  0.0f,  0.0f,       0.0f,  0.0f, id,
                                -0.5f + (float)x,  0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,      -1.0f,  0.0f,  0.0f,       0.0f,  1.0f, id,
                        };
                        vertices.insert(vertices.begin(), std::begin(face), std::end(face));
                    }

                    id = (float)GetID({x, y, z});
                    if(GetBlockID({x,y,z}) == BlockID::Grass) {
                        id = (float)1;
                    }

                    if(GetBlockID({x+1,y,z}) == BlockID::Air) {
                        //std::cout << "x+  " << "CHAD   " << i << "  x: " << x << " y: "  << y << " z: " << z << std::endl;
                        std::vector<float> face = {
                                0.5f + (float)x,  0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,       1.0f,  0.0f,  0.0f,       0.0f,  1.0f, id, //+x
                                0.5f + (float)x,  0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,       1.0f,  0.0f,  0.0f,       1.0f,  1.0f, id,
                                0.5f + (float)x, -0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,       1.0f,  0.0f,  0.0f,       1.0f,  0.0f, id,
                                0.5f + (float)x, -0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,       1.0f,  0.0f,  0.0f,       1.0f,  0.0f, id,
                                0.5f + (float)x, -0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,       1.0f,  0.0f,  0.0f,       0.0f,  0.0f, id,
                                0.5f + (float)x,  0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,       1.0f,  0.0f,  0.0f,       0.0f,  1.0f, id,
                        };
                        vertices.insert(vertices.begin(), std::begin(face), std::end(face));
                    }

                    id = (float)GetID({x, y, z});

                    if(GetBlockID({x,y-1,z}) == BlockID::Air) {
                        //std::cout << "y-  " << "CHAD   " << i << "  x: " << x << " y: "  << y << " z: " << z << std::endl;
                        std::vector<float> face = {
                                -0.5f + (float)x, -0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,       0.0f, -1.0f,  0.0f,       0.0f,  1.0f, id, //-y
                                 0.5f + (float)x, -0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,       0.0f, -1.0f,  0.0f,       1.0f,  1.0f, id,
                                 0.5f + (float)x, -0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,       0.0f, -1.0f,  0.0f,       1.0f,  0.0f, id,
                                 0.5f + (float)x, -0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,       0.0f, -1.0f,  0.0f,       1.0f,  0.0f, id,
                                -0.5f + (float)x, -0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,       0.0f, -1.0f,  0.0f,       0.0f,  0.0f, id,
                                -0.5f + (float)x, -0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,       0.0f, -1.0f,  0.0f,       0.0f,  1.0f, id,
                        };
                        vertices.insert(vertices.begin(), std::begin(face), std::end(face));

                    }

                    id = (float)GetID({x, y, z});

                    if(GetBlockID({x,y+1,z}) == BlockID::Air) {
                        //std::cout << "y+  " << "CHAD   " << i << "  x: " << x << " y: "  << y << " z: " << z << std::endl;
                        std::vector<float> face = {
                                -0.5f + (float)x,  0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  1.0f,  0.0f,       0.0f,  1.0f, id, //+y
                                 0.5f + (float)x,  0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  1.0f,  0.0f,       1.0f,  1.0f, id,
                                 0.5f + (float)x,  0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  1.0f,  0.0f,       1.0f,  0.0f, id,
                                 0.5f + (float)x,  0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  1.0f,  0.0f,       1.0f,  0.0f, id,
                                -0.5f + (float)x,  0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  1.0f,  0.0f,       0.0f,  0.0f, id,
                                -0.5f + (float)x,  0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  1.0f,  0.0f,       0.0f,  1.0f, id,
                        };
                        vertices.insert(vertices.begin(), std::begin(face), std::end(face));
                    }

                    id = (float)GetID({x, y, z});
                    if(GetBlockID({x,y,z}) == BlockID::Grass) {
                        id = (float)1;
                    }

                    if(GetBlockID({x,y,z-1}) == BlockID::Air) {
                        //std::cout << "z-  " << "CHAD   " << i << "  x: " << x << " y: "  << y << " z: " << z << std::endl;
                        std::vector<float> face = {
                                -0.5f + (float)x, -0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  0.0f, -1.0f,       0.0f,  0.0f, id, // -z
                                 0.5f + (float)x, -0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  0.0f, -1.0f,       1.0f,  0.0f, id,
                                 0.5f + (float)x,  0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  0.0f, -1.0f,       1.0f,  1.0f, id,
                                 0.5f + (float)x,  0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  0.0f, -1.0f,       1.0f,  1.0f, id,
                                -0.5f + (float)x,  0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  0.0f, -1.0f,       0.0f,  1.0f, id,
                                -0.5f + (float)x, -0.5f + (float)y, -0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  0.0f, -1.0f,       0.0f,  0.0f, id,
                        };
                        vertices.insert(vertices.begin(), std::begin(face), std::end(face));
                    }

                    id = (float)GetID({x, y, z});
                    if(GetBlockID({x,y,z}) == BlockID::Grass) {
                        id = (float)1;
                    }

                    if(GetBlockID({x,y,z+1}) == BlockID::Air) {
                        //std::cout << "z+  " << "CHAD   " << i << "  x: " << x << " y: "  << y << " z: " << z << std::endl;
                        std::vector<float> face = {
                                -0.5f + (float)x, -0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  0.0f,  1.0f,       0.0f,  0.0f, id, //+z
                                 0.5f + (float)x, -0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  0.0f,  1.0f,       1.0f,  0.0f, id,
                                 0.5f + (float)x,  0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  0.0f,  1.0f,       1.0f,  1.0f, id,
                                 0.5f + (float)x,  0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  0.0f,  1.0f,       1.0f,  1.0f, id,
                                -0.5f + (float)x,  0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  0.0f,  1.0f,       0.0f,  1.0f, id,
                                -0.5f + (float)x, -0.5f + (float)y,  0.5f + (float)z,       color.x, color.y, color.z,       0.0f,  0.0f,  1.0f,       0.0f,  0.0f, id,
                        };
                        vertices.insert(vertices.begin(), std::begin(face), std::end(face));
                    }
                }
            }
        }
    }

    count = vertices.size();

    if(count != 0) {
        Renderable = true;
        glCreateBuffers(1, &vbo);
        glNamedBufferStorage(vbo, vertices.size() * sizeof(float), &vertices[0], 0);

        /*glCreateBuffers(1, &ibo);
        glNamedBufferStorage(ibo, count * sizeof(uint32_t), &indices[0], 0);*/

        glCreateVertexArrays(1, &vao);
        glBindVertexArray(vao);

        struct vertex { glm::vec3 pos, col, nrm; glm::vec2 tex; float id; };

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);

        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, pos));
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, col));
        glVertexAttribFormat(2, 3, GL_FLOAT, GL_FALSE, offsetof(vertex, nrm));
        glVertexAttribFormat(3, 2, GL_FLOAT, GL_FALSE, offsetof(vertex, tex));
        glVertexAttribFormat(4, 1, GL_FLOAT, GL_FALSE, offsetof(vertex, id));

        glVertexAttribBinding(0, 0);
        glVertexAttribBinding(1, 0);
        glVertexAttribBinding(2, 0);
        glVertexAttribBinding(3, 0);
        glVertexAttribBinding(4, 0);


        glVertexArrayVertexBuffer(vao, 0, vbo, 0, 12 * sizeof(float));
        //glVertexArrayElementBuffer(vao, ibo);
    }


}

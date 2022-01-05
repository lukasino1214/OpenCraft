//
// Created by lukas on 22.12.21.
//

#ifndef OPENCRAFT_CHUNKMANAGER_H
#define OPENCRAFT_CHUNKMANAGER_H

#include "Chunk.h"

#include <FastNoise/FastNoise.h>

#include <glm/glm.hpp>
#include <unordered_map>
#include <map>
#include <utility>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include "../../Tracy/Tracy.hpp"



class ChunkManager {
public:
    ChunkManager(FastNoise::SmartNode<> generator) : m_fnGenerator(std::move(generator)) {};

    void AddChunk(const glm::ivec3& pos) {
        std::vector<float> noiseOutput(16 * 16 * 16);
        m_fnGenerator->GenUniformGrid3D(noiseOutput.data(), 16 * pos.z, 16 * pos.y, 16 * pos.x, 16, 16, 16, 0.05f, 1337);


        Chunk chunk(pos, noiseOutput);
        /*chunk.neighbors.px = &m_Chunks[{pos.x + 1, pos.y, pos.z}];
        chunk.neighbors.py = &m_Chunks[{pos.x, pos.y + 1, pos.z}];
        chunk.neighbors.pz = &m_Chunks[{pos.x, pos.y, pos.z + 1}];
        chunk.neighbors.nx = &m_Chunks[{pos.x - 1, pos.y, pos.z}];
        chunk.neighbors.ny = &m_Chunks[{pos.x, pos.y - 1, pos.z}];
        chunk.neighbors.nz = &m_Chunks[{pos.x, pos.y, pos.z - 1}];*/
        m_Chunks.insert(std::pair<glm::ivec3, Chunk>(pos, chunk));
        //m_Chunks.push_back(chunk);
    }

    void Render(Shader& shader) {
        for(auto& e : m_Chunks) {
            //std::cout << glm::to_string(e.second.getPosition()) << std::endl;
            e.second.Render(shader);
        }
    }

private:
    FastNoise::SmartNode<> m_fnGenerator;
    //std::vector<newChunk> m_Chunks;
    std::unordered_map<glm::ivec3, Chunk> m_Chunks;
};


#endif //OPENCRAFT_CHUNKMANAGER_H

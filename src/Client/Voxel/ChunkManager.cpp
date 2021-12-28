//
// Created by lukas on 22.12.21.
//

#include "ChunkManager.h"

/*void ChunkManager::AddChunk(const glm::ivec3 &position) {
    std::vector<float> noiseOutput(16 * 16 * 16);
    m_fnGenerator->GenUniformGrid3D(noiseOutput.data(), 16 * position.z, 16 * position.y, 16 * position.x, 16, 16, 16,
                                    0.05f, 1337);

    Chunk chunk(position, noiseOutput);
    //Chunks.insert(std::pair<glm::ivec3, Chunk>(position, chunk));
    Chunks.push_back(chunk);
    RenderData data;
    auto ver = chunk.GetData().ver;
    auto ind = chunk.GetData().ind;
    data.SetData(ver, ind);
    renderData.push_back(data);
}*/
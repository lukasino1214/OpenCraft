//
// Created by lukas on 28.12.21.
//

#ifndef OPENCRAFT_BLOCKS_H
#define OPENCRAFT_BLOCKS_H

#include <cstdint>

enum class BlockID : uint32_t
{
    Air,
    Dirt
};

struct Block {
    BlockID id;

    bool is_transparent() const {
        switch (id) {
            case BlockID::Air:
            default: return false;
        }
    }
};


#endif //OPENCRAFT_BLOCKS_H

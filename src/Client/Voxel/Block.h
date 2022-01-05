//
// Created by lukas on 28.12.21.
//

#ifndef OPENCRAFT_BLOCKS_H
#define OPENCRAFT_BLOCKS_H

#include <cstdint>

enum class BlockID : uint32_t
{
    Air,
    Solid,
    Grass,
    Dirt,
    Stone
};

struct Block {
    BlockID id;

    bool is_transparent() const {
        switch (id) {
            case BlockID::Air:
            default: return false;
        }
    }

    bool is_solid() const {
        switch (id) {
            case BlockID::Air: return false;
            default: return true;
        }
    }
};


#endif //OPENCRAFT_BLOCKS_H

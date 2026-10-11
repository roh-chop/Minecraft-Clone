#ifndef CHUNK_H
#define CHUNK_H
#include <iostream>
#include<array>


struct Chunk{
    std::array<short,32768> blocks{};

    Chunk* chunkNorth{nullptr};
    Chunk* chunkSouth{nullptr};
    Chunk* chunkWest{nullptr};
    Chunk* chunkEast{nullptr};
    Chunk* chunkUp{nullptr};
    Chunk* chunkDown{nullptr};

    bool pushed = false;
    bool init = false;

    int cx{0};
    int cy{0};
    int cz{0};
};
#endif
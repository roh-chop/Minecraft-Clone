#include <vector>

struct ChunkMesh{
   
    std::vector<float> blockfaces{};
    std::vector<float> texCoords{};
    std::vector<int> indices{};

    std::vector<float> waterfaces{};
    std::vector<float> watertexCoords{};
    std::vector<int> waterindices{};

    std::vector<float>colMul{};


    std::vector<GLuint>m_vaos{};
    std::vector<GLuint>m_vbos{};
    std::vector<GLuint>m_ebos{};

    std::vector<float>transparent_isWater{};
    std::vector<float>opaque_isWater{};

    int cx{-70};
    int cz{-70};

    bool ready{false};

};
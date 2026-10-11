#define WIDTH 16
#define HEIGHT 4
#define AREA WIDTH*WIDTH*HEIGHT
#include "glad/gl.h"
#include "cube.h"
#include "chunk.h"
#include "chunkMesh.h" 
#include <iostream>
#include "camera.h"
#include <queue>
#include <mutex>

class Renderer{
                                                 
    private:
        std::vector<ChunkMesh>m_chunkmeshes{};


    public:
        void createMesh(std::queue<Chunk>& m_chunks,std::mutex& mut);
        void bindData(ChunkMesh& m_chunkmesh);
        bool checkQueue(std::mutex& mut, std::queue<Chunk>& queue);
        Chunk getQueue(std::mutex& mut, std::queue<Chunk>& queue);
        void renderChunks(std::mutex& mut);
        void renderWater();
        void addFaces(bool isWater,int id,int face_id,int& numfaces ,std::array<float,12>&tecords ,int x,int y,int z,int cx, int cy, int cz, std::array<GLfloat, 16> face, ChunkMesh& chunkMesh);
        bool chunkMeshTest_1(std::mutex& mut, Chunk& chunk);
        bool chunkMeshTest_2(std::mutex& mut, ChunkMesh& chunkMesh);
        bool chunkMeshTest_3(std::mutex& mut, int& out, Chunk& chunk);
        void debug();
        void sortChunks();
        void sortArray(std::vector<int>& nums);
        void Renderer::quickSort(int begin, int end);



        Renderer(){
            m_chunkmeshes.resize(AREA);
        }
};
























































































































































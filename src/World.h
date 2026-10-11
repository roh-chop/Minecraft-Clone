#include <unordered_map>
#include "chunk.h"
#include "camera.h"
#include "column.h"
#include "Noise.h"
#include <queue>
#include <mutex>
#ifndef WIDTH
#define WIDTH 16
#define HEIGHT 4
#define AREA WIDTH*WIDTH*HEIGHT
#endif

class World{
    private:
        Noise m_noise{15};
        Camera& m_camera;
        std::unordered_map<float,Column>columns;
        glm::vec3 camRay{0,0,-4.5};
         
        
        
        
    public:
        static inline int oldcx;
        static inline int oldcz;
        static inline int bottomcx;
        static inline int bottomcz;
        static inline int topcx;
        static inline int topcz;
        std::unordered_map<float,Chunk>chunks;
        static float hash(int x, int z);
        static float hash(int x,int y,int z);
        World(Camera& camera)
            :m_camera{camera}
        {
            oldcx = m_camera.position.x>0?m_camera.position.x/32:m_camera.position.x/32-1; 
            oldcz = m_camera.position.z>0?m_camera.position.z/32:m_camera.position.z/32-1; 
            bottomcx = oldcx - (WIDTH-1)/2;
            bottomcz = oldcz - (WIDTH-1)/2;
            topcx = bottomcx+WIDTH-1;
            topcz = bottomcz+WIDTH-1;

            for(int i{0};i<WIDTH*WIDTH;i++){
                int cx = i%WIDTH+bottomcx;
                int cz = i/WIDTH+bottomcz;
                columns.insert({hash(cx,cz),{cx,cz}});
                int x{};
                int z{};
                columns[hash(cx,cz)].height.reserve(1024);
                for(int j{0};j<1024;j++){
                    x=columns[hash(cx,cz)].cx*32+j%32;
                    z=columns[hash(cx,cz)].cz*32+j/32;
                    columns[hash(cx,cz)].height.emplace_back(32+((m_noise.fbm(x+10342,z+12345)+1)*96/2));
                }

            }
            camera.position.y = columns[hash(oldcx,oldcz)].height[(int)camera.position.z%32*32+(int)camera.position.x%32]+1;

        }



        void initChunks(std::queue<Chunk>&out, std::mutex& mut);
        void genWorld(std::queue<Chunk>&out, std::mutex& mut, std::queue<int>& in);
        void initChunk(Chunk& chunk);
        void createChunk(Chunk& chunk, int cx, int cy, int cz);
        bool checkQueue(std::mutex& mut, std::queue<int>& in);
        void spawnTree(Chunk& chunk, int j, int x, int z);
        void findCameraRay();
};
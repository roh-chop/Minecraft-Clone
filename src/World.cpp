#include "World.h"
#include <random>

bool World::checkQueue(std::mutex& mut, std::queue<int>& in){
    std::lock_guard guard(mut);
    return in.empty();
}

void World::initChunks(std::queue<Chunk>&out, std::mutex& mut){
    for(int i{0};i<AREA;i++){ 
        int cx = (i/HEIGHT)%WIDTH+bottomcx;
        int cy = i%HEIGHT;
        int cz = (i/HEIGHT)/WIDTH+bottomcz;
        Chunk& chunk{chunks[hash(cx,cy,cz)]};
        createChunk(chunk,cx,cy,cz);
        std::lock_guard guard(mut);
        out.push(chunk);
    }
}




float World::hash(int x, int z){
    return (x * 18397) + (z * 29303); 
}

float World::hash(int x,int y,int z){
    return (x * 104729) + (y * 20483) + (z * 1299721); 
}


void World::genWorld(std::queue<Chunk>&out,std::mutex& mut, std::queue<int>& in){
    while(!checkQueue(mut,in)){
        int cx;
        int cy;
        int cz;
        {
            std::lock_guard guard(mut);
            cx = in.front();
            in.pop();
            cy = in.front();
            in.pop();
            cz = in.front();
            in.pop();
        }
        createChunk(chunks[hash(cx,cy,cz)],cx,cy,cz);
        std::lock_guard guard(mut);
        out.push(chunks[hash(cx,cy,cz)]);
    }
}






void World::spawnTree(Chunk& chunk, int j, int x, int z){
    int chunkUps = 0;
    for(int i=0;i<4;i++){
        if(j+1024<32768)
            chunk.blocks[j+1024]=5;
        else if(chunk.chunkUp){
            
            chunk.chunkUp->blocks[1024*chunkUps+z*32+x]=5;
            chunkUps++;
        }
        j+=1024;
    }
    if(j+1024<32768){
        chunk.blocks[j+1024]=6;
        if(x-1>0){
            chunk.blocks[j+1023]=6;
        }
    }        
    else if(chunk.chunkUp){
            chunk.chunkUp->blocks[1024*chunkUps+z*32+x]=6;
            chunkUps++;
        }
        
    
}






void World::initChunk(Chunk& chunk){
    if(columns[hash(chunk.cx,chunk.cz)].height.empty()){
        int x{};
        int z{};
        for(int j{0};j<1024;j++){
                    x=chunk.cx*32+j%32;
                    z=chunk.cz*32+j/32;
                    columns[hash(chunk.cx,chunk.cz)].height.emplace_back(32+((m_noise.fbm(x+10342,z+12345)+1)*96/2));
                }
    }


    if(chunk.cy<HEIGHT-1){
            
            chunk.chunkUp = &chunks[hash(chunk.cx,chunk.cy+1,chunk.cz)];

            chunk.chunkUp->cx = chunk.cx;
            chunk.chunkUp->cy = chunk.cy+1;
            chunk.chunkUp->cz = chunk.cz;
            if(chunk.chunkUp->init == false){
                
                initChunk(*chunk.chunkUp);
                chunk.chunkUp->init = true;
            }
            
    }
    if(chunk.cy>0){
        chunk.chunkDown = &chunks[hash(chunk.cx,chunk.cy-1,chunk.cz)];
        chunk.chunkDown->cx = chunk.cx;
        chunk.chunkDown->cy = chunk.cy-1;
        chunk.chunkDown->cz = chunk.cz;
    }

    chunk.chunkNorth = &chunks[hash(chunk.cx,chunk.cy,chunk.cz-1)];
    chunk.chunkNorth->cx = chunk.cx;
    chunk.chunkNorth->cy = chunk.cy;
    chunk.chunkNorth->cz = chunk.cz-1;

    chunk.chunkSouth = &chunks[hash(chunk.cx,chunk.cy,chunk.cz+1)];
    chunk.chunkSouth->cx = chunk.cx;
    chunk.chunkSouth->cy = chunk.cy;
    chunk.chunkSouth->cz = chunk.cz+1;

    chunk.chunkWest = &chunks[hash(chunk.cx-1,chunk.cy,chunk.cz)];
    chunk.chunkWest->cx = chunk.cx-1;
    chunk.chunkWest->cy = chunk.cy;
    chunk.chunkWest->cz = chunk.cz;


    chunk.chunkEast = &chunks[hash(chunk.cx+1,chunk.cy,chunk.cz)];
    chunk.chunkEast->cx = chunk.cx+1;
    chunk.chunkEast->cy = chunk.cy;
    chunk.chunkEast->cz = chunk.cz;


        for(int j{0};j<32768;j++){
            int x = j%32;
            int globy = j/1024+(chunk.cy*32);
            int z =(j/32)%32;

            if(globy<columns[hash(chunk.cx,chunk.cz)].height[z*32+x]){
                if(globy<columns[hash(chunk.cx,chunk.cz)].height[z*32+x]-3){
                    chunk.blocks[j]= 3;
                }
                else if(globy<columns[hash(chunk.cx,chunk.cz)].height[z*32+x]-1){
                    chunk.blocks[j]= 2;
                }
                else{
                    if(globy>=64){
                        chunk.blocks[j]= 1;
                        std::random_device rd;
                        std::mt19937 gen(rd()); 
                        std::uniform_int_distribution<> distrib(1, 10000);
                        int rand = distrib(gen);
                        if(rand<25){
                            spawnTree(chunk,j,x,z);
                        }
                    }
                    else chunk.blocks[j] = 4;
                }     
            }
            else{
                if(globy<64){
                    chunk.blocks[j]= 4;
                }
                else if(chunk.blocks[j]!=5&&chunk.blocks[j]!=6){
                    chunk.blocks[j]=(0);
                }
                
            }
            
        }
         
}

void World::createChunk(Chunk& chunk,int cx, int cy, int cz){
        chunk.cx = cx;
        chunk.cy = cy;
        chunk.cz = cz;
        
        if(chunk.init == false){
            initChunk(chunk);
            chunk.init == true;
        }

        if(chunk.chunkNorth->init==false){
            initChunk(*chunk.chunkNorth);
            chunk.chunkNorth->init = true;
        }

     
        if(chunk.chunkSouth->init==false){
            initChunk(*chunk.chunkSouth);
            chunk.chunkSouth->init = true;
        }

      
        if(chunk.chunkEast->init==false){
            initChunk(*chunk.chunkEast);
            chunk.chunkEast->init = true;
        }

 
        if(chunk.chunkWest->init==false){
            initChunk(*chunk.chunkWest);
            chunk.chunkWest->init = true;
        }

        if(chunk.cy>0){
          
            if(chunk.chunkDown->init==false){
                initChunk(*chunk.chunkDown);
                chunk.chunkDown->init = true;
            }
        }

}


void World::findCameraRay(){
    camRay.y = (float)std::sin(std::_Pi_val*(-1*m_camera.rotation.x)/180)*4.5f;

    float length = (float)std::sqrt(std::pow(4.5,2)-std::pow(camRay.y,2));


    camRay.x = (float)std::cos(std::_Pi_val*(m_camera.rotation.y-90)/180)*length;
    camRay.z = (float)std::sin(std::_Pi_val*(m_camera.rotation.y-90)/180)*length;
}
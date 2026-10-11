#include "Renderer.h"
#include "blockDir.h"
#include "World.h"


bool Renderer::checkQueue(std::mutex& mut, std::queue<Chunk>& queue){
    std::lock_guard guard5(mut);
    return queue.empty();
}

Chunk Renderer::getQueue(std::mutex& mut, std::queue<Chunk>& queue){
    std::lock_guard guard(mut);
    return queue.front();
}   



void Renderer::createMesh(std::queue<Chunk>& m_chunks,std::mutex& mut){
    static int m =0;
    bool init{};
    if(m<AREA){
        init = true;
    }
    if(!checkQueue(mut,m_chunks)){
    bool done{false};
    bool pop{true};
    std::uint16_t index{0};
    std::array<GLfloat,16>face{};
    std::array<float,12>tecords{};

    int ind{};

    Chunk chunk = getQueue(mut, m_chunks);
    {
        std::lock_guard guard(mut);
        m_chunks.pop();
    }
  
    while(((chunkMeshTest_1(mut,chunk))||(chunkMeshTest_3(mut,ind,chunk)))&&pop){
            if(!checkQueue(mut,m_chunks)){
                std::lock_guard guard(mut);
                chunk = m_chunks.front();
                m_chunks.pop();
             }
            else{
                pop=false;
                done=true;
            } 
        
        
    }

    
   

    if(!done){

        
        
        ChunkMesh& chunkMesh = m_chunkmeshes[ind];

        if(chunkMeshTest_2(mut,chunkMesh)){
            chunkMesh.ready = false;
            chunkMesh.m_vaos.resize(2);
            chunkMesh.m_vbos.resize(6);
            chunkMesh.m_ebos.resize(2);
            glDeleteVertexArrays(2,chunkMesh.m_vaos.data());
            glDeleteBuffers(6,chunkMesh.m_vbos.data());
            glDeleteBuffers(2,chunkMesh.m_ebos.data());
            chunkMesh.m_vaos.resize(2);
            chunkMesh.m_vbos.resize(6);
            chunkMesh.m_ebos.resize(2);
            int cy = chunk.cy;
            int cz = chunk.cz;
            int cx = chunk.cx;
            chunkMesh.cx = cx;
            chunkMesh.cz = cz;
            std::array<short,32768>& blocks = chunk.blocks;
            
            int numfaces{0};
            int numwaterfaces{0};

            
            chunkMesh.blockfaces.clear();
            chunkMesh.texCoords.clear();
            chunkMesh.indices.clear();
            chunkMesh.waterfaces.clear();
            chunkMesh.watertexCoords.clear();
            chunkMesh.waterindices.clear();
            chunkMesh.transparent_isWater.clear();
            chunkMesh.opaque_isWater.clear();
            chunkMesh.ready = false;
            
            for(int i{0};i<32768;i++){
                int id = blocks[i];
                if(id==4||id==6){
                    int x = i%32;
                    int y = i/1024;
                    int z =(i/32)%32;
                
                    if(((x+1)>=32||(isTransparent(blocks[i+1])&&id!=blocks[i+1]))){
                        if((x+1)<32||!(chunk.chunkEast)||(isTransparent(chunk.chunkEast->blocks[i-31])&&id!=chunk.chunkEast->blocks[i-31])){
                           
                           addFaces(true,id,0,numwaterfaces,tecords,x,y,z,cx,cy,cz,Cube::eastFace,chunkMesh);
                           
                            
                        }   
                    }
                if(((x-1)<0||(isTransparent(blocks[i-1])&&id!=blocks[i-1]))){
                    if((x-1)>=0||!(chunk.chunkWest)||(isTransparent(chunk.chunkWest->blocks[i+31])&&id!=chunk.chunkWest->blocks[i+31])){
                    
                    
                    addFaces(true,id,0,numwaterfaces,tecords,x,y,z,cx,cy,cz,Cube::westFace,chunkMesh);
                    
                   
                }
                }
                if(((z-1<0)||(isTransparent(blocks[i-32])&&id!=blocks[i-32]))){
                    if((z-1)>=0||!(chunk.chunkNorth)||(isTransparent(chunk.chunkNorth->blocks[i+(32*31)])&&id!=chunk.chunkNorth->blocks[i+(32*31)])){
                       
                       addFaces(true,id,0,numwaterfaces,tecords,x,y,z,cx,cy,cz,Cube::northFace,chunkMesh);
                       
                        
                    }
                }
                if(((z+1)>=32||(isTransparent(blocks[i+32])&&id!=blocks[i+32]))){
                    if((z+1)<32||!(chunk.chunkSouth)||(isTransparent(chunk.chunkSouth->blocks[i-(32*31)])&&id!=chunk.chunkSouth->blocks[i-(32*31)])){
                    
                        addFaces(true,id,0,numwaterfaces,tecords,x,y,z,cx,cy,cz,Cube::southFace,chunkMesh);
                    
                    
    
                    }
                }
                if(((y-1)<0||(isTransparent(blocks[i-1024])&&id!=blocks[i-1024]))){
                    if((y-1)>=0||!(chunk.chunkDown)||(isTransparent(chunk.chunkDown->blocks[i+(1024*31)])&&chunk.chunkDown->blocks[i+(1024*31)]!=id)){
                  
                    addFaces(true,id,1,numwaterfaces,tecords,x,y,z,cx,cy,cz,Cube::bottomFace,chunkMesh);

    
                }
                    
                }
                if(((y+1)>=32||(isTransparent(blocks[i+1024])&&id!=blocks[i+1024]))){
                    if((y+1)<32||!(chunk.chunkUp)||(isTransparent(chunk.chunkUp->blocks[i-(1024*31)])&&id!=chunk.chunkUp->blocks[i-(1024*31)])){
                    
                    
                        addFaces(true,id,2,numwaterfaces,tecords,x,y,z,cx,cy,cz,Cube::topFace,chunkMesh);

                }
                    
                }
                
            }
            else if(id!=0){
                int x = i%32;
                int y = i/1024;
                int z =(i/32)%32;
                
                if(((x+1)>=32||isTransparent(blocks[i+1]))){
                    if((x+1)<32||!(chunk.chunkEast)||isTransparent(chunk.chunkEast->blocks[i-31])){

                        addFaces(false,id,0,numfaces,tecords,x,y,z,cx,cy,cz,Cube::eastFace,chunkMesh);
                    
                }
                }
                if(((x-1)<0||isTransparent(blocks[i-1]))){
                    if((x-1)>=0||!(chunk.chunkWest)||isTransparent(chunk.chunkWest->blocks[i+31])){

                    addFaces(false,id,0,numfaces,tecords,x,y,z,cx,cy,cz,Cube::westFace,chunkMesh);

                }
                }
                if(((z-1<0)||isTransparent(blocks[i-32]))){
                    if((z-1)>=0||!(chunk.chunkNorth)||isTransparent(chunk.chunkNorth->blocks[i+(32*31)])){
                        addFaces(false,id,0,numfaces,tecords,x,y,z,cx,cy,cz,Cube::northFace,chunkMesh);
                    }
                }
                if(((z+1)>=32||isTransparent(blocks[i+32]))){
                    if((z+1)<32||!(chunk.chunkSouth)||isTransparent(chunk.chunkSouth->blocks[i-(32*31)])){

                        addFaces(false,id,0,numfaces,tecords,x,y,z,cx,cy,cz,Cube::southFace,chunkMesh);

                }
                }
                if(((y-1)<0||isTransparent(blocks[i-1024]))){
                    if((y-1)>=0||!(chunk.chunkDown)||isTransparent(chunk.chunkDown->blocks[i+(1024*31)])){
                    addFaces(false,id,1,numfaces,tecords,x,y,z,cx,cy,cz,Cube::bottomFace,chunkMesh);
                }
                    
                }
                if(((y+1)>=32||isTransparent(blocks[i+1024]))){
                    if((y+1)<32||!(chunk.chunkUp)||isTransparent(chunk.chunkUp->blocks[i-(1024*31)])){
                    addFaces(false,id,2,numfaces,tecords,x,y,z,cx,cy,cz,Cube::topFace,chunkMesh);

                   
                }
                    
                }
                
                
            }
            
            
        }

                
        
        
        bindData(chunkMesh);
        chunkMesh.ready = true;
        done = true;
        }
        
    }
}
   
    
}




void Renderer::bindData(ChunkMesh& m_chunkmesh){
    

        

      

        glGenVertexArrays(2,m_chunkmesh.m_vaos.data());
        glBindVertexArray(m_chunkmesh.m_vaos[0]);
        glGenBuffers(2,m_chunkmesh.m_vbos.data());
        glGenBuffers(1,m_chunkmesh.m_ebos.data());

        glBindBuffer(GL_ARRAY_BUFFER,m_chunkmesh.m_vbos[0]);
        glBufferData(GL_ARRAY_BUFFER,std::size(m_chunkmesh.blockfaces)*sizeof(GL_FLOAT),m_chunkmesh.blockfaces.data(),GL_STATIC_DRAW);
        glVertexAttribPointer(0,4,GL_FLOAT,false,0,0);
        glBindBuffer(GL_ARRAY_BUFFER,m_chunkmesh.m_vbos[1]);
        glBufferData(GL_ARRAY_BUFFER,std::size(m_chunkmesh.texCoords)*sizeof(GL_FLOAT),m_chunkmesh.texCoords.data(),GL_STATIC_DRAW);
        glVertexAttribPointer(1,3,GL_FLOAT,false,0,0);
        glBindBuffer(GL_ARRAY_BUFFER,m_chunkmesh.m_vbos[5]);
        glBufferData(GL_ARRAY_BUFFER,std::size(m_chunkmesh.opaque_isWater)*sizeof(GL_FLOAT),m_chunkmesh.opaque_isWater.data(),GL_STATIC_DRAW);
        glVertexAttribPointer(2,1,GL_FLOAT,false,0,0);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
     
   


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_chunkmesh.m_ebos[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,std::size(m_chunkmesh.indices)*sizeof(GL_UNSIGNED_INT),m_chunkmesh.indices.data(),GL_STATIC_DRAW);
        
        
            glBindVertexArray(m_chunkmesh.m_vaos[1]);
            glGenBuffers(2,m_chunkmesh.m_vbos.data()+2);
            glGenBuffers(1,m_chunkmesh.m_ebos.data()+1);

            glBindBuffer(GL_ARRAY_BUFFER,m_chunkmesh.m_vbos[2]);
            glBufferData(GL_ARRAY_BUFFER,std::size(m_chunkmesh.waterfaces)*sizeof(GL_FLOAT),m_chunkmesh.waterfaces.data(),GL_STATIC_DRAW);
            glVertexAttribPointer(0,4,GL_FLOAT,false,0,0);
            glBindBuffer(GL_ARRAY_BUFFER,m_chunkmesh.m_vbos[3]);
            glBufferData(GL_ARRAY_BUFFER,std::size(m_chunkmesh.watertexCoords)*sizeof(GL_FLOAT),m_chunkmesh.watertexCoords.data(),GL_STATIC_DRAW);
            glVertexAttribPointer(1,3,GL_FLOAT,false,0,0);
            glBindBuffer(GL_ARRAY_BUFFER,m_chunkmesh.m_vbos[4]);
            glBufferData(GL_ARRAY_BUFFER,std::size(m_chunkmesh.transparent_isWater)*sizeof(GL_FLOAT),m_chunkmesh.transparent_isWater.data(),GL_STATIC_DRAW);
            glVertexAttribPointer(2,1,GL_FLOAT,false,0,0);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);



   

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_chunkmesh.m_ebos[1]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,std::size(m_chunkmesh.waterindices)*sizeof(GL_UNSIGNED_INT),m_chunkmesh.waterindices.data(),GL_STATIC_DRAW);

        
    
}


void Renderer::renderChunks(std::mutex& mut){

    for(int i{AREA-1};i>=0;i--){
        ChunkMesh& m_chunkmesh = m_chunkmeshes[i];
        if(m_chunkmesh.ready){
            glBindVertexArray(m_chunkmesh.m_vaos[0]);
            glDrawElements(GL_TRIANGLES,std::size(m_chunkmesh.indices),GL_UNSIGNED_INT,0); 
        }
        
    }
  
}

void Renderer::renderWater(){
    
    for(int i{AREA-1};i>=0;i--){
        ChunkMesh& m_chunkmesh = m_chunkmeshes[i];
        if(m_chunkmesh.ready&&!m_chunkmesh.waterfaces.empty()){
            glBindVertexArray(m_chunkmesh.m_vaos[1]);
            glDrawElements(GL_TRIANGLES,std::size(m_chunkmesh.waterindices),GL_UNSIGNED_INT,0);
           
        }  
    }
}




void Renderer::addFaces(bool isWater,int id,int face_id,int& numfaces ,std::array<float,12>&tecords ,int x,int y,int z,int cx, int cy, int cz, std::array<GLfloat, 16> face, ChunkMesh& chunkMesh){
                if(isWater){
                    tecords = getTexCoords(id,face_id);
                    for(int j{0};j<16;j+=4){
                        face[j]+=32*cx+x;
                        face[j+1]+=32*cy+y;
                        face[j+2]+=32*cz+z;
                    }
                    
                    chunkMesh.waterfaces.insert(chunkMesh.waterfaces.end(),face.begin(),face.end());
                    chunkMesh.watertexCoords.insert(chunkMesh.watertexCoords.end(),tecords.begin(),tecords.end());
                    chunkMesh.waterindices.push_back(0+4*numfaces);
                    chunkMesh.waterindices.push_back(1+4*numfaces);
                    chunkMesh.waterindices.push_back(2+4*numfaces);
                    chunkMesh.waterindices.push_back(2+4*numfaces);
                    chunkMesh.waterindices.push_back(3+4*numfaces);
                    chunkMesh.waterindices.push_back(0+4*numfaces);
                    numfaces++; 

                    if(id==4){
                    chunkMesh.transparent_isWater.push_back(1);
                    chunkMesh.transparent_isWater.push_back(1);
                    chunkMesh.transparent_isWater.push_back(1);
                    chunkMesh.transparent_isWater.push_back(1);
                }
                else{
                    chunkMesh.transparent_isWater.push_back(0);
                    chunkMesh.transparent_isWater.push_back(0);
                    chunkMesh.transparent_isWater.push_back(0);
                    chunkMesh.transparent_isWater.push_back(0);
                }

                }      
                else{
                    tecords = getTexCoords(id,face_id);
                    for(int j{0};j<16;j+=4){
                        face[j]+=32*cx+x;
                        face[j+1]+=32*cy+y;
                        face[j+2]+=32*cz+z;
                    }
                    
                    chunkMesh.blockfaces.insert(chunkMesh.blockfaces.end(),face.begin(),face.end());
                    chunkMesh.texCoords.insert(chunkMesh.texCoords.end(),tecords.begin(),tecords.end());
                    chunkMesh.indices.push_back(0+4*numfaces);
                    chunkMesh.indices.push_back(1+4*numfaces);
                    chunkMesh.indices.push_back(2+4*numfaces);
                    chunkMesh.indices.push_back(2+4*numfaces);
                    chunkMesh.indices.push_back(3+4*numfaces);
                    chunkMesh.indices.push_back(0+4*numfaces);
                    numfaces++; 

                    chunkMesh.opaque_isWater.push_back(0);
                    chunkMesh.opaque_isWater.push_back(0);
                    chunkMesh.opaque_isWater.push_back(0);
                    chunkMesh.opaque_isWater.push_back(0);
                }


                                             
}



bool Renderer::chunkMeshTest_1(std::mutex& mut, Chunk& chunk){
    std::lock_guard guard(mut);
    return (chunk.cx>World::topcx||chunk.cx<World::bottomcx||chunk.cz>World::topcz||chunk.cz<World::bottomcz);


}
        



bool Renderer::chunkMeshTest_2(std::mutex& mut, ChunkMesh& chunkMesh){
    std::lock_guard guard(mut);
    return (chunkMesh.cx>World::topcx||chunkMesh.cx<World::bottomcx||chunkMesh.cz>World::topcz||chunkMesh.cz<World::bottomcz);

}

bool Renderer::chunkMeshTest_3(std::mutex& mut, int& out, Chunk& chunk){
    bool found = false; 
    int n = 0;
    for(int i=0;i<AREA;i++){
        if(n<4){
            if(!found&&chunkMeshTest_2(mut, m_chunkmeshes[i])){
                out = i;
                found = true;
            }
            else if(chunk.cx == m_chunkmeshes[i].cx && chunk.cz == m_chunkmeshes[i].cz){
                n++;
            }
        }   

    }
    if(n==4)return true;
    return false;
}

void Renderer::debug(){
    std::array<int,AREA> num{};
    for(int i=0;i<AREA;i++){
        ChunkMesh& chunkmesh = m_chunkmeshes[i];
        int cx = chunkmesh.cx;
        int cz = chunkmesh.cz;
        num[(cx-World::bottomcx)*WIDTH+(cz-World::bottomcz)]++;  
    }

    std::cout<<*std::min_element(num.begin(),num.end());

}





void Renderer::quickSort(int begin, int end){
        if(begin>=end){
            return;
        }
        int part = end;
        int i = begin-1;
        int j = begin;


        for(int k=0;k<end-begin;k++){
            if(std::sqrt(std::exp2(m_chunkmeshes[j].cx-(World::bottomcx+World::topcx)/2)+std::exp2(m_chunkmeshes[j].cz-(World::bottomcz+World::topcz)/2))<std::sqrt(std::exp2(m_chunkmeshes[part].cx-(World::bottomcx+World::topcx)/2)+std::exp2(m_chunkmeshes[part].cz-(World::bottomcz+World::topcz)/2))){
                i++;
                ChunkMesh& temp = m_chunkmeshes[i];
                m_chunkmeshes[i] = m_chunkmeshes[j];
                m_chunkmeshes[j] = temp;
            }
            j++;
        }
        i++;
        ChunkMesh& temp = m_chunkmeshes[i];
        m_chunkmeshes[i] = m_chunkmeshes[part];
        m_chunkmeshes[part] = temp;
        part = i;
        quickSort(begin,part-1);
        quickSort(part+1,end);
    }











       
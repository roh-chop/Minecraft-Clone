#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

struct Camera{
    glm::vec3 position{2016,96,2560};
    glm::vec2 rotation{0,0};
    

    void rotCam(float x, float y){
        rotation.x += glm::radians(x);
        rotation.y += glm::radians(y);
    }

    void movCam(float x, float y, float z){
        if(z!=0){
            position.z+=-1*glm::cos(rotation.y)*z;
            position.x+=glm::sin(rotation.y)*z;
        }
        if(x!=0){
            position.x+=-1*glm::cos(-rotation.y)*x;
            position.z+=glm::sin(-rotation.y)*x;
        }
        position.y+=y;
    }
    
};
#endif
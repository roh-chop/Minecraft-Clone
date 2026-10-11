#include "Transformations.h"

glm::mat4 Transformations::createUniformGlobalMatrix(float fovy, float aspect, float zNear, float zFar){
    return  glm::perspective(glm::radians(fovy),aspect,zNear,zFar);
}

glm::mat4 Transformations::createCamMatrix(const Camera& camera){
    float x = camera.position.x;
    float y = camera.position.y;
    float z = camera.position.z;
    glm::mat4 translation{1.0f};
    return glm::translate(translation,glm::vec3(-x,-y,-z));
}

glm::mat4 Transformations::createCamRotMatrix(const Camera& camera){
    float x = camera.rotation.x;
    float y = camera.rotation.y;
    glm::mat4 rotationx{1.0f};
    rotationx = glm::rotate(rotationx,x,glm::vec3(1.0f,0,0));
    glm::mat4 rotationy{1.0f};
    rotationy = glm::rotate(rotationy,y,glm::vec3(0,1.0f,0));
    return rotationx*rotationy;
}
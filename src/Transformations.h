#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include "camera.h"

namespace Transformations{
    glm::mat4 createUniformGlobalMatrix(float fovy, float aspect, float zNear, float zFar);
    glm::mat4 createCamMatrix(const Camera& camera);
    glm::mat4 createCamRotMatrix(const Camera& camera);
}
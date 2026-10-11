#ifndef SHADERS_H
#define SHADERS_H
#include <glad/gl.h>
#include "util.h"
#include "Transformations.h"
#include <string_view>


struct Shader{
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint shaderProgram;
    GLuint texId;

    void createShader();
    void bindUniform(glm::mat4 matrix, const char* name);
    void bindUniform(bool boolean, const char* name);
    void loadTexture();
    void regShader();
    void waterShader();
};
#endif
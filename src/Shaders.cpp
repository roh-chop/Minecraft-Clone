#include "Shaders.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"




void Shader::createShader(){
    std::string vertCode{};
    load_vert(vertCode);
    const char* vertShaderCode = vertCode.c_str();

    std::string fragCode{};
    load_frag(fragCode);
    const char* fragShaderCode = fragCode.c_str();

    
    
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  

    glShaderSource(vertexShader,1,&vertShaderCode,0);
    glCompileShader(vertexShader);

    glShaderSource(fragmentShader,1,&fragShaderCode,0);
    glCompileShader(fragmentShader);

 
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);


    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
}






void Shader::bindUniform(glm::mat4 matrix, const char* name){
    GLint uniformLoc = glGetUniformLocation(shaderProgram,name);
    glUniformMatrix4fv(uniformLoc,1,GL_FALSE,glm::value_ptr(matrix));
}

void Shader::bindUniform(bool boolean, const char* name){
    GLint uniformLoc = glGetUniformLocation(shaderProgram,name);
    glUniform1i(uniformLoc,boolean);
}

void Shader::loadTexture(){
    stbi_set_flip_vertically_on_load(true);
    int width,height,channels;
    unsigned char* data = stbi_load("C:\\Users\\rohan\\OneDrive\\Programming Projects\\C++\\VoxelEngine\\src\\resources\\tatlas.png",&width,&height,&channels,STBI_rgb_alpha);
    glGenTextures(1,&texId);
    glBindTexture(GL_TEXTURE_2D,texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}
#include <glad/gl.h>
#include <glfw/glfw3.h>
#include "Window.h"
#include <iostream>

  
    GLFWwindow* init(){
        if(!glfwInit()){
            std::cerr<<"Init failed";
        } 
        GLFWwindow* window = glfwCreateWindow(640,480,"Window",NULL,NULL);
      

        glfwMakeContextCurrent(window); 
        int version = gladLoadGL(glfwGetProcAddress);

        glFrontFace(GL_CW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.529f,0.808f,0.922f,1.0);
        return window;
    }

    


    
    
    

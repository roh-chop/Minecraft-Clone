#include "Application.h"
#include <chrono>
#include <thread>

namespace{
        bool isLeftPressed{};
        double oldX{-1};
        double oldY{-1};
        float incx{0};
        float incy{0};
        float incz{0};
        int width_{640};
        int height_{480};
    }

static void resizeCallback(GLFWwindow* window, int width, int height){
        glViewport(0,0,width,height);
        width_ = width;
        height_ = height;
    }

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode){
        if(action == GLFW_PRESS){
            if(key == GLFW_KEY_W){
                incz = 60.0/Application::fps;
            }
            if(key == GLFW_KEY_S){
              incz = -60.0/Application::fps;
            }
            if(key == GLFW_KEY_A){
                incx = 60.0/Application::fps;
            }
            if(key == GLFW_KEY_D){
              incx = -60.0/Application::fps;
            }
            if(key == GLFW_KEY_SPACE){
                incy = 60.0/Application::fps;
            }
            if(key == GLFW_KEY_LEFT_SHIFT){
                incy = -60.0/Application::fps;
            }
            if(key == GLFW_KEY_ESCAPE){
                glfwSetWindowShouldClose(Application::m_window,true);
            }
                    
        }
        else if(action == GLFW_RELEASE){
            incx = 0;
            incy = 0;
            incz = 0;
        }
    }

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
        if(button == GLFW_MOUSE_BUTTON_LEFT){
            if(action == GLFW_PRESS){
                isLeftPressed = true;
                
            }   
            else{
                isLeftPressed = false;
            }        
        }
    }

    static void cursorMoveCallback(GLFWwindow* window, double xpos, double ypos){
        
            Application::camera.rotCam((ypos-oldY)*0.3,(xpos-oldX)*0.3);
            oldX = width_/2;
            oldY = height_/2;
            glfwSetCursorPos(Application::m_window, width_/2,height_/2);
        
    }


bool Application::checkBuffer(){
    std::lock_guard guard{m_mut};
    return m_chunkBuffer.empty();
}


void Application::rendering(){
        bool print{false};
        glfwSwapInterval(0);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.529f,0.808f,0.922f,1.0);
        m_shader.createShader();
        m_shader.loadTexture();
        initInput();
        int frameCount{0};
        auto prevTime = std::chrono::high_resolution_clock::now();
        while(!glfwWindowShouldClose(m_window)){

           
            m_world.findCameraRay();

            glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
            handleInput();

            glEnable(GL_CULL_FACE);
    
            //glDisable(GL_BLEND);
            m_shader.bindUniform(false,"isWater");
            m_renderer.renderChunks(m_mut);
            glDisable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            m_shader.bindUniform(true,"isWater");
            m_renderer.renderWater();

            Application::camera.movCam(incx,incy,incz);      
            
         

            m_renderer.createMesh(m_chunkBuffer,m_mut);

            frameCount++;
            auto currTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double>elapsed = currTime-prevTime;
            if(elapsed.count()>=1.0){
                std::cout<<frameCount<<"\n";
                Application::fps = frameCount;
                std::cout<<Application::camera.position.x/32<<"x"<<camera.position.y<<"y"<<"\n";
                prevTime = currTime;
                frameCount = 0;

            }

            
            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
    }


    void Application::worldLoading(){
        m_world.initChunks(m_chunkBuffer,m_mut);
        while(!glfwWindowShouldClose(m_window)){
            m_world.genWorld(m_chunkBuffer,m_mut,m_worldBuffer);
        }
    }

    void Application::loc_tracking(){
        while(!glfwWindowShouldClose(m_window)){
             loadWorld();
        }
    }




void Application::initInput(){
    m_shader.bindUniform(Transformations::createUniformGlobalMatrix(60,640.f/480,0.05f,500),"globalMatrix");
    glfwSetFramebufferSizeCallback(m_window,resizeCallback);
    glfwSetKeyCallback(m_window,keyCallback);
    glfwSetMouseButtonCallback(m_window,mouseButtonCallback);
    glfwSetCursorPosCallback(m_window,cursorMoveCallback);
}

void Application::handleInput(){
    static int oldWidth, oldHeight;
    int width,height;
    glfwGetFramebufferSize(m_window,&width,&height);
    if(width!=oldWidth||height!=oldHeight){
        oldWidth = width;
        oldHeight = height;
        m_shader.bindUniform(Transformations::createUniformGlobalMatrix(60,static_cast<float>(width)/height,0.05f,1000),"globalMatrix");
    }
    m_shader.bindUniform(Transformations::createCamMatrix(Application::camera),"camPos");
    m_shader.bindUniform(Transformations::createCamRotMatrix(Application::camera),"camRot");
}

void Application::run(){
    

    std::thread world  (&Application::worldLoading,this);
    std::thread locTrace (&Application::loc_tracking, this);
    locTrace.detach();
    world.detach();
    rendering();
    m_renderer.debug();
}




void Application::loadWorld(){
    int camcx = Application::camera.position.x>0?Application::camera.position.x/32:Application::camera.position.x/32-1; 
    int camcz = Application::camera.position.z>0?Application::camera.position.z/32:Application::camera.position.z/32-1; 

    int diffcx = camcx-World::oldcx;
    int diffcz = camcz-World::oldcz;


    if(camcx!=World::oldcx){
        if(camcx>World::oldcx){
            int cx{};
            int cz{};
            {
                std::lock_guard guard(m_mut);
                World::bottomcx+=diffcx;
                World::topcx+=diffcx;
            }
            
            for(int i{0};i<WIDTH;i++){
                cx = World::topcx;
                cz = World::bottomcz+i;
                for(int j{0};j<HEIGHT;j++){
                        int cy{j};
                        std::lock_guard guard(m_mut);
                        m_worldBuffer.push(cx);
                        m_worldBuffer.push(cy);
                        m_worldBuffer.push(cz);
                }
            }
            std::lock_guard guard(m_mut);
            World::oldcx=camcx;
        }
        else{
            int cx{};
            int cz{};
            {
                std::lock_guard guard(m_mut);
                World::bottomcx+=diffcx;
            World::topcx+=diffcx;
            }
            
            for(int i{0};i<WIDTH;i++){
                cx = World:: bottomcx;
                cz = World::bottomcz+i;
                for(int j{0};j<HEIGHT;j++){
                        int cy{j};
                        std::lock_guard guard(m_mut);
                        m_worldBuffer.push(cx);
                        m_worldBuffer.push(cy);
                        m_worldBuffer.push(cz);
                }
            }
            std::lock_guard guard(m_mut);
            World::oldcx=camcx;
        }
    }
    if(camcz!=World::oldcz){
        if(camcz>World::oldcz){
            int cx{};
            int cz{};
            {
                std::lock_guard guard(m_mut);
                World::bottomcz+=diffcz;
            World:: topcz+=diffcz;
            }
            
            for(int i{0};i<WIDTH;i++){
                cx = World::bottomcx+i;
                cz = World::topcz;
                for(int j{0};j<HEIGHT;j++){
                        int cy{j};
                        std::lock_guard guard(m_mut);
                        m_worldBuffer.push(cx);
                        m_worldBuffer.push(cy);
                        m_worldBuffer.push(cz);
                }
            } 
            std::lock_guard guard(m_mut);
            World::oldcz=camcz; 
        }
        else{
            int cx{};
            int cz{};
            {
                std::lock_guard guard(m_mut);
                 World::bottomcz+=diffcz;
            World::topcz+=diffcz;
            }
           
            for(int i{0};i<WIDTH;i++){
                cx = World::bottomcx+i;
                cz = World::bottomcz;
                for(int j{0};j<HEIGHT;j++){
                        int cy{j};
                        std::lock_guard guard(m_mut);
                        m_worldBuffer.push(cx);
                        m_worldBuffer.push(cy);
                        m_worldBuffer.push(cz);
                }
            }
            std::lock_guard guard(m_mut);
            World::oldcz=camcz;

        }
    }
}







 



















































































































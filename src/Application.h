#include "Renderer.h"
#include "Window.h"
#include "camera.h"
#include "Shaders.h"
#include "World.h"
#include <mutex>

class Application{
    private:
        
        Renderer m_renderer{};
        Shader m_shader{};
        std::queue<Chunk>m_chunkBuffer{};
        std::queue<int>m_worldBuffer{};
        std::mutex m_mut;
        World m_world{camera};
        




    public:
        static inline GLFWwindow* m_window{init()};
        static inline Camera camera;
        bool checkBuffer();
        void loop();
        void initInput(); 
        void handleInput();
        void rendering();
        void worldLoading();
        void run();
        void meshCreation();
        void loadWorld();
        void loc_tracking();
        static inline int fps;
};
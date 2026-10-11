cmake_minimum_required(VERSION 3.11)
Project(Game)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e 
)

FetchContent_MakeAvailable(glm)



find_package(OpenGL REQUIRED)
add_subdirectory(glfw)


file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")
add_subdirectory(glad/)
add_executable(Game ${SOURCES})
target_link_libraries(Game  glfw)
target_link_libraries(Game  OpenGL::GL glad glm::glm)
target_include_directories(Game PUBLIC glfw/deps)


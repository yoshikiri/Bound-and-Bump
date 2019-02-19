cmake_minimum_required(VERSION 3.1)
project(BaB CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-pthread")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
link_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})

add_executable(BaB main.cpp Play.cpp Result.cpp Title.cpp
)

target_link_libraries(BaB ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw3 openal alure2 IrrKlang Xrandr Xinerama Xcursor Xi Xxf86vm X11 rt m dl)


cmake_minimum_required(VERSION 3.10)
project(demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${ASSIMP_INCLUDE_DIRS})
include_directories(/opt/homebrew/include)

# Quell-Dateien
add_executable(demo
    main.cpp
    Cam.cpp
    Grass.cpp
    Plane.cpp
    ShaderLoader.cpp
)

# OpenGL & GLUT Frameworks für macOS
target_link_libraries(demo
/opt/homebrew/lib/libGLEW.dylib
/opt/homebrew/lib/libassimp.dylib
    "-framework OpenGL"
    "-framework GLUT"
)
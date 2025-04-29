cmake_minimum_required(VERSION 3.10)
project(grass)

set(CMAKE_CXX_STANDARD 17)

include_directories(/opt/homebrew/include)

# Quell-Dateien
add_executable(grass
    main.cpp
    Plane.cpp
    Cam.cpp
    Grass.cpp
    ShaderLoader.cpp
)

# OpenGL & GLUT Frameworks für macOS
target_link_libraries(grass
    "-framework OpenGL"
    "-framework GLUT"
)
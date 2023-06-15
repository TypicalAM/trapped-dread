cmake_minimum_required(VERSION 3.12)
project(game)

set(CMAKE_CXX_STANDARD 17)

# Include CMake's built-in ExternalProject module
include(ExternalProject)

# Set the destination directory for downloaded libraries
set(LIBRARY_INSTALL_DIR ${CMAKE_BINARY_DIR}/lib)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# GLFW
find_package(GLFW3 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# ASSIMP
find_package(ASSIMP REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIRS})

set(LIBS
    ${OPENGL_LIBRARIES}
    glfw
    GLEW
    assimp
)

set(SOURCES
    src/Lodepng.cpp
    src/Main.cpp
    src/Shaderprogram.cpp
    src/Player.cpp
    src/CollidableObj.cpp
    src/Floor.cpp
    src/MapLoader.cpp
    src/Map.cpp
    src/Wall.cpp
    src/Altar.cpp
    src/Skull.cpp
    src/Exit.cpp
)

add_executable(game ${SOURCES})
target_link_libraries(game ${LIBS})

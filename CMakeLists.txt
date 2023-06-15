cmake_minimum_required(VERSION 3.12)
project(game)

set(CMAKE_CXX_STANDARD 17)

set(LIBS -lGL -lglfw -lGLEW -lassimp)

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
target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

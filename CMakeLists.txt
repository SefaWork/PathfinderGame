cmake_minimum_required(VERSION 3.31)
project(PathfinderGame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "C:/SDL2-2.32.6/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(
        ${SDL2_INCLUDE_DIR}
)

add_executable(PathfinderGame
        # Sources
        source/main.cpp

        # Headers
        include/enemy.h
        include/gameManager.h
        include/tile.h
        include/map.h
        include/movementStrategy.h
        include/levelConfig.h
        include/player.h
        include/pathfindingSystem.h
        include/movement/randomMovement.h
        include/movement/wavefrontChase.h

        source/gameManager.cpp
        source/tile.cpp
        source/player.cpp
        source/map.cpp
        source/movement/randomMovement.cpp
        source/enemy.cpp
        source/pathingSystem.cpp
        source/movement/wavefrontChase.cpp
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
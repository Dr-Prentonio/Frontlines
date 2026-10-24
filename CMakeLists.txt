cmake_minimum_required(VERSION 4.3)
project(strategie_game)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image REQUIRED CONFIG)
find_package(SDL2_ttf REQUIRED CONFIG)

add_executable(strategie_game

        # GAME LOOP RELATED
        gameloop/main.cpp
        gameloop/initFuncs.cpp
        gameloop/drawScene.cpp
        gameloop/inputFuncs.cpp
        gameloop/uiFuncs.cpp

        # MAIN HEADERS
        header/generalClasses.hpp
        header/UIClasses.hpp
        header/buildingsClasses.hpp
        header/generalFuncs.hpp

        # MAP RELATED
        map/territoryFuncss.cpp
        map/terrainFuncs.cpp

        # PLAYER RELATED
        player/playerFuncs.cpp
        player/factorysFuncs.cpp
        player/civilizationFuncs.cpp

        # OTHER STUFF
        other/FastNoiseLite.hpp
        dev/developmentModeFuncs.cpp
        header/devModeClasses.hpp
        header/debugLogging.hpp
        dev/uiFuncsforDev.cpp
        dev/readOutput.cpp
        map/specializedFields/initSpecializedFields.cpp
        header/specializedFieldClasses.hpp
        map/specializedFields/generateSpecializedFields.cpp
)

target_link_libraries(strategie_game PRIVATE
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf)
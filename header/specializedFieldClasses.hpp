//
// Created by tobias on 24.10.26.
//

#ifndef STRATEGIE_GAME_SPECIALIZEDFIELDCLASSES_HPP
#define STRATEGIE_GAME_SPECIALIZEDFIELDCLASSES_HPP

#include <SDL2/SDL.h>

#include <iostream>

#include "glm/glm.hpp"

namespace FOREST {

    // textures
    struct Textures {
        // not so dense forest
        SDL_Texture* oak;
        SDL_Texture* spruce;
    };

    // types of forests = {light = small forests, dense, big forests}
    enum Types {
        ERR,
        OAK,
        SPRUCE,
    };

    inline Textures textures;

    inline SDL_Texture* worldTex;

    inline std::vector<SDL_Rect> forestTiles;
//    inline std::vector<SDL_Rect> tempTiles;

    void drawCircle(int size, glm::ivec2 pos);
    void spawnTrees(int treesAmount);

    void drawForest(int radius, glm::ivec2 pos);

    void init();
    void generate();
}

#endif //STRATEGIE_GAME_SPECIALIZEDFIELDCLASSES_HPP

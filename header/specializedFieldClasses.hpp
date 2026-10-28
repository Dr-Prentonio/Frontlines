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
        SDL_Texture* lightOak;
        SDL_Texture* lightSpruce;

        // dense forest
        SDL_Texture* denseOak;
        SDL_Texture* denseSpruce;
    };

    // types of forests = {light = small forests, dense, big forests}
    enum Types {
        ERR,
        LIGHT_OAK,
        LIGHT_SPRUCE,
        DENSE_OAK,
        DENSE_SPRUCE
    };

    inline Textures textures;

    inline SDL_Texture* worldTex;

    inline std::vector<SDL_Rect> innerCircle;
    inline std::vector<SDL_Rect> outerCircle;

    void drawCircle(int size, glm::ivec2 pos);
    void spawnTrees(int treesAmount);

    void init();
    void generate();
}

#endif //STRATEGIE_GAME_SPECIALIZEDFIELDCLASSES_HPP

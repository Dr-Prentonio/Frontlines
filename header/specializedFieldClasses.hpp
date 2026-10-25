//
// Created by tobias on 24.10.26.
//

#ifndef STRATEGIE_GAME_SPECIALIZEDFIELDCLASSES_HPP
#define STRATEGIE_GAME_SPECIALIZEDFIELDCLASSES_HPP

#include <SDL2/SDL.h>

#include <iostream>

#include "glm/glm.hpp"

namespace FIELD_GENERATION {

    void manager();
}

namespace FORREST {

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

    // template for trees (position, type and parent group)
    struct TemplateStruct {

        SDL_Rect rect = {0,0, 0, 0};         // position of tree
        SDL_Rect hitbox = {0, 0, 0, 0};
        Types type = ERR;  // type of forest
        int groupID = 0;                    // what forest group they belong too
    };

    inline std::vector<TemplateStruct> trees;
    inline std::vector<SDL_Rect> forestTiles(TERRAIN::mapSize.x * TERRAIN::mapSize.y);
    inline Textures textures;

    void init();
}

#endif //STRATEGIE_GAME_SPECIALIZEDFIELDCLASSES_HPP

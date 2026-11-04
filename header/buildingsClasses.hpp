//
// Created by tobias on 16.10.26.
//

#ifndef STRATEGIE_GAME_BUILDINGS_HPP
#define STRATEGIE_GAME_BUILDINGS_HPP

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "glm/glm.hpp"

namespace FACTORY {
    enum factoryType {
        clothing = 1,
        wood,
        stone,
        steel,
        oil
    };
    struct Building {
        factoryType facType = clothing; // Type of factory

        SDL_Texture* facTex = nullptr; // fac tex
        SDL_Rect facRect = {0,0,0,0}; // pos and width of fac tex

        // only stores the facs hitbox
        SDL_Rect hitbox = {0,0,0,0};

        int level = 1; // how many are stacked

        SDL_Texture* levelTex = nullptr;
        SDL_Rect levelRect = {0,0,0,0};

        // if the factory is on for example a forest
        bool onSpecField = false;
    };
    struct Icons {
        SDL_Texture* clothingTex;
        SDL_Texture* woodTex;
        SDL_Texture* stoneTex;
        SDL_Texture* steelTex;
        SDL_Texture* oilTex;
    };

    inline Icons icons;
    inline std::vector<Building> factorys;


    int init();
    void mouseClick();
    void spawnNewFac(glm::ivec2 mousePos, factoryType type, bool debug);
    void update();
}

namespace CITY {

    struct Building {

        SDL_Rect cityRect;
        SDL_Rect hitbox;

        int level = 1;

        SDL_Texture* levelTex = nullptr;
        SDL_Rect levelRect;
    };

    inline SDL_Texture* cityTex;

    inline std::vector<Building> citys;

    int init();

    void update();
    void mouseClick();

    void spawnNew(glm::ivec2 mousePos, bool debug);
}

#endif //STRATEGIE_GAME_BUILDINGS_HPP

//
// Created by tobias on 13.10.26.
//

#ifndef STRATEGIE_GAME_UICLASSES_HPP
#define STRATEGIE_GAME_UICLASSES_HPP

// SDL2 Includes
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include "glm/glm.hpp"

namespace UI {

    // structs
    struct Fonts {
        TTF_Font* terminessItalic   = nullptr;
        TTF_Font* terminessNormal   = nullptr;
        TTF_Font* terminessBold     = nullptr;
        TTF_Font* RobotoMonoBold    = nullptr;
    };
    struct TopUiBar {
        glm::ivec2 pos = {10, 10};
        glm::ivec2 size;
    };
    struct BottomLeftUI {
        int index = 1; // 1-5
    };

    // variables
    inline TopUiBar topUIBar;
    inline Fonts fonts;
    inline BottomLeftUI bottomLeftUi;

    // funcs
    void topUiBar(SDL_Renderer* rend);
    void rightUiBar(SDL_Renderer *rend);
    void UIManager(SDL_Renderer* rend);
    void bottomLeftUI(SDL_Renderer *rend);

    // namespaces
    namespace TOOLTIP {

        inline bool enable = false;
        inline std::string text;
        inline std::string title;

        void surfaceMode(SDL_Renderer* rend);
        void manager();
    }
}


#endif //STRATEGIE_GAME_UICLASSES_HPP

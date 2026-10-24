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

    struct Fonts {

        TTF_Font* terminessItalic;
        TTF_Font* terminessNormal;
        TTF_Font* terminessBold;
        TTF_Font* RobotoMonoBold;

    }; inline Fonts fonts;

    struct ToolTip {

        bool drawToolTip = false;
        std::string toolTipText;
        std::string toolTipTitle;
        SDL_Rect toolTipRect;

    }; inline ToolTip toolTip;

    void drawToolTip(SDL_Renderer* rend);

/*    struct TroopOverview {

        glm::ivec2 position;
        glm::ivec2 size;

    }; inline TroopOverview troopOverview;
*/
//    void drawTroopOverview(SDL_Renderer* rend);
//    void drawTileAmount(SDL_Renderer* rend);

    void UIManager(SDL_Renderer* rend);

    struct TopUiBar {

        glm::ivec2 pos = {
            10, 10
        };
        glm::ivec2 size;
    }; inline TopUiBar topUIBar;

    void topUiBar(SDL_Renderer* rend);
    void rightUiBar(SDL_Renderer *rend);

    struct BottomLeftUI {
        int index = 1; // 1-5
    };

    inline BottomLeftUI bottomLeftUi;
    void bottomLeftUI(SDL_Renderer *rend);
}


#endif //STRATEGIE_GAME_UICLASSES_HPP

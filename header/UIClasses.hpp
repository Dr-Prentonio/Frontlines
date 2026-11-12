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
    void UIManager(SDL_Renderer* rend, SDL_Event &e);
    void bottomLeftUI(SDL_Renderer *rend);

    // namespaces
    namespace TOOLTIP {

        inline bool enable = false;
        inline std::string text;
        inline std::string title;

        void surfaceMode(SDL_Renderer* rend);
        void manager();
    }

    namespace TOOLSELECT {
        enum ToolIndex {
            war,
            build
        };
        struct Textures_Def {
            SDL_Texture* build  = nullptr;
            SDL_Texture* war    = nullptr;
        };

        // rect of the ui element
        inline SDL_Rect bgRect = {0,0,0,0};

        inline Textures_Def textures;

        // default should be war but for debuging it might be build
        inline ToolIndex index = build;

        int init();
        void drawBar(SDL_Event &e);
    }

    namespace BUILDTOOL {
        inline SDL_Rect baseRect = {0,0,0,0};

        // colors
        static const SDL_Color bgColor  = {55, 55, 55, 220};
        static const SDL_Color bgBorder = {100, 100, 100, 220};
        static const SDL_Color hover    = {255, 120, 120, 220};
        static const SDL_Color clicked  = {120, 120, 255, 220};

        namespace SUBPANEL {

            // for factory placement
            inline bool enableFactory  = false;
            void drawFactorySub(SDL_Event &e);

            // for city placement
            inline bool enableCity     = false;

        };

        void draw(SDL_Event &e);
    }
}


#endif //STRATEGIE_GAME_UICLASSES_HPP

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
        int index = 1;
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

    // for tools
    namespace TOOL {

        void mouseAction(SDL_Rect& collisionRect,   // rect of the ui element
            bool& clickState,                       // state of the boolean
            Uint32& timer,                          // timeout for design
            SDL_Event& e);                          // event for mouseclick

        // selection of which tool to use
        namespace SELECT {
            enum ToolIndex {
                war,
                build,
                mine
            };
            struct Textures_Def {
                SDL_Texture* build  = nullptr;
                SDL_Texture* war    = nullptr;
                SDL_Texture* mine   = nullptr;
            };

            // rect of the ui element
            inline SDL_Rect bgRect = {0,0,0,0};

            inline Textures_Def textures;

            // default should be war but for debuging it might be build
            inline ToolIndex index = build;

            int init();
            void drawBar(SDL_Event &e);
        }

        // building menu
        namespace BUILDTOOL {
            struct Textures_Def {
                SDL_Texture* factory = nullptr;
                SDL_Texture* city = nullptr;
            };

            inline Textures_Def textures;

            inline SDL_Rect baseRect = {0,0,0,0};

            // colors
            static const SDL_Color bgColor  = {55, 55, 55, 220};
            static const SDL_Color bgBorder = {100, 100, 100, 220};
            static const SDL_Color hover    = {255, 120, 120, 220};
            static const SDL_Color clicked  = {120, 120, 255, 220};

            // subpanels of each category
            namespace SUBPANEL {

                // for factory placement
                inline bool enableFactory  = false;
                void drawFactorySub(SDL_Event &e);

                // for city placement
                inline bool enableCity     = false;
                void drawCitySub(SDL_Event &e);
            }

            int init();
            void draw(SDL_Event &e);
        }
    }
}


#endif //STRATEGIE_GAME_UICLASSES_HPP

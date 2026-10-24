//
// Created by tobias on 23.10.26.
//

#ifndef STRATEGIE_GAME_DEVMODECLASSES_HPP
#define STRATEGIE_GAME_DEVMODECLASSES_HPP

#include <SDL_ttf.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include <iostream>

namespace DEVMODE {

    // main loop funcs
    int init();
    void loop();
    void draw();
    void input(SDL_Event &e);
    int cleanup();

    void readOutput();  // also deletes old textures
    int useStaticMaps();

    // window
    struct Window {

        glm::ivec2 size = {400,200};

        SDL_Window* win = nullptr;
        SDL_Renderer* rend = nullptr;
    };

    inline Window window;

}

// some basic ui funcs
namespace UI {
    struct CheckBoxClass {

        // if checkBox is being checked
        SDL_Rect drect;
        bool checked = false;

    };

    // checkBox 0 = god mode
    // checkBox 1 = static map
    inline std::vector<CheckBoxClass> checkBoxes;

    void initCheckBox(SDL_Rect drect, bool checked);

    void drawCheckBox(
        SDL_Renderer* rend,
        SDL_Rect drect,
        bool isChecked);

    namespace TEXT {

        inline TTF_Font* labelFont;

        struct LabelClass {
            SDL_Rect drect = {0,0,0,0};
            SDL_Texture* texture = nullptr;
        };

        inline std::vector<LabelClass> labels;

        void initLabel(SDL_Rect drect, std::string text);
    }
}

#endif //STRATEGIE_GAME_DEVMODECLASSES_HPP

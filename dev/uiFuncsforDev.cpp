//
// Created by tobias on 23.10.26.
//

#include "../header/debugLogging.hpp"
#include "../header/devModeClasses.hpp"

void UI::drawCheckBox(
    SDL_Renderer* rend,
    SDL_Rect drect,
    bool isChecked) {

    SDL_SetRenderDrawColor(rend, 155, 155, 155, 255);
    for (int i = 0; i < 3; i++) {
        SDL_Rect rect = {
            drect.x + i,
            drect.y + i,
            drect.w - (i * 2),
            drect.h - (i * 2),
        };

        SDL_RenderDrawRect(rend, &rect);
    }

    if (isChecked) {
        SDL_SetRenderDrawColor(rend, 205, 0, 0 ,255);

        static const int thickness = 3;

        for (int i = 0; i < thickness;  i++) {

            SDL_RenderDrawLine(rend,
                drect.x + i,
                drect.y,
                drect.w + i + drect.x - thickness,
                drect.h + drect.y);

            SDL_RenderDrawLine(rend,
                drect.x + i,
                drect.y + drect.h,
                drect.w + i + drect.x - thickness,
                drect.y);
        }
    }
}

void UI::initCheckBox(SDL_Rect drect, bool checked) {
    CheckBoxClass checkBox;

    checkBox.drect = drect;
    checkBox.checked = checked;

    checkBoxes.push_back(checkBox);
}

void UI::TEXT::initLabel(SDL_Rect drect, std::string text) {
    SDL_Renderer* rend = DEVMODE::window.rend;

    // render text as an tex
    SDL_Surface* tempSur = TTF_RenderUTF8_Solid(
        labelFont,
        text.c_str(),
        (SDL_Color){85,85,85,255});

    SDL_Texture* tempTex = SDL_CreateTextureFromSurface(rend, tempSur);

    int width = tempSur->w;
    int height = tempSur->h;

    LabelClass label;

    // update label
    label.drect = {
        drect.x,
        drect.y,
        drect.w + width,
        drect.h + height,
    };

    label.texture = tempTex;

    labels.push_back(label);

    // free memory
    SDL_FreeSurface(tempSur);
}

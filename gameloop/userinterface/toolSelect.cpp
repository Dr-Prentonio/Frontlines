//
// Created by tobias on 09.11.26.
//

#include "../../header/debugLogging.hpp"
#include "../../header/UIClasses.hpp"
#include "../../header/generalClasses.hpp"
#include "../../header/generalFuncs.hpp"

// |===================|
// |    Tool select    |
// |===================|

// initialize textures
int UI::TOOLSELECT::init() {

    SDL_Renderer* rend = window.rend;

    // build icon
    SDL_Surface* tempSurf = IMG_Load("assets/Icon/UI/hammer.png");
    textures.build = SDL_CreateTextureFromSurface(rend, tempSurf);
    if (textures.build == nullptr) {
        std::string err = SDL_GetError();
        if (err.empty()) err = "couldnt get sdl error";
        LOG::INIT_ERR("Couldnt load hammer.png " + err);
        return -1;
    }

    // war icon
    tempSurf = IMG_Load("assets/Icon/UI/swords.png");
    textures.war = SDL_CreateTextureFromSurface(rend, tempSurf);
    if (textures.war == nullptr) {
        std::string err = SDL_GetError();
        if (err.empty()) err = "couldnt get sdl error";
        LOG::INIT_ERR("Couldnt load swords.png " + err);
        return -1;
    }

    return 0;
}

// func for rendering the entire menu bar
void UI::TOOLSELECT::drawBar(SDL_Event &e) {
    SDL_Renderer* rend = window.rend;

    // colors are here defined
    static const SDL_Color bgColor  = {55, 55, 55, 220};
    static const SDL_Color bgBorder = {100, 100, 100, 220};
    static const SDL_Color hover    = {255, 120, 120, 220};
    static const SDL_Color clicked  = {120, 120, 255, 220};

    // background rect
    bgRect = {
        10,
        window.scale.y / 2 - 125,
        75,
        250
    };

    // intialize hitbox
    static bool runOnce = true;
    if (runOnce) {
        TOOLBOX::UIHitbox.push_back(bgRect);
        runOnce = false;
    }

    // Draws background rect with a border
    SDL_SetRenderDrawColor(rend, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(rend, &bgRect);
    SDL_SetRenderDrawColor(rend, bgBorder.r, bgBorder.g, bgBorder.b, bgBorder.a);
    SDL_RenderDrawRect(rend, &bgRect);

    // 1st icon (hammer)
    SDL_Rect icon1 = {
        bgRect.x + 5,
        bgRect.y + 5,
        bgRect.w - 10,
        bgRect.w - 10
    };

    // check for mouse
    glm::ivec2 mousePos;
    SDL_GetMouseState(&mousePos.x, &mousePos.y);

    if (TOOLBOX::checkMouseCollision(mousePos, icon1)) {
        // if the mouse hovers over the button
        SDL_SetRenderDrawColor(rend, hover.r, hover.g, hover.b, hover.a);

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                SDL_SetRenderDrawColor(rend, clicked.r, clicked.g, clicked.b, clicked.a);

                index = build;
            }
        }

        SDL_RenderFillRect(rend, &icon1);

        // change back to normal fill color
        SDL_SetRenderDrawColor(rend, bgBorder.r, bgBorder.g, bgBorder.b, bgBorder.a);
    }

    SDL_RenderCopy(rend, textures.build, nullptr, &icon1);
    SDL_RenderDrawRect(rend, &icon1);

    // 2nd icon (swords)
    SDL_Rect icon2 = {
        bgRect.x + 5,
        icon1.y + icon1.h + 5,
        bgRect.w - 10,
        bgRect.w - 10
    };

    // check
    if (TOOLBOX::checkMouseCollision(mousePos, icon2)) {
        // if the mouse hovers over the button
        SDL_SetRenderDrawColor(rend, hover.r, hover.g, hover.b, hover.a);

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                SDL_SetRenderDrawColor(rend, clicked.r, clicked.g, clicked.b, clicked.a);

                index = war;
            }
        }

        SDL_RenderFillRect(rend, &icon2);

        // change back to normal fill color
        SDL_SetRenderDrawColor(rend, bgBorder.r, bgBorder.g, bgBorder.b, bgBorder.a);
    }

    SDL_RenderCopy(rend, textures.war, nullptr, &icon2);
    SDL_RenderDrawRect(rend, &icon2);
}

// |==================|
// |    Build tool    |
// |==================|

void UI::BUILDTOOL::draw(SDL_Event &e) {
    SDL_Renderer* rend = window.rend;

    baseRect = {
        10,
        window.scale.y - 10 - 80,
        400,
        80
    };

    // initialize collision once
    static bool runOnce = true;
    if (runOnce) {
        TOOLBOX::UIHitbox.push_back(baseRect);
        runOnce = false;
    }

    SDL_SetRenderDrawColor(rend, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(rend, &baseRect);
    SDL_SetRenderDrawColor(rend, bgBorder.r, bgBorder.g, bgBorder.b, bgBorder.a);
    SDL_RenderDrawRect(rend, &baseRect);

    // |=============|
    // |    Icons    |
    // |=============|

    // handels things like hovering and clicking on icons
    auto mouseAction = [rend, e](SDL_Rect& rect, bool& click, Uint32& oldTime) {
        glm::ivec2 mousePos;
        SDL_GetMouseState(&mousePos.x, &mousePos.y);

        // cooldown bs
        const Uint32 cooldown = 200;
        Uint32 time = SDL_GetTicks();

        // mouse-hover or mouse click
        if (TOOLBOX::checkMouseCollision(mousePos, rect)) {
            SDL_SetRenderDrawColor(rend, hover.r, hover.g, hover.b, hover.a);

            // mouse Press
            if (e.type == SDL_MOUSEBUTTONDOWN && time - oldTime >= cooldown) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    SDL_SetRenderDrawColor(rend, clicked.r, clicked.g, clicked.b, clicked.a);

                    click = !click;
                }

                oldTime = time;
            }

            SDL_RenderFillRect(rend, &rect);

            // change back to normal fill color
            SDL_SetRenderDrawColor(rend, bgBorder.r, bgBorder.g, bgBorder.b, bgBorder.a);
        }
    };

    SDL_Rect factoryIcon = {
        baseRect.x + 5,
        baseRect.y + 5,
        baseRect.h - 10,
        baseRect.h - 10
    };

    // Render frame
    SDL_RenderDrawRect(rend, &factoryIcon);

    static bool facBool;
    static Uint32 facTime;
    mouseAction(factoryIcon, facBool, facTime);
    SUBPANEL::enableFactory = facBool;

    // render icon
    SDL_RenderCopy(rend, FACTORY::icons.clothingTex, nullptr, &factoryIcon);

    SDL_Rect cityIcon = {
        baseRect.x + factoryIcon.w + 10,
        baseRect.y + 5,
        baseRect.h - 10,
        baseRect.h - 10
    };

    SDL_RenderDrawRect(rend, &cityIcon);

    static bool cityBool;
    static Uint32 cityTime;
    mouseAction(cityIcon, cityBool, cityTime);
    SUBPANEL::enableCity = cityBool;

    SDL_RenderCopy(rend, CITY::cityTex, nullptr, &cityIcon);

    if (SUBPANEL::enableFactory) SUBPANEL::drawFactorySub(e);
}

void UI::BUILDTOOL::SUBPANEL::drawFactorySub(SDL_Event &e) {
    SDL_Renderer* rend = window.rend;

    const SDL_Rect baseRect = {
        BUILDTOOL::baseRect.x + BUILDTOOL::baseRect.w + 50,
        BUILDTOOL::baseRect.y,
        BUILDTOOL::baseRect.w,
        BUILDTOOL::baseRect.h
    };

    SDL_SetRenderDrawColor(rend, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(rend, &baseRect);
    SDL_SetRenderDrawColor(rend, bgBorder.r, bgBorder.g, bgBorder.b, bgBorder.a);
    SDL_RenderDrawRect(rend, &baseRect);

    // ICONS

    // have to make a lambda here that makes it easier to draw many icons

    SDL_Rect clothIcon = {
        baseRect.x + 5,
        baseRect.y + 5,
        baseRect.h - 10,
        baseRect.h - 10
    };

    SDL_RenderCopy(rend, FACTORY::icons.clothingTex, nullptr, &clothIcon);

}

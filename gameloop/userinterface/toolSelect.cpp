//
// Created by tobias on 09.11.26.
//

#include "../../header/debugLogging.hpp"
#include "../../header/UIClasses.hpp"
#include "../../header/generalClasses.hpp"
#include "../../header/generalFuncs.hpp"

void UI::TOOL::mouseAction(SDL_Rect& collisionRect, bool& clickState, Uint32& timer, SDL_Event& e) {
    SDL_Renderer *rend {window.rend};

    // get colors
    static const SDL_Color hover     {BUILDTOOL::hover};
    static const SDL_Color clicked   {BUILDTOOL::clicked};
    static const SDL_Color base      {BUILDTOOL::bgBorder};

    glm::ivec2 mousePos;
    SDL_GetMouseState(&mousePos.x, &mousePos.y);

    // cooldown bs
    constexpr Uint32 cooldown {200};
    Uint32 time = SDL_GetTicks();   // btw SDL_GetTicks only works for ~49 days until the intiger gets to big

    // mouse-hover or mouse click
    if (TOOLBOX::checkMouseCollision(mousePos, collisionRect)) {
        SDL_SetRenderDrawColor(rend, hover.r, hover.g, hover.b, hover.a);

        // mouse Press
        if (e.type == SDL_MOUSEBUTTONDOWN && time - timer >= cooldown) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                SDL_SetRenderDrawColor(rend, clicked.r, clicked.g, clicked.b, clicked.a);

                clickState = !clickState;
            }

            timer = time;
        }

        SDL_RenderFillRect(rend, &collisionRect);

        // change back to normal fill color
        SDL_SetRenderDrawColor(rend, base.r, base.g, base.b, base.a);
    }
}

// |===================|
// |    Tool select    |
// |===================|

// initialize textures
int UI::TOOL::SELECT::init() {
    if (TOOLBOX::loadTexture(textures.build, "assets/Icon/UI/hammer.png")   != 0 or
        TOOLBOX::loadTexture(textures.war, "assets/Icon/UI/swords.png")     != 0 or
        TOOLBOX::loadTexture(textures.mine, "assets/Icon/UI/pickaxe.png")   != 0) return -1;

    LOG::INIT_SUCCESS("loaded all tex");
    return 0;
}

// func for rendering the entire menu bar
void UI::TOOL::SELECT::drawBar(SDL_Event &e) {
    SDL_Renderer* rend {window.rend};

    // colors are here defined
    constexpr SDL_Color bgColor  = {55, 55, 55, 220};
    constexpr SDL_Color bgBorder = {100, 100, 100, 220};
    constexpr SDL_Color hover    = {255, 120, 120, 220};
    constexpr SDL_Color clicked  = {120, 120, 255, 220};

    // background rect
    bgRect = {
        10,
        window.scale.y / 2 - 125,
        75,
        250
    };

    // intialize hitbox
    static bool runOnce {true};
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
    SDL_Rect icon1 {
        bgRect.x + 5,
        bgRect.y + 5,
        bgRect.w - 10,
        bgRect.w - 10
    };

    // check for mouse
    glm::ivec2 mousePos {};
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
    SDL_Rect icon2 {
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

    // 3rd icon mining
    SDL_Rect miningRect {
        bgRect.x + 5,
        icon2.y + icon2.h + 5,
        bgRect.w - 10,
        bgRect.w - 10
    };

    if (TOOLBOX::checkMouseCollision(mousePos, miningRect)) {
        // if the mouse hovers over the button
        SDL_SetRenderDrawColor(rend, hover.r, hover.g, hover.b, hover.a);

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                SDL_SetRenderDrawColor(rend, clicked.r, clicked.g, clicked.b, clicked.a);

                index = mine;
            }
        }

        SDL_RenderFillRect(rend, &miningRect);

        // change back to normal fill color
        SDL_SetRenderDrawColor(rend, bgBorder.r, bgBorder.g, bgBorder.b, bgBorder.a);
    }

    SDL_RenderCopy(rend, textures.mine, nullptr, &miningRect);
    SDL_RenderDrawRect(rend, &miningRect);
}

// |==================|
// |    Build tool    |
// |==================|

int UI::TOOL::BUILDTOOL::init() {
    if (TOOLBOX::loadTexture(textures.factory, "assets/Icon/UI/factoryBuild.png")   != 0 or
        TOOLBOX::loadTexture(textures.city, "assets/Icon/UI/cityBuild.png")         != 0) return -1;

    LOG::INIT_SUCCESS("Successfully inited textures");
    return 0;
}

void UI::TOOL::BUILDTOOL::draw(SDL_Event &e) {
    SDL_Renderer* rend {window.rend};

    baseRect = {
        10,
        window.scale.y - 10 - 80,
        400,
        80
    };

    // initialize collision once
    static bool runOnce {true};
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

    SDL_Rect factoryIcon {
        baseRect.x + 5,
        baseRect.y + 5,
        baseRect.h - 10,
        baseRect.h - 10
    };

    // Render frame
    SDL_RenderDrawRect(rend, &factoryIcon);

    static bool facBool {};
    static Uint32 facTime {};
    mouseAction(factoryIcon, facBool, facTime, e);
    SUBPANEL::enableFactory = facBool;

    // render icon
    SDL_RenderCopy(rend, textures.factory, nullptr, &factoryIcon);

    SDL_Rect cityIcon {
        baseRect.x + factoryIcon.w + 10,
        baseRect.y + 5,
        baseRect.h - 10,
        baseRect.h - 10
    };

    SDL_RenderDrawRect(rend, &cityIcon);

    static bool cityBool {};
    static Uint32 cityTime {};
    mouseAction(cityIcon, cityBool, cityTime, e);
    SUBPANEL::enableCity = cityBool;

    SDL_RenderCopy(rend, textures.city, nullptr, &cityIcon);

    SDL_Rect militaryIcon {
        cityIcon.x + cityIcon.w + 5,
        baseRect.y + 5,
        baseRect.h - 10,
        baseRect.h - 10
    };

    SDL_RenderDrawRect(rend, &militaryIcon);

    static bool militaryBool {};
    static Uint32 militaryTime {};
    mouseAction(militaryIcon, militaryBool, militaryTime, e);

    SDL_RenderCopy(rend, MILITARYCAMP::campTex, nullptr, &militaryIcon);

    // selection part
    if (SUBPANEL::enableFactory) {
        cityBool = false;
        militaryBool = false;
        SUBPANEL::drawFactorySub(e);
    }
    if (SUBPANEL::enableCity) {
        facBool = false;
        militaryBool = false;
        SUBPANEL::drawCitySub(e);
    }
    if (militaryBool) {
        cityBool = false;
        facBool = false;
        bottomLeftUi.index = 8;
    }
}

// factory build tool selection menu
void UI::TOOL::BUILDTOOL::SUBPANEL::drawFactorySub(SDL_Event &e) {
    SDL_Renderer* rend {window.rend};

    static const SDL_Rect baseRect {
        BUILDTOOL::baseRect.x + BUILDTOOL::baseRect.w + 50,
        BUILDTOOL::baseRect.y,
        (5 + (BUILDTOOL::baseRect.h - 10)) * 6 + 5, // calcs the exact width needed
        BUILDTOOL::baseRect.h
    };

    SDL_SetRenderDrawColor(rend, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(rend, &baseRect);
    SDL_SetRenderDrawColor(rend, bgBorder.r, bgBorder.g, bgBorder.b, bgBorder.a);
    SDL_RenderDrawRect(rend, &baseRect);

    // ICONS

    static Uint32 time {};

    // clothing icon
    static SDL_Rect clothIcon {
        baseRect.x + 5,
        baseRect.y + 5,
        baseRect.h - 10,
        baseRect.h - 10
    };
    bool clothBool {};
    mouseAction(clothIcon, clothBool, time, e);
    if (clothBool) bottomLeftUi.index = 1;
    SDL_RenderCopy(rend, FACTORY::icons.clothingTex, nullptr, &clothIcon);

    // wood icon
    static SDL_Rect woodIcon {
        clothIcon.x + clothIcon.w + 5,
        baseRect.y + 5,
        baseRect.h - 10,
        baseRect.h - 10
    };
    bool woodBool {};
    mouseAction(woodIcon, woodBool, time, e);
    if (woodBool) bottomLeftUi.index = 2;
    SDL_RenderCopy(rend, FACTORY::icons.woodTex, nullptr, &woodIcon);

    // stone icon
    static SDL_Rect stoneIcon {
        woodIcon.x + woodIcon.w + 5,
        baseRect.y + 5,
        baseRect.h - 10,
        baseRect.h - 10
    };
    bool stoneBool {};
    mouseAction(stoneIcon, stoneBool, time, e);
    if (stoneBool) bottomLeftUi.index = 3;
    SDL_RenderCopy(rend, FACTORY::icons.stoneTex, nullptr, &stoneIcon);

    // steal icon
    static SDL_Rect steelIcon {
        stoneIcon.x + stoneIcon.w + 5,
        baseRect.y + 5,
        baseRect.h - 10,
        baseRect.h - 10
    };
    bool steelBool {};
    mouseAction(steelIcon, steelBool, time, e);
    if (steelBool) bottomLeftUi.index = 4;
    SDL_RenderCopy(rend, FACTORY::icons.steelTex, nullptr, &steelIcon);

    // oil icon
    static SDL_Rect oilIcon {
        steelIcon.x + steelIcon.w + 5,
        baseRect.y + 5,
        baseRect.h - 10,
        baseRect.h - 10
    };
    bool oilBool {};
    mouseAction(oilIcon, oilBool, time, e);
    if (oilBool) bottomLeftUi.index = 5;
    SDL_RenderCopy(rend, FACTORY::icons.oilTex, nullptr, &oilIcon);

    // weapon icon
    static SDL_Rect weaponIcon {
        oilIcon.x + oilIcon.w + 5,
        baseRect.y + 5,
        baseRect.h - 10,
        baseRect.h - 10
    };
    bool weaponBool {};
    mouseAction(weaponIcon, weaponBool, time, e);
    if (weaponBool) bottomLeftUi.index = 6;
    SDL_RenderCopy(rend, FACTORY::icons.weaponTex, nullptr, &weaponIcon);
}

// city build tool selection menu
void UI::TOOL::BUILDTOOL::SUBPANEL::drawCitySub(SDL_Event &e) {
    SDL_Renderer* rend = window.rend;

    static const SDL_Rect baseRect {
        BUILDTOOL::baseRect.x + BUILDTOOL::baseRect.w + 50,
        BUILDTOOL::baseRect.y,
        (5 + (BUILDTOOL::baseRect.h - 10)) * 2 + 5, // calcs the exact width needed
        BUILDTOOL::baseRect.h
    };

    SDL_SetRenderDrawColor(rend, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(rend, &baseRect);
    SDL_SetRenderDrawColor(rend, bgBorder.r, bgBorder.g, bgBorder.b, bgBorder.a);
    SDL_RenderDrawRect(rend, &baseRect);

    // ICONS
    // have to make a lambda here that makes it easier to draw many icons

    static Uint32 time {};

    // clothing icon
    static SDL_Rect cityIcon {
        baseRect.x + 5,
        baseRect.y + 5,
        baseRect.h - 10,
        baseRect.h - 10
    };
    bool cityBool {};
    mouseAction(cityIcon, cityBool, time, e);
    if (cityBool) bottomLeftUi.index = 7;
    SDL_RenderCopy(rend, CITY::cityTex, nullptr, &cityIcon);
}

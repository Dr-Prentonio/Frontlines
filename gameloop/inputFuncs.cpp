#include "../header/generalClasses.hpp"
#include "../header/generalFuncs.hpp"
#include "../header/UIClasses.hpp"
#include "../header/buildingsClasses.hpp"


void SDL::Gameloop::input(SDL_Event &e) {

    // movment
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    static glm::vec2 movement = {0,0};

    float speed = 1;
    float baseSpeed = 250;

    //
    //  KEYBOARD
    //

    if (state[SDL_SCANCODE_LSHIFT]) {
        speed = 1.5f;
    }

    if (state[SDL_SCANCODE_W] && !player.touchesBorderTop) {
        movement.y += baseSpeed * speed * deltaTime;
        UI::toolTip.drawToolTip = false;
    }

    else if (state[SDL_SCANCODE_S] && !player.touchesBorderBottom) {
        movement.y -= baseSpeed * speed * deltaTime;
        UI::toolTip.drawToolTip = false;
    }

    else if (state[SDL_SCANCODE_A] && !player.touchesBorderLeft) {
        movement.x += baseSpeed * speed * deltaTime;
        UI::toolTip.drawToolTip = false;
    }

    else if (state[SDL_SCANCODE_D] && !player.touchesBorderRight) {
        movement.x -= baseSpeed * speed * deltaTime;
        UI::toolTip.drawToolTip = false;
    }


    // for buildings
    if (state[SDL_SCANCODE_1]) {
        UI::bottomLeftUi.index = 1;
    }
    if (state[SDL_SCANCODE_2]) {
        UI::bottomLeftUi.index = 2;
    }
    if (state[SDL_SCANCODE_3]) {
        UI::bottomLeftUi.index = 3;
    }
    if (state[SDL_SCANCODE_4]) {
        UI::bottomLeftUi.index = 4;
    }
    if (state[SDL_SCANCODE_5]) {
        UI::bottomLeftUi.index = 5;
    }
    if (state[SDL_SCANCODE_6]) {
        UI::bottomLeftUi.index = 6;
    }

    player.camOffset = {
        static_cast<int>(glm::round(movement.x)),
        static_cast<int>(glm::round(movement.y)),
        0,0};

    //
    // MOUSE WHEEL
    //

    // zoom
    if (e.type == SDL_MOUSEWHEEL) {
        if (e.wheel.y > 0) {
            player.target += 0.15f;
            UI::toolTip.drawToolTip = false;
        } else if (e.wheel.y < 0) {
            player.target -= 0.15f;
            UI::toolTip.drawToolTip = false;
        }

        e.wheel.y = 0;

        // min / max zoom
        player.target = glm::clamp(player.target, 1.0f, 7.5f);
    }

    //
    //  MOUSE
    //

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            TERRITORY::mouseClick = true;
        }
        if (e.button.button == SDL_BUTTON_MIDDLE) {
            TERRITORY::toolTip();

            // mouse click on any factory
            for (FACTORY::Building boo : FACTORY::factorys) {
                glm::ivec2 mouseGridPos = TOOLBOX::findMousePos() * TERRAIN::tileSize;

                // if click is inside an already existing building
                if (TOOLBOX::checkMouseCollision(mouseGridPos, boo.hitbox)) {

                    std::string type;

                    switch (boo.facType) {
                        case 1: type = "Clothing";  break;
                        case 2: type = "Wood";      break;
                        case 3: type = "Stone";     break;
                        case 4: type = "Steel";     break;
                        case 5: type = "Oil";       break;

                    }


                    glm::ivec2 pos;
                    SDL_GetMouseState(&pos.x, &pos.y);
                    UI::toolTip.toolTipRect = {pos.x + 30, pos.y + 30, 0,0};
                    UI::toolTip.toolTipTitle = "Factory";
                    UI::toolTip.toolTipText =
                        "Level: " + std::to_string(boo.level) +
                        " | Type: " + type;
                    UI::toolTip.drawToolTip = true;

                    break;
                }
            }

            for (auto& foo : CITY::citys) {
                glm::ivec2 mouseGridPos = TOOLBOX::findMousePos() * TERRAIN::tileSize;

                if (TOOLBOX::checkMouseCollision(mouseGridPos, foo.hitbox)) {

                    glm::ivec2 pos;
                    SDL_GetMouseState(&pos.x, &pos.y);
                    UI::toolTip.toolTipRect = {pos.x + 30, pos.y + 30, 0,0};
                    UI::toolTip.toolTipTitle = "City";
                    UI::toolTip.toolTipText = "Level: " + std::to_string(foo.level);
                    UI::toolTip.drawToolTip = true;

                    break;
                }
            }
        }

        // |===================|
        // |    Spawn House    |
        // |===================|

        static Uint32 diff = 0;
        const Uint32 cooldown = 200;

        if (e.button.button == SDL_BUTTON_RIGHT) {

            // cooldown per click
            Uint32 time = SDL_GetTicks();
            if (time - diff >= cooldown) {

                if (UI::bottomLeftUi.index <= 5) {
                    FACTORY::mouseClick();
                } else {
                    CITY::mouseClick();
                }

                diff = time;
            }
        }

    }
    player.zoom += (player.target - player.zoom) * 0.1f;
}
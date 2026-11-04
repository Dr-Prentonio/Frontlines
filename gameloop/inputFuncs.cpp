#include "../header/generalClasses.hpp"
#include "../header/generalFuncs.hpp"
#include "../header/UIClasses.hpp"
#include "../header/buildingsClasses.hpp"
#include "../header/terrainClasses.hpp"


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
        UI::TOOLTIP::enable = false;
    }

    else if (state[SDL_SCANCODE_S] && !player.touchesBorderBottom) {
        movement.y -= baseSpeed * speed * deltaTime;
        UI::TOOLTIP::enable = false;
    }

    else if (state[SDL_SCANCODE_A] && !player.touchesBorderLeft) {
        movement.x += baseSpeed * speed * deltaTime;
        UI::TOOLTIP::enable = false;
    }

    else if (state[SDL_SCANCODE_D] && !player.touchesBorderRight) {
        movement.x -= baseSpeed * speed * deltaTime;
        UI::TOOLTIP::enable = false;
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
            UI::TOOLTIP::enable = false;
        } else if (e.wheel.y < 0) {
            player.target -= 0.15f;
            UI::TOOLTIP::enable = false;
        }

        e.wheel.y = 0;

        // min / max zoom
        player.target = glm::clamp(player.target, 1.0f, 7.5f);
    }

    if (e.type == SDL_MOUSEMOTION) UI::TOOLTIP::enable = false;

    //
    //  MOUSE
    //

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {
            TERRITORY::mouseClick = true;
        }
        if (e.button.button == SDL_BUTTON_MIDDLE) {
            UI::TOOLTIP::manager();
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

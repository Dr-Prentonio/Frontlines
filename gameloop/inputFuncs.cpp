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

    if (state[SDL_SCANCODE_W] && !PLAYER::touchesBorderTop) {
        movement.y += baseSpeed * speed * deltaTime;
        UI::TOOLTIP::enable = false;
    }

    else if (state[SDL_SCANCODE_S] && !PLAYER::touchesBorderBottom) {
        movement.y -= baseSpeed * speed * deltaTime;
        UI::TOOLTIP::enable = false;
    }

    else if (state[SDL_SCANCODE_A] && !PLAYER::touchesBorderLeft) {
        movement.x += baseSpeed * speed * deltaTime;
        UI::TOOLTIP::enable = false;
    }

    else if (state[SDL_SCANCODE_D] && !PLAYER::touchesBorderRight) {
        movement.x -= baseSpeed * speed * deltaTime;
        UI::TOOLTIP::enable = false;
    }


    // for buildings
    if (state[SDL_SCANCODE_1])      UI::bottomLeftUi.index = 1;
    else if (state[SDL_SCANCODE_2]) UI::bottomLeftUi.index = 2;
    else if (state[SDL_SCANCODE_3]) UI::bottomLeftUi.index = 3;
    else if (state[SDL_SCANCODE_4]) UI::bottomLeftUi.index = 4;
    else if (state[SDL_SCANCODE_5]) UI::bottomLeftUi.index = 5;
    else if (state[SDL_SCANCODE_6]) UI::bottomLeftUi.index = 6;
    else if (state[SDL_SCANCODE_7]) UI::bottomLeftUi.index = 7;

    PLAYER::camOffset = {
        static_cast<int>(glm::round(movement.x)),
        static_cast<int>(glm::round(movement.y)),
        0,
        0
    };

    //
    // MOUSE WHEEL
    //

    // zoom
    if (e.type == SDL_MOUSEWHEEL) {
        if (e.wheel.y > 0) {
            PLAYER::target += 0.15f;
            UI::TOOLTIP::enable = false;
        } else if (e.wheel.y < 0) {
            PLAYER::target -= 0.15f;
            UI::TOOLTIP::enable = false;
        }

        e.wheel.y = 0;

        // min / max zoom
        PLAYER::target = glm::clamp(PLAYER::target, 1.0f, 7.5f);
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

                if (UI::bottomLeftUi.index == 7)        MILITARYCAMP::mouseClick();
                else if (UI::bottomLeftUi.index == 6)   CITY::mouseClick();
                else                                    FACTORY::mouseClick();

                diff = time;
            }
        }

    }
    PLAYER::zoom += (PLAYER::target - PLAYER::zoom) * 0.1f;
}

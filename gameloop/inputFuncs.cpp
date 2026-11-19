#include "../header/generalClasses.hpp"
#include "../header/generalFuncs.hpp"
#include "../header/UIClasses.hpp"
#include "../header/buildingsClasses.hpp"
#include "../header/terrainClasses.hpp"

void SDL::MAIN::input(SDL_Event &e) {

    // get key states
    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    static glm::vec2 movement {};

    // |================|
    // |    Keyboard    |
    // |================|

    [state]{
        float shift {1};
        constexpr int baseSpeed {250};

        // faster movement
        if (state[SDL_SCANCODE_LSHIFT]) {
            shift = 2.0f;
        }
        else if (state[SDL_SCANCODE_LCTRL]) {
            shift = 0.5f;
        }

        // wasd
        if (state[SDL_SCANCODE_W] && !PLAYER::touchesBorderTop) {
            movement.y += baseSpeed * shift * deltaTime;
            UI::TOOLTIP::enable = false;
        }
        else if (state[SDL_SCANCODE_S] && !PLAYER::touchesBorderBottom) {
            movement.y -= baseSpeed * shift * deltaTime;
            UI::TOOLTIP::enable = false;
        }
        if (state[SDL_SCANCODE_A] && !PLAYER::touchesBorderLeft) {
            movement.x += baseSpeed * shift * deltaTime;
            UI::TOOLTIP::enable = false;
        }
        else if (state[SDL_SCANCODE_D] && !PLAYER::touchesBorderRight) {
            movement.x -= baseSpeed * shift * deltaTime;
            UI::TOOLTIP::enable = false;
        }

        PLAYER::camOffset = {
            static_cast<int>(glm::round(movement.x)),
            static_cast<int>(glm::round(movement.y)),
            0,
            0
        };
    }();

    // |===================|
    // |    MOUSE WHEEL    |
    // |===================|

    [&e] {
        if (e.type == SDL_MOUSEWHEEL) {

            // zoom sensitivity
            constexpr float zoomSens {0.2f};

            if (e.wheel.y > 0) {
                PLAYER::target += zoomSens;
                UI::TOOLTIP::enable = false;
            } else if (e.wheel.y < 0) {
                PLAYER::target -= zoomSens;
                UI::TOOLTIP::enable = false;
            }

            // reseting event
            e.wheel.y = 0;

            // min / max zoom
            constexpr float maxZoom {15.0f};
            PLAYER::target = glm::clamp(PLAYER::target, 1.0f, maxZoom);
        }

        PLAYER::zoom += (PLAYER::target - PLAYER::zoom) * 0.1f;
    }();

    // |=============|
    // |    MOUSE    |
    // |=============|

    [&e] {
        // make it that the tooltip doesnt get disabled instantly
        static glm::ivec2 oldMousePos {};
        static glm::ivec2 newMousePos {};

        SDL_GetMouseState(&newMousePos.x, &newMousePos.y);

        // num thats always positiv
        glm::ivec2 difMousePos = glm::abs(oldMousePos - newMousePos);

        // delay
        if (e.type == SDL_MOUSEMOTION and (difMousePos.x > 10  or difMousePos.y > 10 )) {
            SDL_GetMouseState(&oldMousePos.x, &oldMousePos.y);
            UI::TOOLTIP::enable = false;
        }

        // mouse button events
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            [&e]{
                // check if its the right event, if the tool is selected and if its NOT hitting the ui
                if (e.button.button == SDL_BUTTON_LEFT) {
                    // war tool
                    if (UI::TOOL::SELECT::index == UI::TOOL::SELECT::ToolIndex::war &&
                        !TOOLBOX::checkMouseUICollision(newMousePos)) {

                        TERRITORY::mouseClick = true;
                        }

                    // mining tool
                    static Uint32 delta {};
                    constexpr Uint32 cooldown {333};

                    Uint32 time = SDL_GetTicks();
                    if (time - delta >= cooldown) {
                        if (UI::TOOL::SELECT::index == UI::TOOL::SELECT::ToolIndex::mine and
                            !TOOLBOX::checkMouseUICollision(newMousePos)) {

                            PLAYER::MINING::mouseClick();
                            }

                        delta = time;
                    }
                }
            }();

            // tooltip
            if (e.button.button == SDL_BUTTON_MIDDLE) {
                UI::TOOLTIP::manager();
            }


            static Uint32 diff {};
            constexpr Uint32 cooldown {200};

            if (e.button.button == SDL_BUTTON_RIGHT) {

                // cooldown per click
                Uint32 time = SDL_GetTicks();
                if (time - diff >= cooldown) {

                    if (UI::bottomLeftUi.index == 8)        MILITARYCAMP::mouseClick();
                    else if (UI::bottomLeftUi.index == 7)   CITY::mouseClick();
                    else                                    FACTORY::mouseClick();

                    diff = time;
                }
            }
        }
    }();
}

//
// Created by tobias on 23.10.26.
//

#include <thread>

#include "../header/devModeClasses.hpp"
#include "../header/debugLogging.hpp"
#include "../header/generalFuncs.hpp"

int DEVMODE::init() {

    window.win = ::window.win;
    if (window.win == nullptr) {
        std::string err = SDL_GetError();
        LOG::INIT_ERR("Couldnt create dev window: " + err);
        return -1;
    }
    SDL_SetWindowSize(window.win, 400, 200);
    SDL_SetWindowTitle(window.win, "Devmode - Settings");
    LOG::INIT_SUCCESS("Created window");

    window.rend = ::window.rend;
    if (window.rend == nullptr) {
        std::string err = SDL_GetError();
        LOG::INIT_ERR("Couldnt create renderer: " + err);
        return -1;
    }
    LOG::INIT_SUCCESS("Created renderer");

    if (TTF_Init() != 0) {
        LOG::INIT_ERR("Couldnt init TTF");
        return -1;
    }

    LOG::INIT_SUCCESS("TTF init successfull");

    std::string font = "assets/Font/TerminessNerdFont-Bold.ttf";
    UI::TEXT::labelFont = TTF_OpenFont(font.c_str(), 24);

    if (UI::TEXT::labelFont == nullptr) {
        LOG::INIT_ERR("Couldnt open font file -> " + font + " >> " + TTF_GetError());
    }
    LOG::INIT_SUCCESS("Opened file " + font + " Successfully");

    return 0;
}

void DEVMODE::draw() {
    SDL_Renderer* rend = window.rend;

    SDL_SetRenderDrawColor(rend, 0,0,0,255);
    SDL_RenderClear(rend);

    // Draw all checkboxes
    for (auto& checkBox : UI::checkBoxes) {
        UI::drawCheckBox(rend, checkBox.drect, checkBox.checked);
    }
    for (auto& label : UI::TEXT::labels) {
        SDL_RenderCopy(rend, label.texture, nullptr, &label.drect);
    }

    SDL_RenderPresent(rend);
}

void DEVMODE::input(SDL_Event &e) {

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        if (e.button.button == SDL_BUTTON_LEFT) {

            glm::ivec2 mouse;
            SDL_GetMouseState(&mouse.x, &mouse.y);

            static Uint32 diff = 0;
            static const Uint32 cooldown = 175;

            Uint32 time = SDL_GetTicks();
            if (time - diff >= cooldown) {

                // look which checkbox got clicked
                for (auto& checkBox : UI::checkBoxes) {
                    if (TOOLBOX::checkMouseCollision(mouse, checkBox.drect)) {

                        if (checkBox.checked == false) {
                            checkBox.checked = true;
                        } else {
                            checkBox.checked = false;
                        }
                    }
                }

                diff = time;
            }
        }
    }
}

void DEVMODE::loop() {

    static SDL_Event event;
    bool run = true;

    UI::initCheckBox((SDL_Rect){15,25,25,25}, true);
    UI::initCheckBox((SDL_Rect){15,75,25,25}, true);

    UI::TEXT::initLabel((SDL_Rect){60,25,0,0}, "Enable Dev - Mode");
    UI::TEXT::initLabel((SDL_Rect){60,75,0,0}, "Enable Pre Generated Map");
    UI::TEXT::initLabel((SDL_Rect){25,120,0,0}, "Space for new map and Godmode");

    while (run) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run = false;
                break;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    run = false;
                    break;
                }

                // if space is pressed use basic preset
                if (event.key.keysym.sym == SDLK_SPACE) {
                    run = false;

                    UI::checkBoxes.at(0).checked = true;
                    UI::checkBoxes.at(1).checked = false;

                    break;
                }
            }
        }

        input(event);
        draw();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

}

int DEVMODE::cleanup() {
    LOG::INFO("Devmode cleanup");

    SDL_DestroyRenderer(window.rend);
    SDL_DestroyWindow(window.win);

    return 0;
}
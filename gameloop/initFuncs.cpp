#include "../header/generalClasses.hpp"
#include <cstdlib>
#include <random>

#include "../header/UIClasses.hpp"
#include "../header/debugLogging.hpp"

int SDL::Window::init() {

    // SDL INIT
    LOG::INIT_SUCCESS("Begin SDL init");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0) {
        LOG::INIT_ERR("SDL init error");
        return -1;
    }
    LOG::INIT_SUCCESS("SDL init success");

    // Init Window
    win = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        scale.x,
        scale.y,
        SDL_WINDOW_SHOWN
        );
    if (win == nullptr) {
        LOG::INIT_ERR("SDL window couldnt be created");
        return -1;
    }

    SDL_Surface* icon = IMG_Load("knigaCar.jpg");

    if (icon == nullptr) {
        std::string err = SDL_GetError();
        LOG::WARN("Window icon couldnt be loaded: " + err);
    } else {
        SDL_SetWindowIcon(win, icon);
    }

    // Renderer Init
    rend = SDL_CreateRenderer(
        win,
        -1,
        SDL_RENDERER_ACCELERATED
        );
    if (rend == nullptr) {
        LOG::INIT_ERR("SDL renderer couldnt be created");
        return -1;
    }
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

    LOG::INIT_SUCCESS("SDL Init success, window and renderer initialized");

    // TTF Init
    if (TTF_Init() < 0) {
        LOG::INIT_ERR("TTF couldnt be initialized");
        return -1;
    }

    UI::fonts.terminessItalic   = TTF_OpenFont("assets/Font/TerminessNerdFont-Italic.ttf",  20);
    UI::fonts.terminessNormal   = TTF_OpenFont("assets/Font/TerminessNerdFont-Regular.ttf", 24);
    UI::fonts.terminessBold     = TTF_OpenFont("assets/Font/TerminessNerdFont-Bold.ttf",    24);
    UI::fonts.RobotoMonoBold    = TTF_OpenFont("assets/Font/RobotoMono-Bold.ttf",           22);

    if (UI::fonts.terminessItalic == nullptr    ||
        UI::fonts.terminessBold == nullptr      ||
        UI::fonts.terminessNormal == nullptr    ||
        UI::fonts.terminessNormal == nullptr    ) {

        LOG::INIT_ERR(TTF_GetError());
    }

    LOG::INIT_SUCCESS("Initialized TTF success");

    return 0;
}

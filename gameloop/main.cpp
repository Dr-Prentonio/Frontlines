#include <thread>

// sdl2
#include <SDL2/SDL.h>

// own classes
#include "../header/generalClasses.hpp"
#include "../header/buildingsClasses.hpp"
#include "../header/devModeClasses.hpp"
#include "../header/debugLogging.hpp"
#include "../header/specializedFieldClasses.hpp"

SDL::Gameloop gameloop;
SDL::Window window;
Player player;

//
// GRR FOR loops Very VERRY bad grr
//

// cleanup func
void cleanup() {

    LOG::INFO("Cleaning Up");

    SDL_DestroyTexture(TERRITORY::layerTexture);

    SDL_DestroyTexture(TERRAIN::LOD.LoD1);
    SDL_DestroyTexture(TERRAIN::LOD.LoD2);
    SDL_DestroyTexture(TERRAIN::LOD.LoD3);

    SDL_DestroyRenderer(window.rend);
    SDL_DestroyWindow(window.win);

    LOG::INFO("Cleaning up done");

}

int main() {

    bool run = true;    // main game loop
    bool devMode = true; // enable / disable the developer Mode
    SDL_Event event;

    // |=============================|
    // |    Window Initialization    |
    // |=============================|

    // init the window
    if (window.init() != 0) {
        LOG::INIT_ERR("Failed to initialize SDL, aborting");
        return -1;
    }

    // |===============================|
    // |    Developer Mode Settings    |
    // |===============================|

    // developer mode (has its own rend and window)
    if (devMode) {
        // init
        if (DEVMODE::init() != 0) LOG::INIT_ERR("Devmode couldnt be initialized, starting game init");
        else LOG::INIT_SUCCESS("Devmode initialized and starting");

        DEVMODE::loop();

        LOG::INFO("Devmode stoped, starting Game");

        DEVMODE::readOutput();

        // set the window back to its original size
        SDL_SetWindowSize(window.win, window.scale.x, window.scale.y);
        SDL_SetWindowPosition(window.win, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_SetWindowTitle(window.win, window.title);
    }

    // |=================|
    // |    Map Setup    |
    // |=================|

    // choose between random map and static map
    if (!SDL::useStaticMaps) TERRAIN::generateTerrain();
    else DEVMODE::useStaticMaps();

    FORREST::init(); // load forrest textures
    FIELD_GENERATION::manager();

    TERRITORY::setupLayer(); // setup the player territory layer

    // |=====================|
    // |    Building Init    |
    // |=====================|

    if (FACTORY::init() != 0) {
        LOG::INIT_ERR("Couldnt initialize factorys");
        return -1;
    }

    if (CITY::init() != 0) {
        LOG::INIT_ERR("Couldnt initialize citys");
        return -1;
    }

    // |=================|
    // |    Game Loop    |
    // |=================|

    Uint64 lastFrame = SDL_GetPerformanceCounter();
    Uint64 newFrame = 0;

    while (run) {

        // delta frame
        newFrame = SDL_GetPerformanceCounter();
        gameloop.deltaTime = (float)(newFrame - lastFrame) / (float) SDL_GetPerformanceFrequency();
        lastFrame = newFrame;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run = false;
                break;
            }
        }

        gameloop.input(event);

        TERRITORY::updateLayer();

        gameloop.draw();

        player.update(gameloop.deltaTime);

        FACTORY::update();

        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }

    cleanup();

    return 0;
}
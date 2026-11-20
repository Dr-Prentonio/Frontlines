#include <thread>

// sdl2
#include <SDL2/SDL.h>

// own classes
#include "../header/generalClasses.hpp"
#include "../header/buildingsClasses.hpp"
#include "../header/devModeClasses.hpp"
#include "../header/debugLogging.hpp"
#include "../header/terrainClasses.hpp"

//
// GRR FOR loops Very VERRY bad grr
//

// cleanup func
void cleanup() {
    LOG::INFO("Cleaning Up");

    SDL_DestroyTexture(TERRITORY::layerTexture);

    // free map textures
    SDL_DestroyTexture(TERRAIN::LOD::LoD1);
    SDL_DestroyTexture(TERRAIN::LOD::LoD2);
    SDL_DestroyTexture(TERRAIN::LOD::LoD3);

    // free building icons
    SDL_DestroyTexture(FACTORY::icons.clothingTex);
    SDL_DestroyTexture(FACTORY::icons.oilTex);
    SDL_DestroyTexture(FACTORY::icons.steelTex);
    SDL_DestroyTexture(FACTORY::icons.stoneTex);
    SDL_DestroyTexture(FACTORY::icons.woodTex);
    SDL_DestroyTexture(CITY::cityTex);
    SDL_DestroyTexture(MILITARYCAMP::campTex);

    // free terrain icons
    SDL_DestroyTexture(TERRAIN::FOLIAGE::GENERAL::textures.cotton);
    SDL_DestroyTexture(TERRAIN::FOLIAGE::GENERAL::textures.spruce);
    SDL_DestroyTexture(TERRAIN::FOLIAGE::GENERAL::textures.oak);

    // free ui icons
    SDL_DestroyTexture(UI::TOOL::SELECT::textures.build);
    SDL_DestroyTexture(UI::TOOL::SELECT::textures.war);
    SDL_DestroyTexture(UI::TOOL::BUILDTOOL::textures.city);
    SDL_DestroyTexture(UI::TOOL::BUILDTOOL::textures.factory);

    SDL_DestroyRenderer(SDL::WINDOW::rend);
    SDL_DestroyWindow(SDL::WINDOW::win);

    LOG::INFO("Cleaning up done");
}

int devmodeManager() {
    // init
    if (DEVMODE::init() != 0) {
        LOG::INIT_ERR("Devmode couldnt be initialized");
        return -1;
    }
    LOG::INIT_SUCCESS("Devmode initialized and starting");

    DEVMODE::loop();

    LOG::INFO("Devmode stoped, starting Game");

    DEVMODE::readOutput();

    // set the window back to its original size
    SDL_SetWindowSize(SDL::WINDOW::win, SDL::WINDOW::scale.x, SDL::WINDOW::scale.y);
    SDL_SetWindowPosition(SDL::WINDOW::win, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowTitle(SDL::WINDOW::win, SDL::WINDOW::title);

    return 0;
}

int main() {

    bool run {true};    // main game loop
    bool devMode {true}; // enable / disable the developer Mode
    SDL_Event event {};

    // |=============================|
    // |    Window Initialization    |
    // |=============================|

    // init the window
    if (SDL::WINDOW::init() != 0) {
        LOG::INIT_ERR("Failed to initialize SDL, aborting");
        return -1;
    }

    // |===============================|
    // |    Developer Mode Settings    |
    // |===============================|

    // developer mode (has its own rend and window)
    if (devMode) {
        if (devmodeManager() != 0)
            LOG::ERR("Devmode error, starting game init");
    }

    // |=================|
    // |    Map Setup    |
    // |=================|

    // choose between random map and static map
    if (!SDL::useStaticMaps) TERRAIN::SURFACE::generateTerrain();
    else DEVMODE::useStaticMaps();

    // generate random forests etc
    TERRAIN::FOLIAGE::GENERAL::init(); // load textures

    TERRAIN::FOLIAGE::FOREST::generate();
    TERRAIN::FOLIAGE::COTTON::generate();
    TERRAIN::FOLIAGE::OIL::generate();
    TERRAIN::FOLIAGE::STONE::generate();

    TERRAIN::FOLIAGE::GENERAL::drawIcons();

    // generate lods (needs foliage and surface to run before)
    TERRAIN::LOD::generateLoDs();

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

    if (MILITARYCAMP::init() != 0) {
        LOG::INIT_ERR("Couldnt initialize military camps");
        return -1;
    }

    // |===============|
    // |    UI Init    |
    // |===============|

    if (UI::TOOL::SELECT::init() != 0) {
        LOG::INIT_ERR("Couldnt init tool select");
        return -1;
    }

    if (UI::TOOL::BUILDTOOL::init() != 0) {
        LOG::INIT_ERR("Couldnt init build tool");
        return -1;
    }

    // |=================|
    // |    Game Loop    |
    // |=================|

    uint64_t lastFrame = SDL_GetPerformanceCounter();
    uint64_t newFrame = {};

    while (run) {

        // delta frame
        newFrame = SDL_GetPerformanceCounter();
        SDL::MAIN::deltaTime = (float)(newFrame - lastFrame) / (float) SDL_GetPerformanceFrequency();
        lastFrame = newFrame;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run = false;
                break;
            }
        }

        SDL::MAIN::input(event);

        TERRITORY::updateLayer();

        SDL::MAIN::draw(event);

        PLAYER::update(SDL::MAIN::deltaTime);

        FACTORY::update();
        MILITARYCAMP::update();

        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }

    cleanup();

    return 0;
}
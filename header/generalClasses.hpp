// 09.10.2026

#ifndef STRATEGIE_GAME_GENERALCLASSES_HPP
#define STRATEGIE_GAME_GENERALCLASSES_HPP

#include <chrono>
#include <fstream>

#include "buildingsClasses.hpp"
#include "UIClasses.hpp"

namespace ch = std::chrono;

#include "glm/glm.hpp"
#include "glm/gtc/noise.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// SDL2
namespace SDL {

    // if in dev settings is selected a static
    // texture will be loaded
    inline bool useStaticMaps = false;

    // Window data
    struct Window {

        SDL_Renderer* rend;

        SDL_Window* win;
        glm::ivec2 scale = glm::ivec2(1920, 1080);
        const char* title = "Strategie Game";

        int init();
    };

    struct Gameloop {

        void draw(SDL_Event &e);

        void input(SDL_Event &e);

        // delta time in seconds
        float deltaTime;
    };

}

// player
namespace PLAYER {
    // Resources and factorys
    struct Resources_Def {
        Uint32 wood       = 100; // 100
        Uint32 stone      = 100; // 100
        Uint32 clothes    = 0;   // 0
        Uint32 steel      = 100; // 100
        Uint32 oil        = 0;   // 0
        Uint32 weapons    = 50;
    };
    // factorys take 75 citizen
    struct Factory_Def {
        Uint32 clothesFactorys  = 0;
        Uint32 steelFactorys    = 0;
        Uint32 oilFactorys      = 0;
        Uint32 woodFactorys     = 0;
        Uint32 stoneFactorys    = 0;
        Uint32 weaponFactorys   = 0;
    };
    // citys give 400 citizen
    struct Citizen_Def {
        Uint32 citysTotal = 0;
        Uint32 citizen = 1000;
    };
    struct Military_Def {
        int troops = 5000;
        Uint32 militaryCamps = 0;
    };
    struct OwnedTiles {

        glm::ivec2 pos; // pos in world cords

        // very important if a wrong number is passed
        // the whole game might crash
        int worldIndex; // index of the same tiles (map, layer)

        // if its either an country tile or
        // an water tile
        bool topBlocked = false;
        bool bottomBlocked = false;
        bool leftBlocked = false;
        bool rightBlocked = false;

        bool fullBoxed = false;
    };

    inline Citizen_Def citizen;
    inline Factory_Def factorys;
    inline Resources_Def resources;
    inline Military_Def military;

    inline SDL_Rect camOffset;
    inline float zoom = 1;
    inline float target = 1;

    // if touches map border and cant move anymore
    inline bool touchesBorderRight;
    inline bool touchesBorderLeft;
    inline bool touchesBorderTop;
    inline bool touchesBorderBottom;

    inline bool playerOwnsAKingdom = false;

    // update player tiles in progress
    inline std::vector<OwnedTiles> playerTiles;
    void update(float deltaTime);
}

// definition of the firste few colors
#define Red (SDL_Color){245, 0, 0, 125};
#define Red_Border (SDL_Color){245, 200, 200, 125};
#define Clear (SDL_Color){0, 0, 0, 0};

// territory asignment
namespace TERRITORY {

    struct territoryTile {

        SDL_Rect rect = {0,0,0,0};  // size and pos of the tile
        SDL_Color color = Clear;    // color of the field
        int ownerID = 0;    // the owner of this field

    };

    inline std::vector<territoryTile> layer;
    inline SDL_Texture* layerTexture;

    inline bool mouseClick = false;

    void setupLayer();
    void updateLayer();
    void expandTerritory();

    void toolTip();

    void spawnTerritory(glm::vec2 pos, int size);

}

extern SDL::Window window;
extern SDL::Gameloop gameloop;

#endif //STRATEGIE_GAME_GENERALCLASSES_HPP

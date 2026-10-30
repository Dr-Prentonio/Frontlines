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

        void draw();

        void input(SDL_Event &e);

        // delta time in seconds
        float deltaTime;
    };

}

// player
struct Player {

    SDL_Rect camOffset;
    float zoom = 1;
    float target = 1;

    // if touches map border and cant move anymore
    bool touchesBorderRight;
    bool touchesBorderLeft;
    bool touchesBorderTop;
    bool touchesBorderBottom;

    bool playerOwnsAKingdom = false;
    int troopAmount = 1000;

    // Resources and factorys
    struct Resources {
        Uint32 woodAmount = 100;    // 100
        Uint32 stoneAmount = 100;   // 100
        Uint32 clothesAmount = 0; // 0
        Uint32 steelAmount = 100;   // 100
        Uint32 oilAmount = 0;     // 0
    };

    // factorys take 75 citizen
    struct Factory {
        Uint32 clothesFactorys = 0;
        Uint32 steelFactorys = 0;
        Uint32 oilFactorys = 0;
        Uint32 woodFactorys = 0;
        Uint32 stoneFactorys = 0;
    };

    // citys give 400 citizen
    struct Citizen {
        Uint32 citysTotal = 0;
        Uint32 citizen = 1000;
    };
    Citizen citizen;

    Factory factorys;
    Resources resources;

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

    std::vector<OwnedTiles> playerTiles;

    void update(float deltaTime);
};

// Terrain
namespace TERRAIN {
    enum TerrainType {
        DEEP_SEA,
        SEA,
        SHORE,
        GRASS,
        TAIGA,
        MOUNTAIN,
        SNOW
    };

    struct TerrainTile {
        SDL_Rect rect;
        TerrainType type;
    };

    // corrosion simulation
    struct Droplet {

        glm::vec2 pos;
        glm::vec2 speed = {0,0};
        float amount = 1.0f;
        float sediment = 0.0f;
    };

    struct LOD_Types {
        SDL_Texture* LoD1; // smoothest
        SDL_Texture* LoD2;
        SDL_Texture* LoD3; // Crapiest
    };

    inline LOD_Types LOD;

    const int tileSize = 32;
    const int scale = 14; // whole game relies on THIS one number TwT

    inline std::vector<TerrainTile> map; // map vector

    inline SDL_Texture* mapTexture;

    inline glm::ivec2 mapSize; // size of the map

    void generateTerrain();
    void generateTile(SDL_Renderer* rend);
    void generateLoDs();
}

// definition of the firste few colors
#define Red (SDL_Color){245, 0, 0, 125};
#define Red_Border (SDL_Color){245, 200, 200, 125};
#define Clear (SDL_Color){0, 0, 0, 0};

// territory asignment
namespace TERRITORY {

    struct territoryTile {

        SDL_Rect rect;  // size and pos of the tile
        SDL_Color color = Clear;    // color of the field
        int ownerID;    // the owner of this field

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
extern Player player;

#endif //STRATEGIE_GAME_GENERALCLASSES_HPP

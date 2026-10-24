//
// Created by tobias on 23.10.26.
//

#include "../header/devModeClasses.hpp"
#include "../header/debugLogging.hpp"
#include "../header/generalFuncs.hpp"

void DEVMODE::readOutput() {

    if (UI::checkBoxes.at(0).checked) {
        LOG::INFO("God Mode activated");

        // Rescource                      Amount
        player.resources.clothesAmount = 1000000;
        player.resources.oilAmount     = 1000000;
        player.resources.steelAmount   = 1000000;
        player.resources.stoneAmount   = 1000000;
        player.resources.woodAmount    = 1000000;

        player.citizen.citizen         = 1000000;
        player.troopAmount             = 1000000;
    }

    if (UI::checkBoxes.at(1).checked) {
        LOG::INFO("Using Static Maps");

        SDL::useStaticMaps = true;
    }
}

int DEVMODE::useStaticMaps() {

    std::ifstream inFile("staticMapArray.txt");

    if (!inFile.is_open()) {
        LOG::ERR("Couldnt open staticMapArray.txt");
        return -1;
    }

    SDL::Window win;
    TERRAIN::TerrainTile tile;

    LOG::INFO("Window size: " + std::to_string(win.scale.x) + " " + std::to_string(win.scale.y));

    int totalTiles = win.scale.x / TERRAIN::tileSize * TERRAIN::scale * win.scale.y / TERRAIN::tileSize * TERRAIN::scale;

    std::vector<TERRAIN::TerrainTile> map;
    map.reserve(totalTiles);

    glm::ivec2 pos;
    int type;

    while (inFile >> pos.x >> pos.y >> type) {

        tile.rect = {
            pos.x,
            pos.y,
            TERRAIN::tileSize,
            TERRAIN::tileSize
        };

        switch (type) {
            case 0: tile.type = TERRAIN::DEEP_SEA;  break;
            case 1: tile.type = TERRAIN::SEA;       break;
            case 2: tile.type = TERRAIN::SHORE;     break;
            case 3: tile.type = TERRAIN::GRASS;     break;
            case 4: tile.type = TERRAIN::TAIGA;     break;
            case 5: tile.type = TERRAIN::MOUNTAIN;  break;
            case 6: tile.type = TERRAIN::SNOW;      break;
            default: break;
        }

        map.push_back(tile);
    }

    TERRAIN::map = map;

    LOG::INFO("Succesfully read " + std::to_string(map.size()) + " tiles");

    SDL_Renderer* rend = window.rend;

    if (rend == nullptr) {
        LOG::ERR("Couldnt create renderer");
        return -1;
    }

    glm::ivec2 mapSize = glm::ivec2(
        win.scale.x / TERRAIN::tileSize * TERRAIN::scale,
        win.scale.y / TERRAIN::tileSize * TERRAIN::scale
        );

    TERRAIN::mapSize = mapSize;

    SDL_Texture* tempMap;
    tempMap = SDL_CreateTexture(
        rend,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        mapSize.x * TERRAIN::tileSize,
        mapSize.y * TERRAIN::tileSize
    );

    if (tempMap == nullptr) {
        std::string err = SDL_GetError();

        LOG::ERR("Couldnt create world texture: " + err);
        return -1;
    }

    SDL_SetRenderTarget(rend, tempMap);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);

    TERRAIN::generateTile(rend);

    SDL_SetRenderTarget(rend, nullptr);

    int width = 0, height = 0;

    SDL_QueryTexture(tempMap, nullptr, nullptr, &width, &height);

    // Generate LODs
    TERRAIN::generateLOD(rend, tempMap, {width / 2, height / 2}, TERRAIN::LOD.LoD1);
    TERRAIN::generateLOD(rend, tempMap, {width / 4, height / 4}, TERRAIN::LOD.LoD2);
    TERRAIN::generateLOD(rend, tempMap, {width / 8, height / 8}, TERRAIN::LOD.LoD3);

    LOG::INFO("Generated LODs");

    SDL_SetRenderTarget(rend, nullptr);
    SDL_DestroyTexture(tempMap);

    return 0;
}
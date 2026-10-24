#include "../header/generalClasses.hpp"
#include "../header/debugLogging.hpp"
#include "../header/UIClasses.hpp"
#include <random>

void TERRITORY::setupLayer() {
    LOG::INFO("Setup for secondary layer < STARTED >");

    // assign new layer same pos and sizes
    for (int y = 0; y < TERRAIN::mapSize.y; y++) {
        for (int x = 0; x < TERRAIN::mapSize.x; x++) {

            territoryTile tile;
            tile.rect = {x * TERRAIN::tileSize, y * TERRAIN::tileSize, TERRAIN::tileSize, TERRAIN::tileSize};

            layer.push_back(tile);
        }
    }

    SDL_Renderer* rend = window.rend;

    layerTexture = SDL_CreateTexture(
        rend,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        TERRAIN::mapSize.x * TERRAIN::tileSize,
        TERRAIN::mapSize.y * TERRAIN::tileSize
    );

    SDL_SetTextureBlendMode(layerTexture, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(rend, layerTexture);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
    SDL_RenderClear(rend);

    for (int i = 0; i < layer.size(); i++) {

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
        SDL_RenderFillRect(rend, &layer[i].rect);
    }
    SDL_SetRenderTarget(rend, NULL);

    LOG::INFO("Setup for secondary layer < DONE >");
}

//
// ================================================
//
// default print size = window scale / map size  <-- how big are the tiles by default
// current tile size = default * zoom
//
// ================================================
//
// float defaultSize = window.scale / mapSize;
// float currentSize = defaultSize * player.zoom;
//
// for (int y = 0; y < mapSize.y; y++) {
//      for (int x = 0; x < mapSize.x; x++) {
//
//          glm::vec2 newPos = {
//               currentSize * x,
//               currentSize * y
//          };
//      }
// }
//

// |================|
// | START POSITION |
// |================|

// spawns a circle (start territory)
void TERRITORY::spawnTerritory(glm::vec2 pos, int size) {

    for (int y = -size; y <= size; y++) {
        for (int x = -size; x <= size; x++) {

            int distance2 = (x * x) + (y * y);

            if (distance2 <= (size * size)) {
                glm::ivec2 tilePos = { pos.x + x, pos.y + y };
                int tileIndex = tilePos.x + tilePos.y * TERRAIN::mapSize.x;

                if (tilePos.x < 0 || tilePos.x >= TERRAIN::mapSize.x || tilePos.y < 0 || tilePos.y >= TERRAIN::mapSize.y) {
                    continue;
                }

                // is the territory on water?
                if (TERRAIN::map[tileIndex].type > 1) {
                    layer[tileIndex].color = Red;
                    layer[tileIndex].ownerID = 1;       // owner id

                    // |=========================|
                    // | Performance Calculation |
                    // |=========================|

                    Player::OwnedTiles ownedTiles;

                    int width = TERRAIN::mapSize.x;
                    int height = TERRAIN::mapSize.y;
                    int type;
                    int ownerID;

                    int xx = tilePos.x;
                    int yy = tilePos.y;

                    if (xx < width - 1) {
                        // RIGHT
                        int rightIndex      = (xx + 1) + yy * width;
                        type = TERRAIN::map.at(rightIndex).type;
                        ownerID = layer.at(rightIndex).ownerID;

                        if (type <= 1 or ownerID == 1) {
                            ownedTiles.rightBlocked = true;
                        }
                    }

                    if (xx > 0) {
                        // LEFT
                        int leftIndex       = (xx - 1) + yy * width;
                        type = TERRAIN::map.at(leftIndex).type;
                        ownerID = layer.at(leftIndex).ownerID;

                        if (type <= 1 or ownerID == 1) {
                            ownedTiles.leftBlocked = true;
                        }
                    }

                    if (yy > 0) {
                        // TOP
                        int topIndex        = xx + (yy - 1) * width;
                        type = TERRAIN::map.at(topIndex).type;
                        ownerID = layer.at(topIndex).ownerID;

                        if (type <= 1 or ownerID == 1) {
                            ownedTiles.topBlocked = true;
                        }
                    }

                    if (yy < height - 1) {
                        // BOTTOM
                        int bottomIndex     = xx + (yy + 1) * width;
                        type = TERRAIN::map.at(bottomIndex).type;
                        ownerID = layer.at(bottomIndex).ownerID;

                        if (type <= 1 or ownerID == 1) {
                            ownedTiles.bottomBlocked = true;
                        }
                    }

                    if (ownedTiles.rightBlocked &&
                        ownedTiles.leftBlocked &&
                        ownedTiles.topBlocked &&
                        ownedTiles.bottomBlocked) {

                        ownedTiles.fullBoxed = true;
                    }

                    ownedTiles.pos.x = tilePos.x;
                    ownedTiles.pos.y = tilePos.y;

                    ownedTiles.worldIndex = tileIndex;

                    player.playerTiles.push_back(ownedTiles);

                    //
                    //
                    //

                    SDL_Renderer* rend = window.rend;

                    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

                    SDL_SetRenderTarget(rend, layerTexture);
                    SDL_SetRenderDrawColor(rend,
                        layer[tileIndex].color.r, layer[tileIndex].color.g,
                        layer[tileIndex].color.b, layer[tileIndex].color.a);
                    SDL_RenderFillRect(rend, &layer[tileIndex].rect);

                    SDL_SetRenderTarget(rend, NULL);
                }
            }
        }
    }
}

// find which tile is currently under the mouse
int findClickIndex() {
    // default size of the tiles
    glm::vec2 defaultSize = {
        (static_cast<float>(window.scale.x) / TERRAIN::mapSize.x),
        (static_cast<float>(window.scale.y) / TERRAIN::mapSize.y)
    };
    //        glm::vec2 currentSize = defaultSize * player.zoom;

    glm::ivec2 mousePos;
    SDL_GetMouseState(&mousePos.x, &mousePos.y);

    int tileIndex;

    // mouse pos in world cords
    glm::vec2 mouseWorldPos = {
        mousePos.x - (player.camOffset.x * player.zoom),
        mousePos.y - (player.camOffset.y * player.zoom)
    };

    // tile size with the zoom
    glm::vec2 tileSize = {
        defaultSize.x * player.zoom,
        defaultSize.y * player.zoom
    };

    // tile pos
    glm::ivec2 tilePos = {
        static_cast<int>(mouseWorldPos.x / tileSize.x),
        static_cast<int>(mouseWorldPos.y / tileSize.y)
    };

    // check if tile is inside the world
    if (tilePos.x >= 0 && tilePos.x < TERRAIN::mapSize.x && tilePos.y >= 0 && tilePos.y < TERRAIN::mapSize.y) {
        tileIndex = tilePos.x + tilePos.y * TERRAIN::mapSize.x; // find out tile index (unused currently)

        return tileIndex;
    } else {
        return -1;
    }
}


//
//  ITTS WORKINGGGGG
//
//  nga
//
//  have to add troops to the system so that you cant expand infinitly
//

// displays toolTip when middle mouse button is clicked
void TERRITORY::toolTip() {
    // tooltip
    glm::ivec2 pos;
    SDL_GetMouseState(&pos.x, &pos.y);

    int index = findClickIndex();
    if (index == -1) return;

    int type = TERRAIN::map[index].type;

    std::string name;
    std::string troopCost;

    switch (type) {
        case 0:
            name = "Deep Sea Tile";
            troopCost = "-";
            break;
        case 1:
            name = "Sea Tile";
            troopCost = "-";
            break;
        case 2:
            name = "Shore Tile";
            troopCost = "100";
            break;
        case 3:
            name = "Grass Tile";
            troopCost = "75";
            break;
        case 4:
            name = "Taiga Tile";
            troopCost = "100";
            break;
        case 5:
            name = "Mountain Tile";
            troopCost = "125";
            break;
        case 6:
            name = "Snow Tile";
            troopCost = "250";
            break;
        default:
            name = "err: unknown";
            break;
    }

    UI::toolTip.toolTipTitle = name;
    UI::toolTip.toolTipText = "Troop cost: " + troopCost + " | Owner: " + std::to_string(layer[index].ownerID);
    UI::toolTip.toolTipRect = {pos.x + 30, pos.y + 30, 0,0};

    UI::toolTip.drawToolTip = true;
}



void TERRITORY::updateLayer() {
    if (mouseClick) {


        // default size of the tiles
        glm::vec2 defaultSize = {
            (static_cast<float>(window.scale.x) / TERRAIN::mapSize.x),
            (static_cast<float>(window.scale.y) / TERRAIN::mapSize.y)
        };
//        glm::vec2 currentSize = defaultSize * player.zoom;

        glm::ivec2 mousePos;
        SDL_GetMouseState(&mousePos.x, &mousePos.y);

        int tileIndex;

        // mouse pos in world cords
        glm::vec2 mouseWorldPos = {
            mousePos.x - (player.camOffset.x * player.zoom),
            mousePos.y - (player.camOffset.y * player.zoom)
        };

        // tile size with the zoom
        glm::vec2 tileSize = {
            defaultSize.x * player.zoom,
            defaultSize.y * player.zoom
        };

        // tile pos
        glm::ivec2 tilePos = {
            static_cast<int>(mouseWorldPos.x / tileSize.x),
            static_cast<int>(mouseWorldPos.y / tileSize.y)
        };


        // |=====================================|
        // | MANAGER FOR TERRITORY RELATED FUNCS |
        // |=====================================|


        // check if tile is inside the world
        if (tilePos.x >= 0 && tilePos.x < TERRAIN::mapSize.x && tilePos.y >= 0 && tilePos.y < TERRAIN::mapSize.y) {
            tileIndex = tilePos.x + tilePos.y * TERRAIN::mapSize.x;

            if (player.playerOwnsAKingdom == false and TERRAIN::map.at(tileIndex).type > 1) {
                spawnTerritory(tilePos, 6);
                player.playerOwnsAKingdom = true;
            } else {
                if (player.troopAmount <= 0) return;
                expandTerritory();
            }
        }

        mouseClick = false;
    }
}

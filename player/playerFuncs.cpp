#include "../header/UIClasses.hpp"
#include "../header/generalClasses.hpp"

#include <random>
#include <vector>

struct FetchNeighbours {
    int topIndex;
    int bottomIndex;
    int leftIndex;
    int rightIndex;
};

struct PendingTiles {
    bool leftBlocked = false;
    bool rightBlocked = false;
    bool topBlocked = false;
    bool bottomBlocked = false;

    int troopCost;
    float timeCost;

    float currentTimer;

    int globalIndex;
};

struct TileData {
    int troopCost;
    float timeCost;
};

std::vector<PendingTiles> pendingTiles;

FetchNeighbours fetchNeighbour(int index) {

    const int width = TERRAIN::mapSize.x;
    const int height = TERRAIN::mapSize.y;

    const int x = index % width;
    const int y = index / width;

    // Init
    FetchNeighbours neg = {-1,-1,-1,-1};

    // Top & Bottom
    if (y > 0) neg.topIndex =  x + (y - 1) * width;
    if (y + 1 < height) neg.bottomIndex =  x + (y + 1) * width;

    // Left & Right
    if (x > 0) neg.leftIndex = (x - 1) + y * width;
    if (x + 1 < width) neg.rightIndex =  (x + 1) + y * width;

    return neg;
}

// fetch troop & time cost
TileData fetchData(int index) {
    switch (TERRAIN::map.at(index).type) {
        // the bigger the number the longer it takes (8 is already slow ash)
        case 2: return {15, 2};        // Shore
        case 3: return {10,  1};      // Grass
        case 4: return {15, 2};        // Taiga
        case 5: return {25, 4};     // Mountain
        case 6: return {40, 8};      // Snow
        default: return {0,0};
    }
}

//
// Yes sir it works
//

void checkTile(auto& tile) {
    if (tile.fullBoxed) return;

    if (tile.topBlocked && tile.bottomBlocked && tile.leftBlocked && tile.rightBlocked) {

        tile.fullBoxed = true;
        int index = tile.worldIndex;
        TERRITORY::layer[index].color = Red;

        SDL_Renderer* rend = window.rend;
        SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(rend, TERRITORY::layerTexture);
        SDL_SetRenderDrawColor(rend,
            TERRITORY::layer[index].color.r,
            TERRITORY::layer[index].color.g,
            TERRITORY::layer[index].color.b,
            TERRITORY::layer[index].color.a);

        SDL_RenderFillRect(rend, &TERRITORY::layer[index].rect);
        SDL_SetRenderTarget(rend, nullptr);
    }
}

// updates the pending tiles (every frame)
void Player::update(float deltaTime) {
    if (pendingTiles.empty()) return;

    SDL_Renderer* rend = window.rend;

    for (int i = 0; i < pendingTiles.size(); i++) {

        if (pendingTiles[i].currentTimer >= pendingTiles[i].timeCost) {

            int index = pendingTiles[i].globalIndex;
            TERRITORY::territoryTile& updatedTile = TERRITORY::layer[index];

            player.troopAmount -= pendingTiles[i].troopCost;

            updatedTile.ownerID = 1;
            updatedTile.color = Red_Border;

            OwnedTiles newTile;

            newTile.worldIndex = index;
            newTile.bottomBlocked = pendingTiles[i].bottomBlocked;
            newTile.rightBlocked = pendingTiles[i].rightBlocked;
            newTile.topBlocked = pendingTiles[i].topBlocked;
            newTile.leftBlocked = pendingTiles[i].leftBlocked;

            playerTiles.push_back(newTile);

            pendingTiles.erase(pendingTiles.begin() + i);
            i--;

            SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
            SDL_SetRenderTarget(rend, TERRITORY::layerTexture);
            SDL_SetRenderDrawColor(rend,
                updatedTile.color.r,
                updatedTile.color.g,
                updatedTile.color.b,
                updatedTile.color.a);

            SDL_RenderFillRect(rend, &updatedTile.rect);
            SDL_SetRenderTarget(rend, nullptr);
        } else {
            static std::random_device device;
            static std::mt19937 generator(device());

            std::uniform_real_distribution dis1(0.1f, 25.0f);
            std::uniform_real_distribution dis2(0.1f, 25.0f);

            pendingTiles[i].currentTimer +=
                (deltaTime * dis2(generator)) / dis1(generator); // kniga
        }
    }
}

bool isAvailable(int index) {
    if (TERRITORY::layer[index].ownerID == 1) return false;

    // if is being already processed
    for (const auto& pending : pendingTiles) {
        if (pending.globalIndex == index) return false;
    }

    return true;
}

// requesting new tiles (once every mouse click)
void TERRITORY::expandTerritory() {
    for (int i = 0; i < player.playerTiles.size(); i++) {
        Player::OwnedTiles& tile = player.playerTiles[i];

        if (tile.fullBoxed) continue;

        int index = tile.worldIndex;
        FetchNeighbours neg = fetchNeighbour(index);

        // top check
        if (!tile.topBlocked) {
            int topIndex = neg.topIndex;

            checkTile(player.playerTiles[i]);

            if (neg.topIndex == -1 or TERRAIN::map[topIndex].type <= 1) {
                tile.topBlocked = true;
                continue;
            }

            if (isAvailable(topIndex)) {
                PendingTiles newTile;

                newTile.bottomBlocked   = true;
                tile.topBlocked        = true;

                newTile.troopCost      = fetchData(tile.worldIndex).troopCost;
                newTile.timeCost       = fetchData(tile.worldIndex).timeCost;
                newTile.globalIndex    = topIndex;

                pendingTiles.push_back(newTile);
            } else {
                tile.topBlocked        = true;
            }
        }

        // bottom check
        if (!tile.bottomBlocked) {
            int bottomIndex = neg.bottomIndex;

            checkTile(player.playerTiles[i]);

            if (neg.bottomIndex == -1 or TERRAIN::map[bottomIndex].type <= 1) {
                tile.bottomBlocked = true;
                continue;
            }

            if (isAvailable(bottomIndex)) {
                PendingTiles newTile;

                tile.bottomBlocked    = true;
                newTile.topBlocked     = true;

                newTile.troopCost      = fetchData(tile.worldIndex).troopCost;
                newTile.timeCost       = fetchData(tile.worldIndex).timeCost;
                newTile.globalIndex    = bottomIndex;

                pendingTiles.push_back(newTile);
            } else {
                tile.bottomBlocked    = true;
            }
        }

        // left check
        if (!tile.leftBlocked) {
            int leftIndex = neg.leftIndex;

            checkTile(player.playerTiles[i]);

            if (neg.leftIndex == -1 or TERRAIN::map[leftIndex].type <= 1) {
                tile.leftBlocked = true;
                continue;
            }

            if (isAvailable(leftIndex)) {
                PendingTiles newTile;

                tile.leftBlocked        = true;
                newTile.rightBlocked   = true;

                newTile.troopCost      = fetchData(tile.worldIndex).troopCost;
                newTile.timeCost       = fetchData(tile.worldIndex).timeCost;
                newTile.globalIndex    = leftIndex;

                pendingTiles.push_back(newTile);
            } else {
                tile.leftBlocked    = true;
            }
        }

        // right check
        if (!tile.rightBlocked) {
            int rightIndex = neg.rightIndex;

            checkTile(player.playerTiles[i]);

            if (neg.rightIndex == -1 or TERRAIN::map[rightIndex].type <= 1) {
                tile.rightBlocked = true;
                continue;
            }

            if (isAvailable(rightIndex)) {
                PendingTiles newTile;

                tile.rightBlocked    = true;
                newTile.leftBlocked     = true;

                newTile.troopCost      = fetchData(tile.worldIndex).troopCost;
                newTile.timeCost       = fetchData(tile.worldIndex).timeCost;
                newTile.globalIndex    = rightIndex;

                pendingTiles.push_back(newTile);
            } else {
                tile.rightBlocked    = true;
            }
        }

        if (tile.topBlocked && tile.bottomBlocked && tile.leftBlocked && tile.rightBlocked) {
            if (tile.fullBoxed) continue;

            tile.fullBoxed = true;
            int index = tile.worldIndex;

            SDL_Renderer* rend = window.rend;

            layer[index].color = Red;

            SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_NONE);
            SDL_SetRenderTarget(rend, layerTexture);
            SDL_SetRenderDrawColor(rend,
                layer[index].color.r,
                layer[index].color.g,
                layer[index].color.b,
                layer[index].color.a);

            SDL_RenderFillRect(rend, &layer[index].rect);
            SDL_SetRenderTarget(rend, nullptr);
        }

        checkTile(tile);
    }
}
//
// Created by tobias on 24.10.26.
//

#include <random>

#include "../../header/debugLogging.hpp"
#include "../../header/generalClasses.hpp"
#include "../../header/generalFuncs.hpp"
#include "../../header/terrainClasses.hpp"

// |===============|
// |    General    |
// |===============|

glm::ivec4 checkNeighbour(int index) {

    // how to calc index:
    // left:   (x - 1) + y * mapSize.x
    // right   (x + 1) + y * mapSize.x
    // top:    x + (y - 1) * mapSize.x
    // bottom  x + (y + 1) * mapSize.x

    // top, bottom, left, right index
    glm::ivec4 neg;

    const int width = TERRAIN::SURFACE::mapSize.x;
    const int height = TERRAIN::SURFACE::mapSize.y;

    // what the flying fuck
    const int x = index % width;
    const int y = index / width;

    neg = {-1, -1, -1, -1};

    // Top & Bottom
    if (y > 0)
        neg.x =  x + (y - 1) * width;

    if (y + 1 < height)
        neg.y =  x + (y + 1) * width;

    // Left & Right
    if (x > 0)
        neg.z = (x - 1) + y * width;

    if (x + 1 < width)
        neg.w =  (x + 1) + y * width;

    return neg;
}

// |===============|
// |    Forests    |
// |===============|

void TERRAIN::FOLIAGE::FOREST::drawTiles(
    int size,                       // size of the base
    glm::ivec2 pos,                 // pos of the center of the base
    SDL_Color normalColor,          // color of the tiles
    std::vector<SDL_Rect>& tiles    // vector to safe tiles to
    ) {

    SDL_Renderer* rend = window.rend;

    for (int y = -size; y < size; y++) {
        for (int x = -size; x < size; x++) {

            float sqrt = std::sqrt(x * x + y * y);
            if (sqrt > size) continue;

            // cords + loop for perlin
            glm::vec2 noiseCords = {
                pos.x + x,
                pos.y + y
            };

            // aplay perlin to radius
            float noise = glm::perlin(noiseCords * 0.06f);
            float radius = size + noise * 8.0f;

            if (sqrt <= radius) {

                glm::ivec2 kniga = noiseCords;

                // safety check
                if (kniga.x < 0 || kniga.x >= SURFACE::mapSize.x ||
                    kniga.y < 0 || kniga.y >= SURFACE::mapSize.y)
                    continue;

                // check terrain type underneath
                int index = kniga.x + kniga.y * SURFACE::mapSize.x;
                if (SURFACE::map.at(index).type <= 2 || SURFACE::map.at(index).type >= 5) continue;

                SDL_Rect drect = {
                    kniga.x * SURFACE::tileSize,
                    kniga.y * SURFACE::tileSize,
                    SURFACE::tileSize,
                    SURFACE::tileSize
                };

                // check if theres already an forest tile
                if (GENERAL::isUsed.at(index)) continue;

                // push back vectors
                GENERAL::isUsed.at(index) = true;
                tiles.push_back(drect);

                //
                //  Draw
                //

                SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
                SDL_SetRenderTarget(rend, GENERAL::worldTex);

                SDL_SetRenderDrawColor(rend, normalColor.r, normalColor.g, normalColor.b, normalColor.a);
                SDL_RenderFillRect(rend, &drect);

                SDL_SetRenderTarget(rend, nullptr);
            }
        }
    }
}

// spawns the trees
void TERRAIN::FOLIAGE::FOREST::spawnTrees(int treesAmount) {

    // init vector for hitboxes
    static std::vector<SDL_Rect> treesHitbox;
    treesHitbox.clear();

    for (int i = 0; i < treesAmount; i++) {

        if (forestTiles.empty()) return;
        int randomIndex = TOOLBOX::randomInt(0, forestTiles.size() - 1);

        // get width, height
        int width, height;
        SDL_QueryTexture(GENERAL::textures.oak, nullptr, nullptr, &width, &height);

        // size, hitbox and collision detection
        SDL_Rect temp = {
            forestTiles.at(randomIndex).x + TOOLBOX::randomInt(-1, 1),
            forestTiles.at(randomIndex).y + TOOLBOX::randomInt(-1, 1),
            width / 4,
            height / 4
        };

        SDL_Rect tempHitbox = {
            temp.x - 5, temp.y - 5, temp.w + 10, temp.h + 10
        };

        bool skip = false;
        for (auto& kk : treesHitbox) {
            if (TOOLBOX::checkHitboxCollision(tempHitbox, kk)) {
                skip = true;
            }
        }
        if (skip) continue;

        // set renderer
        SDL_Renderer* rend = window.rend;
        SDL_SetRenderTarget(rend, GENERAL::worldTex);

        // switch for random tree type
        if (TOOLBOX::randomBool(0.5)) SDL_RenderCopy(rend, GENERAL::textures.oak, nullptr, &temp);
        else SDL_RenderCopy(rend, GENERAL::textures.spruce, nullptr, &temp);

        SDL_SetRenderTarget(rend, nullptr);

        // new vector entry
        treesHitbox.push_back(tempHitbox);
    }
}

void TERRAIN::FOLIAGE::FOREST::generate() {

    LOG::INFO("Starting Forest Generation");

    const int spawnRate = 50;   // FOREST spawn rate

    // initializing world tex if not already happen
    if (GENERAL::worldTex == nullptr) {
        LOG::INFO("Initialising World Tex");

        GENERAL::worldTex = SDL_CreateTexture(
            window.rend,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            SURFACE::mapSize.x * SURFACE::tileSize,
            SURFACE::mapSize.y * SURFACE::tileSize);

        if (GENERAL::worldTex == nullptr) {
            std::string err = SDL_GetError();
            if (err.empty()) err = "Couldnt get error";

            LOG::INIT_ERR("Couldnt init world Tex: " + err);
            return;
        }

        // clear tex
        SDL_SetTextureBlendMode(GENERAL::worldTex, SDL_BLENDMODE_BLEND);

        SDL_SetRenderTarget(window.rend, GENERAL::worldTex);
        SDL_SetRenderDrawColor(window.rend, 0, 0, 0, 0);
        SDL_RenderClear(window.rend);

        SDL_SetRenderTarget(window.rend, nullptr);
    }

    // clear all vectors
    forestTiles.clear();
    COTTON::cottonTiles.clear();
    GENERAL::isUsed.assign(SURFACE::mapSize.x * SURFACE::mapSize.y, false);

    // amount of forests
    for (int i = 0; i < spawnRate; i++) {

        // random position
        int x = static_cast<int>(TOOLBOX::randomFloat(0.0f, static_cast<float>(SURFACE::mapSize.x)));
        int y = static_cast<int>(TOOLBOX::randomFloat(0.0f, static_cast<float>(SURFACE::mapSize.y)));

        // draws floor
        drawTiles(20, glm::ivec2(x, y), (SDL_Color){ 0, 125, 0, 155 }, forestTiles);
    }

    LOG::INFO("Spawned Forests");
}

// |=====================|
// |    Cotton Fields    |
// |=====================|

void TERRAIN::FOLIAGE::COTTON::drawTiles(
    int size,                       // size of the base
    glm::ivec2 pos,                 // pos of the center of the base
    SDL_Color normalColor,          // color of the tiles
    std::vector<SDL_Rect>& tiles    // vector to safe tiles to
    ) {

    SDL_Renderer* rend = window.rend;

    for (int y = -size; y < size; y++) {
        for (int x = -size; x < size; x++) {

            float sqrt = std::sqrt(x * x + y * y);
            if (sqrt > size) continue;

            // cords + loop for perlin
            glm::vec2 noiseCords = {
                pos.x + x,
                pos.y + y
            };

            // aplay perlin to radius
            float noise = glm::perlin(noiseCords * 0.06f);
            float radius = size + noise * 8.0f;

            if (sqrt <= radius) {

                glm::ivec2 kniga = noiseCords;

                // safety check
                if (kniga.x < 0 || kniga.x >= SURFACE::mapSize.x ||
                    kniga.y < 0 || kniga.y >= SURFACE::mapSize.y)
                    continue;

                // check terrain type underneath
                int index = kniga.x + kniga.y * SURFACE::mapSize.x;
                if (SURFACE::map.at(index).type <= 2 || SURFACE::map.at(index).type >= 5) continue;

                SDL_Rect drect = {
                    kniga.x * SURFACE::tileSize,
                    kniga.y * SURFACE::tileSize,
                    SURFACE::tileSize,
                    SURFACE::tileSize
                };

                // check if theres already an forest tile
                if (GENERAL::isUsed.at(index)) continue;

                // push back vectors
                GENERAL::isUsed.at(index) = true;
                tiles.push_back(drect);

                //
                //  Draw
                //

                SDL_SetRenderTarget(rend, GENERAL::worldTex);

                SDL_SetRenderDrawColor(rend, normalColor.r, normalColor.g, normalColor.b, normalColor.a);
                SDL_RenderFillRect(rend, &drect);

                SDL_SetRenderTarget(rend, nullptr);
            }
        }
    }
}

void TERRAIN::FOLIAGE::COTTON::spawnCotton(int treesAmount) {

    // init vector for hitboxes
    static std::vector<SDL_Rect> treesHitbox;
    treesHitbox.clear();

    for (int i = 0; i < treesAmount; i++) {

        if (cottonTiles.empty()) return;
        int randomIndex = TOOLBOX::randomInt(0, cottonTiles.size() - 1);

        // get width, height
        int width, height;
        SDL_QueryTexture(GENERAL::textures.oak, nullptr, nullptr, &width, &height);

        // size, hitbox and collision detection
        SDL_Rect temp = {
            cottonTiles.at(randomIndex).x + TOOLBOX::randomInt(-1, 1),
            cottonTiles.at(randomIndex).y + TOOLBOX::randomInt(-1, 1),
            width / 4,
            height / 4
        };

        SDL_Rect tempHitbox = {
            temp.x - 5, temp.y - 5, temp.w + 10, temp.h + 10
        };

        bool skip = false;
        for (auto& kk : treesHitbox) {
            if (TOOLBOX::checkHitboxCollision(tempHitbox, kk)) {
                skip = true;
            }
        }
        if (skip) continue;

        // set renderer
        SDL_Renderer* rend = window.rend;
        SDL_SetRenderTarget(rend, GENERAL::worldTex);

        // switch for random tree type
        SDL_RenderCopy(rend, GENERAL::textures.cotton, nullptr, &temp);

        SDL_SetRenderTarget(rend, nullptr);

        // new vector entry
        treesHitbox.push_back(tempHitbox);
    }
}

void TERRAIN::FOLIAGE::COTTON::generate() {

    LOG::INFO("Starting cotton Generation");

    SDL_Renderer* rend = window.rend;

    // initializing world tex if not already happen
    if (GENERAL::worldTex == nullptr) {
        LOG::INFO("Initialising World Tex");

        GENERAL::worldTex = SDL_CreateTexture(
            rend,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            SURFACE::mapSize.x * SURFACE::tileSize,
            SURFACE::mapSize.y * SURFACE::tileSize);

        if (GENERAL::worldTex == nullptr) {
            std::string err = SDL_GetError();
            if (err.empty()) err = "Couldnt get error";

            LOG::INIT_ERR("Couldnt init world Tex: " + err);
            return;
        }

        // clear tex
        SDL_SetTextureBlendMode(GENERAL::worldTex, SDL_BLENDMODE_BLEND);

        SDL_SetRenderTarget(window.rend, GENERAL::worldTex);

        SDL_SetRenderDrawColor(window.rend, 0, 0, 0, 0);
        SDL_RenderClear(window.rend);

        SDL_SetRenderTarget(window.rend, nullptr);
    }

    for (int i = 0; i < spawnRate; i++) {

        // random position
        int x = static_cast<int>(TOOLBOX::randomFloat(0.0f, static_cast<float>(SURFACE::mapSize.x)));
        int y = static_cast<int>(TOOLBOX::randomFloat(0.0f, static_cast<float>(SURFACE::mapSize.y)));

        // draws floor
        drawTiles(20, glm::ivec2(x, y), (SDL_Color){ 0, 125, 0, 155 }, cottonTiles);
    }

    LOG::INFO("Done");
}

void TERRAIN::FOLIAGE::GENERAL::drawIcons() {
    LOG::INFO("Starting loading icons");

    // spawn icons
    COTTON::spawnCotton(TOOLBOX::randomInt(60, 100) * COTTON::spawnRate);
    FOREST::spawnTrees(TOOLBOX::randomInt(60, 100) * FOREST::spawnRate);

    LOG::INFO("Done");
}
//
// Created by tobias on 24.10.26.
//

#include <random>

#include "../../header/debugLogging.hpp"
#include "../../header/generalClasses.hpp"
#include "../../header/generalFuncs.hpp"
#include "../../header/specializedFieldClasses.hpp"

// get texture size
glm::ivec2 getTexSize(int num) {
    glm::ivec2 size;

    switch (num) {
        case 1: SDL_QueryTexture(FOREST::textures.lightOak,    0, 0, &size.x, &size.y);    break;
        case 2: SDL_QueryTexture(FOREST::textures.lightSpruce, 0, 0, &size.x, &size.y);    break;
        case 3: SDL_QueryTexture(FOREST::textures.denseOak,    0, 0, &size.x, &size.y);    break;
        case 4: SDL_QueryTexture(FOREST::textures.denseSpruce, 0, 0, &size.x, &size.y);    break;
        default: return {0,0};
    }

    return size;
}

// spawns the trees
void FOREST::spawnTrees(int treesAmount) {

    // init vector for hitboxes
    static std::vector<SDL_Rect> treesHitbox;
    treesHitbox.clear();

    for (int i = 0; i < treesAmount; i++) {

        if (innerCircle.empty()) return;
        int randomIndex = TOOLBOX::randomInt(0, innerCircle.size() - 1);

        // get width, height
        int width, height;
        SDL_QueryTexture(textures.lightOak, nullptr, nullptr, &width, &height);

        // size, hitbox and collision detection
        SDL_Rect temp = {
            innerCircle.at(randomIndex).x + TOOLBOX::randomInt(-1, 1),
            innerCircle.at(randomIndex).y + TOOLBOX::randomInt(-1, 1),
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
        SDL_SetRenderTarget(rend, worldTex);

        // debug hitbox
        SDL_SetRenderDrawColor(rend, 125, 125, 125, 155);
        SDL_RenderFillRect(rend, &tempHitbox);

        // switch for random tree type
        if (TOOLBOX::randomBool(0.5)) SDL_RenderCopy(rend, textures.lightOak, nullptr, &temp);
        else SDL_RenderCopy(rend, textures.denseSpruce, nullptr, &temp);

        SDL_SetRenderTarget(rend, nullptr);

        // new vector entry
        treesHitbox.push_back(tempHitbox);
    }

    for (int i = 0; i < treesAmount / 2; i++) {

        if (outerCircle.empty()) return;

        int randomIndex = TOOLBOX::randomInt(0, outerCircle.size() - 1);

        int width, height;
        SDL_QueryTexture(textures.lightOak, nullptr, nullptr, &width, &height);

        // size, hitbox and collision detection
        SDL_Rect temp = {
            outerCircle.at(randomIndex).x + TOOLBOX::randomInt(-1, 1),
            outerCircle.at(randomIndex).y + TOOLBOX::randomInt(-1, 1),
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
        SDL_SetRenderTarget(rend, worldTex);

        // debug hitbox
        SDL_SetRenderDrawColor(rend, 125, 125, 125, 155);
        SDL_RenderFillRect(rend, &tempHitbox);

        // switch for random tree type
        if (TOOLBOX::randomBool(0.5)) SDL_RenderCopy(rend, textures.lightOak, nullptr, &temp);
        else SDL_RenderCopy(rend, textures.denseSpruce, nullptr, &temp);

        SDL_SetRenderTarget(rend, nullptr);

        // new vector entry
        treesHitbox.push_back(tempHitbox);
    }
}

// draws the forest zone
void FOREST::drawCircle(int size, glm::ivec2 pos) {

    int outerCircleSize = size * 2;

    SDL_Color floorColor = {0,115,0,155};

    // outer circle
    for (int y = -outerCircleSize; y <= outerCircleSize; y++) {
        for (int x = -outerCircleSize; x <= outerCircleSize; x++) {

            int distance2 = (x * x) + (y * y);

            if (distance2 <= (outerCircleSize * outerCircleSize)) {
                glm::ivec2 tilePos = { pos.x + x, pos.y + y };
                int tileIndex = tilePos.x + tilePos.y * TERRAIN::mapSize.x;

                if (tilePos.x < 0 || tilePos.x >= TERRAIN::mapSize.x || tilePos.y < 0 || tilePos.y >= TERRAIN::mapSize.y) {
                    continue;
                }

                if (TERRAIN::map.at(tileIndex).type <= 2) continue;

                SDL_Rect drect = {
                    tilePos.x * TERRAIN::tileSize,
                    tilePos.y * TERRAIN::tileSize,
                    TERRAIN::tileSize,
                    TERRAIN::tileSize
                };

                SDL_Renderer* rend = window.rend;

                SDL_SetTextureBlendMode(worldTex, SDL_BLENDMODE_BLEND);

                SDL_SetRenderTarget(rend, worldTex);

                SDL_SetRenderDrawColor(rend, floorColor.r, floorColor.g, floorColor.b, floorColor.a);
                SDL_RenderFillRect(rend, &drect);

                SDL_SetRenderTarget(rend, nullptr);

                outerCircle.push_back(drect);
            }
        }
    }

    // inner circle on top of the outer
    for (int y = -size; y <= size; y++) {
        for (int x = -size; x <= size; x++) {

            int distance2 = (x * x) + (y * y);

            if (distance2 <= (size * size)) {
                glm::ivec2 tilePos = { pos.x + x, pos.y + y };
                int tileIndex = tilePos.x + tilePos.y * TERRAIN::mapSize.x;

                if (tilePos.x < 0 || tilePos.x >= TERRAIN::mapSize.x || tilePos.y < 0 || tilePos.y >= TERRAIN::mapSize.y) {
                    continue;
                }

                if (TERRAIN::map.at(tileIndex).type <= 2) continue;

                SDL_Rect drect = {
                    tilePos.x * TERRAIN::tileSize,
                    tilePos.y * TERRAIN::tileSize,
                    TERRAIN::tileSize,
                    TERRAIN::tileSize
                };

                SDL_Renderer* rend = window.rend;

                SDL_SetTextureBlendMode(worldTex, SDL_BLENDMODE_BLEND);

                SDL_SetRenderTarget(rend, worldTex);

                SDL_SetRenderDrawColor(rend, floorColor.r, floorColor.g, floorColor.b, floorColor.a);
                SDL_RenderFillRect(rend, &drect);

                SDL_SetRenderTarget(rend, nullptr);

                innerCircle.push_back(drect);
            }
        }
    }
}

void FOREST::generate() {

    LOG::INFO("Starting Forest Generation");

    const int spawnRate = 50;

    worldTex = SDL_CreateTexture(
        window.rend,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        TERRAIN::mapSize.x * TERRAIN::tileSize,
        TERRAIN::mapSize.y * TERRAIN::tileSize);

    // error detection
    if (worldTex == nullptr) {
        std::string err = SDL_GetError();
        if (err.empty()) err = "Couldnt get error";

        LOG::INIT_ERR("Couldnt init world Tex: " + err);
        return;
    }

    SDL_SetTextureBlendMode(worldTex, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(window.rend, worldTex);
    SDL_SetRenderDrawColor(window.rend, 0, 0, 0, 0);
    SDL_RenderClear(window.rend);
    SDL_SetRenderTarget(window.rend, nullptr);

    // amount of forests
    for (int i = 0; i < spawnRate; i++) {

        int x = static_cast<int>(TOOLBOX::randomFloat(0.0f, static_cast<float>(TERRAIN::mapSize.x)));
        int y = static_cast<int>(TOOLBOX::randomFloat(0.0f, static_cast<float>(TERRAIN::mapSize.y)));

        innerCircle.clear();
        outerCircle.clear();

        // draws floor
        drawCircle(10, glm::ivec2(x, y));

        // spawns trees
        spawnTrees(80);
    }
}
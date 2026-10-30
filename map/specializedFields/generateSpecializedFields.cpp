//
// Created by tobias on 24.10.26.
//

#include <random>

#include "../../header/debugLogging.hpp"
#include "../../header/generalClasses.hpp"
#include "../../header/generalFuncs.hpp"
#include "../../header/specializedFieldClasses.hpp"

// spawns the trees
void FOREST::spawnTrees(int treesAmount) {

    // init vector for hitboxes
    static std::vector<SDL_Rect> treesHitbox;
    treesHitbox.clear();

    for (int i = 0; i < treesAmount; i++) {

        if (forestTiles.empty()) return;
        int randomIndex = TOOLBOX::randomInt(0, forestTiles.size() - 1);

        // get width, height
        int width, height;
        SDL_QueryTexture(textures.oak, nullptr, nullptr, &width, &height);

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
        SDL_SetRenderTarget(rend, worldTex);

        // switch for random tree type
        if (TOOLBOX::randomBool(0.5)) SDL_RenderCopy(rend, textures.oak, nullptr, &temp);
        else SDL_RenderCopy(rend, textures.spruce, nullptr, &temp);

        SDL_SetRenderTarget(rend, nullptr);

        // new vector entry
        treesHitbox.push_back(tempHitbox);
    }
}

void FOREST::drawForest(int size, glm::ivec2 pos) {

    const SDL_Color normalColor = {0, 135, 0, 155};
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
                if (kniga.x < 0 || kniga.x >= TERRAIN::mapSize.x ||
                    kniga.y < 0 || kniga.y >= TERRAIN::mapSize.y)
                    continue;

                // check terrain type underneath
                int index = kniga.x + kniga.y * TERRAIN::mapSize.x;
                if (TERRAIN::map.at(index).type <= 2 || TERRAIN::map.at(index).type >= 5) continue;

                SDL_Rect drect = {
                    kniga.x * TERRAIN::tileSize,
                    kniga.y * TERRAIN::tileSize,
                    TERRAIN::tileSize,
                    TERRAIN::tileSize
                };

                // check if theres already an forest tile
                bool skip = false;
                for (auto& kk : forestTiles) {
                    if (TOOLBOX::checkHitboxCollision(drect, kk)) {
                        skip = true;
                        break;
                    }
                }
                if (skip) continue;

                //
                //  Draw
                //

                SDL_SetTextureBlendMode(worldTex, SDL_BLENDMODE_BLEND);
                SDL_SetRenderTarget(rend, worldTex);

                SDL_SetRenderDrawColor(rend, normalColor.r, normalColor.g, normalColor.b, normalColor.a);
                SDL_RenderFillRect(rend, &drect);

                SDL_SetRenderDrawColor(rend,
                    normalColor.r,
                    normalColor.g - 50,
                    normalColor.b,
                    normalColor.a);

                int border = 3;
                for (int a = 0; a < border; a++) {
                    SDL_Rect drect = {
                        drect.x + a,
                        drect.y + a,
                        drect.w - (a * 2),
                        drect.h - (a* 2),
                    };

                    SDL_RenderDrawRect(rend, &drect);
                }

                forestTiles.push_back(drect);
            }
        }
    }
}

void FOREST::generate() {

    LOG::INFO("Starting Forest Generation");

    const int spawnRate = 50;   // FOREST spawn rate

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

        // random position
        int x = static_cast<int>(TOOLBOX::randomFloat(0.0f, static_cast<float>(TERRAIN::mapSize.x)));
        int y = static_cast<int>(TOOLBOX::randomFloat(0.0f, static_cast<float>(TERRAIN::mapSize.y)));

        // draws floor
        drawForest(20, glm::ivec2(x, y));
    }

    // spawns trees
    spawnTrees(TOOLBOX::randomInt(60, 100) * spawnRate);

    LOG::INFO("Spawned Forests");
}
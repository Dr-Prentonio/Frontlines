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
        case 1: SDL_QueryTexture(FORREST::textures.lightOak,    0, 0, &size.x, &size.y);    break;
        case 2: SDL_QueryTexture(FORREST::textures.lightSpruce, 0, 0, &size.x, &size.y);    break;
        case 3: SDL_QueryTexture(FORREST::textures.denseOak,    0, 0, &size.x, &size.y);    break;
        case 4: SDL_QueryTexture(FORREST::textures.denseSpruce, 0, 0, &size.x, &size.y);    break;
        default: return {0,0};
    }

    return size;
}

void FIELD_GENERATION::manager() {
    LOG::INFO("Generating Forests");

    const float perlinNoiseScale = 0.02f;
    const glm::ivec2 mapSize = TERRAIN::mapSize;

    LOG::DEBUG("Mapsize x: " + std::to_string(mapSize.x) + " y: " + std::to_string(mapSize.y));

    float seed = TOOLBOX::randomFloat(-1.0f, 1.0f);
    float seed2 = TOOLBOX::randomFloat(-1.0f, 1.0f);

    std::vector<float> heightMap(mapSize.x * mapSize.y);

    // make new heightmap
    for (int y = 0; y < mapSize.y; y++) {
        for (int x = 0; x < mapSize.x; x++) {

            float noiseValue = 0;
            float noiseValue2 = 0;

            noiseValue = glm::perlin(
                glm::vec2(
                    x * perlinNoiseScale * seed,
                    y * perlinNoiseScale * seed));

            noiseValue2 = glm::perlin(
                glm::vec2(
                    x * perlinNoiseScale * seed2,
                    y * perlinNoiseScale * seed2));

            float finalNoise = noiseValue * noiseValue2;

            heightMap[x + y * mapSize.x] = finalNoise;
        }
    }

    /*
    // choose tree;
    for (int y = 0; y < mapSize.y ; y++) {
        for (int x = 0; x < mapSize.x ; x++) {

            FORREST::TemplateStruct tree;

            int index = x + y * mapSize.x;
            float terrainNoise = heightMap[index];

            // if tree is inside water or sand -> skip
            if (TERRAIN::map.at(index).type > 2) {
                if (terrainNoise > -0.1) tree.type = FORREST::LIGHT_OAK;
                else if (terrainNoise > 0.3) tree.type = FORREST::DENSE_OAK;
            }
            else continue;

            float spawnChance = TOOLBOX::randomFloat(0.0f, 1.0f);

            // if spawnChance ist highr than 0.99 then -> skip
            if (spawnChance < 0.99f) continue;

            glm::ivec2 size = getTexSize(tree.type);

            SDL_Rect tempRect = {
                x * TERRAIN::tileSize,
                y * TERRAIN::tileSize,
                size.x / 4,
                size.y / 4
            };

            tempRect.x += static_cast<int>(glm::round(TOOLBOX::randomFloat(-5.0f, 5.0f)));
            tempRect.y += static_cast<int>(glm::round(TOOLBOX::randomFloat(-5.0f, 5.0f)));

            SDL_Rect tempHitbox = {
                tempRect.x - 10,
                tempRect.y - 10,
                tempRect.w + 20,
                tempRect.h + 20
            };

            // AABB Ansatz
            bool collision = false;
            for (auto& trees : FORREST::trees) {
                if (TOOLBOX::checkHitboxCollision(tempHitbox, trees.hitbox)) {
                    collision = true;
                    break;
                }
            }
            if (collision) continue;

            // assign temp var to vector
            tree.rect = tempRect;
            tree.hitbox = tempHitbox;

            FORREST::trees.push_back(tree);
        }
    }

    LOG::INFO("Generated Trees: " + std::to_string(FORREST::trees.size()));

*/

    for (int y = 0; y < mapSize.y; y++) {
        for (int x = 0; x < mapSize.x; x++) {

            int index = y + x * mapSize.x;

            if (TERRAIN::map.at(index).type <= 2) continue;

            int spawnChance = static_cast<int>(glm::round(TOOLBOX::randomFloat(0.0f, 100.0f)));

            if (spawnChance < 90) continue;

            SDL_Rect newRect = {
                x * TERRAIN::tileSize,
                y * TERRAIN::tileSize,
                TERRAIN::tileSize,
                TERRAIN::tileSize,
            };

            FORREST::forestTiles.push_back(newRect);
        }
    }
}
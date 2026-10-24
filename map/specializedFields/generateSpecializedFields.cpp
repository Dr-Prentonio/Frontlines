//
// Created by tobias on 24.10.26.
//

#include <random>

#include "../../header/debugLogging.hpp"
#include "../../header/generalClasses.hpp"
#include "../../header/specializedFieldClasses.hpp"

// translate number to enum
FORREST::Types findType(int num) {
    switch (num) {
        case 1: return FORREST::LIGHT_OAK;
        case 2: return FORREST::LIGHT_SPRUCE;
        case 3: return FORREST::DENSE_OAK;
        case 4: return FORREST::DENSE_SPRUCE;
        default: return FORREST::ERR;
    }
}

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

    // random gen
    std::random_device device;
    std::mt19937 gen(device());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    std::uniform_real_distribution<float> dis2(0.0f, 2.0f);

    float seed = dis(gen);

    std::vector<float> heightMap(mapSize.x * mapSize.y);

    // make new heightmap
    for (int y = 0; y < mapSize.y; y++) {
        for (int x = 0; x < mapSize.x; x++) {

            float noiseValue;
            noiseValue = glm::perlin(
                glm::vec2(
                    x * perlinNoiseScale * seed,
                    y * perlinNoiseScale * seed));

            heightMap[x + y * mapSize.x] = noiseValue;
        }
    }

    // choose tree;
    for (int y = 0; y < mapSize.y; y++) {
        for (int x = 0; x < mapSize.x; x++) {

            FORREST::TemplateStruct tree;

            int index = x + y * mapSize.x;
            float terrainNoise = heightMap[index];

            int randomType = static_cast<int>(glm::round(dis2(gen)));

            // if its any sea or shore tile it skips
            if (TERRAIN::map[index].type > 2) {

                // get type
                if (terrainNoise < 0)       tree.type = findType(randomType);
                else if (terrainNoise > 0)  tree.type = findType(randomType * 2);
            } else {
                continue;
            }

            glm::ivec2 size = getTexSize(randomType);

            tree.rect =  { x, y,  };

            FORREST::trees.push_back(tree);
        }
    }
}
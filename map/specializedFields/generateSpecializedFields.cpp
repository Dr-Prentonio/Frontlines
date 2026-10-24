//
// Created by tobias on 24.10.26.
//

#include <random>

#include "../../header/debugLogging.hpp"
#include "../../header/generalClasses.hpp"
#include "../../header/specializedFieldClasses.hpp"

auto getTreeType(int type) {
    switch (type) {
        case 1:
            return;
    }
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

    float seed = dis(gen);

    std::vector<float> heightMap(mapSize.x * mapSize.y);

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

    for (int y = 0; y < mapSize.y; y++) {
        for (int x = 0; x < mapSize.x; x++) {

            FORREST::TemplateStruct tree;

            float terrainNoise = heightMap[x + y * mapSize.x];
            int type = static_cast<int>(glm::round(dis(gen)));

            // > -1.15  => grass; < 0.7 => taiga
            if (terrainNoise < 0.4f) {
                tree.type =
            }
            else if (terrainNoise < 0.7) terrainTile.type = TAIGA;

            FORREST::trees.push_back(tree);
        }
    }
}

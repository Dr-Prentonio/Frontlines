#include <iomanip>

#include "../header/generalClasses.hpp"
#include "../header/debugLogging.hpp"
#include <random>

#include "../header/generalFuncs.hpp"
#include "../header/specializedFieldClasses.hpp"

float interpolatedHeight(glm::vec2 pos, glm::ivec2 size, const std::vector<float>& heightMap) {
    // pos is pos
    // size.x = width; size.y = height

    // idfk
    glm::ivec2 ploh1 = {
        static_cast<int>(pos.x),
        static_cast<int>(pos.y)
    };

    glm::ivec2 ploh2 = {
        ploh1.x + 1,
        ploh1.y + 1
    };

    // if droplet is at edge of map
    if (ploh1.x < 0 || ploh2.x >= size.x || ploh1.y < 0 || ploh2.y >= size.y) return 0.0f;

    glm::vec2 prz = {
        pos.x - ploh1.x,    // prz from +x to -x tile
        pos.y - ploh1.y     // prz from +y to -y tile
    };

    // top left and right + bottom left and right tiles
    glm::vec4 tiles = {
        heightMap[ploh1.x + ploh1.y * size.x],
        heightMap[ploh2.x + ploh1.y * size.x],
        heightMap[ploh1.x + ploh2.y * size.x],
        heightMap[ploh2.x + ploh2.y * size.x]
    };

    // mix theses togehter
    float top = glm::mix(tiles.x, tiles.y, prz.x);
    float bottom = glm::mix(tiles.z, tiles.w, prz.x);

    return glm::mix(top, bottom, prz.y);
}

void interpolatedSediment(glm::vec2 pos, glm::ivec2 mapSize, std::vector<float>& heightMap, float amount) {
    // pos is pos
    // size.x = width; size.y = height

    // idfk
    glm::ivec2 ploh1 = {
        static_cast<int>(pos.x),
        static_cast<int>(pos.y)
    };

    glm::ivec2 ploh2 = {
        ploh1.x + 1,
        ploh1.y + 1
    };

    glm::vec2 prz = {
        pos.x - ploh1.x,    // prz from +x to -x tile
        pos.y - ploh1.y     // prz from +y to -y tile
    };

    heightMap[ploh1.y * mapSize.x + ploh1.x] += amount * (1.0f - prz.x) * (1.0f - prz.y); // top left
    heightMap[ploh1.y * mapSize.x + ploh2.x] += amount * prz.x * (1.0f - prz.y);
    heightMap[ploh2.y * mapSize.x + ploh1.x] += amount * (1.0f - prz.x) * prz.y;
    heightMap[ploh2.y * mapSize.x + ploh2.x] += amount * prz.x * prz.y;
}

void erosion(glm::ivec2 mapSize, std::vector<float>& heightMap, int amount = 30000) {
    std::vector<TERRAIN::TerrainTile> map = TERRAIN::map;

    for (int i = 0; i < amount; i++) {

        TERRAIN::Droplet drop;
        drop.pos.x = rand() % mapSize.x;
        drop.pos.y = rand() % mapSize.y;

        drop.amount = 1.0f;
        glm::vec2 dropSpeed = {0,0};

        while (drop.amount > 0.01f) {
            float height = interpolatedHeight(drop.pos, mapSize, heightMap);

            // reset var
            const float inertia = 0.2f;
            const float gravity = 4.0f;

            float offset = 0.5f;
            float heightLeft   = interpolatedHeight(glm::vec2(drop.pos.x - offset, drop.pos.y), mapSize, heightMap);
            float heightRight  = interpolatedHeight(glm::vec2(drop.pos.x + offset, drop.pos.y), mapSize, heightMap);
            float heightTop    = interpolatedHeight(glm::vec2(drop.pos.x, drop.pos.y - offset), mapSize, heightMap);
            float heightBottom = interpolatedHeight(glm::vec2(drop.pos.x, drop.pos.y + offset), mapSize, heightMap);

            glm::vec2 gradient = { heightRight - heightLeft, heightBottom - heightTop };

            dropSpeed = dropSpeed * inertia - gradient * (1.0f - inertia);
            drop.pos += dropSpeed;

            // safety checks
            if (drop.pos.x < 0 || drop.pos.x >= mapSize.x - 1 || drop.pos.y < 0 || drop.pos.y >= mapSize.y - 1) break;

            float newHeight = interpolatedHeight(drop.pos, mapSize, heightMap);
            float deltaHeight = newHeight - height;

            float sedimentDelta = deltaHeight * 0.5f;
            drop.sediment += sedimentDelta;

            interpolatedSediment(drop.pos, mapSize, heightMap, sedimentDelta);
            drop.amount -= 0.01f;
        }
    }
}

void TERRAIN::generateTile(SDL_Renderer* rend) {

    for (int i = 0; i < map.size(); i++) {

        if (map[i].type == SEA) {

            SDL_SetRenderDrawColor(rend, 0, 0, 200, 255);
            SDL_RenderFillRect(rend, &map[i].rect);

            SDL_SetRenderDrawColor(rend, 0, 0, 185, 255);

            int border = 3;
            for (int a = 0; a < border; a++) {
                SDL_Rect drect = {
                    map[i].rect.x + a,
                    map[i].rect.y + a,
                    map[i].rect.w - (a * 2),
                    map[i].rect.h - (a* 2),
                };

                SDL_RenderDrawRect(rend, &drect);
            }
        }
        else if (map[i].type == DEEP_SEA) {

            SDL_SetRenderDrawColor(rend, 0, 0, 150, 255);
            SDL_RenderFillRect(rend, &map[i].rect);

            SDL_SetRenderDrawColor(rend, 0, 0, 135, 255);

            int border = 3;
            for (int a = 0; a < border; a++) {
                SDL_Rect drect = {
                    map[i].rect.x + a,
                    map[i].rect.y + a,
                    map[i].rect.w - (a * 2),
                    map[i].rect.h - (a * 2),
                };

                SDL_RenderDrawRect(rend, &drect);
            }
        }

        else if (map[i].type == SHORE) {

            SDL_SetRenderDrawColor(rend, 250, 225, 100, 255);
            SDL_RenderFillRect(rend, &map[i].rect);

            SDL_SetRenderDrawColor(rend, 235, 210, 85, 255);

            int border = 3;
            for (int a = 0; a < border; a++) {
                SDL_Rect drect = {
                    map[i].rect.x + a,
                    map[i].rect.y + a,
                    map[i].rect.w - (a * 2),
                    map[i].rect.h - (a* 2),
                };

                SDL_RenderDrawRect(rend, &drect);
            }
        }
        else if (map[i].type == GRASS) {

            SDL_SetRenderDrawColor(rend, 0, 200, 0, 255);
            SDL_RenderFillRect(rend, &map[i].rect);

            SDL_SetRenderDrawColor(rend, 0, 185, 0, 255);

            int border = 3;
            for (int a = 0; a < border; a++) {
                SDL_Rect drect = {
                    map[i].rect.x + a,
                    map[i].rect.y + a,
                    map[i].rect.w - (a * 2),
                    map[i].rect.h - (a* 2),
                };

                SDL_RenderDrawRect(rend, &drect);
            }
        }
        else if (map[i].type == TAIGA) {

            SDL_SetRenderDrawColor(rend, 0, 150, 0, 255);
            SDL_RenderFillRect(rend, &map[i].rect);

            SDL_SetRenderDrawColor(rend, 0, 135, 0, 255);

            int border = 3;
            for (int a = 0; a < border; a++) {
                SDL_Rect drect = {
                    map[i].rect.x + a,
                    map[i].rect.y + a,
                    map[i].rect.w - (a * 2),
                    map[i].rect.h - (a * 2),
                };

                SDL_RenderDrawRect(rend, &drect);
            }
        }

        else if (map[i].type == MOUNTAIN) {

            SDL_SetRenderDrawColor(rend, 150, 150, 150, 255);
            SDL_RenderFillRect(rend, &map[i].rect);

            SDL_SetRenderDrawColor(rend, 135, 135, 135, 255);

            int border = 3;
            for (int a = 0; a < border; a++) {
                SDL_Rect drect = {
                    map[i].rect.x + a,
                    map[i].rect.y + a,
                    map[i].rect.w - (a * 2),
                    map[i].rect.h - (a* 2),
                };

                SDL_RenderDrawRect(rend, &drect);
            }
        }
        else if (map[i].type == SNOW) {

            SDL_SetRenderDrawColor(rend, 230, 230, 230, 255);
            SDL_RenderFillRect(rend, &map[i].rect);

            SDL_SetRenderDrawColor(rend, 215, 215, 215, 255);

            int border = 3;
            for (int a = 0; a < border; a++) {
                SDL_Rect drect = {
                    map[i].rect.x + a,
                    map[i].rect.y + a,
                    map[i].rect.w - (a * 2),
                    map[i].rect.h - (a* 2),
                };

                SDL_RenderDrawRect(rend, &drect);
            }
        }

    }

}

void TERRAIN::generateLoDs() {

    SDL_Texture* staticTex;
    SDL_Renderer* rend = window.rend;

    staticTex = SDL_CreateTexture(
        rend,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_TARGET,
        mapSize.x * tileSize,
        mapSize.y * tileSize);

    if (staticTex == nullptr) {
        std::string err = SDL_GetError();
        LOG::ERR("Critical Error " + err);
        return;
    }

    // get texture size
    int width, height;

    SDL_QueryTexture(mapTexture, nullptr, nullptr, &width, &height);
    SDL_Rect temp = {0,0,width, height};

    SDL_QueryTexture(FOREST::worldTex, nullptr, nullptr, &width, &height);
    SDL_Rect temp2 = {0,0,width,height};

    // draw into one texture
    SDL_SetTextureBlendMode(staticTex, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(rend, staticTex);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);

    SDL_RenderCopy(rend, mapTexture, nullptr, &temp);
    SDL_RenderCopy(rend, FOREST::worldTex, nullptr, &temp2);

    SDL_SetRenderTarget(rend, nullptr);

    // Generate LODs
    TOOLBOX::lodMaker(2, staticTex, LOD.LoD1);
    TOOLBOX::lodMaker(4, staticTex, LOD.LoD2);
    TOOLBOX::lodMaker(8, staticTex, LOD.LoD3);

    LOG::INFO("Generated lods");

    SDL_DestroyTexture(mapTexture);
    SDL_DestroyTexture(FOREST::worldTex);
}

void TERRAIN::generateTerrain() {
    LOG::INFO("Generating terrain < STARTED >");

    const float perlinNoiseScale = 0.02f;
    mapSize = glm::ivec2(window.scale.x / tileSize * scale, window.scale.y / tileSize * scale);

    LOG::DEBUG("Mapsize -> X: " + std::to_string(mapSize.x) + " Y: " + std::to_string(mapSize.y));

    // random gen
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

    float seed = distribution(generator);

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

    LOG::INFO("Running hydraulic erosion < STARTED >");
    erosion(mapSize, heightMap, 100000);
    LOG::INFO("Running hydraulic erosion < DONE >");

    for (int y = 0; y < mapSize.y; y++) {
        for (int x = 0; x < mapSize.x; x++) {

            TerrainTile terrainTile;
            terrainTile.rect = {x * tileSize, y * tileSize, tileSize, tileSize};

             float terrainNoise = heightMap[x + y * mapSize.x];

            if (terrainNoise < -0.35f) terrainTile.type = DEEP_SEA;
            else if (terrainNoise < -0.2f) terrainTile.type = SEA;
            else if (terrainNoise < -0.15f) terrainTile.type = SHORE;
            else if (terrainNoise < 0.4f) terrainTile.type = GRASS;
            else if (terrainNoise < 0.7 ) terrainTile.type = TAIGA;
            else if (terrainNoise < 0.85) terrainTile.type = MOUNTAIN;
            else terrainTile.type = SNOW;

            map.push_back(terrainTile);
        }
    }

    LOG::INFO("Generating terrain < DONE >");
    LOG::INFO("Optimizing terrain < STARTED >");

    SDL_Renderer* rend = window.rend;

    mapTexture = SDL_CreateTexture(
        rend,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        mapSize.x * tileSize,
        mapSize.y * tileSize
    );

    SDL_SetRenderTarget(rend, mapTexture);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);

    // generate the tiles color and border
    generateTile(rend);

    int width = 0, height = 0;

    SDL_QueryTexture(mapTexture, nullptr, nullptr, &width, &height);

    SDL_SetRenderTarget(rend, nullptr);

    // |=========================================|
    // |    Write tiles to Array for Dev mode    |
    // |=========================================|

    std::ofstream outFile("staticMapArray.txt");

    if (!outFile.is_open()) {
        LOG::WARN("Couldnt load staticMapArray.txt file");
        LOG::INFO("Optimizing terrain < DONE >");
        return;
    }

    for (int i = 0; i < map.size(); i++) {
        auto& kniga = map[i];

        outFile
        << kniga.rect.x     << " "          // position x
        << kniga.rect.y     << " "          // position y
        << kniga.type                       // type
        << std::endl;
    }

    outFile.close();

    LOG::INFO("Writen to staticMapArray.txt file successfully");
    LOG::INFO("Optimizing terrain < DONE >");
}